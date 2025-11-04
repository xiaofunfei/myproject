#ifndef VLCKITSH_H
#define VLCKITSH_H
#include <QObject>
#include <vlc/vlc.h>
#include <vector>
using std::vector;
typedef enum{
    NOTHING,
    PLAYING,
    PAUSED,
    STOPPED,
    OTHERS
}MediaPlayerState_t;
class VLCKits : public QObject
{
    Q_OBJECT
public:
        explicit VLCKits(QObject *parent = nullptr);
        ~VLCKits();
        bool initVLC();
        bool play(const QString & name, void * hwnd);
        void play();
        void pause();
        void stop();
        MediaPlayerState_t getMediaPlayerState();
        void setVolume(int value);
        void setPosition(int value);
        libvlc_media_player_t * mediaPlayer() const { return _pMediaPlayer; }
        libvlc_media_list_t * mediaPlayList() const { return _pMediaList; }
        libvlc_time_t durations() const { return _totalSec; }
        //void setVolumePos(int value);
        //播放列表添加的函数
        bool play(const QStringList & names, void * hwnd);
        //void addMediaIndex();
        //int currentIndex() const { return _currentIndex; }
        const vector<libvlc_time_t> & durationArr() const
        { return _durationArr; }
        void playByIndex(int index,const QStringList & names,void * hwnd);
        int getCurrentIndex();
        int getTotalItemsCount();
        bool addMediaToMediaList(const QStringList & names);
        void clearMediaList();
signals://自定义信号
        void sigTimeSliderPos(int value);
        void sigUsedTimeText(const QString & str);
        void sigRemainTimeText(const QString & str);
        void sigVolumeSliderPos(int value);
private:
        libvlc_instance_t *         _pInstance = nullptr;
        libvlc_media_t *            _pMedia = nullptr;
        libvlc_media_player_t *     _pMediaPlayer = nullptr;
        libvlc_event_manager_t *    _pEventManager = nullptr;
        libvlc_time_t               _totalSec = 0;//一个文件的总时间

        //播放列表添加的成员
        libvlc_media_list_player_t *_pMediaListPlayer = nullptr;
        libvlc_media_list_t *       _pMediaList = nullptr;
        vector<libvlc_time_t>       _durationArr;
        int                         _currentIndex = -1;//表示当前播放的媒体下标
        QStringList                 _filePaths;
};

#endif // VLCKITSH_H
