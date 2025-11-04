#ifndef CAMERAHANDLER_H
#define CAMERAHANDLER_H
#include "Message.h"
#include <QObject>
#include <QImage>
#include <CameraDeCoder.h>
#include <QDebug>
#include <QThread>
extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>
}
class CameraVideoHandler : public QObject,public IMessageHandler
{
    Q_OBJECT
public:
    explicit CameraVideoHandler(int cameraId,std::shared_ptr<CameraDeCoder> cDecoder,QObject *parent=nullptr);
    ~CameraVideoHandler();
    //线程池中执行：负责解码
    void handle(VideoFrame &vf_msg) override{
        qDebug()<<"CameraId: "<<_cameraId<<" enter handler!!!";
        if(!_cDecoder || !_cDecoder->m_codecCtx || !avcodec_is_open(_cDecoder->m_codecCtx)){
            qWarning()<<"解码器无效或未打开！: cameraId = "<<_cameraId;
            return;
        }
        //自动加锁，不能让多个线程同时打开一个解码器
        QMutexLocker locker(&_cDecoder->getMutex());
        AVPacket *pkt=av_packet_alloc();
        if(!pkt){
            qWarning()<<"AVPacket alloc 失败！！！: cameraID = "<<_cameraId;
            return;
        }

        pkt->data=(uint8_t*)av_malloc(vf_msg.size);
        if(!pkt->data){
            av_packet_free(&pkt);
            qWarning()<<"AVPacket 数据内存分配失败!<< cameraiD= "<<_cameraId;
            return;
        }
        memcpy(pkt->data,vf_msg.data.data(),vf_msg.data.size());
        pkt->size=vf_msg.data.size();
        pkt->pts=vf_msg.pts;
        pkt->dts=vf_msg.dts;
        pkt->stream_index=0;
        //上面已经初始化完一个AVPakcet数据包了
        //解码
        int ret1=avcodec_send_packet(_cDecoder->m_codecCtx,pkt);
        av_packet_unref(pkt);
        av_packet_free(&pkt);
        if(ret1<0){
            if(ret1 !=AVERROR(EAGAIN)){
                char errbuf[1024];
                av_strerror(ret1,errbuf,sizeof(errbuf));
                qWarning()<<"avcodec_send_packet失败: "<<errbuf<<" ,cameraId= "<<_cameraId;
            }
            return;
        }
        //接收解码之后得帧
        AVFrame* frame=av_frame_alloc();
        while(true){
           int ret=avcodec_receive_frame(_cDecoder->m_codecCtx,frame);
            if(ret==AVERROR(EAGAIN)){
                //暂时无帧退出循环
                //QThread::msleep(10);
                //ret1=avcodec_send_packet(_cDecoder->m_codecCtx,pkt);
                break;
            }else if(ret==AVERROR_EOF){
                qDebug()<<"解码结束: cameraId= "<<_cameraId;
                break;
            }else if(ret<0){
                char errbuf[1024];
                av_strerror(ret, errbuf, sizeof(errbuf));
                qWarning() << "avcodec_receive_frame失败：" << errbuf << "，cameraId=" << _cameraId;
                break;
            }
            //转换YUV帧为rgb的QImage
            QImage rgbImg=convertFrameToimage(frame);
            if(!rgbImg.isNull()){
                int cid=vf_msg.cameraId;
                //emit  frameDecoded(vf_msg.cameraId,rgbImg);
                QMetaObject::invokeMethod(
                            this,[this,cid,rgbImg](){
                    emit frameDecoded(cid,rgbImg);
                },
                Qt::QueuedConnection);
                qDebug() << "解码线程ID：" << QThread::currentThreadId() << "，发送信号";
                qDebug()<<"我处理完了，现在有一张Image等待认领！！！";
            }
        }
        av_frame_free(&frame);
}
    void feelfree(){
        qDebug()<<"wcnmdsbqt";
    }

signals:
    //解码完成信号
    void frameDecoded(int cameraId,const QImage& image);
private:
    QImage convertFrameToimage(AVFrame *frame){
        if(!frame || frame->width<=0 || frame->height<=0){
            return      QImage();
        }

        //初始化格式转换上下文
        //输入格式
        AVPixelFormat srcFmt=(AVPixelFormat)frame->format;
        int srcW=frame->width;
        int srcH=frame->height;
        if(!_swsCtx || _lastwidth != srcW || _lastheight!=srcH || _lastFmt!=srcFmt){
            //释放旧的上下文
            if(_swsCtx){
                sws_freeContext(_swsCtx);
            }
            //创建新的转换上下文
            _swsCtx = sws_getContext(srcW,srcH,srcFmt,srcW,srcH,AV_PIX_FMT_RGB32,SWS_BILINEAR,nullptr,nullptr,nullptr);
            //更新缓存的参数
            _lastwidth=srcW;
            _lastheight=srcH;
            _lastFmt=srcFmt;
        }
        QImage rgbimg(srcW,srcH,QImage::Format_RGB32);
        //RGB数据首地址
        uint8_t * dstData[1] = {(uint8_t*) rgbimg.bits()};
        //每行字节数(宽*4）
        int dstLinesize[1]  =   {srcW * 4};
        sws_scale(
                  _swsCtx,
                    frame->data,frame->linesize,
                    0,srcH,
                    dstData,dstLinesize);
        return rgbimg;
    }
private:
    int _cameraId;//关联摄像头的ID
    std::shared_ptr<CameraDeCoder>  _cDecoder;
    //格式转换用的上下文
    SwsContext *            _swsCtx;
    //缓存上一次转换用的参数
    int                     _lastwidth;
    int                     _lastheight;
    AVPixelFormat           _lastFmt=AV_PIX_FMT_NONE;
    //QThread*                _decodeThread;
};

#endif // CAMERAHANDLER_H
