#ifndef LOGININTERFACE_H
#define LOGININTERFACE_H
#include "RegisterDialog.h"
#include "AnimatedSplashScreen.h"
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
using std::cout;
using std::endl;
using std::string;
using std::vector;
class LoginInterface : public QWidget
{
    Q_OBJECT
public:
    explicit LoginInterface(QWidget *parent = nullptr);
    QString getUsername()const;
    QString getPassword()const;
    int readPacket(Packet &packet);
    int readn(char *buf,int len);
signals:
    void sendUser(vector<vector<int>>);
private slots:
    void on_register_Button();
    void on_login_button();
    void on_handleRead();
    void on_sendUsername();
private:
    void initUI();
    void connectSlots();
public:
    vector<vector<int>> _userData;
private:
    QLineEdit *         _usernameEdit;
    QLineEdit *         _passwordEdit;
    QPushButton*        _loginButton;
    QPushButton*        _registerButton;
    Register*           _registerlog;
    QTcpSocket*         _tcpClient;
    QString             _pwd;
    QString             _name;
    QByteArray          _recvBuffer;
    QString             _setting;
};

#endif // LOGININTERFACE_H
