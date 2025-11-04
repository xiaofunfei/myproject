    #include "MainWindow.h"
    #include "LoginWidget.h"
    #include "LoginInterface.h"
    #include "EndScreen.h"
    //#include "DeviceListWidget.h"
    //#include "CloudControl.h"
    //#include "LeftWidget.h"
    //#include "CentralWidget.h"
    //#include "WindowMessage.h"
    #include <QApplication>

    int main(int argc, char *argv[])
    {
        QApplication a(argc, argv);
        //LoginWidget login;
        //login.show();
        LoginInterface login;
        login.show();
        //MainWindow m;
        //m.show();
        return a.exec();
    }
