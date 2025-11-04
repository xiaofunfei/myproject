#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__
#include "Socket.h"
#include "SocketIO.h"
#include "InetAddress.h"
#include <memory>
#include <functional>
#include <mutex>
using std::shared_ptr;
using std::function;

namespace SOS
{
class EventLoop;

class TcpConnection;
using TcpConnectionPtr=shared_ptr<TcpConnection>;
using TcpConnectionCallback=function<void(TcpConnectionPtr)>;

class TcpConnection
//辅助类，为了在成员函数内部通过this直接获取本对象的智能指针shared_ptr
: public std::enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(int fd, EventLoop* loop);
    ~TcpConnection();

    //TcpConnection内部的setAllCallbacks函数不能采用移动语义
    //必须要表达值语义, 即不能把EventLoop对象的三个函数对象
    //直接转移到TcpConnection对象中, 因为TcpConenction对象不止一个
    void setAllCallbacks(const TcpConnectionCallback & cb1, 
                         const TcpConnectionCallback & cb2,
                         const TcpConnectionCallback & cb3)
    {
        _onConnection = cb1;
        _onMessage = cb2;
        _onClose = cb3;
    }

    string receive();
    int readPacket(Packet &);
    void send(const string & msg);
    void sendInLoop(const string & msg);
    void sendInLoop(const TLV &);
    bool isClosed() const;
    void shutdown();
    string toString() const;

    void handleNewConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();
private:
    InetAddress getLocalAddress();
    InetAddress getPeerAddress();

private:
    Socket _sock;
    SocketIO _sockIO;
    InetAddress _localAddr;
    InetAddress _peerAddr;
    EventLoop * _loop;
    std::mutex _sendMutex;
    TcpConnectionCallback _onConnection;
    TcpConnectionCallback _onMessage;
    TcpConnectionCallback _onClose;
};


}//end of namespace SOS;


#endif

