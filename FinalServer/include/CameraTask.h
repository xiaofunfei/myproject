#ifndef __CAMERATASK_H
#define __CAMERATASK_H
#include "Message.h"
#include <unistd.h>
#include "TcpConnection.h"
#include "MySQLClient.h"
#include <nlohmann/json.hpp>
#include <thread>
#include <time.h>
#include <atomic>
#include <mutex>
#include <condition_variable>
//#include <condition_variable>
extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>
}
//我在获取数据的时候为了以后拓展功能，此处传一份给客户端作为实时播放和事后回放
namespace SOS{
class CameraTask{
public:
    CameraTask(TcpConnectionPtr conn,const CameraMessage &cmsg)
    :_conn(conn)
    ,_cmsg(cmsg)
    ,_isRunning(false)
    {}
    ~CameraTask(){}
    void process();
    void startTask();
    void stopTask();
    void sendToClient(int type,const void *data,size_t len);
    void sendMetaData(AVFormatContext *f,int sIdx);
    void sendPacketToClient(AVPacket *pkt);
    void fragmentAndSend(const char *data,size_t data_len,uint8_t type);
    bool isRunning(){return _isRunning.load();}
    void waitForExit(){
        std::unique_lock<std::mutex> lock(_cvmutex);
        _cv.wait(lock,[this]{return !_isRunning.load();});
    }
private:
    TcpConnectionPtr    _conn;
    CameraMessage       _cmsg;
    std::atomic<bool>   _isRunning{false};
    AVFormatContext* _fmtCtx = nullptr; // 保存格式上下文指针
    //std::condition_variable     _cv;
    std::condition_variable     _cv;    // 条件变量，用于通知任务退出
    std::mutex                  _cvmutex;   // 保护条件变量的互斥锁
    std::mutex                  _fmtCtxMutex;//保护_fmtCtx的读取
};
}
#endif