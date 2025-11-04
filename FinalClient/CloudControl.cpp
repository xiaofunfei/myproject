#include "CloudControl.h"
#include <QGridLayout>
#include <string>
#include <QDateTime>
using std::string;
CloudControl::CloudControl(QWidget *parent) : QWidget(parent)
{
    initUI();
    this->setDirctionButtonsConnection();
}

void CloudControl::initUI(){
    setAttribute(Qt::WA_StyledBackground);
    //QWidget* widget = new QWidget(this);
    _pPb1 = new QPushButton();
    //_pPb1->setFixedSize(100,100);
    _pPb1->setStyleSheet("QPushButton{border-image:url(:/resources/icons/pb1.png);background-color:rgb(55,63,61);background-position:center;background-repeat: no-repeat;border:none;}"
                         "QPushButton:hover{image:url(:/resources/icons/pb1.png);padding: 4px;}"
                         "QPushButton:pressed{image:url(:/resources/icons/pb1.png);padding: 8px;}"
                         );
    _pPb1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _pPb2 = new QPushButton();
    //_pPb2->setFixedSize(100,100);
    _pPb2->setStyleSheet("QPushButton{border-image:url(:/resources/icons/pb2.png);background-color:rgb(55,63,61);background-position:center;background-repeat: no-repeat;border:none;}"
                         "QPushButton:hover{image:url(:/resources/icons/pb2.png);padding: 4px;}"
                         "QPushButton:pressed{image:url(:/resources/icons/pb2.png);padding: 8px;}"
                         );
    _pPb2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _pPb3 = new QPushButton();
    //_pPb3->setFixedSize(100,100);
    _pPb3->setStyleSheet("QPushButton{border-image:url(:/resources/icons/pb3.png);background-color:rgb(55,63,61);background-position:center;background-repeat: no-repeat;border:none;}"
                         "QPushButton:hover{image:url(:/resources/icons/pb3.png);padding: 4px;}"
                         "QPushButton:pressed{image:url(:/resources/icons/pb3.png);padding: 8px;}"
                         );
    _pPb3->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _pPb4 = new QPushButton();
    //_pPb4->setFixedSize(100,100);
    _pPb4->setStyleSheet("QPushButton{border-image:url(:/resources/icons/pb4.png);background-color:rgb(55,63,61);background-position:center;background-repeat: no-repeat;border:none;}"
                         "QPushButton:hover{image:url(:/resources/icons/pb4.png);padding: 4px;}"
                         "QPushButton:pressed{image:url(:/resources/icons/pb4.png);padding: 8px;}"
                         );
    _pPb4->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _pPb5 = new QPushButton();
    //_pPb5->setFixedSize(100,100);
    _pPb5->setStyleSheet("QPushButton{border-image:url(:/resources/icons/pb5.png);background-color:rgb(55,63,61);background-position:center;background-repeat: no-repeat;border:none;}"
                         "QPushButton:hover{image:url(:/resources/icons/pb5.png);padding: 4px;}"
                         "QPushButton:pressed{image:url(:/resources/icons/pb5.png);padding: 8px;}"
                         );
    _pPb5->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _pPb6 = new QPushButton();
    //_pPb6->setFixedSize(100,100);
    _pPb6->setStyleSheet("QPushButton{border-image:url(:/resources/icons/pb6.png);background-color:rgb(55,63,61);background-position:center;background-repeat: no-repeat;border:none;}"
                         "QPushButton:hover{image:url(:/resources/icons/pb6.png);padding: 4px;}"
                         "QPushButton:pressed{image:url(:/resources/icons/pb6.png);padding: 8px;}"
                         );
    _pPb6->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _pPb7 = new QPushButton();
    //_pPb7->setFixedSize(100,100);
    _pPb7->setStyleSheet("QPushButton{border-image:url(:/resources/icons/pb7.png);background-color:rgb(55,63,61);background-position:center;background-repeat: no-repeat;border:none;}"
                         "QPushButton:hover{image:url(:/resources/icons/pb7.png);padding: 4px;}"
                         "QPushButton:pressed{image:url(:/resources/icons/pb7.png);padding: 8px;}"
                         );
    _pPb7->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _pPb8 = new QPushButton();
    //_pPb8->setFixedSize(100,100);
    _pPb8->setStyleSheet("QPushButton{border-image:url(:/resources/icons/pb8.png);background-color:rgb(55,63,61);background-position:center;background-repeat: no-repeat;border:none;}"
                         "QPushButton:hover{image:url(:/resources/icons/pb8.png);padding: 4px;}"
                         "QPushButton:pressed{image:url(:/resources/icons/pb8.png);padding: 8px;}"
                         );
    _pPb8->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _pPb9 = new QPushButton();
    //_pPb9->setFixedSize(100,100);
    _pPb9->setStyleSheet("QPushButton{border-image:url(:/resources/icons/pb9.png);background-color:rgb(55,63,61);background-position:center;background-repeat: no-repeat;border:none;}"
                         "QPushButton:hover{image:url(:/resources/icons/pb9.png);padding: 4px;}"
                         "QPushButton:pressed{image:url(:/resources/icons/pb9.png);padding: 8px;}"
                         );
    _pPb9->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QGridLayout * pGridLayout= new QGridLayout(this);
    pGridLayout->setColumnStretch(0, 1);
    pGridLayout->setColumnStretch(1, 1);
    pGridLayout->setColumnStretch(2, 1);

    // 设置行拉伸因子（3行平均分配高度）
    pGridLayout->setRowStretch(0, 1);
    pGridLayout->setRowStretch(1, 1);
    pGridLayout->setRowStretch(2, 1);
    pGridLayout->addWidget(_pPb1,0,0);
    pGridLayout->addWidget(_pPb2,0,1);
    pGridLayout->addWidget(_pPb3,0,2);
    pGridLayout->addWidget(_pPb4,1,0);
    pGridLayout->addWidget(_pPb5,1,1);
    pGridLayout->addWidget(_pPb6,1,2);
    pGridLayout->addWidget(_pPb7,2,0);
    pGridLayout->addWidget(_pPb8,2,1);
    pGridLayout->addWidget(_pPb9,2,2);
    pGridLayout->setSpacing(0);
    pGridLayout->setContentsMargins(0,0,0,0);

}

