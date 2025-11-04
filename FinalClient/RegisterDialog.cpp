#include "RegisterDialog.h"
#include <QDebug>
#include <QMessageBox>
#include <QtEndian>
#include <cstdint>
#include <QTimer>
Register::Register(QWidget *parent)
    :QDialog(parent)
{
    setWindowTitle("用户注册");
    setModal(true);
    _tcpClient = new QTcpSocket(this);
    //与服务器建立连接
    _tcpClient->connectToHost("192.168.253.130",8087);

    //处理服务器反应的响应
    /*connect(_tcpClient,&QTcpSocket::readyRead,this,[=](){
        //读取当前接收缓冲区中的所有数据
        QString response=_tcpClient->readAll();
        qDebug()<<response;
    });*/
    if(!_tcpClient->waitForConnected()){
        QMessageBox::critical(this,"连接服务器出错！",_tcpClient->errorString());
        return;
    }
    qDebug()<<"TcpSocket connect success!";
    QLabel *userlabel=new QLabel("用户名",this);
    _username=new QLineEdit(this);

    QLabel *pwdlabel=new QLabel("密码",this);
    _userpwd =new QLineEdit(this);
    _userpwd->setEchoMode(QLineEdit::Password);
    _register =new QPushButton("注册",this);
    _cancel   =new QPushButton("取消",this);
    //注册布局
    QFormLayout *formlayout=new QFormLayout(this);
    formlayout->addRow(userlabel,_username);
    formlayout->addRow(pwdlabel,_userpwd);
    formlayout->addRow( _register,_cancel);
    formlayout->setSpacing(8);
    formlayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
    formlayout->setLabelAlignment(Qt::AlignLeft);
    connect(_tcpClient,&QTcpSocket::readyRead,this,&Register::onHandleRead);
    connect(_register,&QPushButton::clicked,this,&Register::on_registerclick);
    connect(_username,&QLineEdit::editingFinished,this,&Register::on_sendUsername);
    connect(_cancel,&QPushButton::clicked,this,&QDialog::reject);
    connect(_cancel,&QPushButton::clicked,[](){qDebug()<<"取消按钮已被按下";});
}
static string TLVtoString(const TLV&data){
    uint32_t netType=data.type;
    uint32_t netLength=data.length;
    string msg;
    msg.reserve(8+data.length);
    msg.append((const char*)&netType,4);
    msg.append((const char*)&netLength,4);
    msg.append(data.data,data.length);
    return msg;
}
int Register::readn(char *buff,int len){
    int left = len;
    char * pbuf = buff;
    while(left > 0) {
        int ret = _tcpClient->read(pbuf,len);
        if(ret == -1 && errno == EINTR) {
            continue;
        } else if(ret == -1) {
            perror("read");
            return ret;
        } else if(ret == 0){
            return len - left;
        } else {//ret > 0的情况
            left -= ret;
            pbuf += ret;
        }
    }
    return len;
}
int Register::readPacket(Packet &packet){
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
void Register::on_registerclick(){
    //点击注册按钮，要将加密的数据发送给Server
    _pwd=_userpwd->text();
    QString data=_setting+_pwd;
    CryptString code(data);
    QString encoded=code.generateMD5();
    qDebug()<<"加密后的密码为 : "<<encoded;
    //密码已经加密，接下来构建TLV格式发送回去，进入section2
    //因为要解决此处只穿了加密报文而没有传输用户名的问题，因此我在此处后面再加一个以@为分界符后面均为用户名的格式数据
    encoded=encoded+"@"+_name;
    qDebug()<<"要传输的数据(密码+用户名)"<<encoded;
    TLV tlv;
    tlv.type=TASK_REGISTER_SECTION2;
    QByteArray utf8Data=encoded.toUtf8();
    tlv.length=utf8Data.size();
    if(tlv.length>1023) tlv.length=1023;
    strncpy(tlv.data,utf8Data.constData(),tlv.length);
    string msg=TLVtoString(tlv);
    //这个地方已经完成了密码的构造所以将此处的密文发送
    _tcpClient->write(msg.data(),msg.size());
    _tcpClient->flush();
    qDebug()<<"加密数据已发送，接下来准备进入section2的reply";
}
void Register::on_sendUsername(){
    QString username=_username->text();
    _name=username;
    //发送构建一个TVL包
    TLV tlv;
    tlv.type=TASK_REGISTER_SECTION1;
    QByteArray utf8Data=username.toUtf8();
    tlv.length=utf8Data.size();
    //防止越界
    if(tlv.length>1023) tlv.length=1023;
    strncpy(tlv.data,utf8Data.constData(),tlv.length);
    string msg=TLVtoString(tlv);
    //TLV数据构建完成现在负责发送
    _tcpClient->write(msg.data(),msg.size());
    _tcpClient->flush();
}
void Register::onHandleRead(){
    qDebug()<<"onHandleRead processing!";
    //先把所有数据读入缓存
    _recvBuffer.append(_tcpClient->readAll());
    while(true){
        Packet pack;
        int ret=readPacket(pack);
        if(ret==0){
            //数据不足，准备退出
            break;
        }
        cout<<"read : "<<ret<<"bytes\n"<<endl
            <<"packet.length : "<<pack.length<<endl
            <<"packet.data : "<<pack.msg<<endl;
        switch(pack.type){
        case TASK_REGISTER_SECTION1_RESP_OK:{
            //此时收到了ok，数据包里面应该是传过来的盐值，接下来需要将盐值和密码相加在MD5传回去
            _setting=QString::fromStdString(pack.msg);
        }
            break;
        case TASK_REGISTER_SECTION1_RESP_ERROR:{
            //此时收到了失败,代表用户名已经在数据库中有一位了
            qDebug()<<"该用户已被创建，请重新尝试！";
        }
            break;
        case TASK_REGISTER_SECTION2_RESP_OK:{
            qDebug()<<"注册成功！！！！";
            QMessageBox *msg = new QMessageBox;
            msg->setText("你已经注册成功，接下来2秒自动关闭这个窗口，并且接下来请登录用户");
            QTimer::singleShot(2000, msg, SLOT(close()));
            msg->exec();
            _tcpClient->disconnectFromHost();
            accept();

            //接下来是对包的处理或者其它处理
        }
            break;
        case TASK_REGISTER_SECTION2_RESP_ERROR:{
            qDebug()<<"注册失败!!!!";
            //接下来是对包的处理或者其它处理
        }
            break;
        }
    }
}
QString Register::getPassword() const{
    return _userpwd->text();
}
QString Register::getUsername() const{
    return _username->text();
}
