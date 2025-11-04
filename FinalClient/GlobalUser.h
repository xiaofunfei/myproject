#ifndef GLOBALUSER_H
#define GLOBALUSER_H
#include <QObject>
#include<vector>
using std::vector;
class GlobalUser:public QObject
{
    Q_OBJECT
public:
    static GlobalUser* getInstance(){
        if(m_instance == nullptr){
            m_instance=new GlobalUser();
        }
        return m_instance;
    }
    GlobalUser(const GlobalUser&) = delete;
    GlobalUser operator=(const GlobalUser&)=delete;
    vector<vector<int>>     userCamera;
    //全局使用：当前界面所展示的页面是CurrentWindow个摄像头
    int                     CurrentWindow;
    //当前云台控制所控制的是摄像头CurrentCamId
    int                     CurrentCamId;
    //当前云台控制所操控的摄像头是CurrentChaId
    int                     CurrentChaId;
    //标识当前界面是否开始播放视频流
    int                     CurrentFlag;
    //标识旧的控制摄像头
    int                     OldCamId;
    //标识旧的摄像头通道
    int                     OldChaId;
private:
    GlobalUser(QObject * parent=nullptr):QObject(parent){}
    static GlobalUser*  m_instance;
};

#endif // GLOBALUSER_H
