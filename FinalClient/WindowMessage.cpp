#include "WindowMessage.h"
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QBrush>
#include <QDebug>
#include <QColor>
WindowMessage::WindowMessage(QWidget *parent)
    :QWidget(parent)
{
    _pTalbleWidget = new QTableWidget(this);
    _pTalbleWidget->setMinimumSize(350,400);
    setupLogTable(_pTalbleWidget);

}


void WindowMessage::setupLogTable(QTableWidget* table) {
    // ===== 1. 基本表格配置 =====
    table->setColumnCount(2);                     // 两列 (时间 + 消息)
    table->setRowCount(5);                        // 五行数据
    table->setHorizontalHeaderLabels({"时间", "消息"}); // 中文字体支持

    // ===== 2. 样式配置 (深色主题) =====
    QString tableStyle = R"(
        QTableWidget {
            background-color: rgb(55,63,61);           /* 深灰背景 */
            color: #FFFFFF;                       /* 白色文字 */
            font-size: 16px;
            gridline-color: transparent;          /* 隐藏网格线 */
            border: 1px solid #444444;            /* 细边框 */
        }
        QHeaderView::section {
            background-color: #0078D7;            /* 蓝绿色表头 */
            color: white;
            padding: 6px 4px;                     /* 表头内边距 */
            font-weight: bold;
            border: none;
        }
        QTableWidget::item {
            padding: 5px;
            border-bottom: 1px solid #3E3E3E;     /* 行间分隔线 */
        }
    )";
    table->setStyleSheet(tableStyle);
    table->setShowGrid(false);                     // 完全隐藏网格线

    // ===== 3. 填充数据 =====
   /* const QStringList timeData = {
        "00:25:23", "00:25:23", "00:25:19", "00:25:15", "00:25:06"
    };

    const QStringList msgData = {
        "通道 4 关闭视频",
        "按下按钮 banner_btnClose",
        "按下按钮 banner_btnSound",
        "按下按钮 banner_btnMuted",
        "通道 04 打开 主码流"
    };
    // 填充时间列 (左对齐)
    for (int row = 0; row < timeData.size(); ++row) {
        QTableWidgetItem* timeItem = new QTableWidgetItem(timeData[row]);
        timeItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        table->setItem(row, 0, timeItem);
    }

    // 填充消息列 (左对齐)
    for (int row = 0; row < msgData.size(); ++row) {
        QTableWidgetItem* msgItem = new QTableWidgetItem(msgData[row]);
        msgItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        table->setItem(row, 1, msgItem);
    }*/

    // ===== 4. 布局优化 =====
    // 列宽分配 (时间列固定宽度，消息列自动拉伸)
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    // 行高优化
    table->verticalHeader()->setDefaultSectionSize(32); // 统一行高
    table->verticalHeader()->setVisible(false);         // 隐藏行号

    // ===== 5. 交互设置 =====
    table->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁用编辑
    table->setSelectionMode(QAbstractItemView::NoSelection);  // 禁用选择
    table->setFocusPolicy(Qt::NoFocus);                       // 移除焦点边框
}

void WindowMessage::addLogEntry(QTableWidget* table, const QString& time, const QString& message) {
    table->insertRow(0);  // 新条目插入顶部
    qDebug()<<"!!!!!";
    QTableWidgetItem* timeItem = new QTableWidgetItem(time);
    timeItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    table->setItem(0, 0, timeItem);
    qDebug()<<"22222";
    QTableWidgetItem* msgItem = new QTableWidgetItem(message);
    msgItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    table->setItem(0, 1, msgItem);
    qDebug()<<"44444";
    // 保持最多100条记录（可选）
    if(table->rowCount() > 100) table->removeRow(table->rowCount()-1);
}
void WindowMessage::handleMessage(const QString &time,const QString &messsage){
    qDebug()<<"handleMessage recv news :"<<time<<"message is "<<messsage;
    addLogEntry(_pTalbleWidget,time,messsage);
}
