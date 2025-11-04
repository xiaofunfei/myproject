#ifndef __USERMANAGER_H
#define __USERMANAGER_H
#include "Message.h"
#include "CameraTask.h"
#include <map>
#include <memory>
using std::map;
using std::unique_ptr;
namespace SOS{
class UserManager{
public:
    void insert(int CameraId,unique_ptr<CameraTask>);
    void erase(int CameraId);
    void stop(int CameraId);
    void start(int CameraId);
private:
    map<int,unique_ptr<CameraTask>>     _manager;
};

}
#endif