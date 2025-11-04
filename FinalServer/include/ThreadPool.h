#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include "Task.h"
#include "TaskQueue.h"
#include "Thread.h"

#include <vector>
#include <memory>

using std::vector;
using std::unique_ptr;

namespace SOS
{


class ThreadPool
{
public:
    ThreadPool(int,int);

    void start();//开启线程池的运行
    void stop();//停止线程池的运行
    //Task是一个函数对象，用右值引用来表示
    void addTask(Task && cb);
    
private:
    //不是接口，放在了私有的区域
    void doTask();//每一个子线程都要做的事儿

private:
    vector<unique_ptr<Thread>>  _threads;
    int                         _threadNum;
    int                         _queSize;
    TaskQueue                   _taskque;
    bool                        _isExit;
};

}//end of namespace SOS

#endif

