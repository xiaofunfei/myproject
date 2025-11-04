#ifndef FFMEPEGKITS_H
#define FFMEPEGKITS_H

#include <QThread>
#include <QImage>
extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>
}
class FFmepegKits : public QThread
{
    Q_OBJECT
public:
    explicit FFmepegKits(QObject *parent=nullptr);
    ~FFmepegKits(){}
    void startPlay(QString url);
    void stop();
protected:
    void run() override;
signals:
    void sigGetOneFrame(QImage Image);
private:
    QString _url;
    volatile bool _isRunning;
};

#endif // FFMEPEGKITS_H
