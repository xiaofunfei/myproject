#ifndef WINDOWMESSAGE_H
#define WINDOWMESSAGE_H

#include <QWidget>
#include <QTableWidget>
class WindowMessage : public QWidget
{
    Q_OBJECT
public:
    explicit WindowMessage(QWidget* parent=nullptr);

    void addLogEntry(QTableWidget* table, const QString& time, const QString& message);
public slots:
    void handleMessage(const QString&time ,const QString &message);
private:
    void setupLogTable(QTableWidget* table);

private:
    QTableWidget *  _pTalbleWidget=nullptr;
};

#endif // WINDOWMESSAGE_H
