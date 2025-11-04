#ifndef CLOUDCONTROL_H
#define CLOUDCONTROL_H

#include <QWidget>
#include <QPushButton>
#include "GlobalUser.h"
#include "NetWorkReactor.h"
#include <QMessageBox>
class CloudControl : public QWidget
{
    Q_OBJECT
public:
    explicit CloudControl(QWidget *parent = nullptr);
void setDirctionButtonsConnection();
signals:
    void updateMessage(const QString &time,const QString &message);
private:
    void initUI();
private slots:
    void  on_leftU_clicked();
    void  on_up_clicked();
    void  on_rightU_clicked();
    void  on_left_clicked();
    void  on_Fresh_clicked();//刷新页面
    void  on_right_clicked();
    void  on_leftD_clicked();
    void  on_down_clicked();
    void  on_rightD_clicked();
private:
    QPushButton*  _pPb1;
    QPushButton*  _pPb2;
    QPushButton*  _pPb3;
    QPushButton*  _pPb4;
    QPushButton*  _pPb5;
    QPushButton*  _pPb6;
    QPushButton*  _pPb7;
    QPushButton*  _pPb8;
    QPushButton*  _pPb9;
};

#endif // CLOUDCONTROL_H
