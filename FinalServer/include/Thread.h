#ifndef __THREAD_H__
#define __THREAD_H__

#include "NonCopyable.h"
#include <pthread.h>

#include <functional>

namespace SOS
{
using ThreadCallback=std::function<void()>;

//具体类
class Thread : NonCopyable
{
public:
    //参数为右值引用,表示要绑定右值
    Thread(ThreadCallback && cb);
    ~Thread() {}

    void start();//启动一个子线程
    void join();//等待一个子线程运行结束

private:

    //子线程入口函数
    static void * startRoutine(void*);

private:
    pthread_t      _pthid;//线程id
    bool           _isRunning;//线程的状态是否在运行
    ThreadCallback _cb;
};

}//end of namespace SOS


#endif

