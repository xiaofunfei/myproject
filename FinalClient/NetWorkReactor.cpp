#include "NetWorkReactor.h"
static string TLVtoString(const TLV&data){
    qDebug()<<"1";
    uint32_t netType=data.type;
    uint32_t netLength=data.length;
    qDebug()<<"2"<<data.length;
    string msg;
    msg.reserve(8+data.length);
    qDebug()<<"3";
    msg.append((const char*)&netType,4);
    msg.append((const char*)&netLength,4);
    msg.append(data.data,data.length);
    qDebug()<<"4";
    return msg;
}
int NetWorkReactor::readPacket(Packet &packet){
    if(_recvBuffer.size()<4){
        return 0;
    }
    int type;
    memcpy(&type,_recvBuffer.data(),4);
    _recvBuffer.remove(0,4);

    //读取length 4 字节
    if(_recvBuffer.size()<4){
        return 0;
    }
    int length;
    memcpy(&length,_recvBuffer.data(),4);
    _recvBuffer.remove(0,4);

    //读取msg(length个字节)
    if(_recvBuffer.size()<length){
        //数据不足就把前面的8字节放回去
        _recvBuffer.prepend((const char*)&length, 4);
        _recvBuffer.prepend((const char*)&type, 4);
        return 0;
    }
    QByteArray msgBytes = _recvBuffer.left(length);
    _recvBuffer.remove(0,length);
    packet.type=type;
    packet.length = length;
    packet.msg=msgBytes.toStdString();

    return 8+length;
}
NetWorkReactor::NetWorkReactor(QObject *parent)
    :QObject(parent)
    ,_reactorRunning(false)
{
    _socket=new QTcpSocket(this);
    _threadPool = new QThreadPool(this);
    connect(_socket,&QTcpSocket::readyRead,this,&NetWorkReactor::onReadyRead);
    connect(_socket,&QTcpSocket::disconnected,this,&NetWorkReactor::onDisconnected);
    connect(_socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error),this,&NetWorkReactor::onError);
}
void NetWorkReactor::startReactor(const QString &ip,quint16 port){
    if(_reactorRunning){
        qDebug()<<"Reactor has already been running!";
        return;
    }
    _reactorRunning=true;
    connectToServer(ip,port);
}
void NetWorkReactor::stopReactor(){
    if(!_reactorRunning){
        qDebug()<<"Reactor has already been stop!";
        return;
    }
    _reactorRunning=false;
    if(_socket->isOpen()){
        _socket->close();
    }
    //之后可放清楚摄像头的数组函数
}
void NetWorkReactor::initThreadPool(int maxThreads){

     _threadPool->setMaxThreadCount(maxThreads);
     qDebug()<<"线程池初始化完毕!线程数为 ： "<<maxThreads;
}
void NetWorkReactor::registerHandler(int cameraId, TaskType type,std::shared_ptr<IMessageHandler> handler){
     _handlers[{cameraId,type}]=handler;
     emit handlerRegistered(cameraId, type, handler.get());
     /*auto videoHandler = dynamic_cast<CameraVideoHandler*>(handler.get());
     if(videoHandler){
         connect(this,&NetWorkReactor::decodeFrame,videoHandler,&CameraVideoHandler::handle,Qt::QueuedConnection);
     }*/
}
bool NetWorkReactor::connectToServer(const QString &ip, quint16 port){
    if(_socket->isOpen()) _socket->close();
    _socket->connectToHost(ip,port);
    return _socket->waitForConnected(3000);
}
void NetWorkReactor::onReadyRead(){
    if(!_reactorRunning) return;
    //读取所有消息
    qDebug()<<"有消息过来！！！";
    _recvBuffer.append(_socket->readAll());
    while(true){
        Packet pack;
        int ret=readPacket(pack);
        if(ret==0){
            qDebug()<<"没有pack可以读了，我就马上要退出了！";
            return;
        }
        //CameraMessage msg=parseMessage(pack);
        if(pack.type==TASK_STREAM_DATA){
            //这个包套了两层这个地方需要做的是把这个解析两次
            qDebug()<<"我收到服务发来的TASK_STREAM_DATA包";
            qDebug()<<"packet 的type 是: "<<pack.type;
            qDebug()<<"packet 的length 长度为 "<<pack.length;
            DataFragment data;
            if(pack.length>sizeof(DataFragment)){
                qDebug()<<"pack.length>sizeof(DataFragment) ";
                return;
            }
            memcpy(&data,pack.msg.data(),pack.length);
            if(data.type==0){
                VideoMetadata meta;
                memcpy(&meta,data.data,24);
                meta.spsSPS=std::string(data.data+24,data.dataLen-24);
                //创建解码器
                auto decoder=std::make_shared<CameraDeCoder>(meta);
                _deCoder[meta.cameraId]=decoder;
                //此时对应得解码器已经插入到对于得位置了
                qDebug()<<"解码器创建完成";
                auto videoHandler=std::make_shared<CameraVideoHandler>(meta.cameraId,decoder,this);
                registerHandler(meta.cameraId,TASK_STREAM_DATA,videoHandler);
                auto user=GlobalUser::getInstance();
                if(user->CurrentFlag==0){
                    qDebug()<<"收到数据帧前的刷新信号已发送";
                    user->CurrentFlag=1;
                    emit(Refresh(user->CurrentWindow));
                }
                qDebug()<<"完成了初始化CameraDecoder";
                qDebug()<<"handler注册完成!!!";
            }else if(data.type==1){
                qDebug()<<"我进入了data.type=1的循环";
                uint32_t packetId = data.packerId;
                int total = data.totalFragments;
                int current = data.currentFragment;
                int dataLen = data.dataLen;
                // 1. 检查分片索引合法性（必须在 0~total-1 范围内，否则丢弃）
                if (current < 0 || current >= total) {
                    qWarning() << "无效的分片索引：packetId=" << packetId
                               << ", current=" << current << ", total=" << total;
                    return;
                }
                //这个流程中需要对数据AVPacket数据进行解码,但前提条件是能够正确组装发来的所有数据
                //第一件事是判断这个数据分片是不是第一次到达
                auto it=_frameBuffers.find(packetId);
                if(it == _frameBuffers.end()){
                    vector<QByteArray> fragments;
                    //预留数组不能用reserve
                    fragments.resize(total);
                    _frameBuffers[packetId]=fragments;
                    //更新迭代器
                    it=_frameBuffers.find(packetId);
                }
                it.value()[current]=QByteArray(data.data,dataLen);
                //接下来是判断是否所有的数据都接收了
                bool allRecvflag=true;
                for(const auto&frag: it.value()){
                    if(frag.isEmpty()){
                        allRecvflag=false;
                        break;
                    }
                }
                if(allRecvflag){
                    //如果已经全部收到那么就开始重组所有分片数据
                    QByteArray completeData;
                    for(const auto &frag : it.value()){
                        completeData.append(frag);
                    }

                    //接下来的逻辑就是构造一个完整的AVPacket数据包然后交给处理函数
                    //解码器初始化成功之后，现在要做的就是将帧解析也就是给AVPacket包打包发送
                    //构造一个AVPacket包
                    VideoFrame vf1;
                    memcpy(&vf1,completeData,24);
                    qDebug()<<"AVPacket cameraId"<<vf1.cameraId;
                    qDebug()<<"AVPacket size"<<vf1.size;
                    qDebug()<<"AVPacket pts"<<vf1.pts;
                    qDebug()<<"AVPacket dts"<<vf1.dts;
                    vf1.data.resize(completeData.size()-24);
                    memcpy(vf1.data.data(),completeData.data()+24,vf1.data.size());
                    //修正size
                    vf1.size=vf1.data.size();
                    qDebug() << "重组完成：cameraId=" << vf1.cameraId << "，数据长度=" << vf1.size;
                    auto handlerkey=std::make_pair(vf1.cameraId,TASK_STREAM_DATA);
                    if(_handlers.contains(handlerkey)){
                        //此时查找出来的handler就已经是处理函数了
                        auto handler=_handlers[handlerkey];
                        auto task = new MessageTask(vf1,handler);
                        _threadPool->start(task);
                        qDebug()<<"任务已经提交给线程池了！！";
                    }
                    else{
                        qWarning()<<"未找到handler ：cameraId= "<<vf1.cameraId;
                    }
                    //AVPacket包对应的数据结构已完成，
                    //此时这个分片得数据已提交处理，所以可以将它进行回收
                    _frameBuffers.erase(it);
                }
            }

        }else if(pack.type==TYPE_GET_STREAM_OPEN_ERROR){
            QByteArray jsonData = QByteArray::fromStdString(pack.msg);
            auto user=GlobalUser::getInstance();
            user->CurrentCamId=user->OldCamId;
            user->CurrentChaId=user->OldChaId;
            QJsonParseError parseError;
            QJsonDocument jsonDoc=QJsonDocument::fromJson(jsonData,&parseError);
            if (parseError.error != QJsonParseError::NoError) {
                qDebug() << "读取 TYPE_GET_STREAM_OPEN_ERROR 数据时JSON解析失败：" << parseError.errorString();
                return;
            }
            if(jsonDoc.isObject()){
                QJsonObject obj=jsonDoc.object();
                if (!obj.contains("camera")) {
                    QMessageBox::warning(nullptr, "警告", "无效的JSON数据：缺少camera字段");
                    return;
                }
                int cameraId=obj["camera"].toInt();
                QString str=QString("摄像头%1获取视频流错误，请检查摄像头是否正常工作！").arg(cameraId);
                QMessageBox::critical(nullptr,"错误",str);
            }
        }
    }
}
void NetWorkReactor::onDisconnected(){
            qDebug()<<"服务器断开连接";
}
void NetWorkReactor::onError(QAbstractSocket::SocketError err){
            qDebug()<<"网络错误: "<<_socket->errorString();
}
int NetWorkReactor::sendCommand(TLV tlv){
    qDebug()<<"已经调用了这个函数";
    string msg=TLVtoString(tlv);
    qDebug()<<QString::fromStdString(msg);
    int ret=_socket->write(msg.data(),msg.size());
    if(ret < 0){
        qDebug()<<"发送失败";
        return false;
    }else{
        qDebug()<<"发送成功";
        return true;
    }
}
