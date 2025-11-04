#ifndef ENDSCREEN_H
#define ENDSCREEN_H

#include <QDialog>
#include <QLabel>
#include <QMovie>
#include <QTimer>

class EndScreen:public   QDialog
{
    Q_OBJECT
public:
    explicit EndScreen(QWidget *parent = nullptr);
    ~EndScreen() override;
    void startEndScreen(int duration=2000);
private:
    QLabel *m_animationLabel;   //显示动画的标签
    QMovie *m_movie;            //加载GIF动画
    QTimer *m_timer;            //控制动画时长
};

#endif //ENDSCREEN_H


