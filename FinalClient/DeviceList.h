#ifndef DEVICELISTWIDGET_H
#define DEVICELISTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTreeWidget>
class DeviceList : public QTreeWidget
{
    Q_OBJECT
public:
    explicit DeviceList(QWidget *parent = nullptr);
private slots:
    void onItemDoubleClicked(QTreeWidgetItem *item,int column);
signals:
    void sendMessage(const QString &time,const QString &msg);
private:
    void initUI();
    void createTreeStructure();
    QIcon createGroupIcon();
    QIcon createCameraIcon();
    QIcon createDeviceIcon();
    QIcon createStreamIcon();
    void expandTreeItems();
private:
//    QLabel*        _pTextLabel;
//    QTreeWidget*   _pDeviceList;
};

#endif // DEVICELISTWIDGET_H
