#include "LeftWidget.h"
#include "PhotoWarning.h"
#include "WindowMessage.h"
#include <QVBoxLayout>
LeftWidget::LeftWidget(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void LeftWidget::initUI(){
    setAttribute(Qt::WA_StyledBackground);
    setWindowFlags(Qt::FramelessWindowHint);
    setStyleSheet("background-color:rgb(55,63,61)");
    //setFixedWidth(250);


    _pTextLabel=new QLabel();
    //_pTextLabel->setFixedSize(350,50);
    _pTextLabel->setText("窗口信息");
    _pTextLabel->setAlignment(Qt::AlignCenter);
    _pTextLabel->setStyleSheet("background-color:rgb(40,47,48);font-size: 30px; color: white;");

    _pTextLabel2=new QLabel();
    //_pTextLabel2->setFixedSize(350,50);
    _pTextLabel2->setText("图文警情");
    _pTextLabel2->setAlignment(Qt::AlignCenter);
    _pTextLabel2->setStyleSheet("background-color:rgb(40,47,48);font-size: 30px; color: white;");

    _pWindowMessage = new WindowMessage();
    //_pWindowMessage->setFixedSize(350,400);

    _pPhotoWarning = new PhotoWarning();
    //_pPhotoWarning->setFixedWidth(350);

    QVBoxLayout * pVLayout=new QVBoxLayout(this);
    pVLayout->addSpacing(10);
    pVLayout->addWidget(_pTextLabel,1);
    pVLayout->addSpacing(10);
    pVLayout->addWidget(_pWindowMessage,4);
    pVLayout->addSpacing(10);
    pVLayout->addWidget(_pTextLabel2,1);
    pVLayout->addSpacing(10);
    pVLayout->addWidget(_pPhotoWarning,4);
    pVLayout->setContentsMargins(0,0,0,0);

}