void  CloudControl::on_leftU_clicked(){
     qDebug()<<"left is process!!!!!!!";
     NetWorkReactor &reactor = NetWorkReactor::instance();
     GlobalUser *guser = GlobalUser::getInstance();
     QDateTime currentDateTime = QDateTime::currentDateTime();
     QString currentDateTimeString = currentDateTime.toString("hh:mm:ss");
     const QString time(currentDateTimeString);
      qDebug()<<"CurrentCamId is"<<guser->CurrentCamId<<" CurrentChaid is "<<guser->CurrentChaId;
     if(guser->CurrentCamId == -1 || guser->CurrentChaId==-1){
         QMessageBox msg(QMessageBox::Critical,"错误","为选取要控制的摄像头，请通过点击右侧列表的通道来确定选取的摄像头");
         msg.exec();
         QString msg1("左上按钮按下,摄像头控制失败！");
         emit updateMessage(time,msg1);
         return;
     }
     QString cameraId = QString::number(guser->CurrentCamId+1);
     QString channelID = QString::number(guser->CurrentChaId);
     qDebug()<<"CurrentCamId is"<<cameraId<<" CurrentChaid is "<<channelID;
     //QString cameraId="1";
     //QString channelID="1";
     TLV tlv;
     QString input = "$" + cameraId +"$" + channelID + "$1$65$"; //第一部分代表方向，第二部分代表速度
     QByteArray temp = input.toUtf8();
     qDebug()<<"input data is"<<input;
     tlv.length = input.size();
     if(tlv.length > 1023 ) tlv.length = 1023;
     strncpy(tlv.data,temp.constData(),tlv.length);
     tlv.type = TASK_TYPE_HTTP_REQUEST_TURN;
     //tlv.length = 8 + sizeof(tlv.data);
     reactor.sendCommand(tlv);
     QString msg2("左上按钮按下,摄像头控制成功！");
     emit updateMessage(time,msg2);
}

