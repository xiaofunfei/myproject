#include "MainWindow.h"
#include "TopWidget.h"
#include "LeftWidget.h"
#include "CentralWidget.h"
#include <QScreen>
#include "Message.h"
#include "CloudControl.h"
#include "WindowMessage.h"
#include "NetWorkReactor.h"
#include "RightWidget.h"
#include "DeviceList.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <GlobalUser.h>
#include <QDebug>
#include <QMessageBox>
#include <QKeyEvent>
MainWindow::MainWindow(QWidget *parent)
    :CFramelessWidget(parent)
{
    initUI();
    connect(_pTopWidget, &TopWidget::sigClose, this, &MainWindow::closeSlot);
    connect(_pCentralWidget,&CentralWidget::sigFullScreen,this,&MainWindow::onFullScreen);
}

void MainWindow::initUI(){
    setWindowIcon(QIcon(":/resources/icons/SOS.jpg"));
    resize(800,600);
    auto user=GlobalUser::getInstance();
    user->CurrentWindow=1;
    user->CurrentFlag=0;
    user->CurrentCamId=-1;
    user->CurrentChaId=-1;
    user->OldCamId=-1;
    user->OldChaId=-1;
    qDebug()<<"已经进入到了MainWindow的initUi当中";
    for (size_t i = 0; i <GlobalUser::getInstance()->userCamera.size(); ++i) {  // 外层：每个摄像头
            // 摄像头ID通常是索引+1（因为存储时用了cameraId-1）
            int cameraId = i + 1;
            // 跳过没有通道的摄像头（如果有）
            if (GlobalUser::getInstance()->userCamera[i].empty()) {
                qDebug() << "摄像头" << cameraId << "：无通道";
                continue;
            }
            // 内层：该摄像头的所有通道
            QString channels;
            for (size_t j = 0; j < GlobalUser::getInstance()->userCamera[i].size(); ++j) {
                channels += QString::number(GlobalUser::getInstance()->userCamera[i][j]) + " ";
            }
            qDebug() << "摄像头" << cameraId << "的通道：" << channels.trimmed();
        }
    qDebug()<<"打印数组代码已过";
    QScreen *screen=QGuiApplication::primaryScreen();
    if(screen){
        QRect screenRec = screen->availableGeometry();
        // 设置窗口为屏幕的90%大小
        int w = screenRec.width() * 0.9;
        int h = screenRec.height() * 0.9;
        resize(w, h);

        // 居中显示
        move((screenRec.width() - w) / 2, (screenRec.height() - h) / 2);
    }
    setStyleSheet("background-color:rgb(53,63,61)");
    _pTopWidget = new TopWidget();
    _pLeftWidget = new LeftWidget();
    _pLeftWidget->setFixedWidth(350);
    _pCentralWidget = new CentralWidget();
    _pRightWidget = new RightWidget();

    QHBoxLayout* pHLayout= new QHBoxLayout();
    pHLayout->addWidget(_pLeftWidget,2);
    pHLayout->addWidget(_pCentralWidget,6);
    pHLayout->addWidget(_pRightWidget,2);

    QVBoxLayout* pVLayout= new QVBoxLayout(this);
    pVLayout->addWidget(_pTopWidget,1);
    pVLayout->addLayout(pHLayout,9);
    setLayout(pVLayout);
    pVLayout->setContentsMargins(0,0,0,0);
    NetWorkReactor &reactor = NetWorkReactor::instance();
    reactor.initThreadPool();
    reactor.startReactor("192.168.253.130",8087);
    connect(&reactor,&NetWorkReactor::Refresh,_pCentralWidget,&CentralWidget::RecvFresh,Qt::QueuedConnection);
    connect(_pRightWidget->getCloudControl(),&CloudControl::updateMessage,_pLeftWidget->getWindowMessage(),&WindowMessage::handleMessage,Qt::QueuedConnection);
    connect(_pRightWidget->getDeviceList(),&DeviceList::sendMessage,_pLeftWidget->getWindowMessage(),&WindowMessage::handleMessage,Qt::QueuedConnection);
    qDebug()<<"NetWorkReactor已经完成";
}


void MainWindow::closeSlot()
{
    QMessageBox::StandardButton isExit = QMessageBox::warning(nullptr, "提示",
                                                              "你确定要退出吗？",
                                                              QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(isExit == QMessageBox::Yes) {
        close();
        EndScreen endScreen(this);
        endScreen.startEndScreen(4000); // 显示4秒后自动关闭
    }
}
void MainWindow::onFullScreen(){
    QWidget * pwindow = _pCentralWidget->window();
    if(pwindow->isMaximized()) {
        pwindow->showNormal();
        _pTopWidget->show();
        _pLeftWidget->show();
        _pRightWidget->show();
    } else {
        _pTopWidget->hide();
        _pLeftWidget->hide();
        _pRightWidget->hide();
        pwindow->showMaximized();
    }
}
//Esc退出全屏
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    QWidget * pwindow = _pCentralWidget->window();
    if(event->key() == Qt::Key_Escape) {
        pwindow->showNormal();
        _pTopWidget->show();
        _pLeftWidget->show();
        _pRightWidget->show();
    }
}
