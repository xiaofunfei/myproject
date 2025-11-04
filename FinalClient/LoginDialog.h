#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H
#include "CryptString.h"

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <qformlayout.h>
#include <QTcpSocket>
#include <string>
#include <iostream>
#include "Message.h"
#include <vector>
#include "GlobalUser.h"
using std::cout;
using std::endl;
using std::string;
using std::vector;
class LoginDialog:public QDialog
{
    Q_OBJECT
public:
    explicit LoginDialog(QWidget * parent=nullptr);
    QString getUsername()const;
    QString getPassword()const;
    int readPacket(Packet &packet);
    int readn(char *buf,int len);
private slots:
    void on_login_button();
    void on_handleRead();
    void on_sendUsername();
signals:
    void sendUser(vector<vector<int>>);
public:
    vector<vector<int>> _userData;
private:
    QLineEdit *         m_usernameEdit;
    QLineEdit *         m_passwordEdit;
    QPushButton *       m_loginButton;
    QPushButton *       m_cancelButton;
    QTcpSocket*         _tcpClient;
    QString             _pwd;
    QString             _name;
    QByteArray          _recvBuffer;
    QString             _setting;

};

#endif // LOGINDIALOG_H
