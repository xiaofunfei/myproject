#include "EndScreen.h"
#include <QVBoxLayout>
#include <QDesktopWidget>
EndScreen::EndScreen(QWidget *parent):QDialog(parent)
{
    //去窗口边框
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setModal(true);
    resize(800, 600); // 动画窗口大小
    // 1. 创建容器控件（用于固定图片和文字的相对位置）
    QWidget *contentWidget = new QWidget(this);
    contentWidget->setFixedSize(800, 600); // 容器大小=图片+文字总大小
    // 初始化动画标签
    m_animationLabel = new QLabel(this);
    m_animationLabel->setAlignment(Qt::AlignCenter);
    m_animationLabel->setFixedSize(800,580);
    // 加载GIF动画（替换为你的动画资源，如"loading.gif"）
    // 关键：设置标签内容自动缩放以适应标签大小（按比例拉伸）
    m_animationLabel->setScaledContents(true);
    m_movie = new QMovie(":/resources/icons/zhangwei1.png", QByteArray(), this);
    m_animationLabel->setMovie(m_movie);
    // 3. 文字标签（紧挨着图片下方）
        QLabel *textLabel = new QLabel("保存用户数据...请稍候...", contentWidget);
        textLabel->setFixedSize(800, 20); // 文字高度20px（根据字体大小调整）
        textLabel->setAlignment(Qt::AlignCenter); // 文字居中
        textLabel->setStyleSheet(
            "font-size: 14px;"
            "font-weight: bold;"
            "color: #0b000f;" // 文字颜色（与图片背景匹配）
            "background-color: transparent;" // 透明背景，避免遮挡
        );
        // 4. 容器内布局（垂直排列，图片在上，文字在下，间距为0）
           QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
           contentLayout->addWidget(m_animationLabel);  // 图片
           contentLayout->addWidget(textLabel);         // 文字
           contentLayout->setContentsMargins(0, 0, 0, 0); // 容器内边距为0
           contentLayout->setSpacing(0);                // 图片与文字间距为0（紧挨着）

           // 5. 窗口主布局（让容器居中显示）
           QVBoxLayout *mainLayout = new QVBoxLayout(this);
           mainLayout->addWidget(contentWidget, 0, Qt::AlignCenter); // 容器居中
           mainLayout->setContentsMargins(0, 0, 0, 0); // 窗口边距为0
    // 初始化定时器（控制动画时长）
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &EndScreen::accept);
}
EndScreen::~EndScreen() {
    m_movie->stop();
    delete m_movie;
}
void EndScreen::startEndScreen(int duration) {
    m_movie->start();       // 开始播放动画
    m_timer->start(duration); // 动画持续duration毫秒后关闭窗口
    exec(); // 模态显示
}
