#include "CameraWidget.h"
#include <QVBoxLayout>
#include <QDebug>
CameraWidget::CameraWidget(int cameraId,QWidget * parent)
    :QWidget(parent)
    ,_cameraId(cameraId+1)
{
    _frameLabel=new QLabel(this);
    _frameLabel ->setStyleSheet("background-color: #333; color: white; border: 1px solid gray;");
    _frameLabel->setAlignment(Qt::AlignCenter);
    _frameLabel->setText(QString("摄像头 %1").arg(_cameraId));
    _frameLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    _frameLabel->setScaledContents(true); // 让图片自适应标签大小（不超过标签）
    //窗口布局
    QVBoxLayout *layout=new QVBoxLayout (this);
    layout->addWidget(_frameLabel);
    setLayout(layout);
}

//更新帧
void CameraWidget::onFrameDecoded(int cameraId,const QImage & image){
    qDebug()<<"cameraWidget 的槽函数触发了！！";
    if(!image.isNull()){
        _frameLabel->setPixmap(QPixmap::fromImage(image));
    }
}
