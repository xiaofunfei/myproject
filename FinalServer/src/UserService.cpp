#include "UserService.h"
#include "MyLogger.h"
#include <shadow.h>
#include <string.h>
#include <iostream>
#include <nlohmann/json.hpp>
using json=nlohmann::json;
using std::cout;
using std::endl;
namespace SOS{
char * GenRandomString(int length){
    int flag,i;
    char * string;
    srand((unsigned) time(NULL));
    if((string = (char *)malloc(length))==NULL){
        printf("malloc failed!flag:14\n");
        return NULL;
    }
    for(int i=0;i<length+1;i++){
        flag=rand()%3;
        switch (flag)
        {
        case 0: string[i]= 'A' +rand()%26;break;
        case 1: string[i]= 'a' +rand()%26;break;
        case 2: string[i]= '0' +rand()%10;break;
        default: string[i]='x';break;    
        }
    }
    string[length]='\0';
    return string;
}
void UserLoginSection1::process(){
    cout<<"UserLoginSection1 processing!"<<endl;
    if(_pack.type == TASK_LOGIN_SECTION1){
        string username=_pack.msg;
        //从数据库查询是否有这个用户
        string host("localhost");
        string user("root");
        string pwd("123");
        string db("Project");
        int ret=_sql.connect(host,user,pwd,db,0);
        if(ret) {
            printf("UserRegisterSection connect db success!\n");
        }else{
            printf("UserRegisterSection connect db failed!\n");
        }
        char sentence[256];
        sprintf(sentence,"select setting from user where username='%s'",username.c_str());
        vector<vector<string>> res=_sql.readOperationQuery(sentence);
        if(res.size()!=0){
            //结果集不为0代表有结果
            string setting=res[1][0];
            LogDebug("setting: %s\n",setting.c_str());
            //构建TLV 发送
            TLV tlv;
            tlv.type=TASK_LOGIN_SECTION1_RESP_OK;
            tlv.length = setting.length();
            strncpy(tlv.data,setting.c_str(),tlv.length);
            _conn->sendInLoop(tlv);
        }else{
            TLV tlv;
            tlv.type=TASK_LOGIN_SECTION1_RESP_ERROR;
            _conn->sendInLoop(tlv);
        }
    }
}
void UserLoginSection2::process(){
    cout<<"login section2 is processing"<<endl;
    if(_pack.type == TASK_LOGIN_SECTION2){
        //这个时候传过来packet里面的数据应该是加密后的密文
        //接下来用得到的这个密文和数据库中的密码进行对比
         //连接数据库
        string host("localhost");
        string user("root");
        string pwd("123");
        string db("Project");
        int ret=_sql.connect(host,user,pwd,db,0);
        if(ret) {
            printf("UserRegisterSection connect db success!\n");
        }else{
            printf("UserRegisterSection connect db failed!\n");
        }
        //接下来开始解析字符串，因为我在QT端传送的时候用户名一起传过来了
        //且用户是固定在数据最后，并且以@符号作开始标志位
        //因此这个地方先把最后一个@处开始的字符串取走
        string str=_pack.msg;
        string name;
        int local=0;
        //找到str中最后一个@所在的位置
        for(int i=str.size()-1;i>0;i--){
            if(str[i]=='@'){
                local=i;
                break;
            }
        }
        //截取str中local之后的字符串
        name=str.substr(local+1);
        printf("Register Section2 name is %s \n",name.c_str());
        string encoded=str.substr(0,local);
        cout<<"encoded substr is:"<<encoded<<endl;
        printf("Register Section2 encoded is %s \n",encoded.c_str());
        char sentence[256];
        sprintf(sentence,"select encrypt from user where username='%s'",name.c_str());
        vector<vector<string>> res=_sql.readOperationQuery(sentence);
        cout<<"encoded is :"<<res[1][0];
        _sql.dump(res);
        printf("res 打印结束!\n");
        if(res.size()!=0){
            cout<<"res is not null!";
            //代表查找到对应的加密数据，此时还需要对密文是否正确进行判断
            if(encoded==res[1][0]){
                cout<<"password right!";
                //正确！
                //这个地方成功发送登陆成功的消息,接下来需要紧接着发送这个用户所拥有的摄像头信息
                char sentence1[256];
                //查询用户对应的id
                sprintf(sentence1,"select id from user where username='%s'",name.c_str());
                vector<vector<string>> res1=_sql.readOperationQuery(sentence1);
                printf("user id is %s",res1[1][0].c_str());
                //这个时候取出来用户对应的id,接下来需要将用户所拥有的摄像头id和通道发过去
                char sentence2[256];
                sprintf(sentence2,"select cid,channels from u_c_relation where uid=%s",res1[1][0].c_str());
                vector<vector<string>> res2=_sql.readOperationQuery(sentence2);
                _sql.dump(res2);
                //这边已经正常获取了结果集,结果是两列,前面的是摄像头id后面的通道id,这边的处理逻辑就是一次以json格式处理一条直到数据处理完成
                for(int i=1;i<res2.size();i++){
                    json obj;
                    //printf("flag1111======%d\n",i);
                    int cameraId=std::stoi(res2[i][0]);
                    //printf("flag4444======%d\n",i);
                    int channelId=std::stoi(res2[i][1]);
                    //printf("flag3333======%d\n",i);
                    obj["cameraId"]=cameraId;
                    obj["channel"]=channelId;
                    //printf("flag2222=========%d\n",i);
                    //此处json串构建完成
                    //接下来构建TLV包来发送
                    string msg=obj.dump();
                    cout<<"发送的Data Ready"<<msg;
                    TLV tlv;
                    tlv.type=TASK_USER_READY_DATA;
                    tlv.length=msg.size();
                    strncpy(tlv.data,msg.c_str(),tlv.length);
                    _conn->sendInLoop(tlv);
                    //然后接下来想进一步完善的话就从客户端对服务器发送应答指令
                }
                cout<<"Ready data end!!!!!!!!!!";
                TLV tlv;
                tlv.type=TASK_LOGIN_SECTION2_RESP_OK;
                string msg=("Login Success!");
                tlv.length=msg.size();
                strncpy(tlv.data,msg.c_str(),tlv.length);
                _conn->sendInLoop(tlv);
            }else{
                cout<<"password wrong!";
                //错误!密码错误不让进
                TLV tlv;
                tlv.type=TASK_LOGIN_SECTION2_RESP_ERROR;
                string msg=("Login Failed!");
                tlv.length=msg.size();
                strncpy(tlv.data,msg.c_str(),tlv.length);
                _conn->sendInLoop(tlv);
            }
        }else{
            cout<<"res is null!";
            //错误!有用户名没有密码的不让进
            TLV tlv;
            tlv.type=TASK_LOGIN_SECTION2_RESP_ERROR;
            string msg=("Login Failed!");
            tlv.length=msg.size();
            strncpy(tlv.data,msg.c_str(),tlv.length);
            _conn->sendInLoop(tlv);
        }
    }
}
void UserRegisterSection1::process(){
    cout<<"UserRegisterSection1 processing!"<<endl;
    if(_pack.type==TASK_REGISTER_SECTION1){
        //收到了注册请求
        string username=_pack.msg;
        //从数据库查询是否有这个用户
        string host("localhost");
        string user("root");
        string pwd("123");
        string db("Project");
        int ret=_sql.connect(host,user,pwd,db,0);
        if(ret) {
            printf("UserRegisterSection connect db success!\n");
        }else{
            printf("UserRegisterSection connect db failed!\n");
        }
        //数据库连接完成接下来就是查询用户
        char sentence[1024];
        sprintf(sentence,"select setting from user where username='%s'",username.c_str());
        vector<vector<string>> res =_sql.readOperationQuery(sentence);
        if(res.size()!=0){
            //此时用户已存在，注册不合法，因此回传一个register_resp_error请求
            TLV tlv;
            tlv.type=TASK_REGISTER_SECTION1_RESP_ERROR;
            tlv.length=0;
            _conn->sendInLoop(tlv);
        }else{
            //用户不存在此时可以注册,传一个setting过去
            TLV tlv;
            tlv.type=TASK_REGISTER_SECTION1_RESP_OK;
            string msg="$1$";
            char * res=GenRandomString(8);
            msg=msg+res+"$";
            //生成的盐值在此时和用户一起存入
            char sentence[256];
            sprintf(sentence,"insert into user (username,setting) values ('%s','%s')",username.c_str(),msg.c_str());
            if(_sql.writeOperationQuery(sentence)){
                printf("TASK_REGISTER_SECTION1 数据插入成功\n");
            }else{
                printf("TASK_REGISTER_SECTION1 数据插入失败！\n");
            }
            tlv.length=msg.size();
            //此时setting已经构建完成，再接下来构建TLV的data
            strncpy(tlv.data,msg.c_str(),tlv.length);
            _conn->sendInLoop(tlv);
            printf("TASK_REGISTER_SECTION1_RESP_OK tlv has been sent!\n");
        }
    }
}
void UserRegisterSection2::process(){
    if(_pack.type==TASK_REGISTER_SECTION2){
        //连接数据库
        string host("localhost");
        string user("root");
        string pwd("123");
        string db("Project");
        int ret=_sql.connect(host,user,pwd,db,0);
        if(ret) {
            printf("UserRegisterSection connect db success!\n");
        }else{
            printf("UserRegisterSection connect db failed!\n");
        }
        //接下来开始解析字符串，因为我在QT端传送的时候用户名一起传过来了
        //且用户是固定在数据最后，并且以@符号作开始标志位
        //因此这个地方先把最后一个@处开始的字符串取走
        string str=_pack.msg;
        string name;
        int local=0;
        //找到str中最后一个@所在的位置
        for(int i=str.size()-1;i>0;i--){
            if(str[i]=='@'){
                local=i;
                break;
            }
        }
        //截取str中local之后的字符串
        name=str.substr(local+1);
        printf("Register Section2 name is %s \n",name.c_str());
        string encoded=str.substr(0,local);
        printf("Register Section2 encoded is %s \n",encoded.c_str());
        char sentence[256];
        sprintf(sentence,"update user set encrypt='%s' where username='%s'",encoded.c_str(),name.c_str());
        if(_sql.writeOperationQuery(sentence)){
            printf("Register Section2语句更新成功!!!\n");
            //成功的话就要回传一个SECTION_2_OK
            string msg("Register success!");
            TLV tlv;
            tlv.type=TASK_REGISTER_SECTION2_RESP_OK;
            tlv.length=msg.size();
            strncpy(tlv.data,msg.c_str(),tlv.length);
            _conn->sendInLoop(tlv);
        }else{
            printf("Register Section2语句更新失败!!!\n");
            //失败的话就要回传一个SECTION_2_ERROR
            string msg("Register Failed!");
            TLV tlv;
            tlv.type=TASK_REGISTER_SECTION2_RESP_ERROR;
            tlv.length=msg.size();
            strncpy(tlv.data,msg.c_str(),tlv.length);
            _conn->sendInLoop(tlv);
        }
    }
}
}