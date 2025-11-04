#include "VLCKits.h"
#include <QDebug>
#include <QDir>
#include <QUrl>
VLCKits::VLCKits(QObject *parent)
: QObject(parent)
,_durationArr()
,_filePaths()
{
    initVLC();
}
VLCKits::~VLCKits() {
    libvlc_media_player_release(_pMediaPlayer);
    libvlc_media_release(_pMedia);
    libvlc_release(_pInstance);
}
static void processPosition(VLCKits * pkits)
{
    //百分比
    float pos = libvlc_media_player_get_position(pkits->mediaPlayer());
    emit pkits->sigTimeSliderPos(pos*100);
    vector<libvlc_time_t> vec=pkits->durationArr();
    int index=pkits->getCurrentIndex();
    libvlc_time_t totalSecs = vec[index];
    libvlc_time_t usedSecs =
            libvlc_media_player_get_time(pkits->mediaPlayer()) / 1000;
    libvlc_time_t remainSecs = totalSecs-usedSecs;
    int hh = usedSecs / 3600;
    int mm = usedSecs % 3600 / 60;
    int ss = usedSecs % 60;
    int HH = remainSecs / 3600;
    int MM = remainSecs % 3600 / 60;
    int SS = remainSecs % 60;
    char usedBuff[64] = {0};
    sprintf(usedBuff, "%2d:%02d:%02d",hh, mm, ss);
    QString str(usedBuff);
    char remainBuff[64] = {0};
    sprintf(remainBuff, "%2d:%02d:%02d",HH, MM, SS);
    QString str2(remainBuff);

    emit pkits->sigUsedTimeText(str);
    emit pkits->sigRemainTimeText(str2);
}
static void processVolume(VLCKits * pkits)
{
    int val = libvlc_audio_get_volume(pkits->mediaPlayer());
    qDebug() << "volume:" << val;
    if(val == -1) {
        return;
    }
    emit pkits->sigVolumeSliderPos(val);
}
static void processMediaChanged(VLCKits* pkits)
{
    qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>> processMediaChanged()";
    //获取当前新媒体
    libvlc_media_t *media = libvlc_media_player_get_media(pkits->mediaPlayer());
    if(media){

        //emit sigCurMediaChanged();
    }
}
//void VLCKits::addMediaIndex() {
//    ++_currentIndex;
//    _currentIndex %= _durationArr.size();
//}
static void vlc_callback( const struct libvlc_event_t *p_event, void *p_data)
{
    VLCKits * pkits = static_cast<VLCKits*>(p_data);
    if(pkits) {
        switch(p_event->type) {
        case libvlc_MediaPlayerPositionChanged://播放位置变化时
            processPosition(pkits); break;
        case libvlc_MediaPlayerAudioVolume://音量变化时
            qDebug()<<"volumeChanged";
            processVolume(pkits); break;
        case libvlc_MediaPlayerMediaChanged://播放的媒体变化时
            qDebug()<<"mediaChanged";
            processMediaChanged(pkits); break;
        }
    }
}
bool VLCKits::initVLC()
{
    _pInstance = libvlc_new(0, NULL);
    if(_pInstance) {
        _pMediaPlayer = libvlc_media_player_new(_pInstance);
        if(_pMediaPlayer) {
            libvlc_audio_set_volume(_pMediaPlayer, 50);
            _pEventManager =
                libvlc_media_player_event_manager(_pMediaPlayer);
            if(_pEventManager) {
                //注册媒体播放器的事件处理器
                //1. 位置发生改变时
                libvlc_event_attach(_pEventManager,
                                    libvlc_MediaPlayerPositionChanged,
                                    vlc_callback, this);
                //2. 声音变化时
                libvlc_event_attach(_pEventManager,
                                    libvlc_MediaPlayerAudioVolume,
                                    vlc_callback, this);
                //3. 媒体改变时
                libvlc_event_attach(_pEventManager,
                                    libvlc_MediaPlayerMediaChanged,
                                    vlc_callback, this);
                return true;
            } else {
                libvlc_media_player_release(_pMediaPlayer);
                libvlc_release(_pInstance);
                return false;
            }
        } else {
            libvlc_release(_pInstance);
            return false;
        }
    } else {
        return false;
    }
}
void VLCKits::playByIndex(int index,const QStringList & names,void* hwnd){
    if(_pMediaListPlayer){
        int ret = libvlc_media_list_player_play_item_at_index(_pMediaListPlayer,index);
        if(ret==-1){
            qDebug()<<"文件不在列表中";
            return;
        }
    }else{
        play(names,hwnd);
    }

}
bool VLCKits::play(const QStringList &paths, void *hwnd)
{
    _pMediaListPlayer = libvlc_media_list_player_new(_pInstance);
    if(!_pMediaListPlayer) {
        return false;
    }

    _pMediaList = libvlc_media_list_new(_pInstance);
    if(!_pMediaList) {
        return false;
    }
    addMediaToMediaList(paths);
    //列表播放器关联媒体列表
    libvlc_media_list_player_set_media_list(_pMediaListPlayer, _pMediaList);
    //设置播放模式：列表循环播放
    libvlc_media_list_player_set_playback_mode(_pMediaListPlayer,
                                               libvlc_playback_mode_loop);
    //媒体列表播放器关联媒体播放器(必须加)
    libvlc_media_list_player_set_media_player(_pMediaListPlayer,
                                              _pMediaPlayer);
    libvlc_media_player_set_hwnd(_pMediaPlayer, hwnd);//设置播放窗口
    libvlc_media_list_player_play(_pMediaListPlayer);//开始播放
    return true;
}
bool VLCKits::addMediaToMediaList(const QStringList &names){
    _filePaths+=names;
    for(int i = 0; i < names.size(); ++i) {
        QString filename = names[i];
        filename = QDir::toNativeSeparators(filename);
        _pMedia = libvlc_media_new_path(_pInstance,
                                        filename.toStdString().c_str());
        if(!_pMedia) {
            return false;
        }
        //将媒体添加到播放列表中
        libvlc_media_list_add_media(_pMediaList, _pMedia);
        //解析媒体元数据
        libvlc_media_parse(_pMedia);
        libvlc_time_t durationSecs = libvlc_media_get_duration(_pMedia) /1000;
        _durationArr.push_back(durationSecs);
        libvlc_media_release(_pMedia);
    }
    return true;
}
bool VLCKits::play(const QString &name, void *hwnd)
{
    libvlc_media_player_stop(_pMediaPlayer);
    //1. 设置媒体
    _pMedia = libvlc_media_new_path(_pInstance, name.toStdString().c_str());
    if(!_pMedia) {
        return false;
    }
    //得到了媒体文件后
    //2. 解析文件
    libvlc_media_parse(_pMedia);
    _totalSec = libvlc_media_get_duration(_pMedia) / 1000;
    qDebug()<<"total time:"<<_totalSec;
    //3. 设置媒体播放器
    libvlc_media_player_set_media(_pMediaPlayer, _pMedia);
    libvlc_media_release(_pMedia); // 设置后释放，因为媒体播放器会自己持有
    //4. 设置播放窗口句柄
    libvlc_media_player_set_hwnd(_pMediaPlayer, hwnd);
    //5. 播放视频

    int ret = libvlc_media_player_play(_pMediaPlayer);
    if(ret < 0) {
        return false;
    } else{
        return true;
    }
}
void VLCKits::play()
{
    MediaPlayerState_t state=getMediaPlayerState();
    if(state!=NOTHING){
        if(state==PAUSED||state==STOPPED){
            libvlc_media_player_play(_pMediaPlayer);
        }
    }
}
void VLCKits::pause()
{
    MediaPlayerState_t state=getMediaPlayerState();
    if(state!=NOTHING){
        if(state==PLAYING){
            libvlc_media_player_pause(_pMediaPlayer);
        }
    }
}
void VLCKits::stop()
{
    MediaPlayerState_t state=getMediaPlayerState();
    if(state!=NOTHING){
        if(state==PLAYING||state==PAUSED){
            libvlc_media_player_stop(_pMediaPlayer);
            emit sigTimeSliderPos(0);
        }
    }
}
void VLCKits::setVolume(int value)
{
    libvlc_audio_set_volume(_pMediaPlayer, value);
}
void VLCKits::setPosition(int value)
{
    libvlc_media_player_set_position(_pMediaPlayer, value / 100.0);
}

