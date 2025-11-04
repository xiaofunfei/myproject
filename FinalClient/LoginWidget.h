#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include "GlobalUser.h"
#include "LoginDialog.h"
#include "RegisterDialog.h"
#include "MainWindow.h"
class LoginWidget:public QWidget
{
    Q_OBJECT
public:
    explicit LoginWidget(QWidget *parent=nullptr);
    ~LoginWidget(){}
private slots:
    void login_Button();
    void register_Button();
    void recvUserData(vector<vector<int>>);
signals:
    void login_Success();
private:
    QPushButton* _login;
    QPushButton* _register;
    LoginDialog* _logindialog;
    Register*    _registerlog;
    vector<vector<int>> _user;
};

#endif // LOGINWIDGET_H
