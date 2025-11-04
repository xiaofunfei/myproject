#ifndef MESSAGE_H
#define MESSAGE_H
#include <string>
#include <QByteArray>
#include <vector>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
using std::string;
enum TaskType{
    TASK_LOGIN_SECTION1 =1,
    TASK_LOGIN_SECTION1_RESP_OK,
    TASK_LOGIN_SECTION1_RESP_ERROR,
    TASK_LOGIN_SECTION2,
    TASK_LOGIN_SECTION2_RESP_OK,
    TASK_LOGIN_SECTION2_RESP_ERROR,

    //register
    TASK_REGISTER_SECTION1,
    TASK_REGISTER_SECTION1_RESP_OK,
    TASK_REGISTER_SECTION1_RESP_ERROR,
    TASK_REGISTER_SECTION2,
    TASK_REGISTER_SECTION2_RESP_OK,
    TASK_REGISTER_SECTION2_RESP_ERROR,
    TASK_USER_READY_DATA,
    TASK_GET_STREAM,
    TASK_STREAM_DATA,
    TYPE_META_DATA,
    TYPE_VIDEO_FRAME,
    TYPE_GET_STREAM_OPEN_ERROR,
    TEST,
    //close用来关闭摄像头获取流的
    CLOSE,
    TASK_TYPE_HTTP_REQUEST_TURN,//20
    TASK_TYPE_HTTP_REQUEST_TURN_RESP_OK,
    TASK_TYPE_HTTP_REQUEST_TURN_RESP_ERROR,
};
struct TLV
{
    uint32_t type;
    uint32_t length;
    char data[1024];
};
struct Packet{
    int type;
    int length;
    string msg;
};
//下面是用来获取解码器的元数据帧
struct VideoMetadata{
    int cameraId;
    int codecId;
    int width;
    int height;
    int timeBaseNum;
    int timeBaseDen;
    string spsSPS;      //H.264/H.265的SPS/PPS数据//
};
struct VideoFrame{
    int         cameraId;
    uint32_t    size;           //数据长度
    int64_t     pts;            //时间戳
    int64_t     dts;            // 解码时间戳（网络字节序）
    std::vector<uint8_t>    data;           //编码数据(指向AVPacket的Data)
};
struct CameraMessage{
    int cameraId;   //摄像头的id
    TaskType type;  //消息类型
    QByteArray data;//AVPacket的原始数据
};
struct DataFragment{
    uint8_t     type;
    uint8_t     isFragmented;
    uint16_t    totalFragments;
    uint16_t    currentFragment;
    uint32_t    packerId;
    uint16_t    dataLen;
    char data[1380];
};
class IMessageHandler
{
public:
    virtual ~IMessageHandler()=default;
    virtual  void handle(VideoFrame & msg)=0;
};
#endif // MESSAGE_H
