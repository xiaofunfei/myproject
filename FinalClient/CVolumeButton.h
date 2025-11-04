#ifndef CVOLUMEBUTTON_H
#define CVOLUMEBUTTON_H
#include "CVolumeSliderDialog.h"
#include <QPushButton>
class CVolumeButton : public QPushButton
{
    Q_OBJECT
public:
        explicit CVolumeButton(QWidget *parent = nullptr);
        ~CVolumeButton();
signals:
        void sigVolumeSliderMoved(int);
protected:
        void paintEvent(QPaintEvent* event) override;
        void enterEvent(QEvent* event) override;
        //void leaveEvent(QEvent* event) override;
        void timerEvent(QTimerEvent* event) override;
        //音量按钮被按下时，调用mousePressEvent函数
        void mousePressEvent(QMouseEvent* event) override;
        void hide();
private:
        CVolumeSliderDialog * _pVolumeSliderDlg=nullptr;
        int                   _timerId = 0;//定时器id
        bool                  _isRunning = false;//定时器开启的标志位
        bool                  _isMute = false;//是否静音
        int                   _volumeValue = 0;//音量值
};
#endif // CVOLUMEBUTTON_H
