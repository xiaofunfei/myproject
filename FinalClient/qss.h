#ifndef QSS_H
#define QSS_H
#include <string>
using std::string;
string buttonPlayQss = R"(QPushButton{background-image:url(:/resources/playctrlBar/play.svg);
                            background-position:center;
                            background-repeat: no-repeat;
                            border:none;})";
string buttonPauseQss = R"(QPushButton{background-image:url(:/resources/playctrlBar/pause.svg);
                            background-position:center;
                            background-repeat: no-repeat;
                            border:none;})";
string buttonQss = R"(
                   QPushButton{font:26px "Microsoft YaHei";
                        color:rgb(255,255,255);
                        border:none}
                   QPushButton::menu-indicator:open{
                        image:url(:/resources/videoWidget/up_arrow.svg);
                        subcontrol-position:right center;
                        subcontrol-origin:padding;border:none;}
                   QPushButton::menu-indicator:closed{
                        image:url(:/resources/videoWidget/down_arrow.svg);
                        subcontrol-position:right center;
                        subcontrol-origin:padding;border:none;})";
string menuQss = R"(
                 QMenu{background-color:rgb(53, 63, 73);}
                 QMenu::item{font:24px;color:white;
                    background-color:rgb(53, 63, 73);
                    padding:8px 32px;
                    margin:8px 8px;
                 /*border-bottom:1px solid #DBDBDB; item底部颜色*/}
                 /*选择项设置*/
                 QMenu::item:selected{
                    background-color:rgb(54, 54, 54);})";
string menuItemQss = R"(
                 QMenu{background-color:rgb(73, 73, 73);}
                 QMenu::item{font:24px;color:white;
                    background-color:rgb(73, 73, 73);
                    padding:8px 32px;
                    margin:8px 8px;
                    /*border-bottom:1px solid #DBDBDB; item底部颜色*/}
                    /*选择项设置*/
                 QMenu::item:selected{
                    background-color:rgb(54, 54, 54);})";
#endif // QSS_H