void  CloudControl::on_up_clicked(){
    NetWorkReactor &reactor = NetWorkReactor::instance();
    GlobalUser *guser = GlobalUser::getInstance();
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString currentDateTimeString = currentDateTime.toString("hh:mm:ss");
    const QString time(currentDateTimeString);
     qDebug()<<"CurrentCamId is"<<guser->CurrentCamId<<" CurrentChaid is "<<guser->CurrentChaId;
    if(guser->CurrentCamId == -1 || guser->CurrentChaId==-1){
        QMessageBox msg(QMessageBox::Critical,"错误","为选取要控制的摄像头，请通过点击右侧列表的通道来确定选取的摄像头");
        msg.exec();
        QString msg1("向上按钮按下,摄像头控制失败！");
        emit updateMessage(time,msg1);
        return;
    }
    QString cameraId = QString::number(guser->CurrentCamId+1);
    QString channelID = QString::number(guser->CurrentChaId);

    TLV tlv;
    QString input = "$" + cameraId +"$" + channelID + "$u$65$"; //第一部分代表方向，第二部分代表速度
    QByteArray temp = input.toUtf8();
    tlv.length = input.size();
    if(tlv.length > 1023 ) tlv.length = 1023;
    strncpy(tlv.data,temp.constData(),tlv.length);
    tlv.type = TASK_TYPE_HTTP_REQUEST_TURN;
    //tlv.length = 8 + sizeof(tlv.data);
    reactor.sendCommand(tlv);
    QString msg2("向上按钮已经按下！");
    emit updateMessage(time,msg2);
}

void  CloudControl::on_rightU_clicked(){
    NetWorkReactor &reactor = NetWorkReactor::instance();
    GlobalUser *guser = GlobalUser::getInstance();
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString currentDateTimeString = currentDateTime.toString("hh:mm:ss");
    const QString time(currentDateTimeString);
     qDebug()<<"CurrentCamId is"<<guser->CurrentCamId<<" CurrentChaid is "<<guser->CurrentChaId;
    if(guser->CurrentCamId == -1 || guser->CurrentChaId==-1){
        QMessageBox msg(QMessageBox::Critical,"错误","为选取要控制的摄像头，请通过点击右侧列表的通道来确定选取的摄像头");
        msg.exec();
        QString msg1("右上按钮按下,摄像头控制失败！");
        emit updateMessage(time,msg1);
        return;
    }
    QString cameraId = QString::number(guser->CurrentCamId+1);
    QString channelID = QString::number(guser->CurrentChaId);

    TLV tlv;
    QString input = "$" + cameraId +"$" + channelID + "$2$65$"; //第一部分代表方向，第二部分代表速度
    QByteArray temp = input.toUtf8();
    tlv.length = input.size();
    if(tlv.length > 1023 ) tlv.length = 1023;
    strncpy(tlv.data,temp.constData(),tlv.length);
    tlv.type = TASK_TYPE_HTTP_REQUEST_TURN;
    //tlv.length = 8 + sizeof(tlv.data);
    reactor.sendCommand(tlv);
    QString msg2("右上按钮已经按下！");
    emit updateMessage(time,msg2);
}

