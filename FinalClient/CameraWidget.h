#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H
#include <QWidget>
#include <QLabel>
#include <FFmepegKits.h>
#include <memory>
#include <QImage>
using std::unique_ptr;
class CameraWidget:public QWidget
{
    Q_OBJECT
public:
    explicit CameraWidget(int cameraId,QWidget *parent=nullptr);
    //暴露这个摄像头id用于匹配
    int cameraId() const {return _cameraId;}
public slots:
    void onFrameDecoded(int cameraId,const QImage &image);
private:
    int     _cameraId;
    QLabel *_frameLabel;
};

#endif // CAMERAWIDGET_H
