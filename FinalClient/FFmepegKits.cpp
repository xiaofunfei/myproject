#include "FFmepegKits.h"
#include <QDebug>

FFmepegKits::FFmepegKits(QObject *parent)
    : QThread(parent)
    ,_isRunning(false)
{}
void FFmepegKits::startPlay(QString url)
{
    _url = url;
    _isRunning=true;
    this->start();//开始运行耗时线程
}
void FFmepegKits::stop(){
    _isRunning=false;
    wait();
}
void FFmepegKits::run(){
    qDebug() << "FFmpegKits::run()..." << endl;
    AVFormatContext *pFormatCtx = nullptr; //音视频封装格式上下文结构体
    AVCodecContext  *pCodecCtx = nullptr;  //音视频编码器上下文结构体
    AVCodec *pCodec = nullptr; //音视频编码器结构体
    AVFrame *pFrame = nullptr; //存储一帧解码后像素数据
    AVFrame *pFrameRGB = nullptr;
    AVPacket *pPacket = nullptr; //存储一帧压缩编码数据
    uint8_t *pOutBuffer = nullptr;
    static struct SwsContext *pImgConvertCtx = nullptr;
    //avformat_network_init();   //初始化FFmpeg网络模块
    //Allocate an AVFormatContext.分配一个音视频封装格式上下文结构体
    pFormatCtx = avformat_alloc_context();
    //AVDictionary
    AVDictionary *avdic=nullptr;
    char option_key[] = "rtsp_transport";
    char option_value[] = "tcp";
    av_dict_set(&avdic, option_key, option_value, 0);

    char option_key2[] = "max_delay";
    char option_value2[] = "100";
    av_dict_set(&avdic, option_key2, option_value2, 0);
    av_dict_set(&avdic, "stimeout", "5000000", 0); // 超时5秒（微秒）
    // 增大TCP接收缓冲区（单位：字节，根据网络情况调整）
    av_dict_set(&avdic, "buffer_size", "1024000", 0);
    if (avformat_open_input(&pFormatCtx, _url.toStdString().data(), nullptr, &avdic) != 0)
       {
           printf("无法打开文件：%s\n", _url.toStdString().c_str());
           return;
       }

       if (avformat_find_stream_info(pFormatCtx, nullptr) < 0)
       {
           printf("无法获取流信息。文件可能已损坏或格式不支持。\n");
           avformat_close_input(&pFormatCtx);  // 释放资源****------>>自己加的
           return;
       }
       //查找视频中包含的流信息，音频流先不处理
     int videoStreamIdx = -1;
     qDebug("apFormatCtx->nb_streams:%d", pFormatCtx->nb_streams);//打印出有几条流
     videoStreamIdx = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);//找到最佳视频流
     if(videoStreamIdx < 0) {
          qDebug() << "av_find_best_stream error:" << av_get_media_type_string(AVMEDIA_TYPE_VIDEO);
          return ;
     }
     qDebug("video stream idx: %d\n", videoStreamIdx);
         //查找解码器
     qDebug("avcodec_find_decoder...\n");
     qDebug() << "AVCodecID:" << pFormatCtx->streams[videoStreamIdx]->codecpar->codec_id << endl;//找到流的解码格式
     pCodec = avcodec_find_decoder(pFormatCtx->streams[videoStreamIdx]->codecpar->codec_id);//在FFmepeg中查找对应的解码器

     if (pCodec == nullptr)
     {
         qDebug("Codec not found.\n");
             return;
     }

     pCodecCtx = avcodec_alloc_context3(pCodec);//为解码器分配一个上下文结构体
     //拷贝解码器参数
     avcodec_parameters_to_context(pCodecCtx, pFormatCtx->streams[videoStreamIdx]->codecpar);//知道要解码的视频是什么格式
     qDebug("pCodecCtx->codec_id: %d\n", pCodecCtx->codec_id);
     //初始化解码器参数
     pCodecCtx->bit_rate = 0;   //初始化为0  ************硬编码设置格式不推荐！
     pCodecCtx->time_base.num = 1;  //下面两行：一秒钟25帧
     pCodecCtx->time_base.den = 25;
     pCodecCtx->frame_number = 1;  //每包一个视频帧
     //打开解码器
     if (avcodec_open2(pCodecCtx, pCodec, nullptr) < 0)
     {
         printf("Could not open codec.\n");
         return;
     }
     //将解码后的YUV数据转换成RGB32
     //创建转换上下文
     pImgConvertCtx = sws_getContext(
     pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
     pCodecCtx->width, pCodecCtx->height,AV_PIX_FMT_RGB32,
     SWS_BICUBIC, nullptr, nullptr, nullptr);//转换格式

     //计算所需内存大小
     int numBytes = avpicture_get_size(AV_PIX_FMT_RGB32, pCodecCtx->width,pCodecCtx->height);
     //新版本获取
     //int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB32, pCodecCtx->width, pCodecCtx->height, 1);
     //准备源和目标数据缓冲区
     pFrame     = av_frame_alloc();//开辟空间
     pFrameRGB  = av_frame_alloc();//开辟空间
     pOutBuffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
     avpicture_fill((AVPicture *) pFrameRGB, pOutBuffer,
                       AV_PIX_FMT_RGB32, pCodecCtx->width, pCodecCtx->height);

     pPacket = (AVPacket *) malloc(sizeof(AVPacket)); //分配一个packet
     //pPacket = av_packet_alloc();  // 替代 malloc，推荐用法更安全的版本
     int y_size = pCodecCtx->width * pCodecCtx->height;
     av_new_packet(pPacket, y_size); //分配packet的数据
     while (_isRunning) {
         if (av_read_frame(pFormatCtx, pPacket) < 0) {//读取数据包
             break; //这里认为视频读取完了
         }

         if (pPacket->stream_index == videoStreamIdx) {
             int got_picture;
                 //执行解码
             int ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture,pPacket);//解析数据包将得到的原始图像数据存入pFrame
                                                                                      //然后返回一个值给got_picutre，为1则成功，反之则失败
             if (ret < 0)
             {
                 printf("decode error.\n");
                 return;
             }

             if (got_picture)
             {
              //执行转换
                 sws_scale(pImgConvertCtx, (uint8_t const * const *) pFrame->data, pFrame->linesize,
                           0, pCodecCtx->height, pFrameRGB->data, pFrameRGB->linesize);//转换图片格式

                 //把这个RGB数据 用QImage加载
                 QImage tmpImg((uchar *)pOutBuffer,
                                   pCodecCtx->width, pCodecCtx->height, QImage::Format_RGB32);
                 QImage image = tmpImg.copy(); //把图像复制一份 传递给界面显示
                 emit sigGetOneFrame(image);  //发送信号
              }
         }
             av_free_packet(pPacket);
         }
     av_free(pOutBuffer);
     av_free(pFrameRGB);
     avcodec_close(pCodecCtx);
     avformat_close_input(&pFormatCtx);
}
