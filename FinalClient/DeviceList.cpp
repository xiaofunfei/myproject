#include "DeviceList.h"
#include "GlobalUser.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QFile>
#include <QPalette>
#include <QStyleFactory>
#include <QHeaderView>
#include <QDebug>
#include "Message.h"
#include <QDateTime>
#include "NetWorkReactor.h"
DeviceList::DeviceList(QWidget *parent) : QTreeWidget(parent) {
    initUI();
    createTreeStructure();
    connect(this,&QTreeWidget::itemDoubleClicked,this,&DeviceList::onItemDoubleClicked);
}

void DeviceList::initUI(){
        setAttribute(Qt::WA_StyledBackground);
        setHeaderHidden(true);
        setRootIsDecorated(true);
        setAnimated(true);
        setIndentation(15);
        setFocusPolicy(Qt::NoFocus);
        //header()->setSectionResizeMode(QHeaderView::ResizeToContents);
            // 应用深色主题样式
        setStyleSheet(
                "DeviceListWidget {"
                "  background-color: #2D2D30;"
                "  color: #E0E0E0;"
                "  border: none;"
                "  border-radius: 5px;"
                "}"
                "QTreeWidget::item {"
                "  padding: 8px 0;"
                "  border: none;"
                "  font-size: 12px;"
                "color:white;"
                "}"
                "QTreeWidget::item:selected {"
                "  background-color: #0078D7;"
                "  color: white;"
                "  border: none;"
                "  border-radius: 3px;"
                "}"
                "QTreeView::branch:open:has-children {"
                "  image: url(:/resources/icons/minus.png);"   // 减号展开图标
                "}"
                "QTreeView::branch:closed:has-children {"
                "  image: url(:/resources/icons/add.png);"    // 加号折叠图标
                "}"
            );
}
void DeviceList::createTreeStructure() {
       // 创建顶级分组 - 默认分组
       setIndentation(30);
       QTreeWidgetItem *defaultGroup = new QTreeWidgetItem(this);
       defaultGroup->setText(0, "默认分组");
       defaultGroup->setIcon(0, createGroupIcon());
       defaultGroup->setExpanded(true);

       // 二级分组 - 网络视频
       QTreeWidgetItem *networkGroup = new QTreeWidgetItem(defaultGroup);
       networkGroup->setText(0, "网络视频");
       networkGroup->setIcon(0, createCameraIcon());
       networkGroup->setExpanded(true);
       auto & userCamera = GlobalUser::getInstance()->userCamera;
       for(int i=0;i<userCamera.size();i++){
           auto &channels=userCamera[i];
           QString str=QString("摄像头%1").arg(i+1);
           QTreeWidgetItem *device=new QTreeWidgetItem(networkGroup);
           device->setText(0,str);
           device->setIcon(0,createDeviceIcon());
           device->setExpanded(true);
           for(int j=0;j<channels.size();j++){
               QString cstr=QString("通道%1").arg(channels[j]);
                QTreeWidgetItem *channel=new QTreeWidgetItem(device);
                channel->setData(0,Qt::UserRole,i);
                channel->setData(0,Qt::UserRole+1,channels[j]);
                channel->setText(0, cstr);
                channel->setIcon(0, createStreamIcon());
                channel->setSelected(true); // 设置为选中状态
           }
       }
//       // 设备1 - 网络视频1
//       QTreeWidgetItem *device1 = new QTreeWidgetItem(networkGroup);
//       device1->setText(0, "球机");
//       device1->setIcon(0, createDeviceIcon());
//       device1->setExpanded(true);

//       // 设备2 - 网络视频2
//       QTreeWidgetItem *device2 = new QTreeWidgetItem(networkGroup);
//       device2->setText(0, "枪机");
//       device2->setIcon(0, createDeviceIcon());
//       device2->setExpanded(true);

//       // 添加主码流
//       QTreeWidgetItem *mainStream = new QTreeWidgetItem(device1);
//       mainStream->setText(0, "主码流");
//       mainStream->setIcon(0, createStreamIcon());
//       mainStream->setSelected(true); // 设置为选中状态

//       // 添加子码流
//       QTreeWidgetItem *subStream = new QTreeWidgetItem(device1);
//       subStream->setText(0, "子码流");
//       subStream->setIcon(0, createStreamIcon());
//       // 添加主码流
//       QTreeWidgetItem *mainStream2 = new QTreeWidgetItem(device2);
//       mainStream2->setText(0, "主码流");
//       mainStream2->setIcon(0, createStreamIcon());
//       mainStream2->setSelected(true); // 设置为选中状态

//       // 添加子码流
//       QTreeWidgetItem *subStream2 = new QTreeWidgetItem(device2);
//       subStream2->setText(0, "子码流");
//       subStream2->setIcon(0, createStreamIcon());


       // 添加其他设备 (折叠状态)
//       const QStringList deviceNames = {"摄像头3号", "摄像头4号"};
//       for (const QString &name : deviceNames) {
//           QTreeWidgetItem *device = new QTreeWidgetItem(networkGroup);
//           device->setText(0, name);
//           device->setIcon(0, createDeviceIcon());
//           device->setExpanded(false);
//       }

       // 添加扩展的树结构
       expandTreeItems();
}
// 创建分组图标（图片中的电脑显示器）
QIcon DeviceList::createGroupIcon() {
        return QIcon(":/resources/icons/group.png"); // 使用资源文件中的图标
}

