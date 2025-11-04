#ifndef CFRAMELESSWIDGET_H
#define CFRAMELESSWIDGET_H
#include <QWidget>
class CFramelessWidget : public QWidget
{
    Q_OBJECT
public:
        CFramelessWidget(QWidget *parent = nullptr);
        ~CFramelessWidget();
protected:
        bool nativeEvent(const QByteArray& eventType, void* message, long*
                         result) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
private:
        int m_nBorderWidth = 5;
        QPoint        _dragPosition;    // 鼠标按下位置（相对窗口）
};
#endif // CFRAMELESSWIDGET_H
