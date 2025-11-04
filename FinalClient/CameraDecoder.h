#ifndef CAMERADECODER_H
#define CAMERADECODER_H
#include <QObject>
#include "Message.h"
#include <QDebug>
#include <QMutex>
extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>
}
class CameraDeCoder : public QObject
{
   Q_OBJECT
public:
    CameraDeCoder(VideoMetadata meta,QObject *parent=nullptr);
    bool initDecoder(VideoMetadata &meta);
    ~CameraDeCoder(){
        avcodec_close(m_codecCtx);
        avcodec_free_context(&m_codecCtx);
    }
    QMutex& getMutex(){
        return _mutex;
    }

public:
    int                 _CameraId;
    AVCodec *           m_codec=nullptr;
    AVCodecContext*     m_codecCtx=nullptr;
    AVCodecID           _codecId;
    QMutex              _mutex;
};

#endif // CAMERADECODER_H
