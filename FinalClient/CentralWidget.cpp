#include "CentralWidget.h"
#include "CVolumeButton.h"
#include "NetWorkReactor.h"
#include <QDebug>
#include <QLabel>
#include <QDateTime>
#include <QTimer>
CentralWidget::CentralWidget(QWidget *parent)
    : QWidget(parent)
{

    initUI();



    connect(this,&CentralWidget::switchSignal,this,&CentralWidget::switchWindows);
}

void CentralWidget::initUI(){
    setAttribute(Qt::WA_StyledBackground);
    setWindowFlags(Qt::FramelessWindowHint);
    setStyleSheet("background-color:rgb(51,51,51)");
    //整体布局是上方显示画面 下方是按钮
    QHBoxLayout *btnlay=new QHBoxLayout();
    for(int count :{1,2,4,9,16}){
        QPushButton *btn=new QPushButton(QString::number(count),this);
        btn->setFixedSize(50,50);
        btn->setStyleSheet("QPushButton{font-size: 50px;font-weight: bold; color: white;border:none;}"
                           "QPushButton:hover{background-color:#0078D7;padding: 4px;}"
                           "QPushButton:pressed{background-color:#0078D7;padding: 8px;}"
                           );
        //点击按钮时触发窗口数量切换
        connect(btn,&QPushButton::clicked,this,[=](){emit(switchSignal(count));});
        btnlay->addWidget(btn);
    }
    QPushButton *btnPhoto=new QPushButton(this);
    btnPhoto->setFixedSize(50,50);
    btnPhoto->setStyleSheet("QPushButton{border-image:url(:/resources/icons/photo.png);background-position:center;background-repeat: no-repeat;border:none;}"
                            "QPushButton:hover{image:url(:/resources/icons/photo.png);background-color:#0078D7;border:none;padding: 4px;}"
                            "QPushButton:pressed{background-color:#0078D7;image:url(:/resources/icons/photo.png);border:none;padding: 8px;}"
                            );
    btnPhoto->setToolTip("截屏");
    _pVolumeButton = new CVolumeButton(this);
    _pVolumeButton->setFixedSize(50, 50);
    QPushButton *btnMax=new QPushButton(this);
    btnMax->setFixedSize(50,50);
    btnMax->setStyleSheet("QPushButton{border-image:url(:/resources/icons/max.png);background-position:center;background-repeat: no-repeat;border:none;}"
                            "QPushButton:hover{image:url(:/resources/icons/max.png);background-color:#0078D7;border:none;padding: 4px;}"
                            "QPushButton:pressed{image:url(:/resources/icons/max.png);background-color:#0078D7;border:none;padding: 8px;}");
    btnMax->setToolTip("全屏");

    //点击全屏
    connect(btnMax,&QPushButton::clicked,this,&CentralWidget::sigFullScreen);


    QLabel *timeLabel = new QLabel(this);
    timeLabel->setFixedHeight(50);
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString currentDateTimeString = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
    timeLabel->setText(currentDateTimeString);
    timeLabel->setStyleSheet("font-size:36px;color:white;font-weight:bold");
    QTimer *pTimer=new QTimer(this);
    pTimer->start(1000);
    connect(pTimer,&QTimer::timeout,this,[=](){
        QDateTime currentDateTime = QDateTime::currentDateTime();
        QString currentDateTimeString = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
        timeLabel->setText(currentDateTimeString);
    });


    btnlay->addStretch();
    btnlay->addWidget(timeLabel);
    btnlay->addSpacing(10);
    btnlay->addWidget(btnPhoto);
    btnlay->addSpacing(10);
    btnlay->addWidget(_pVolumeButton);
    btnlay->addSpacing(10);
    btnlay->addWidget(btnMax);

    _displayWidget=new QWidget(this);
    _displayWidget->setStyleSheet("border:none");
    _grid=new QGridLayout(_displayWidget);
    _grid->setSpacing(0);
    _grid->setContentsMargins(0,0,0,0);
    QVBoxLayout* mainLayout=new QVBoxLayout(this);
    mainLayout->addWidget(_displayWidget);
    mainLayout->addLayout(btnlay);
    mainLayout->setContentsMargins(0,0,0,0);

}
void CentralWidget::RecvFresh(int count){
    switchWindows(count);
}
void CentralWidget::switchWindows(int count){

    qDeleteAll(_cameraWidgets);
    _cameraWidgets.clear();
    while(QLayoutItem *item =_grid->takeAt(0)){
        delete item->widget();
        delete item;
    }

    //根据发过来的信号计算网格行列数
    auto user=GlobalUser::getInstance();
    user->CurrentWindow=count;
    int rows,cols;
    switch(count){
    case 1:rows=1;cols=1;break;
    case 2:rows=1;cols=2;break;
    case 4:rows=2;cols=2;break;
    case 9:rows=3;cols=3;break;
    case 16:rows=4;cols=4;break;
    default:return;
    }
    for(int i=0;i<count;i++){
        int cameraId = i + 1; // 假设cameraId从1开始
        CameraWidget *cam=new CameraWidget(i,_displayWidget);
        _cameraWidgets.append(cam);//存入容器
        // 核心：连接解码信号到当前CameraWidget的渲染槽
        // 从NetWorkReactor获取该cameraId对应的处理器
        auto& reactor = NetWorkReactor::instance();
        auto handlerKey = std::make_pair(cameraId, TASK_STREAM_DATA);
        if (reactor._handlers.contains(handlerKey)) {
            auto handler = reactor._handlers[handlerKey];
            // 将handler的frameDecoded信号连接到cam的onFrameDecoded槽
            // 注意：解码线程可能是子线程，用QueuedConnection确保UI线程更新
            connect(
                   dynamic_cast<CameraVideoHandler*>(handler.get()),
                   &CameraVideoHandler::frameDecoded,
                   cam,
                   &CameraWidget::onFrameDecoded,
                   Qt::QueuedConnection
                    );
                }
        int row=i/cols;
        int col=i%cols;
        _grid->addWidget(cam,row,col);
    }
    _displayWidget->setLayout(_grid);
}

