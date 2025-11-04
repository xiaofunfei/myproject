#ifndef REGISTER_H
#define REGISTER_H
#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <qformlayout.h>
#include <QTcpSocket>
#include <iostream>
#include <string>
#include "Message.h"
#include "CryptString.h"
using std::cout;
using std::endl;
using std::string;
class Register:public QDialog
{
    Q_OBJECT
public:
    explicit Register(QWidget *parent=nullptr);
    QString getUsername()const;
    QString getPassword()const;
    int readn(char *buff,int len);
    int readPacket(Packet &packet);
private slots:
    void onHandleRead();
    void on_registerclick();
    void on_sendUsername();
private:
    QPushButton * _register;
    QPushButton * _cancel;
    QLineEdit   * _username;
    QLineEdit   * _userpwd;
    QTcpSocket  * _tcpClient;
    QString       _pwd;
    QString       _name;
    QByteArray    _recvBuffer;
    QString       _setting;
};

#endif // REGISTER_H
