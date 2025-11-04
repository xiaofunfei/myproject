#ifndef __SmartHomeMonitorServer_H
#define __SmartHomeMonitorServer_H

#include "TcpServer.h"
#include "ThreadPool.h"
#include "CameraTask.h"
#include <map>
#include "MySQLClient.h"
#include <memory>
#include <mutex>
#include <condition_variable>
using std::shared_ptr;
using std::map;
namespace SOS{
class SmartHomeMonitorServer{
public:
    SmartHomeMonitorServer(int threadNum, int taskSize, unsigned short port, const string & ip = "0.0.0.0");
    ~SmartHomeMonitorServer();
    void start();
    void stop();
private:
    void onConnection(SOS::TcpConnectionPtr);
    void onMessage(SOS::TcpConnectionPtr);
    void onClose(SOS::TcpConnectionPtr);
private:
    ThreadPool          _threadpool;
    TcpServer           _tcpServer;
    //MySQLClient         _sql;
    map<int,shared_ptr<CameraTask>>     _cTask;
    std::mutex          _mutex;
    std::condition_variable     _cv;
};

}//end of namespace SOS
#endif