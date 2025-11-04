#ifndef ANIMATEDSPLASHSCREEN_H
#define ANIMATEDSPLASHSCREEN_H

#include <QDialog>
#include <QLabel>
#include <QMovie>
#include <QTimer>

class AnimatedSplashScreen:public   QDialog
{
    Q_OBJECT
public:
    explicit AnimatedSplashScreen(QWidget *parent = nullptr);
    ~AnimatedSplashScreen() override;
    void startAnimation(int duration=2000);
private:
    QLabel *m_animationLabel;   //显示动画的标签
    QMovie *m_movie;            //加载GIF动画
    QTimer *m_timer;            //控制动画时长
};

#endif // ANIMATEDSPLASHSCREEN_H
