#ifndef CVOLUMESLIDERDIALOG_H
#define CVOLUMESLIDERDIALOG_H
#include <QDialog>
#include <QSlider>
#include <QLabel>
class CVolumeSliderDialog : public QDialog
{
    Q_OBJECT
public:
        explicit CVolumeSliderDialog(QWidget * parent = Q_NULLPTR);
        ~CVolumeSliderDialog();
        void  setSliderValue(int);
        int   value();
signals:
        void sigVolumeSliderMoved(int);
private:
        QSlider * _pVolumeSlider = nullptr;
        QLabel *  _pVolumeText = nullptr;
};
#endif // CVOLUMESLIDERDIALOG_H
