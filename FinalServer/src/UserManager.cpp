#include "UserManager.h"
namespace SOS{
void UserManager::insert(int CameraId,unique_ptr<CameraTask> task){
    //这个地方得传入右值引用
    _manager.emplace(CameraId,std::move(task));
}
void UserManager::erase(int CameraId){
    _manager.erase(CameraId);
}
void UserManager::start(int CameraId){
    auto it=_manager.find(CameraId);
    if(it!=_manager.end()){
        it->second->startTask();
    }else{
        //这个地方可以留给后面的日志记录
        printf("任务不存在！\n");
    }
}
void UserManager::stop(int CameraId){
    auto it=_manager.find(CameraId);
    if(it!=_manager.end()){
        it->second->stopTask();
    }else{
        printf("关闭任务时不存在!\n");
    }
}
}