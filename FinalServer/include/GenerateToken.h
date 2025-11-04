#pragma once
#ifndef _GENERATETOKEN_H_
#define _GENERATETOKEN_H_
#include <string>
#include <map>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <openssl/md5.h>
#include <nlohmann/json.hpp>
#include <curl/curl.h>

using std::string;
using std::map;

namespace SOS{


//【CryptString负责加密】
class CryptString
{
public:
    CryptString(const string &data);
    ~CryptString() {};
    string generateMD5()const;//将_data转换为MD5哈希值
    // 设计目的：提供字符串加密功能，用于生成API请求的安全令牌

private:
    string _data;//存储需要加密的原始字符串
};

//【KVQuery负责参数管理】
//构建HTTP请求的查询参数，特别是需要签名的API请求
class KVQuery{
public:
    KVQuery();
    //管理参数集合
    void add(const string &key,const string &value);
    void erase(const string &key);
    void clear();
    //将参数转换为URL查询字符串格式
    string toString()const;
    //生成带签名的查询字符串
    string toCryptString();
private:
    map<string,string> _map;
};

//【CurlKits负责网络通信】
class CurlKits{

public:
    CurlKits();
    ~CurlKits();
    void Perform(const string &prehttp,const string &params,const string &header = "");
private:
private:
    CURL *_curl;

};



} // end of namespace SOS;


#endif