MediaPlayerState_t VLCKits::getMediaPlayerState(){

    libvlc_state_t state = libvlc_media_player_get_state(_pMediaPlayer);
    if(state==libvlc_Playing){
        return PLAYING;
    }else if(state==libvlc_Paused){
        return PAUSED;
    }else if(state==libvlc_Stopped){
        return STOPPED;
    }else if(state==libvlc_NothingSpecial){
        return NOTHING;
    }else{
        return OTHERS;
    }
}
int VLCKits::getCurrentIndex(){
        // 获取当前播放的媒体
        libvlc_media_t *currentMedia = libvlc_media_player_get_media(
            libvlc_media_list_player_get_media_player(_pMediaListPlayer)
        );

        // 在列表中查找索引（索引从 0 开始）
        int currentIndex = libvlc_media_list_index_of_item(_pMediaList, currentMedia);

        if (currentIndex == -1) {
            qDebug()<<"未找到";
            return 0;
        }
        return currentIndex;

}
int VLCKits::getTotalItemsCount(){
    return libvlc_media_list_count(_pMediaList);
}
void VLCKits::clearMediaList(){
    _filePaths.clear();
    libvlc_media_list_release(_pMediaList); // 释放原有列表
    _pMediaList=nullptr;
    libvlc_media_list_player_release(_pMediaListPlayer);
    _pMediaListPlayer=nullptr;
}
