#ifndef LEFTWIDGET_H
#define LEFTWIDGET_H

#include <QWidget>
#include <QLabel>
class PhotoWarning;
class WindowMessage;
class LeftWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LeftWidget(QWidget *parent = nullptr);
    WindowMessage* getWindowMessage(){
        return _pWindowMessage;
    }
signals:
private:
    void initUI();
private:
    QLabel*          _pTextLabel;
    WindowMessage *  _pWindowMessage;
    QLabel*          _pTextLabel2;
    PhotoWarning*    _pPhotoWarning;
};

#endif // LEFTWIDGET_H
