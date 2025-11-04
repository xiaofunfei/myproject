#ifndef __InetAddress_H__
#define __InetAddress_H__

#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
using std::string;

namespace SOS
{

class InetAddress
{
public:
    InetAddress(unsigned short port, const string & ip = "0.0.0.0");
    InetAddress(const struct sockaddr_in &);

    string ip() const;
    unsigned short port() const;

    struct sockaddr_in * getInetAddressPtr() {  return &_addr;}

private:
    struct sockaddr_in _addr;
};

}//end of namespace SOS;


#endif

