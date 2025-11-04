#include "TurnCamera.h"
#include "GenerateToken.h"
#include "MyLogger.h"
#include "MySQLClient.h"
#include <cmath>
#include <unistd.h>


namespace SOS{


void TurnCamera::process(){
    if(_packet.type == TASK_TYPE_HTTP_REQUEST_TURN){
        //ip,请求参数以及请求头
        string ip,params,header = "application/x-www-form-urlencoded";
        bool ret = fillValue(ip,params);
        string prehttp ="http://" + ip + string("/xsw/api/ptz/control?");
        TLV tlv;
        tlv.length = 0;
        if(!ret){
            LogDebug("fillValue error\n");
            tlv.type = TASK_TYPE_HTTP_REQUEST_TURN_RESP_ERROR;
            _conn->sendInLoop(tlv);
            return;
        }
        CurlKits curl;
        curl.Perform(prehttp,params,header);
        sleep(2);
        params.clear();
        ret = fillValue(ip,params,1);
        if(!ret){
            LogDebug("fillValue error\n");
            tlv.type = TASK_TYPE_HTTP_REQUEST_TURN_RESP_ERROR;
            _conn->sendInLoop(tlv);
            return;
        }
        curl.Perform(prehttp,params,header);
        tlv.type = TASK_TYPE_HTTP_REQUEST_TURN_RESP_OK;
        _conn->sendInLoop(tlv);
    }
}



bool TurnCamera::fillValue(string &ip,string &params,int flag){
   string cameraType,channelId,value,speed;
   string t,token;
   string input = _packet.msg;
   std::istringstream ss(input);


    std::string dummy;
    if (input[0] == '$') {
        std::getline(ss, dummy, '$'); // 丢弃第一个空字段
    }

   // 用getline替代istringstream >> 操作
    std::getline(ss,cameraType,'$');  // 直接获取$之间的内容
    std::getline(ss,channelId,'$');
    std::getline(ss,value,'$');
    std::getline(ss,speed,'$');
    if(flag == 1){
        value.clear();
        value = string("s");
    }
   string merge = cameraType + channelId + value + speed;
   printf("235:%s\n",merge.c_str());
   if(ss.fail()){
       LogDebug("fillValue stringstream error\n");
       return false;
   }
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
   sprintf(sentence,"select ip from Camera where id=%d and channels=%d",stoi(cameraType),stoi(channelId));
   vector<vector<string>> res=sql.readOperationQuery(sentence);
   ip = res[1][0]; 
   KVQuery kv;
   kv.add("channelId",channelId);
   kv.add("value",value);
   kv.add("speed",speed);
   params =  kv.toCryptString();
   printf("url = %s\n",params.c_str());
   return true;
}





}//end of namespace SOS


