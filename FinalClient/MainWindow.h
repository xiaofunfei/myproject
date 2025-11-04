#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "GlobalUser.h"
#include "CFramelessWidget.h"
#include "EndScreen.h"
#include <QApplication>
#include <QDesktopWidget>
#include <vector>
using std::vector;
class TopWidget;
class LeftWidget;
class CentralWidget;
class RightWidget;
class MainWindow : public CFramelessWidget
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent=nullptr);
protected:
    void keyPressEvent(QKeyEvent *event)override;
private:
    void initUI();
signals:
private slots:
    void closeSlot();//与子对象TopWidget::sigClose信号绑定
    void onFullScreen();
private:
    TopWidget *     _pTopWidget;
    LeftWidget *    _pLeftWidget;
    CentralWidget * _pCentralWidget;
    RightWidget *   _pRightWidget;

};

#endif // MAINWINDOW_H