void  CloudControl::on_left_clicked(){
    NetWorkReactor &reactor = NetWorkReactor::instance();
    GlobalUser *guser = GlobalUser::getInstance();
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString currentDateTimeString = currentDateTime.toString("hh:mm:ss");
    const QString time(currentDateTimeString);
     qDebug()<<"CurrentCamId is"<<guser->CurrentCamId<<" CurrentChaid is "<<guser->CurrentChaId;
    if(guser->CurrentCamId == -1 || guser->CurrentChaId==-1){
        QMessageBox msg(QMessageBox::Critical,"错误","为选取要控制的摄像头，请通过点击右侧列表的通道来确定选取的摄像头");
        msg.exec();
        QString msg1("向左按钮按下,摄像头控制失败！");
        emit updateMessage(time,msg1);
        return;
    }
    QString cameraId = QString::number(guser->CurrentCamId+1);
    QString channelID = QString::number(guser->CurrentChaId);

    TLV tlv;
    QString input = "$" + cameraId +"$" + channelID + "$l$65$"; //第一部分代表方向，第二部分代表速度
    QByteArray temp = input.toUtf8();
    tlv.length = input.size();
    if(tlv.length > 1023 ) tlv.length = 1023;
    strncpy(tlv.data,temp.constData(),tlv.length);
    tlv.type = TASK_TYPE_HTTP_REQUEST_TURN;
    //tlv.length = 8 + sizeof(tlv.data);
    reactor.sendCommand(tlv);
    QString msg2("向左按钮已经按下！");
    emit updateMessage(time,msg2);
}
//todo:fresh
void  CloudControl::on_Fresh_clicked(){

}

void  CloudControl::on_right_clicked(){

    NetWorkReactor &reactor = NetWorkReactor::instance();
    GlobalUser *guser = GlobalUser::getInstance();
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString currentDateTimeString = currentDateTime.toString("hh:mm:ss");
    const QString time(currentDateTimeString);
     qDebug()<<"CurrentCamId is"<<guser->CurrentCamId<<" CurrentChaid is "<<guser->CurrentChaId;
    if(guser->CurrentCamId == -1 || guser->CurrentChaId==-1){
        QMessageBox msg(QMessageBox::Critical,"错误","为选取要控制的摄像头，请通过点击右侧列表的通道来确定选取的摄像头");
        msg.exec();
        QString msg1("向右按钮按下,摄像头控制失败！");
        emit updateMessage(time,msg1);
        return;
    }
    QString cameraId = QString::number(guser->CurrentCamId+1);
    QString channelID = QString::number(guser->CurrentChaId);

    TLV tlv;
    QString input = "$" + cameraId +"$" + channelID + "$r$65$"; //第一部分代表方向，第二部分代表速度
    QByteArray temp = input.toUtf8();
    tlv.length = input.size();
    if(tlv.length > 1023 ) tlv.length = 1023;
    strncpy(tlv.data,temp.constData(),tlv.length);
    tlv.type = TASK_TYPE_HTTP_REQUEST_TURN;
    //tlv.length = 8 + sizeof(tlv.data);
    reactor.sendCommand(tlv);
    QString msg2("向右按钮已经按下！");
    emit updateMessage(time,msg2);
}

void  CloudControl::on_leftD_clicked(){

    NetWorkReactor &reactor = NetWorkReactor::instance();
    GlobalUser *guser = GlobalUser::getInstance();
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString currentDateTimeString = currentDateTime.toString("hh:mm:ss");
    const QString time(currentDateTimeString);
     qDebug()<<"CurrentCamId is"<<guser->CurrentCamId<<" CurrentChaid is "<<guser->CurrentChaId;
    if(guser->CurrentCamId == -1 || guser->CurrentChaId==-1){
        QMessageBox msg(QMessageBox::Critical,"错误","为选取要控制的摄像头，请通过点击右侧列表的通道来确定选取的摄像头");
        msg.exec();
        QString msg1("左下按钮按下,摄像头控制失败！");
        emit updateMessage(time,msg1);
        return;
    }
    QString cameraId = QString::number(guser->CurrentCamId+1);
    QString channelID = QString::number(guser->CurrentChaId);

    TLV tlv;
    QString input = "$" + cameraId +"$" + channelID + "$3$65$"; //第一部分代表方向，第二部分代表速度
    QByteArray temp = input.toUtf8();
    tlv.length = input.size();
    if(tlv.length > 1023 ) tlv.length = 1023;
    strncpy(tlv.data,temp.constData(),tlv.length);
    tlv.type = TASK_TYPE_HTTP_REQUEST_TURN;
    //tlv.length = 8 + sizeof(tlv.data);
    reactor.sendCommand(tlv);
    QString msg2("左下按钮已经按下！");
    emit updateMessage(time,msg2);
}

