#ifndef NETWORKREACTOR_H
#define NETWORKREACTOR_H
#include <QTcpSocket>
#include <QObject>
#include <QByteArray>
#include <QThreadPool>
#include <QMap>
#include <vector>
#include "CameraHandler.h"
#include "Message.h"
#include "CameraDecoder.h"
#include "GlobalUser.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QMessageBox>
using std::vector;
class NetWorkReactor : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(NetWorkReactor)
public:
    static NetWorkReactor & instance(){
        static NetWorkReactor reactor;
        return  reactor;
    }
    //初始化线程池
    void initThreadPool(int maxThreads=QThread::idealThreadCount()*2);
    //注册摄像头消息处理器(摄像头ID+消息类型)
    void registerHandler(int cameraId,TaskType type,std::shared_ptr<IMessageHandler> handler);
    //连接服务器
    ~NetWorkReactor(){}
    bool connectToServer(const QString &ip,quint16 port);
    void startReactor(const QString &ip,quint16 port);
    void stopReactor();
    int readPacket(Packet &packet);
    int sendCommand(TLV tlv);
signals:
    void decodeFrame(int cameraId,const VideoFrame& frame);
    void handlerRegistered(int cameraId, TaskType type, IMessageHandler* handler);
    void Refresh(int);
private slots:
    //接收网络数据并解析
    void onReadyRead();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError err);
private:
    //消息任务
    NetWorkReactor(QObject *parent=nullptr);
    class MessageTask : public QRunnable{
    public:MessageTask(const VideoFrame &msg,std::shared_ptr<IMessageHandler> handler)
            :m_msg(msg)
            ,m_handler(handler)
        {setAutoDelete(true);}
        void run() override{
            if(m_handler){
                m_handler->handle(m_msg);
            }
        }
    private:
        VideoFrame   m_msg;
        std::shared_ptr<IMessageHandler> m_handler;
    };
private:
    QTcpSocket *    _socket;
    QThreadPool*    _threadPool;
    QByteArray      _recvBuffer;
    bool            _reactorRunning;//运行标志
    //处理器映射
    //绑定的是一个类型

    QMap<uint32_t,vector<QByteArray>>                              _frameBuffers;
    QMap<int,std::shared_ptr<CameraDeCoder>>                       _deCoder;
public:
    QMap<std::pair<int,TaskType>,std::shared_ptr<IMessageHandler>> _handlers;
};

#endif // NETWORKREACTOR_H
