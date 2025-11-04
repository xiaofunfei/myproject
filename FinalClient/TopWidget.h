#ifndef CTITLEBAR_H
#define CTITLEBAR_H
#include <QWidget>
#include <QLabel>
#include <QPushButton>
class CMenuButton;
class TopWidget : public QWidget
{
    Q_OBJECT
public:
        explicit TopWidget(QWidget *parent = nullptr);
signals:
        void sigClose();
        //void sigPbOpen();
private slots:
        void onClickedSlot();
private:
        void initUI();
private:
        QLabel *      _pLogoLabel;
        QLabel *      _pTitleTextLabel;
        QPushButton * _pPlayButton;
        QPushButton * _pReplayButton;
        QPushButton * _pMapButton;
        QPushButton * _pLogButton;
        QPushButton * _pSetButton;
        QPushButton * _pMinButton;
        QPushButton * _pMaxButton;
        QPushButton * _pCloseButton;
};
#endif // CTITLEBAR_H
