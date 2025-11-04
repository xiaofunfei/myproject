#ifndef __USERSERVICE_H
#define __USERSERVICE_H
#include "Message.h"
#include "TcpConnection.h"
#include "MySQLClient.h"

namespace SOS{
class UserLoginSection1{
public:
    UserLoginSection1(TcpConnectionPtr conn,const Packet & p)
    :_conn(conn)
    ,_pack(p)
    ,_sql()
    {}
    ~UserLoginSection1(){}
    void process();
private:
    TcpConnectionPtr _conn;
    Packet           _pack;
    MySQLClient      _sql;
};
class UserLoginSection2{
public:
    UserLoginSection2(TcpConnectionPtr conn,const Packet &p)
    :_conn(conn)
    ,_pack(p)
    ,_sql()
    {}
    ~UserLoginSection2(){}
    void process();
private:
    TcpConnectionPtr _conn;
    Packet           _pack;
    MySQLClient      _sql;
};
class UserRegisterSection1{
public:
    UserRegisterSection1(TcpConnectionPtr conn,const Packet &p)
    :_conn(conn)
    ,_pack(p)
    ,_sql()
    {}
    ~UserRegisterSection1(){}
    void process();
private:
    TcpConnectionPtr _conn;
    Packet           _pack;
    MySQLClient      _sql;
};
class UserRegisterSection2{
public:
    UserRegisterSection2(TcpConnectionPtr conn,const Packet &p)
    :_conn(conn)
    ,_pack(p)
    ,_sql()
    {}
    ~UserRegisterSection2(){}
    void process();
private:
    TcpConnectionPtr _conn;
    Packet           _pack;
    MySQLClient      _sql;
};
}
#endif