// 创建摄像机图标
QIcon DeviceList:: createCameraIcon() {
        return QIcon(":/resources/icons/net.png");
}

    // 创建设备图标
QIcon DeviceList::createDeviceIcon() {
        return QIcon(":/resources/icons/camera.png");
}
    // 创建码流图标（图片中的流/连接图标）
QIcon DeviceList::createStreamIcon() {
      return QIcon(":/resources/icons/stream.png");
}

void DeviceList::expandTreeItems() {
        // 展开所有顶层项目
        for (int i = 0; i < topLevelItemCount(); ++i) {
            topLevelItem(i)->setExpanded(true);
        }
}
void DeviceList::onItemDoubleClicked(QTreeWidgetItem *item,int column){
    int cameraId=item->data(0,Qt::UserRole).toInt();
    int channelId=item->data(0,Qt::UserRole+1).toInt();
    QDateTime currentDateTime=QDateTime::currentDateTime();
    QString currentDateTimeString=currentDateTime.toString("hh:mm:ss");
    const QString time(currentDateTimeString);
    if(cameraId == 0 && channelId == 0){
        //QString msg1=QString("获取摄像头%1 通道%2 流信息失败!");
        //emit sendMessage(time,msg1);
        return;
    }
    qDebug()<<"cameraID is "<<cameraId<<" channelId is "<<channelId<<" column is "<<column;
    TLV tlv;
    tlv.type =TASK_GET_STREAM;
    QJsonObject jobj;
    auto user = GlobalUser::getInstance();
    user->CurrentFlag=0;
    jobj["cid"]=cameraId+1;
    jobj["channel"]=channelId;
    jobj["action"]="0";
    //设置上一次控制的摄像头id和通道id
    user->OldCamId=user->CurrentCamId;
    user->OldChaId=user->CurrentChaId;
    user->CurrentCamId=cameraId;
    user->CurrentChaId=channelId;
    //先对是否是最初设置当前控制摄像头做出判断
    qDebug()<<"user->CurrentCamId is "<<cameraId<<", user->CurrentChaId is "<<channelId;
    QJsonDocument doc(jobj);
    QByteArray utf8Data=doc.toJson(QJsonDocument::Compact);
    tlv.length=utf8Data.size();
    qDebug()<<"tlv data . length"<<tlv.length;
    strncpy(tlv.data,utf8Data.constData(),tlv.length);
    auto &reactor=NetWorkReactor::instance();
    reactor.sendCommand(tlv);
    QString msg1=QString("获取摄像头%1 通道%2 流信息").arg(cameraId+1).arg(channelId);
    emit sendMessage(time,msg1);
    qDebug()<<"tlv数据从DeviceList已发送!";
}
