#ifndef __SocketIO_H__
#define __SocketIO_H__
#include "Message.h"
namespace SOS
{
class SocketIO
{
public:
    SocketIO(int fd)
    : _fd(fd)
    {}

    int readn(char * buff, int len);
    int sendn(const char * buff, int len);
    //maxlen：表示一行最大的字节数
    int readline(char * buff, int maxlen);
    int readPacket(Packet &);
    int recvPeek(char * buff, int maxlen) const;

private:
    int _fd;
};

}//end of namespace SOS;


#endif

