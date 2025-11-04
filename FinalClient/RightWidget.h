#ifndef RIGHTWIDGET_H
#define RIGHTWIDGET_H

#include <QWidget>
#include <QLabel>
class DeviceList;
class CloudControl;
class RightWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RightWidget(QWidget *parent = nullptr);
    QLabel* getDeviceLabel(){
        return _pDeviceLabel;
    }
    QLabel* getCloudLabel(){
        return _pCloudLabel;
    }
    DeviceList* getDeviceList(){
        return _pDeviceList;
    }
    CloudControl*   getCloudControl(){
        return _pCloudControl;
    }
signals:
private:
    void initUI();
private:
    QLabel*        _pDeviceLabel;
    QLabel*        _pCloudLabel;
    DeviceList*    _pDeviceList;
    CloudControl*  _pCloudControl;
};

#endif // RIGHTWIDGET_H
