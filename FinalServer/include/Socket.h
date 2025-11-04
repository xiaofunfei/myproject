#ifndef __Socket_H__
#define __Socket_H__

#include "NonCopyable.h"
namespace SOS
{

class Socket : NonCopyable
{
public:
    Socket();
    explicit Socket(int);
    ~Socket();

    int fd() const {    return _fd; }
    void shutdownWrite();

private:
    int _fd;
};


}//end of namespace SOS;


#endif

