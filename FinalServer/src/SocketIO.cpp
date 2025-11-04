#include "SocketIO.h"
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

namespace SOS
{


int SocketIO::readn(char * buff, int len)
{
    int left = len;
    char * pbuf = buff;
    while(left > 0) {
        int ret = recv(_fd, pbuf, left, 0);
        if(ret == -1 && errno == EINTR) {
            continue;
        } else if(ret == -1) {
            perror("recv");
            return ret;
        } else if(ret == 0){
            return len - left;
        } else {//ret > 0的情况
            left -= ret;
            pbuf += ret;
        }
    }
    return len;
}

int SocketIO::sendn(const char * buff, int len) 
{
    int left = len;
    const char * pbuf = buff;
    while(left > 0) {
        int ret = send(_fd, pbuf, left, 0);
        if(ret < 0) {
            perror("send");
            return ret;
        }else { //ret > 0
            left -= ret;
            pbuf += ret;
        }
    }
    return len;
}

//maxlen：表示一行最大的字节数
int SocketIO::readline(char * buff, int maxlen)
{   //获取的是一行数据, '\n'是作为消息的边界而出现的
    int left = maxlen - 1;
    char * pbuf = buff;
    int total = 0;//表示读取的总的字节数
    while(left > 0) {
        int ret = recvPeek(pbuf, left);
        //查找'\n'
        for(int i = 0; i < ret; ++i) {
            //找到了'\n'
            if(pbuf[i] == '\n') {
                int sz = i + 1;//表示包含\n的字节数
                ret = readn(pbuf, sz);
                total += ret;
                pbuf[i] = '\0';
                return total;
            }
        }
        //这一次没有找到'\n', 从内核接收缓冲区中移走
        ret = readn(pbuf, ret);
        left -= ret;
        pbuf += ret;
        total += ret;
    }
    //没有找到'\n'
    buff[maxlen - 1] = '\0';
    return maxlen - 1;
}

int SocketIO::readPacket(Packet &packet){
    int type,length;
    readn((char *)&type,sizeof(type));
    readn((char*)&length,sizeof(length));
    if(length > 0){
        char *pbuf = new char[length+1]();
        int ret=readn(pbuf,length);
        packet.type=type;
        packet.length=length;
        packet.msg.assign(pbuf,length);
        delete[] pbuf;
        return ret+8;
    }
    //当length为0时,只需要返回TLV的type+length
    return 8;
}

int SocketIO::recvPeek(char * buff, int maxlen) const
{
    int ret = 0;
    do {
        ret = recv(_fd, buff, maxlen, MSG_PEEK);
    }while(ret == -1 && errno == EINTR);
    if(ret < 0) {
        perror("recv");
    }
    return ret;
}
}//end of namespace SOS
