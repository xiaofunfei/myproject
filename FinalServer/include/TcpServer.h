#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__
#include "Acceptor.h"
#include "EventLoop.h"
namespace SOS
{

class TcpServer
{
public:
    TcpServer(unsigned short port, const string & ip="0.0.0.0")
    : _acceptor(port, ip)
    , _loop(_acceptor)
    {}

    void setAllCallbacks(TcpConnectionCallback && cb1,
                         TcpConnectionCallback && cb2,
                         TcpConnectionCallback && cb3)
    {
        _loop.setAllCallbacks(std::move(cb1),
                              std::move(cb2), 
                              std::move(cb3));
    }

    void start() 
    {
        _acceptor.ready();
        _loop.loop();
    }

    //stop函数要与start函数运行在不同的线程
    void stop()
    {
        _loop.unloop();
    }

private:
    Acceptor    _acceptor;
    EventLoop   _loop;
};

}//end of namespace SOS


#endif

