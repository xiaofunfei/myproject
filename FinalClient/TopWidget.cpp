#include "TopWidget.h"
//#include "CMenuButton.h"
#include <QHBoxLayout>
TopWidget::TopWidget(QWidget *parent) : QWidget(parent)
{
    initUI();
}
void TopWidget::initUI()
{
    //禁止父窗口影响子窗口样式,必须加上，否则样式不会起作用
    setAttribute(Qt::WA_StyledBackground);
    setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedHeight(110);
    this->setStyleSheet("background-color:rgb(43,51,51)");

    _pLogoLabel= new QLabel(this);
    _pLogoLabel->setFixedSize(100,100);
    _pLogoLabel->setStyleSheet("QLabel{background-image:url(:/resources/icons/logo.png);background-position:center;background-repeat: no-repeat;border:none}");

    _pTitleTextLabel= new QLabel(this);
    _pTitleTextLabel->setFixedHeight(100);
    _pTitleTextLabel->setText("视频监控管理平台");
    _pTitleTextLabel->setStyleSheet("font-size: 60px;font-weight: bold; color: white;");

    _pPlayButton= new QPushButton();
    _pPlayButton->setFixedSize(120, 100);
    _pPlayButton->setStyleSheet(
                               "QPushButton{background-image:url(:/resources/icons/play.png);font-size: 30px;font-weight: bold; color: white;text-align: bottom center;background-position:center;background-repeat: no-repeat;border:none}"
                               "QPushButton:hover{"
                               "background-color:rgb(57, 122, 139);"
                               "background-image:url(:/resources/icons/play.png);background-position:center;background-repeat: no-repeat;border:none;}");

    _pReplayButton=new QPushButton();
    _pReplayButton->setFixedSize(120, 100);
    _pReplayButton->setStyleSheet(
                               "QPushButton{background-image:url(:/resources/icons/replay.png);background-position:center;background-repeat: no-repeat;border:none}"
                               "QPushButton:hover{"
                               "background-color:rgb(57, 122, 139);"
                               "background-image:url(:/resources/icons/replay.png);background-position:center;background-repeat: no-repeat;border:none;}");
    _pMapButton=new QPushButton();
    _pMapButton->setFixedSize(120, 100);
    _pMapButton->setStyleSheet(
                               "QPushButton{background-image:url(:/resources/icons/map.png);background-position:center;background-repeat: no-repeat;border:none}"
                               "QPushButton:hover{"
                               "background-color:rgb(57, 122, 139);"
                               "background-image:url(:/resources/icons/map.png);background-position:center;background-repeat: no-repeat;border:none;}");
    _pLogButton=new QPushButton();
    _pLogButton->setFixedSize(120, 100);
    _pLogButton->setStyleSheet(
                               "QPushButton{background-image:url(:/resources/icons/log.png);background-position:center;background-repeat: no-repeat;border:none}"
                               "QPushButton:hover{"
                               "background-color:rgb(57, 122, 139);"
                               "background-image:url(:/resources/icons/log.png);background-position:center;background-repeat: no-repeat;border:none;}");

    _pSetButton = new QPushButton(this);
    _pSetButton->setFixedSize(120, 100);
    _pSetButton->setStyleSheet(
                               "QPushButton{background-image:url(:/resources/icons/set.png);background-position:center;background-repeat: no-repeat;border:none}"
                               "QPushButton:hover{"
                               "background-color:rgb(57, 122, 139);"
                               "background-image:url(:/resources/icons/set.png);background-position:center;background-repeat: no-repeat;border:none;}");
    _pSetButton->setToolTip(tr("设置"));

    _pMinButton = new QPushButton(this);
    _pMinButton->setFixedSize(120, 100);
    _pMinButton->setStyleSheet(
                               "QPushButton{background-image:url(:/resources/titlebar/min.svg);background-position:center;background-repeat: no-repeat;border:none}"
                               "QPushButton:hover{"
                               "background-color:rgb(57, 122, 139);"
                               "background-image:url(:/resources/titlebar/min_hover.svg);background-position:center;background-repeat: no-repeat;border:none;}");
    _pMinButton->setToolTip(tr("最小化"));
    _pMaxButton = new QPushButton(this);
    _pMaxButton->setFixedSize(120, 100);
    _pMaxButton->setStyleSheet(
                               "QPushButton{background-image:url(:/resources/titlebar/max.svg);background-position:center;background-repeat: no-repeat;border:none}"
                               "QPushButton:hover{"
                               "background-color:rgb(57, 122, 139);"
                               "background-image:url(:/resources/titlebar/max_hover.svg);background-position:center;background-repeat: no-repeat;border:none;}");
    _pMaxButton->setToolTip(tr("最大化"));
    _pCloseButton = new QPushButton(this);
    _pCloseButton->setFixedSize(120, 100);
    _pCloseButton->setStyleSheet(
                                 "QPushButton{background-image:url(:/resources/titlebar/close.svg);background-position:center;background-repeat: no-repeat;border:none}"
                                 "QPushButton:hover{"
                                 "background-color:rgb(57, 122, 139);"
                                 "background-image:url(:/resources/titlebar/close_hover.svg);background-position:center;background-repeat: no-repeat;border:none;}");
    _pCloseButton->setToolTip(tr("关闭"));

    QHBoxLayout * pHLayout = new QHBoxLayout(this);
    pHLayout->addWidget(_pLogoLabel);
    pHLayout->addWidget(_pTitleTextLabel);
    pHLayout->addStretch();//添加一个弹簧
    pHLayout->addWidget(_pPlayButton);
    pHLayout->addWidget(_pReplayButton);
    pHLayout->addWidget(_pMapButton);
    pHLayout->addWidget(_pLogButton);
    pHLayout->addWidget(_pSetButton);
    pHLayout->addWidget(_pMinButton);
    pHLayout->addWidget(_pMaxButton);
    pHLayout->addWidget(_pCloseButton);
    pHLayout->setContentsMargins(15,5,10,5);
    setLayout(pHLayout);
    connect(_pMinButton, &QPushButton::clicked, this,
            &TopWidget::onClickedSlot);
    connect(_pMaxButton, &QPushButton::clicked, this,
            &TopWidget::onClickedSlot);
    connect(_pCloseButton, &QPushButton::clicked, this,
            &TopWidget::onClickedSlot);
}

void TopWidget::onClickedSlot()
{
    QPushButton * pbtn = qobject_cast<QPushButton*>(sender());
    QWidget * pwindow = this->window();
    if(pbtn == _pMinButton) {
        pwindow->showMinimized();
    } else if(pbtn == _pMaxButton) {
        if(pwindow->isMaximized()) {
            pwindow->showNormal();
            _pMaxButton->setStyleSheet(
                                       "QPushButton{background-image:url(:/resources/titlebar/normal.svg);background-position:center;background-repeat: no-repeat;border:none}"
                                       "QPushButton:hover{"
                                       "background-color:rgb(99, 99, 99);"
                                       "background-image:url(:/resources/titlebar/normal_hover.svg);background-position:center;background-repeat: no-repeat;border:none;}");
        } else {
            pwindow->showMaximized();
            _pMaxButton->setStyleSheet(
                                       "QPushButton{background-image:url(:/resources/titlebar/max.svg);background-position:center;background-repeat: no-repeat;border:none}"
                                       "QPushButton:hover{"
                                       "background-color:rgb(99, 99, 99);"
                                       "background-image:url(:/resources/titlebar/max_hover.svg);background-position:center;background-repeat: no-repeat;border:none;}");
        }
    } else if(pbtn == _pCloseButton) {
        emit sigClose();//发射信号，通知父窗口关闭
    }
}
