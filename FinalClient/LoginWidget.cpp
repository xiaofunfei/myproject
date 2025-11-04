#include "LoginWidget.h"
#include <QMessageBox>
#include <QTimer>
LoginWidget::LoginWidget(QWidget *parent)
    :QWidget(parent)
{
    resize(800,800);
    _login=new QPushButton("登录",this);
    _register=new QPushButton("注册",this);
    QHBoxLayout* main=new QHBoxLayout;
    main->addWidget(_login);
    main->addWidget(_register);
    setLayout(main);
    connect(_login,&QPushButton::clicked,this,&LoginWidget::login_Button);
    connect(_register,&QPushButton::clicked,this,&LoginWidget::register_Button);
}
void LoginWidget::login_Button(){
    _logindialog=new LoginDialog(this);
    connect(_logindialog,&LoginDialog::sendUser,this,&LoginWidget::recvUserData,Qt::QueuedConnection);
    int ret=_logindialog->exec();
    if(ret == QDialog::Accepted){
        //这个地方可以用上面的_logindialog返回用户名和密码
        _logindialog->accept();
        MainWindow *mainwindow=new MainWindow();
        this->close();
        mainwindow->show();
    }
}
void LoginWidget::register_Button(){
    _registerlog=new Register(this);
    int ret=_registerlog->exec();
    if(ret == QDialog::Accepted){
        //注册成功等待登录
        QMessageBox *msg = new QMessageBox;
        msg->setText("你已经注册成功，接下来请登录,3秒后自动关闭");
        QTimer::singleShot(3000, msg, SLOT(close()));
        msg->exec();
    }
}
void LoginWidget::recvUserData(vector<vector<int>> data){
    qDebug()<<"槽函数已触发";
    _user=data;
}
