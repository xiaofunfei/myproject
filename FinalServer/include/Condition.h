#ifndef __CONDITION_H__
#define __CONDITION_H__

#include "NonCopyable.h"

#include <pthread.h>

namespace SOS
{

class MutexLock;//类的前向声明, 防止出现头文件的循环依赖

class Condition : NonCopyable
{
public:
    Condition(MutexLock &);
    ~Condition();

    void wait();
    void notifyOne();
    void notifyAll();

private:
    pthread_cond_t _cond;
    MutexLock & _mutex;//引用占据的空间是一个指针的大小
};

}//end of namespace SOS


#endif

