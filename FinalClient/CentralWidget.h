#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include "CameraWidget.h"
#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <vector>
#include <QString>
#include <memory>
using std::unique_ptr;
class CVolumeButton;
class CentralWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CentralWidget(QWidget *parent = nullptr);
public slots:
    void RecvFresh(int);
signals:
    void switchSignal(int);
    void sigFullScreen();
private slots:
    void switchWindows(int);
private:
    void initUI();
private:
    QWidget*                _displayWidget;//承载摄像头窗口的容器
    QGridLayout*            _grid; // 网格布局管理窗口
    QList<CameraWidget *>   _cameraWidgets;//存储所有摄像头窗口
    CVolumeButton *         _pVolumeButton;
};

#endif // PLAYWIDGET_H