void  CloudControl::on_down_clicked(){
    NetWorkReactor &reactor = NetWorkReactor::instance();
    GlobalUser *guser = GlobalUser::getInstance();
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString currentDateTimeString = currentDateTime.toString("hh:mm:ss");
    const QString time(currentDateTimeString);
     qDebug()<<"CurrentCamId is"<<guser->CurrentCamId<<" CurrentChaid is "<<guser->CurrentChaId;
    if(guser->CurrentCamId == -1 || guser->CurrentChaId==-1){
        QMessageBox msg(QMessageBox::Critical,"错误","为选取要控制的摄像头，请通过点击右侧列表的通道来确定选取的摄像头");
        msg.exec();
        QString msg1("向下按钮按下,摄像头控制失败！");
        emit updateMessage(time,msg1);
        return;
    }
    QString cameraId = QString::number(guser->CurrentCamId+1);
    QString channelID = QString::number(guser->CurrentChaId);

    TLV tlv;
    QString input = "$" + cameraId +"$" + channelID + "$d$65$"; //第一部分代表方向，第二部分代表速度
    QByteArray temp = input.toUtf8();
    tlv.length = input.size();
    if(tlv.length > 1023 ) tlv.length = 1023;
    strncpy(tlv.data,temp.constData(),tlv.length);
    tlv.type = TASK_TYPE_HTTP_REQUEST_TURN;
    //tlv.length = 8 + sizeof(tlv.data);
    reactor.sendCommand(tlv);
    QString msg2("向下按钮已经按下！");
    emit updateMessage(time,msg2);
}

void  CloudControl::on_rightD_clicked(){

    NetWorkReactor &reactor = NetWorkReactor::instance();
    GlobalUser *guser = GlobalUser::getInstance();
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString currentDateTimeString = currentDateTime.toString("hh:mm:ss");
    const QString time(currentDateTimeString);
     qDebug()<<"CurrentCamId is"<<guser->CurrentCamId<<" CurrentChaid is "<<guser->CurrentChaId;
    if(guser->CurrentCamId == -1 || guser->CurrentChaId==-1){
        QMessageBox msg(QMessageBox::Critical,"错误","为选取要控制的摄像头，请通过点击右侧列表的通道来确定选取的摄像头");
        msg.exec();
        QString msg1("右下按钮按下,摄像头控制失败！");
        emit updateMessage(time,msg1);
        return;
    }
    QString cameraId = QString::number(guser->CurrentCamId+1);
    QString channelID = QString::number(guser->CurrentChaId);

    TLV tlv;
    QString input = "$" + cameraId +"$" + channelID + "$4$65$"; //第一部分代表方向，第二部分代表速度
    QByteArray temp = input.toUtf8();
    tlv.length = input.size();
    if(tlv.length > 1023 ) tlv.length = 1023;
    strncpy(tlv.data,temp.constData(),tlv.length);
    tlv.type = TASK_TYPE_HTTP_REQUEST_TURN;
    //tlv.length = 8 + sizeof(tlv.data);
    reactor.sendCommand(tlv);
    QString msg2("右下按钮已经按下！");
    emit updateMessage(time,msg2);
}

//方向键连接槽
void CloudControl::setDirctionButtonsConnection(){
    connect(_pPb1,&QPushButton::clicked,this,&CloudControl::on_leftU_clicked);
    connect(_pPb2,&QPushButton::clicked,this,&CloudControl::on_up_clicked);
    connect(_pPb3,&QPushButton::clicked,this,&CloudControl::on_rightU_clicked);
    connect(_pPb4,&QPushButton::clicked,this,&CloudControl::on_left_clicked);
    connect(_pPb5,&QPushButton::clicked,this,&CloudControl::on_Fresh_clicked);
    connect(_pPb6,&QPushButton::clicked,this,&CloudControl::on_right_clicked);
    connect(_pPb7,&QPushButton::clicked,this,&CloudControl::on_leftD_clicked);
    connect(_pPb8,&QPushButton::clicked,this,&CloudControl::on_down_clicked);
    connect(_pPb9,&QPushButton::clicked,this,&CloudControl::on_rightD_clicked);

}
