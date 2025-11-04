#include "CFramelessWidget.h"
#include <qt_windows.h>
#include <windows.h>
#include <windowsx.h>
#include <QMouseEvent>
#include <QPoint>
#include <QDebug>
CFramelessWidget::CFramelessWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint |
                         Qt::WindowMinMaxButtonsHint);
    setAttribute(Qt::WA_Hover);
}
CFramelessWidget::~CFramelessWidget()
{}
//该函数实现了窗口的拖拽拉伸
bool CFramelessWidget::nativeEvent(const QByteArray& eventType, void*message,
                                   long* result)
{
    MSG* param = static_cast<MSG*>(message);
    switch (param->message)
    {
    case WM_NCHITTEST:
        {
            int nX = GET_X_LPARAM(param->lParam) - this->geometry().x();
            int nY = GET_Y_LPARAM(param->lParam) - this->geometry().y();
            if (nX > m_nBorderWidth && nX < this->width() - m_nBorderWidth &&
                nY > m_nBorderWidth && nY < this->height() - m_nBorderWidth)
            {
                if (childAt(nX, nY) != nullptr)
                    return QWidget::nativeEvent(eventType, message, result);
            }
            if ((nX > 0) && (nX < m_nBorderWidth))
                *result = HTLEFT;
            if ((nX > this->width() - m_nBorderWidth) && (nX < this->width()))
                *result = HTRIGHT;
            if ((nY > 0) && (nY < m_nBorderWidth))
                *result = HTTOP;
            if ((nY > this->height() - m_nBorderWidth) && (nY < this->height()))
                *result = HTBOTTOM;
            if ((nX > 0) && (nX < m_nBorderWidth) && (nY > 0)
                && (nY < m_nBorderWidth))
                *result = HTTOPLEFT;
            if ((nX > this->width() - m_nBorderWidth) && (nX < this->width())
                && (nY > 0) && (nY < m_nBorderWidth))
                *result = HTTOPRIGHT;
            if ((nX > 0) && (nX < m_nBorderWidth)
                && (nY > this->height() - m_nBorderWidth) && (nY < this->height()))
                *result = HTBOTTOMLEFT;
            if ((nX > this->width() - m_nBorderWidth) && (nX < this->width())
                && (nY > this->height() - m_nBorderWidth) && (nY < this->height()))
                *result = HTBOTTOMRIGHT;
            return true;
        }//end of case
    }//end of switch
    return false;
}
void CFramelessWidget::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        // 记录鼠标按下位置（相对于窗口的局部坐标）
        _dragPosition = event->pos();
        //qDebug()<<"_dragPosition:"<<_dragPosition;
    }
    QWidget::mousePressEvent(event);
}

void CFramelessWidget::mouseMoveEvent(QMouseEvent *event){
    if (event->buttons() & Qt::LeftButton) {
        //qDebug()<<"globalpos:"<<event->globalPos();
        QPoint delta = event->globalPos()-_dragPosition;
        //qDebug()<<delta.rx()<<","<<delta.ry();
        // 移动窗口
        move(delta);
    }
    QWidget::mouseMoveEvent(event);
}
