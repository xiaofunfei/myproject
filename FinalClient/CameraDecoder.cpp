#include "CameraDeCoder.h"

CameraDeCoder::CameraDeCoder(VideoMetadata meta,QObject *parent)
    :QObject(parent)
{
    if(initDecoder(meta)){
        qDebug()<<"CameraDecoder 初始化成功！";
    }else{
        qDebug()<<"CameraDocoder  初始化失败!";
    }
}
bool CameraDeCoder::initDecoder(VideoMetadata &meta){
    _CameraId=meta.cameraId;
    _codecId = (AVCodecID)meta.codecId;
    //查找对应解码器
    m_codec = avcodec_find_decoder(_codecId);
    if(!m_codec){
        qWarning()<<"找不到解码器,编码格式: " << _codecId;
        return false;
    }
    m_codecCtx = avcodec_alloc_context3(m_codec);
    if(!m_codecCtx){
        qWarning()<<"无法分配解码器上下文！！！！";
        return false;
    }
    m_codecCtx->width=meta.width;//视频宽度
    m_codecCtx->height=meta.height;//视频高度
    m_codecCtx->pix_fmt=AV_PIX_FMT_YUV420P;//像素格式
    m_codecCtx->time_base.num=meta.timeBaseNum;//时间基分子
    m_codecCtx->time_base.den=meta.timeBaseDen;//时间基分母

    //接下来是关键的处理SPS/PPS的步骤
    int spsPpsSize=meta.spsSPS.size();
    if(spsPpsSize>0){
        //为extradata分配内存(FFmpeg要求按16字节对齐,用av_malloc)
        m_codecCtx->extradata = (uint8_t*)av_malloc(spsPpsSize+AV_INPUT_BUFFER_PADDING_SIZE);
        if(!m_codecCtx->extradata){
            qWarning()<<"无法分配extradata内存";
            avcodec_free_context(&m_codecCtx);
            return false;
        }
        //复制SPS/PPS数据到extradata
        memcpy(m_codecCtx->extradata,meta.spsSPS.data(),spsPpsSize);
        m_codecCtx->extradata_size = spsPpsSize;
        //补充FFmpeg要求的填充字节
        memset(m_codecCtx->extradata + spsPpsSize,0,AV_INPUT_BUFFER_PADDING_SIZE);

        //校验SPS/PPS是否正确
        bool hasValidStartCode = false;
               if (spsPpsSize >= 3) {
                   // 检查0x000001起始码
                   if (m_codecCtx->extradata[0] == 0 &&
                       m_codecCtx->extradata[1] == 0 &&
                       m_codecCtx->extradata[2] == 1) {
                       hasValidStartCode = true;
                   }
                   // 检查0x00000001起始码（4字节）
                   if (spsPpsSize >=4 &&
                       m_codecCtx->extradata[0] == 0 &&
                       m_codecCtx->extradata[1] == 0 &&
                       m_codecCtx->extradata[2] == 0 &&
                       m_codecCtx->extradata[3] == 1) {
                       hasValidStartCode = true;
                   }
               }
               if (!hasValidStartCode) {
                   qWarning() << "SPS/PPS格式错误，缺少起始码（0x000001或0x00000001）";
                   av_free(m_codecCtx->extradata);
                   avcodec_free_context(&m_codecCtx);
                   return false;
               }
    }

    int ret=avcodec_open2(m_codecCtx,m_codec,nullptr);
    if(ret<0){
        char errbuf[1024];
        av_strerror(ret,errbuf,sizeof(errbuf));
        qWarning()<<"无法打开解码器: "<<errbuf;
        av_free(m_codecCtx->extradata);
        avcodec_free_context(&m_codecCtx);
        return false;
    }
    return true;
}
