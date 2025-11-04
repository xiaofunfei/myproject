#include "SmartHomeMonitorServer.h"
#include "MyLogger.h"
#include "Message.h"
#include "UserService.h"
#include "TurnCamera.h"
#include <iostream>
#include <nlohmann/json.hpp>
using json=nlohmann::json;
using namespace std;
namespace SOS{
SmartHomeMonitorServer::SmartHomeMonitorServer(int threadNum, int taskSize, unsigned short port, const string & ip)
:_threadpool(threadNum,taskSize)
,_tcpServer(port,ip)
//,_sql()
{
    using namespace std::placeholders;
    _tcpServer.setAllCallbacks(std::bind(&SmartHomeMonitorServer::onConnection,this,_1)
                               ,std::bind(&SmartHomeMonitorServer::onMessage,this,_1)
                                ,std::bind(&SmartHomeMonitorServer::onClose,this,_1));
}

SmartHomeMonitorServer::~SmartHomeMonitorServer(){

}

void SmartHomeMonitorServer::start(){
    _threadpool.start();
    _tcpServer.start();
}

void SmartHomeMonitorServer::stop(){
    _tcpServer.stop();
    _threadpool.stop();
}

void SmartHomeMonitorServer::onConnection(TcpConnectionPtr conn){
    LogInfo("tcp %s has connected.\n", conn->toString().c_str());
}

void SmartHomeMonitorServer::onMessage(TcpConnectionPtr conn){
    cout<<"onMesaage..."<<endl;
    Packet packet;
    int ret=conn->readPacket(packet);
    cout<<"read : "<<ret<<"bytes"<<endl
        <<"packet.type : "<<packet.type<<endl
        <<"packet.length : "<<packet.length<<endl
        <<"packet.data : "<<packet.msg<<endl;
    if(packet.length!=8){
    cout<<"start switch"<<endl;
    switch(packet.type){
        case TASK_LOGIN_SECTION1:{
            printf("popopoo\n");
            auto userlogin1 = std::make_shared<UserLoginSection1>(conn, packet);
            _threadpool.addTask(std::bind(&UserLoginSection1::process,userlogin1));
        }
        break;
        case TASK_LOGIN_SECTION2:{
            printf("jijijiji\n");
            auto userlogin2 = std::make_shared<UserLoginSection2>(conn, packet);
            _threadpool.addTask(std::bind(&UserLoginSection2::process,userlogin2));
        }
        break;
        case TASK_REGISTER_SECTION1:{
            printf("boboboob\n");

            //不用make_shared这个地方会出现double free问题
            auto userreg1 = std::make_shared<UserRegisterSection1>(conn, packet);
            //UserRegisterSection1 userreg1(conn,packet);
            _threadpool.addTask(std::bind(&UserRegisterSection1::process,userreg1));
        }
        break;
        case TASK_REGISTER_SECTION2:{
            printf("dididida\n");
            auto userreg2 = std::make_shared<UserRegisterSection2>(conn, packet);
            _threadpool.addTask(std::bind(&UserRegisterSection2::process,userreg2));
        }
        break;
        case TEST:{
            printf("BOBOBOBOO\n");
            string data=packet.msg;
            /*json j=json::parse(data);
            int cameraId=j["cid"].get<int>();
            int channel=j["channel"].get<int>();
            string action=j["action"].get<string>();*/
            cout<<"TEST NEWS IS "<<packet.msg;
        }
        break;
        case TASK_GET_STREAM:{
            printf("DIDADIDA\n");
            string data=packet.msg;
            json j=json::parse(data);
            //获得了摄像头id，接下来要去启动摄像头id得到url读取连接
            //也就是我这个时候去数据库中直接查询 之后再根据得到的url连接去解析流
            //又或者是说我的服务器一启动就需要开启拉流的话，那么也得根据数据库表中对应摄像头的url去进行拉流
            int cameraId=j["cid"].get<int>();
            string action=j["action"].get<string>();
            int channel=j["channel"].get<int>();
            //string channel=j["channel"].get<string>();
            // 用户端发过来需要指定摄像头id和channels因为需要id和channels来指定对应的rstp流
            //<<"channels is : "<<channel<<endl;
            CameraMessage cmsg;
            cmsg.CameraId=cameraId;
            //接下来得进行数据库连接获取rtsp/rtmp的地址
            MySQLClient sql;
            string host("localhost");
            string user("root");
            string pwd("123");
            string db("Project");
            int ret=sql.connect(host,user,pwd,db,0);
            if(ret) {
            printf("CameraID get connect db success!\n");
            }else{
            printf("CameraID get connect db failed!\n");
            }
            char sentence[256];
            sprintf(sentence,"select rtsp from Camera where id=%d and channels=%d",cameraId,channel);
            vector<vector<string>> res=sql.readOperationQuery(sentence);
            string url=res[1][0];
            printf("thr rtsp url is %s\n",url.c_str());
            cmsg.url=url;
            //现在已经获取了新的通道和url,得进行加锁停止之前的监控任务
            std::lock_guard<std::mutex> lock(_mutex);
            auto it= _cTask.find(cameraId);
            //关闭旧通道任务
            if(it!=_cTask.end()){
                shared_ptr<CameraTask> oldTask=it->second;
                oldTask->stopTask();
                printf("已经stop了摄像头%d 的旧通道处理任务!\n",cameraId);
                printf("接下里等待旧任务的退出...........\n");
                oldTask->waitForExit();//阻塞等待,直到旧任务调用notifyone
                
                _cTask.erase(it);
                printf("旧任务已完全退出\n");
            }
            //创建新通道Task
            auto CameraTask1 = std::make_shared<CameraTask>(conn,cmsg);
            _cTask[cameraId]=CameraTask1;
            _threadpool.addTask(std::bind(&CameraTask::process,CameraTask1));
            printf("已经启动摄像头%d 的新通道%d 处理任务!\n",cmsg.CameraId,channel);
        }
        break;
        case TASK_TYPE_HTTP_REQUEST_TURN:{
            printf("command enter TASK_TYPE_HTTP_REQUEST_TURN!!!!!!!!!\n");
                                        _threadpool.addTask([=](){
                                        TurnCamera tc(conn,packet);
                                        tc.process();
                                                                 });
                                         }
    }
}
}

void SmartHomeMonitorServer::onClose(TcpConnectionPtr conn){
    LogInfo("tcp %s has closed.\n",conn->toString().c_str());
}
}//endof namespace