QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AnimatedSplashScreen.cpp \
    CFramelessWidget.cpp \
    CVolumeButton.cpp \
    CVolumeSliderDialog.cpp \
    CameraDecoder.cpp \
    CameraHandler.cpp \
    CameraWidget.cpp \
    CentralWidget.cpp \
    CloudControl.cpp \
    CryptString.cpp \
    DeviceList.cpp \
    EndScreen.cpp \
    GlobalUser.cpp \
    LeftWidget.cpp \
    LoginDialog.cpp \
    LoginInterface.cpp \
    LoginWidget.cpp \
    MainWindow.cpp \
    NetWorkReactor.cpp \
    PhotoWarning.cpp \
    RegisterDialog.cpp \
    RightWidget.cpp \
    TopWidget.cpp \
    WindowMessage.cpp \
    main.cpp

HEADERS += \
    AnimatedSplashScreen.h \
    CFramelessWidget.h \
    CVolumeButton.h \
    CVolumeSliderDialog.h \
    CameraDecoder.h \
    CameraHandler.h \
    CameraWidget.h \
    CentralWidget.h \
    CloudControl.h \
    CryptString.h \
    DeviceList.h \
    EndScreen.h \
    GlobalUser.h \
    LeftWidget.h \
    LoginDialog.h \
    LoginInterface.h \
    LoginWidget.h \
    MainWindow.h \
    Message.h \
    NetWorkReactor.h \
    PhotoWarning.h \
    RegisterDialog.h \
    RightWidget.h \
    TopWidget.h \
    WindowMessage.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


#INCLUDEPATH += $$PWD/FFmepeg/include
#LIBS += -L$$PWD/FFmepeg/lib/x86 \
#        -lavcodec \
#        -lavdevice \
#        -lavfilter \
#        -lavformat \
#        -lavutil \
#        -lpostproc \
#        -lswresample \
#        -lswscale

INCLUDEPATH += $$PWD/../../../../SDK/libffmpeg_4.4.r101753_msvc16_x86/include
LIBS += -LE:/SDK/libffmpeg_4.4.r101753_msvc16_x86/lib/x86/ -lavcodec
LIBS += -LE:/SDK/libffmpeg_4.4.r101753_msvc16_x86/lib/x86/ -lavdevice
LIBS += -LE:/SDK/libffmpeg_4.4.r101753_msvc16_x86/lib/x86/ -lavfilter
LIBS += -LE:/SDK/libffmpeg_4.4.r101753_msvc16_x86/lib/x86/ -lavformat
LIBS += -LE:/SDK/libffmpeg_4.4.r101753_msvc16_x86/lib/x86/ -lavutil
LIBS += -LE:/SDK/libffmpeg_4.4.r101753_msvc16_x86/lib/x86/ -lpostproc
LIBS += -LE:/SDK/libffmpeg_4.4.r101753_msvc16_x86/lib/x86/ -lswresample
LIBS += -LE:/SDK/libffmpeg_4.4.r101753_msvc16_x86/lib/x86/ -lswscale



RESOURCES += \
    res.qrc
