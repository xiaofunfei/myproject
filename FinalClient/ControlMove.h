#ifndef CONTROLMOVE_H
#define CONTROLMOVE_H
#include"CryptString.h"
#include <QWidget>
#include <QTcpSocket>
class ControlMove:public QWidget
{
    Q_OBJECT
public:
    ControlMove(QWidget *parent =nullptr);
    void sendCommand(string s);
public slots:
    void upMove();
    void leftMove();
    void rightMove();
    void downMove();
private:
    //QString         _ip;
    QTcpSocket*     _tcpClient;
};

#endif // CONTROLMOVE_H
