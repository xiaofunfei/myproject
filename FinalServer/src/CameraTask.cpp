#include "CameraTask.h"
#include "arpa/inet.h"
using json=nlohmann::json;
namespace SOS{
uint32_t generatePacketId(){
    static uint32_t seq=0;
    uint32_t timestamp=time(nullptr)%0xFFFF;
    return (timestamp<<16) | (seq++%0xFFFF);//高16时间戳，低16则是自增序列号
}
vector<char> serializeMetaData(const VideoMetadata &meta){
    vector<char > data;
    size_t totalSize=sizeof(meta.cameraId)+sizeof(meta.codecId)+sizeof(meta.height)
                    +sizeof(meta.width)+sizeof(meta.timeBaseDen)+sizeof(meta.timeBaseNum)
                    +meta.spsSPS.size();
    //给数组预分配一些内存
    data.reserve(totalSize);
    //接下来一次写入固定段
    auto append=[&](const void* ptr,size_t size){
        data.insert(data.end(),(const char*)ptr,(const char*)ptr+size);
    };

    append(&meta.cameraId,sizeof(meta.cameraId));
    append(&meta.codecId,sizeof(meta.codecId));
    append(&meta.width,sizeof(meta.width));
    append(&meta.height,sizeof(meta.height));
    append(&meta.timeBaseNum,sizeof(meta.timeBaseNum));
    append(&meta.timeBaseDen,sizeof(meta.timeBaseDen));
    append(meta.spsSPS.data(),meta.spsSPS.size());

    return data;
}
void CameraTask::process(){
    startTask();
    avformat_network_init();
    //AVFormatContext* fmtCtx=nullptr;//格式化上下文
    //AVCodecContext  *pCodecCtx = nullptr;  //音视频编码器上下文结构体
    //AVCodec         *pCodec=nullptr;
    //AVPacket        *pPacket=nullptr;
    int videoStreamIdx=-1;
    _fmtCtx=avformat_alloc_context();
    AVDictionary *avdic=nullptr;
    av_dict_set(&avdic,"rtsp_transport","tcp",0);
    av_dict_set(&avdic,"max_delay","100",0);
    av_dict_set(&avdic, "stimeout", "5000000", 0); // 超时5秒（微秒）
    // 增大TCP接收缓冲区（单位：字节，根据网络情况调整）
    av_dict_set(&avdic, "buffer_size", "1024000", 0);
    //打开流
    int ret=avformat_open_input(&_fmtCtx,_cmsg.url.c_str(),nullptr,&avdic);
    av_dict_free(&avdic);


    
    if(ret<0){
        printf("%d 摄像头进程无法打开流 : ",_cmsg.CameraId);
        //在此处往用户转发送一个错误消息
        TLV tlv;
        tlv.type=TYPE_GET_STREAM_OPEN_ERROR;
        json j;
        j["camera"]=_cmsg.CameraId;
        string json_str=j.dump(4);
        tlv.length=json_str.size();
        strncpy(tlv.data,json_str.c_str(),tlv.length);
        _conn->sendInLoop(tlv);
        printf("视频打开流错误流消息已发送!\n");
        avformat_network_deinit();
        return;
    }
    printf("avformat_open_input is over!\n");
    //解析流的信息
    if(avformat_find_stream_info(_fmtCtx,nullptr)<0){
        printf("%d 摄像头进程无法解析流",_cmsg.CameraId);
        avformat_close_input(&_fmtCtx);
        avformat_network_deinit();
        return ;
    }
    printf("avformat_open_input is over!\n");
    //查找视频流
    videoStreamIdx=av_find_best_stream(_fmtCtx,AVMEDIA_TYPE_VIDEO,-1,-1,nullptr,0);
    if(videoStreamIdx<0){
        printf("%d 摄像头进程未找到视频流\n",_cmsg.CameraId);
        avformat_close_input(&_fmtCtx);
        avformat_network_deinit();
        return;
    }
    if (videoStreamIdx >= 0) {
    AVCodecParameters* codecPar = _fmtCtx->streams[videoStreamIdx]->codecpar;
    printf("videoStreamIdx = %d, 流类型 = %d (视频流应为 %d)\n", 
           videoStreamIdx, 
           codecPar->codec_type, 
           AVMEDIA_TYPE_VIDEO);
    }
    printf("av_find_best_strem is over! and now start sending Meta Data!\n");
    //发送元数据
    //这个是让他获取解码器1
    sendMetaData(_fmtCtx,videoStreamIdx);
    printf("MetaDATA has already sent!\n");
    //开始循环读取Packet
    AVPacket* pkt=av_packet_alloc();
    while(_isRunning){
        //todo:从此处获取解析视频流的数据包
        printf("start av_read_frame\n");
        std::lock_guard<std::mutex> lock(_fmtCtxMutex); 
        if (!_isRunning || !_fmtCtx) {
            printf("任务已停止或上下文释放，退出循环\n");
            break;
        }else{
            printf("还可以执行!!!\n");
        }
         ret=av_read_frame(_fmtCtx,pkt);
        if(ret<0){
            printf("av_read_frame failed!\n");
            if (ret == AVERROR_EOF || !_isRunning) { 
            // 遇到EOF或已停止，直接退出
            break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            continue;
        }else{
            printf("读取pkt成功!!");
        }
        printf("=====================av_read_frame success!======================================\n");
        if(pkt->stream_index==videoStreamIdx){
            printf("start send AVPacket!\n");
            sendPacketToClient(pkt);
        }else{
            printf("can not send Packet!\n");
        }
        //及时释放当前帧
        av_packet_unref(pkt);
        printf("now is in process while!\n");
        //_isRunning=false;
    }
    av_packet_free(&pkt);
    {
        std::lock_guard<std::mutex> lock(_fmtCtxMutex); // 加锁释放
        if (_fmtCtx) {
            avformat_close_input(&_fmtCtx);
            _fmtCtx = nullptr;
        }
    }

    printf("摄像头%d 任务已完全退出\n", _cmsg.CameraId);
    avformat_network_deinit();
    _cv.notify_one();
}
void CameraTask::startTask(){
    if(_isRunning){
        printf("%d Camera is running!\n",_cmsg.CameraId);
    }
    _isRunning=true;
}
void CameraTask::stopTask(){
    if(!_isRunning){
        printf("%d Camera has been stopped!\n",_cmsg.CameraId);
         return;
    }
    _isRunning=false;
    // 加锁释放_fmtCtx，避免与process的av_read_frame并发
    std::lock_guard<std::mutex> lock(_fmtCtxMutex); 
    if(_fmtCtx){
        avformat_close_input(&_fmtCtx);
        _fmtCtx=nullptr;
    }
    printf("摄像头%d 旧任务已强制停止\n",_cmsg.CameraId);
}
void CameraTask::sendMetaData(AVFormatContext *fmtCtx,int videoStreamIdx){
    AVStream *stream=fmtCtx->streams[videoStreamIdx];
    AVCodecParameters *codecPar=stream->codecpar;
    VideoMetadata meta;
    meta.cameraId=_cmsg.CameraId;
    printf("meta.cameraId is %d\n",meta.cameraId);
    meta.codecId=codecPar->codec_id;
    printf("meta.codecId IS %d \n",meta.codecId);
    meta.width=codecPar->width;
    printf("meta.width is %d \n", meta.width);
    meta.height=codecPar->height;
    printf("meta.height is %d \n",meta.height);
    meta.timeBaseNum=stream->time_base.num;
    printf("meta.timeBaseNum is %d \n",stream->time_base.num);
    meta.timeBaseDen=stream->time_base.den;
    printf("meta.timeBaseDen is %d \n",stream->time_base.den);
    //meta已经组装完成！
    //TLV tlv1;
    if(codecPar->extradata){
        meta.spsSPS.assign((char *)codecPar->extradata,codecPar->extradata_size);
    }
    printf("meta.spsSPS is %s\n",meta.spsSPS.c_str());
    vector<char> metaData=serializeMetaData(meta);
    printf("元数据总大小：%zu 字节\n",metaData.size());

    //分片发送
    fragmentAndSend(metaData.data(),metaData.size(),0);
    //这个data就是这个msg字符串
}
void CameraTask::sendPacketToClient(AVPacket *pkt){
    VideoFrame frame;
    frame.cameraId=_cmsg.CameraId;
    frame.size=pkt->size;
    frame.pts=pkt->pts;
    frame.dts=pkt->dts;
    printf("datadatadatadata!!!!!!\n");
    frame.data=pkt->data;
    printf("datadatadatadata=============================!!!!!!\n");
    //序列化AVPacket
    vector<char> frameData;
    //写入CameraId
    uint32_t netCameraId=frame.cameraId;
    frameData.insert(frameData.end(),(char*)&netCameraId,(char*)&netCameraId+4);
    uint32_t netSize=frame.size;
    frameData.insert(frameData.end(),(char*)&netSize,(char*)&netSize+4);
    uint64_t netPts=frame.pts;
    frameData.insert(frameData.end(),(char*)&netPts,(char*)&netPts+8);
    uint64_t netDts=frame.dts;
    frameData.insert(frameData.end(),(char*)&netDts,(char*)&netDts+8);
    frameData.insert(frameData.end(),(char*)frame.data,(char*)frame.data+frame.size);
    printf("frameData has already been seralized!\n");
    //此时已经序列化完成，现在要做的是将他分片发送
    fragmentAndSend(frameData.data(),frameData.size(),1);
    printf("fragmentAVPacket has already been sent!\n");
}
void CameraTask::sendToClient(int type,const void *data,size_t len){
    TLV tlv;
    tlv.type=type;
    tlv.length=len;
    //此处用memcpy的原因是因为传输的数据里面会有'\0'，这样用strncpy的话会被截断因此为了避免这个情况就使用这个
    printf("sendToclient has occur the problem!\n");
    memcpy(tlv.data,data,len);
    printf("diopuiopook;l454556646\n");
    _conn->sendInLoop(tlv);
}
void CameraTask::fragmentAndSend(const char *data,size_t data_len,uint8_t type){
    //每片数据帧的最大传输字节
    const size_t MAX_DATA_LENGTH=1380;
    //生成每个包的唯一id
    uint32_t packet_id=generatePacketId();
    //计算需要分多少片
    size_t total_fragments=(data_len+MAX_DATA_LENGTH-1)/MAX_DATA_LENGTH;
    for(size_t i=0;i<(total_fragments);++i){
        DataFragment frag{};
        frag.type=type;
        //超过了1片就分片
        frag.isFragmented=(total_fragments>1)?1:0;
        //再给头部填充
        frag.totalFragments=total_fragments;
        frag.currentFragment=i;
        frag.packerId=packet_id;
        frag.dataLen=std::min(MAX_DATA_LENGTH,data_len-i*MAX_DATA_LENGTH);
        //转回主机字节计算
        size_t copy_len=frag.dataLen;
        memcpy(frag.data,data+i*MAX_DATA_LENGTH,copy_len);
        //到此目前一个分片已经包装完
        //接下来再给他套一层TLV
        TLV tlv;
        tlv.type=TASK_STREAM_DATA;
        tlv.length=sizeof(frag);
        memcpy(tlv.data,&frag,sizeof(frag));
        _conn->sendInLoop(tlv);
        printf("发送了第 %ld 个分片\n",i);
    }
}
}