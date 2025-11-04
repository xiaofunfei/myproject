#include "MutexLock.h" //实现文件中第一个要对应的是自定义头文件
#include <pthread.h>

namespace SOS
{

MutexLock::MutexLock()
{
    pthread_mutex_init(&_mutex, NULL);
}

MutexLock::~MutexLock()
{
    pthread_mutex_destroy(&_mutex);
}

void MutexLock::lock()
{
    pthread_mutex_lock(&_mutex);
}

void MutexLock::unLock()
{
    pthread_mutex_unlock(&_mutex);
}

}//end of namespace SOS
