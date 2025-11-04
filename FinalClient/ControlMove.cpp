#include "ControlMove.h"
#include <QUrl>
#include <QTimer>
#include <QMessageBox>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
ControlMove::ControlMove(QWidget *parent)
    :QWidget(parent)
{}
void ControlMove::sendCommand(string s){
    KVQuery kvs;
    kvs.add("channelId","1");
    kvs.add("value",s);
    kvs.add("speed","69");
    string token1=kvs.getToken();
    kvs.erase("secret");
    QString tmp=QString::fromStdString(kvs.toString());
    tmp=tmp+"&token="+QString::fromStdString(token1);
    QUrl url("http://192.168.5.222/xsw/api/ptz/control");
    url.setQuery(tmp);
    QString httpRequest = QString("POST %1 HTTP/1.1\r\n"
                                 "Host: 192.168.5.222\r\n"  // 主机地址（不含路径）
                                 "Content-Type: application/x-www-form-urlencoded\r\n"
                                  "Connection: keep-alive\r\n"    // 告诉服务器处理完后关闭连接
                                 "\r\n")                    // 空行分隔请求头和请求体
                          .arg(url.path() + "?" + url.query()); // 拼接路径和参数
    _tcpClient->write(httpRequest.toUtf8());
    _tcpClient->flush();
}
void ControlMove::upMove(){
    _tcpClient = new QTcpSocket(this);
    _tcpClient->connectToHost("192.168.5.222",80);
    if(!_tcpClient->waitForConnected()){
        QMessageBox::critical(this,"连接服务器出错！",_tcpClient->errorString());
        return;
    }
    sendCommand("u");
    qDebug()<<"uuu";
    QTimer::singleShot(500, this, [=]() {
        // 检查连接是否仍然有效
        qDebug()<<"PPPPP";
        if (_tcpClient->state() != QAbstractSocket::ConnectedState) {
            qDebug() << "连接已断开，重新连接...";
            _tcpClient->connectToHost("192.168.5.222", 80);
            if (!_tcpClient->waitForConnected(2000)) {
                qDebug() << "重新连接失败，无法发送停止命令";
                return;
            }
        }
        // 发送停止命令
        sendCommand("s");
        // 发送完成后断开连接
        _tcpClient->disconnectFromHost();
    });
}
void ControlMove::downMove(){
    _tcpClient = new QTcpSocket(this);
    _tcpClient->connectToHost("192.168.5.222",80);
    if(!_tcpClient->waitForConnected()){
        QMessageBox::critical(this,"连接服务器出错！",_tcpClient->errorString());
        return;
    }
    sendCommand("d");
    qDebug()<<"DDD";
    QTimer::singleShot(500, this, [=]() {
        // 检查连接是否仍然有效
        qDebug()<<"PPPPP";
        if (_tcpClient->state() != QAbstractSocket::ConnectedState) {
            qDebug() << "连接已断开，重新连接...";
            _tcpClient->connectToHost("192.168.5.222", 80);
            if (!_tcpClient->waitForConnected(2000)) {
                qDebug() << "重新连接失败，无法发送停止命令";
                return;
            }
        }
        // 发送停止命令
        sendCommand("s");
        // 发送完成后断开连接
        _tcpClient->disconnectFromHost();
    });
}
void ControlMove::leftMove(){
    _tcpClient = new QTcpSocket(this);
    _tcpClient->connectToHost("192.168.5.222",80);
    if(!_tcpClient->waitForConnected()){
        QMessageBox::critical(this,"连接服务器出错！",_tcpClient->errorString());
        return;
    }
    sendCommand("l");
    qDebug()<<"lllll";
    QTimer::singleShot(500, this, [=]() {
        // 检查连接是否仍然有效
        qDebug()<<"PPPPP";
        if (_tcpClient->state() != QAbstractSocket::ConnectedState) {
            qDebug() << "连接已断开，重新连接...";
            _tcpClient->connectToHost("192.168.5.222", 80);
            if (!_tcpClient->waitForConnected(2000)) {
                qDebug() << "重新连接失败，无法发送停止命令";
                return;
            }
        }
        // 发送停止命令
        sendCommand("s");
        // 发送完成后断开连接
        _tcpClient->disconnectFromHost();
    });
}
void ControlMove::rightMove(){
    _tcpClient = new QTcpSocket(this);
    _tcpClient->connectToHost("192.168.5.222",80);
    if(!_tcpClient->waitForConnected()){
        QMessageBox::critical(this,"连接服务器出错！",_tcpClient->errorString());
        return;
    }
    sendCommand("r");
    qDebug()<<"rrrr";
    QTimer::singleShot(500, this, [=]() {
        // 检查连接是否仍然有效
        qDebug()<<"PPPPP";
        if (_tcpClient->state() != QAbstractSocket::ConnectedState) {
            qDebug() << "连接已断开，重新连接...";
            _tcpClient->connectToHost("192.168.5.222", 80);
            if (!_tcpClient->waitForConnected(2000)) {
                qDebug() << "重新连接失败，无法发送停止命令";
                return;
            }
        }
        // 发送停止命令
        sendCommand("s");
        // 发送完成后断开连接
        _tcpClient->disconnectFromHost();
    });
}
