#include "CameraHandler.h"

CameraVideoHandler::CameraVideoHandler(int cameraId,std::shared_ptr<CameraDeCoder> cdecoder,QObject *parent)
    :QObject(parent)
    ,_cameraId(cameraId)
    ,_cDecoder(cdecoder)
    ,_swsCtx(nullptr)
    , _lastwidth(0), _lastheight(0), _lastFmt(AV_PIX_FMT_NONE)
{
    //_decodeThread = new QThread(this);
    //this->moveToThread(_decodeThread);
    //启动线程
    //_decodeThread->start();
    //qDebug()<<"CameraVideoHandler线程启动: cameraId=" <<cameraId<<",线程ID= "<<_decodeThread->currentThreadId();
}
CameraVideoHandler::~CameraVideoHandler(){
    if(_swsCtx){
        sws_freeContext(_swsCtx);
        _swsCtx=nullptr;
    }
    qDebug() << "CameraVideoHandler线程停止：cameraId=" << _cameraId;
}
