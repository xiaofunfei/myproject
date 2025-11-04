#include "CVolumeSliderDialog.h"
#include <QHBoxLayout>
CVolumeSliderDialog::CVolumeSliderDialog(QWidget * parent)
    : QDialog(parent)
{
    this->setFixedSize(300, 60);
    this->setAttribute(Qt::WA_StyledBackground);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);
    this->setStyleSheet("QDialog{background-color: rgb(17,17,17);}");
    _pVolumeSlider = new QSlider(this);
    _pVolumeSlider->setOrientation(Qt::Horizontal);
    _pVolumeSlider->setValue(50);
    _pVolumeSlider->setStyleSheet(R"(
        QSlider::groove:horizontal {
            background: rgb(64,64,64);
            height: 6px;
            border:none;
            border-radius: 3px;
        }
        QSlider::sub-page:horizontal {
            background: rgb(0,150,255);  /* 已播放区域颜色 */
            border-radius: 3px;
        }
        QSlider::handle:horizontal {
            background: #FFF;
            width: 20px;
            margin: -5px 0;
            border-radius: 8px;
        }
    )");
    _pVolumeText = new QLabel(this);
    _pVolumeText->setAlignment(Qt::AlignLeft);
    _pVolumeText->setFixedSize(42,30);
    _pVolumeText->setText("50");
    _pVolumeText->setStyleSheet("QLabel{background-color: rgb(17,17,17);font-family:Microsoft YaHei; font-size:24px;color:rgb(255,255,255);}");
    QHBoxLayout * pHLayout = new QHBoxLayout(this);
    pHLayout->setContentsMargins(20,10,10,10);
    pHLayout->addWidget(_pVolumeSlider);
    pHLayout->addWidget(_pVolumeText);
    _pVolumeSlider->setRange(0,100);

    connect(_pVolumeSlider,&QSlider::sliderMoved,[=](int position){
        _pVolumeSlider->setValue(position);
        _pVolumeText->setText(QString::number(position));
        emit sigVolumeSliderMoved(position);
    });
}
CVolumeSliderDialog::~CVolumeSliderDialog(){}


void  CVolumeSliderDialog::setSliderValue(int value){
    _pVolumeSlider->setValue(value);
    _pVolumeText->setText(QString::number(value));
    emit sigVolumeSliderMoved(value);
}

int CVolumeSliderDialog::value(){
    return _pVolumeSlider->value();
}

