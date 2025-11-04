#include "GenerateToken.h"
#include "MyLogger.h"

#include <openssl/md5.h>
#include <curl/curl.h>

#include <iostream>
#include <sstream>
#include <string>
using std::string;

namespace SOS{

static size_t WriteCallback(void *contents,size_t size,size_t nmemb,string *resp){
    size_t total = size * nmemb;
    resp->append((char*)contents,total);
    return total;
}


CryptString::CryptString(const string &data):_data(data){}

//产生MD5
string CryptString::generateMD5()const{
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5_CTX context;

    MD5_Init(&context);
    MD5_Update(&context,_data.c_str(),_data.size());
    MD5_Final(digest,&context);

    std::ostringstream oss;
    for(int i=0;i<MD5_DIGEST_LENGTH;i++){
        oss<<std::hex<<std::setw(2)<<std::setfill('0')
            <<static_cast<int>(digest[i]);
    }
    return oss.str();
}

KVQuery::KVQuery(){
    _map["secret"] = "f6fdffe48c908deb0f4c3bd36c032e72";
    _map["t"] = std::to_string(std::time(nullptr));//获取unix时间戳
}

void KVQuery::add(const string&key,const string &value){
    _map[key] = value;
}

void KVQuery::erase(const string &key){
    _map.erase(key);
}

void KVQuery::clear(){
    _map.clear();
}

string KVQuery::toString()const{
    std::ostringstream oss;
    for(auto it = _map.begin();it!=_map.end();it++){
        if(it!=_map.begin()) oss<<"&";
        oss << it->first << "="<<it->second;
    }
    return oss.str();
}

string KVQuery::toCryptString(){
    string queryStr = toString();
    CryptString cryptstr(queryStr);
    string token = cryptstr.generateMD5();
   // this->add("token",token);
    this->erase("secret");//移除密匙
    string res = this->toString();
    res += "&token=" + token;
    return res;
}


CurlKits::CurlKits():_curl(curl_easy_init()){}

CurlKits::~CurlKits(){
    if(_curl){
        curl_easy_cleanup(_curl);
    }
}

void CurlKits::Perform(const string &prehttp,const string &params,const string &header){
    std::cout<<prehttp<<params<<std::endl;
    string response;
    struct curl_slist* headers = nullptr;
    curl_slist_append(headers,header.c_str());
    if(_curl){
        //设置固定部分
        curl_easy_setopt(_curl,CURLOPT_URL,prehttp.c_str());
        //设置请求头
        curl_easy_setopt(_curl,CURLOPT_HTTPHEADER,headers);
        //设置post字段
        curl_easy_setopt(_curl,CURLOPT_POSTFIELDS,params.c_str());
        //设置响应回调函数
        curl_easy_setopt(_curl,CURLOPT_WRITEFUNCTION,&WriteCallback);
        //todo:这里回写到josn串中
        curl_easy_setopt(_curl,CURLOPT_WRITEDATA,&response);
        CURLcode res = curl_easy_perform(_curl);

        if(res == CURLE_OK){
            try{
                auto json = nlohmann::json::parse(response);
                std::cout<<"状态码:"<<json["code"]<<"\n";
                std::cout<<"消息"<<json["msg"]<<std::endl;

                if(json["code"] == 0){
                    LogInfo("云台控制成功！");
                }else{
                    LogError("云台控制失败\n");
                }
            }catch(...){
                LogError("Json解析失败");
            }
        }else{
            LogDebug("请求失败:",curl_easy_strerror(res)); 
        }
    }
    //释放请求头链表
    curl_slist_free_all(headers);
}



} // end of namespace SOS;

