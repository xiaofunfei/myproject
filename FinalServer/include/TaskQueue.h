#ifndef __TASKQUEUE_H__
#define __TASKQUEUE_H__

#include "Task.h"
#include "MutexLock.h"
#include "Condition.h"

#include <queue>
using std::queue;


namespace SOS
{

using ElemType = Task;

class TaskQueue
{
public:
    TaskQueue(int);
    bool empty() const;
    bool full() const;
    void push(ElemType);
    ElemType pop();
    void wakeUp();

private:
    queue<ElemType>  _que;
    int         _queSize;
    MutexLock   _mutex;
    Condition   _notFull;
    Condition   _notEmpty;
    bool        _flag;
};


}//end of namespace SOS


#endif

