#include "LoginInterface.h"
#include <errno.h>
#include "GlobalUser.h"
#include "MainWindow.h"
#include <QHBoxLayout>
#include <QMessageBox>
#include <QTimer>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
//std::string constructTLV(const TLV &data){
//    int tlvlen=8+data.length;
//    std::string msg;
//    msg.assign((const char*)&data,tlvlen);
//    return msg;
//}

LoginInterface::LoginInterface(QWidget *parent) : QWidget(parent)
{
    //实例化socket对象
    _tcpClient = new QTcpSocket(this);
    //与服务器建立连接
    _tcpClient->connectToHost("192.168.253.130",8087);
    if(!_tcpClient->waitForConnected()){
        QMessageBox::critical(this,"连接服务器出错！",_tcpClient->errorString());
        return;
    }
    initUI();
    connectSlots();



}

void LoginInterface::initUI(){
    setWindowIcon(QIcon(":/resources/icons/SOS.jpg"));
    setFixedSize(800,600);
    //resize(800,600);
    //setWindowIcon(QIcon(":resources/icons/logo.png"));
    setWindowTitle("视频监控管理平台");
    QWidget * widget= new QWidget(this);
    QLabel * userName=new QLabel("用户名");
    QLabel * password=new QLabel("密码");
    userName->setStyleSheet("font-size:30px;font-weight:bold;color:white");
    password->setStyleSheet("font-size:30px;font-weight:bold;color:white");
    _usernameEdit =new QLineEdit();
    _passwordEdit =new QLineEdit();
    _passwordEdit->setEchoMode(QLineEdit::Password);
    _loginButton = new QPushButton("登录");
    _registerButton = new QPushButton("没有账号？点此注册");
    _loginButton->setStyleSheet("font-weight:bold;background:");
    _registerButton->setStyleSheet("font-weight:bold;");
    //布局
    QFormLayout *formlayout=new QFormLayout();
    formlayout->addRow(userName,_usernameEdit);
    formlayout->addRow(password,_passwordEdit);
    formlayout->setSpacing(20);
    formlayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
    formlayout->setLabelAlignment(Qt::AlignLeft);
    QHBoxLayout* hlayout=new QHBoxLayout();
    hlayout->addWidget(_loginButton);
    hlayout->addWidget(_registerButton);
    QVBoxLayout *vlayout=new QVBoxLayout(widget);
    vlayout->addLayout(formlayout);
    vlayout->addLayout(hlayout);
    int x = this->width()/2-widget->width()/2;
    int y = this->height()/ 3+10;
    widget->move(x, y);
    QLabel * background= new QLabel(this);
    background->setScaledContents(true); // 自动缩放图片内容
    //background->setStyleSheet("background-color: transparent;");
    //background->setPixmap(QPixmap(":/resources/icons/back2.png"));
    background->lower(); // 置于底层


    QMovie *movie = new QMovie(":/resources/icons/b3.gif");

    // 关联动画和标签
    background->setMovie(movie);

    // 播放动画
    movie->start();

    QObject::connect(movie, &QMovie::finished, [movie]() {
        movie->start();
    });
    QVBoxLayout *vlaout =new QVBoxLayout();
    vlaout->addWidget(background);

}
void LoginInterface::connectSlots(){
    connect(_usernameEdit,&QLineEdit::editingFinished,this,&LoginInterface::on_sendUsername);
    connect(_loginButton,&QPushButton::clicked,this,&LoginInterface::on_login_button);
    connect(_registerButton,&QPushButton::clicked,this,&LoginInterface::on_register_Button);
    connect(_tcpClient,&QTcpSocket::readyRead,this,&LoginInterface::on_handleRead);
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
int LoginInterface::readPacket(Packet &packet){
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
void LoginInterface::on_login_button(){
    _pwd=_passwordEdit->text();
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
void LoginInterface::on_sendUsername(){//在用户输入完用户名之后就把用户名发送出去
    QString username=_usernameEdit->text();
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
void LoginInterface::on_handleRead(){
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
            AnimatedSplashScreen splash(this);
            splash.startAnimation(4000);//2s

            //
            /*QMessageBox *msg = new QMessageBox;
            msg->setText("正在登录...");
            QTimer::singleShot(2000, msg, SLOT(close()));
            msg->exec();*/
            _tcpClient->disconnectFromHost();
            MainWindow *mainwindow=new MainWindow();
            this->close();
            mainwindow->show();
            //accept();
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
//QString LoginInterface::getUsername() const{
//    return m_usernameEdit->text();
//}
//QString LoginInterface::getPassword() const{
//    return m_passwordEdit->text();
//}

void LoginInterface::on_register_Button(){
    _registerlog=new Register(this);
    _registerlog->exec();

}





