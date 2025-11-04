#ifndef __MUTEXLOCK_H__
#define __MUTEXLOCK_H__

#include "NonCopyable.h"       
#include <pthread.h>

namespace SOS
{


class MutexLock : NonCopyable
{
public:
    MutexLock();
    ~MutexLock();

    void lock();
    void unLock();
    pthread_mutex_t * getMutexLockPtr() 
    {   return &_mutex; }
private:
    pthread_mutex_t _mutex;
};

class MutexLockGuard
{
public:
    MutexLockGuard(MutexLock & m)
    : _mutex(m)
    {
        _mutex.lock();
    }

    ~MutexLockGuard()
    {
        _mutex.unLock();
    }
private:
    MutexLock & _mutex;

};

}//end of namespace SOS


#endif

