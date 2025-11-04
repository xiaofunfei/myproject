#include "RightWidget.h"
#include "DeviceList.h"
#include "CloudControl.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
RightWidget::RightWidget(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void RightWidget::initUI(){
    setAttribute(Qt::WA_StyledBackground);
    setWindowFlags(Qt::FramelessWindowHint);
    setStyleSheet("background-color:rgb(55,63,61)");
    _pDeviceLabel=new QLabel();
    QVBoxLayout *mainLayout=new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    _pDeviceLabel->setText("设备列表");
    _pDeviceLabel->setAlignment(Qt::AlignCenter);
    _pDeviceLabel->setStyleSheet("background-color:rgb(40,47,48);font-size: 30px; color: white;");

    _pCloudLabel=new QLabel();
    _pCloudLabel->setText("云台控制");
    _pCloudLabel->setAlignment(Qt::AlignCenter);
    _pCloudLabel->setStyleSheet("background-color:rgb(40,47,48);font-size: 30px; color: white;");

    _pDeviceList=new DeviceList();
    _pCloudControl= new CloudControl();

    QWidget* widget = new QWidget();
    QVBoxLayout * pVLay=new QVBoxLayout(widget);
    pVLay->addWidget(_pCloudControl);
    pVLay->setContentsMargins(25,0,10,0);

//    QWidget* widget2 = new QWidget();
//    widget2->setFixedWidth(350);
//    QVBoxLayout * pVLayout=new QVBoxLayout(this);
//    pVLayout->addSpacing(10);
//    pVLayout->addWidget(_pDeviceLabel);
//    pVLayout->addSpacing(10);
//    pVLayout->addWidget(_pDeviceList);
//    pVLayout->addSpacing(10);
//    pVLayout->addWidget(_pCloudLabel);
//    pVLayout->addWidget(widget);
//    pVLayout->addSpacing(0);
//    pVLayout->addWidget(widget2);
//    pVLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(_pDeviceLabel,1);
    mainLayout->addWidget(_pDeviceList,4);
    mainLayout->addWidget(_pCloudLabel,1);
    mainLayout->addWidget(_pCloudControl,4);

}










