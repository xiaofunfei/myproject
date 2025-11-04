#include "CVolumeButton.h"
#include <QDebug>
#include <QStylePainter>
#include <QStyleOptionButton>
#include <QMouseEvent>
CVolumeButton::CVolumeButton(QWidget *parent)
: QPushButton(parent)
{
    this->setFixedSize(48,48);
    setStyleSheet("QPushButton{border-image:url(:/resources/icons/volume.png);background-position:center;background-repeat: no-repeat;border:none;}"
                  "QPushButton:hover{image:url(:/resources/icons/volume.png);background-position:center;background-repeat: no-repeat;border:none;padding: 4px;}"
                  "QPushButton:pressed{image:url(:/resources/icons/volume.png);background-position:center;background-repeat: no-repeat;border:none;padding: 8px;}");
    setToolTip(tr("音量"));
}
CVolumeButton::~CVolumeButton()
{}
void CVolumeButton::enterEvent(QEvent *event)
{
    //qDebug() << "CVolumeButton::enterEvent()";
    if (!_pVolumeSliderDlg){
        _pVolumeSliderDlg = new CVolumeSliderDialog(this);
    }
    QPoint p1 = this->mapToGlobal(QPoint(0, 0)); //声音按钮左上角相对于屏幕的绝对位置
    QRect rect1 = this->rect();
    QRect rect2 = _pVolumeSliderDlg->rect(); //rect包含标题栏，去掉标题栏后height不变
    int x = p1.x() + (rect1.width() - rect2.width()) / 2;
    int y = p1.y() - rect2.height() - 5;
    _pVolumeSliderDlg->move(x, y); //move是相对于桌面原点的位置
    _pVolumeSliderDlg->show();
    //一旦音量控制界面出现后，就开启定时器
    if(_isRunning == false) {
    _timerId = startTimer(250);//开启定时器,250毫秒的时间间隔
    _isRunning = true;//定时器启动标志位
    }

    connect(_pVolumeSliderDlg,&CVolumeSliderDialog::sigVolumeSliderMoved,this,
            &CVolumeButton::sigVolumeSliderMoved);
}
void CVolumeButton::timerEvent(QTimerEvent *)
{
    //qDebug() << "CVolumeButton::timerEvent";
    hide();
}
void CVolumeButton::paintEvent(QPaintEvent*)
{
    //qDebug() << "CVolumeButton::paintEvent";
    QStylePainter p(this);
    QStyleOptionButton option;
    initStyleOption(&option);
    p.drawControl(QStyle::CE_PushButton, option);
}
//void CVolumeButton::leaveEvent(QEvent *event)
//{
//    qDebug() << "CVolumeButton::leaveEvent";
//    hide();
//}
//void CVolumeButton::hide()
//{
//    if(_pVolumeSliderDlg) {
//        //根据鼠标的位置判断音量大小窗口是否消失
//        QPoint mousePos = QCursor::pos();//获取鼠标的位置
//        QRect rect = _pVolumeSliderDlg->geometry();
//        if(!rect.contains(mousePos) && _pVolumeSliderDlg->isVisible()) {
//            _pVolumeSliderDlg->hide();
//            qDebug() << ">> hide...";
//        }
//    }
//}
void CVolumeButton::hide()
{
    if(_pVolumeSliderDlg !=nullptr && _pVolumeSliderDlg->isVisible()) {
        QPoint mousePos = QCursor::pos();//获取鼠标的位置
        QRect rect1 = _pVolumeSliderDlg->geometry();
        QRect rect2 = this->geometry();
        QRect rect3(rect1.x(), rect1.y(), rect1.width(), rect1.height() +rect2.height());
        //根据鼠标的位置判断音量大小窗口是否消失 
        if(!rect3.contains(mousePos)) {
            _pVolumeSliderDlg->hide();
            //qDebug() << ">> hide...";
        }
    } else {
        killTimer(_timerId);//当不包含鼠标位置，并且音量控制界面没显示时，就停止定时器
        _isRunning = false;
      }
}
void CVolumeButton::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        _isMute = !_isMute;
        if(_isMute) {//设置为静音
            if(_pVolumeSliderDlg) {
                //保存原来的音量值
                _volumeValue = _pVolumeSliderDlg->value();
                _pVolumeSliderDlg->setSliderValue(0);
                setStyleSheet("QPushButton{border-image:url(:/resources/volume/audio_mute.png);background-position:center;background-repeat: no-repeat;border:none;}"
                              "QPushButton:hover{image:url(:/resources/volume/audio_mute.png);background-position:center;background-repeat: no-repeat;border:none;padding: 4px;}"
                              "QPushButton:pressed{image:url(:/resources/volume/audio_mute.png);background-position:center;background-repeat: no-repeat;border:none;padding: 8px;}");
            }
        } else {
            if(_pVolumeSliderDlg) {
                _pVolumeSliderDlg->setSliderValue(_volumeValue);
                setStyleSheet(
                              "QPushButton{border-image:url(:/resources/icons/volume.png);background-position:center;background-repeat: no-repeat;border:none;}"
                              "QPushButton:hover{image:url(:/resources/icons/volume.png);background-position:center;background-repeat: no-repeat;border:none;padding: 4px;}"
                              "QPushButton:pressed{image:url(:/resources/icons/volume.png);background-position:center;background-repeat: no-repeat;border:none;padding: 8px;}");
            }
        }
    }
}

