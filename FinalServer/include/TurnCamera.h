#ifndef _TURNCAMERA_H_
#define _TURNCAMERA_H_
#include "Message.h"
#include "TcpConnection.h"
#include "MutexLock.h"
namespace SOS{


class TurnCamera{
public:
    TurnCamera(const TcpConnectionPtr &conn,const Packet &packet):_conn(conn),_packet(packet){}
    void process();        
private:
   bool fillValue(string &,string &,int flag = 0);                                                    
private:
  Packet _packet;        
  TcpConnectionPtr _conn;
};

} //namespace SOS


#endif

