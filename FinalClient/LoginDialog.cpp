#include "LoginDialog.h"
#include <QDebug>
#include <QMessageBox>
#include <errno.h>
#include <QMessageBox>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
std::string constructTLV(const TLV &data){
    int tlvlen=8+data.length;
    std::string msg;
    msg.assign((const char*)&data,tlvlen);
    return msg;
}
LoginDialog::LoginDialog(QWidget *parent)
    :QDialog(parent)
{
    setWindowTitle("用户登录");
    //resize(800,800);
    //阻塞主对话窗口
    setModal(true);
    //实例化socket对象
    _tcpClient = new QTcpSocket(this);
    //与服务器建立连接
    _tcpClient->connectToHost("192.168.253.130",8087);
    if(!_tcpClient->waitForConnected()){
        QMessageBox::critical(this,"连接服务器出错！",_tcpClient->errorString());
        return;
    }
    //服务器连接成功现在是构建一个TLV格式消息
    qDebug()<<"1111";
    QLabel *userlabel=new QLabel("用户名",this);
    m_usernameEdit  = new QLineEdit(this);

    QLabel *pwdlabel=new QLabel("密码",this);
    m_passwordEdit =new QLineEdit(this);
    //密码模式
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_loginButton = new QPushButton("登录",this);
    m_cancelButton = new QPushButton("取消",this);

    //布局
    QFormLayout *formlayout=new QFormLayout(this);
    formlayout->addRow(userlabel,m_usernameEdit);
    formlayout->addRow(pwdlabel,m_passwordEdit);
    formlayout->addRow( m_loginButton,m_cancelButton);
    formlayout->setSpacing(8);
    formlayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
    formlayout->setLabelAlignment(Qt::AlignLeft);
    connect(m_usernameEdit,&QLineEdit::editingFinished,this,&LoginDialog::on_sendUsername);
    connect(m_loginButton,&QPushButton::clicked,this,&LoginDialog::on_login_button);
    connect(m_cancelButton,&QPushButton::clicked,[](){
        qDebug()<<"取消按钮已被按下";
    });
    connect(_tcpClient,&QTcpSocket::readyRead,this,&LoginDialog::on_handleRead);
    connect(m_cancelButton,&QPushButton::clicked,this,&QDialog::reject);
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
/*int LoginDialog::readn(char *buf, int len){
    int left =len;
    char *pbuf=buf;
    while(left>0){
        int ret=_tcpClient->read(pbuf,left);
        if(ret == -1 && errno ==EINTR){
            continue;
        } else if(ret == -1) {
            perror("recv");
            return ret;
        } else if(ret == 0){
            return len - left;
        } else {//ret > 0的情况
            left -= ret;
            pbuf += ret;
        }
    }
    return len;
}*/
int LoginDialog::readPacket(Packet &packet){
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
void LoginDialog::on_login_button(){
    _pwd=m_passwordEdit->text();
    QString data=_setting+_pwd;
    CryptString crypt(data);
    QString encoded=crypt.generateMD5();
    qDebug()<<"在登录时我通过盐值在本地计算的密文"<<encoded;
    encoded=encoded+"@"+_name;
    qDebug()<<"要传输的数据(密码+用户名)"<<encoded;
    //构建TLV字符串
    TLV tlv;
    tlv.type=TASK_LOGIN_SECTION2;
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
void LoginDialog::on_sendUsername(){//在用户输入完用户名之后就把用户名发送出去
    QString username=m_usernameEdit->text();
    _name=username;
    //发送构建一个TVL包
    TLV tlv;
    tlv.type=TASK_LOGIN_SECTION1;
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
void LoginDialog::on_handleRead(){
    qDebug()<<"Login onHandleRead processing!";
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
        case TASK_LOGIN_SECTION1_RESP_OK:{
            //此时收到了ok，数据包里面应该是传过来的盐值，接下来需要将盐值和密码相加在MD5传回去
            _setting=QString::fromStdString(pack.msg);
        }
            break;
        case TASK_LOGIN_SECTION1_RESP_ERROR:{
            //此时收到了失败,代表用户名已经在数据库中有一位了
            qDebug()<<"该用户名不存在，请重新尝试！";
        }
            break;
        case TASK_USER_READY_DATA:{
            //此时代表对面发送过来一个用户加载信息数据，对面传过来的是一个JSON形式的格式串
            QString qJson=QString::fromStdString(pack.msg);
            QJsonDocument jsonDoc=QJsonDocument::fromJson(qJson.toUtf8());
            if(jsonDoc.isNull()){
                qWarning()<<"JSON解析失败！！字符串格式错误";
                return;
            }
            if (!jsonDoc.isObject()) {
                qWarning() << "JSON不是对象类型！";
                return;
            }
            vector<int> tempData;
            QJsonObject jsonObj=jsonDoc.object();
            int cameraId=jsonObj["cameraId"].toInt();
            int channelId=jsonObj["channel"].toInt();
            // 获取全局单例的userCamera
            auto& userCamera = GlobalUser::getInstance()->userCamera;

            // 1. 确保外层容器大小 >= cameraId（避免cameraId-1越界）
            if (userCamera.size() < (size_t)cameraId) {
                userCamera.resize(cameraId); // 扩展外层容器到cameraId大小
            }
            // 2. 确保当前摄像头（cameraId-1）的内层容器大小 >= channelId（避免channelId-1越界）
            auto& channels = userCamera[cameraId - 1]; // 获取当前摄像头的通道列表
            if (channels.size() < (size_t)channelId) {
                  channels.resize(channelId); // 扩展内层容器到channelId大小
             }

                // 3. 安全赋值
             channels[channelId - 1] = channelId;

             qDebug() << "已添加摄像头" << cameraId << "的通道" << channelId;
        }
            break;
        case TASK_LOGIN_SECTION2_RESP_OK:{
            qDebug()<<"登录成功！！！！";
            emit(sendUser(_userData));
            qDebug()<<"信号已发送!";
            QMessageBox *msg = new QMessageBox;
            msg->setText("你已经登录成功，接下来3秒后自动登录进入页面");
            QTimer::singleShot(3000, msg, SLOT(close()));
            msg->exec();
            _tcpClient->disconnectFromHost();
            accept();
            //接下来是对包的处理或者其它处理
        }
            break;
        case TASK_LOGIN_SECTION2_RESP_ERROR:{
            qDebug()<<"注册失败!!!!";
            //接下来是对包的处理或者其它处理
        }
            break;
        }
    }
}
QString LoginDialog::getUsername() const{
    return m_usernameEdit->text();
}
QString LoginDialog::getPassword() const{
    return m_passwordEdit->text();
}
