#include "ToolTipWindow.h"
#include <QLabel>
#include <QVBoxLayout>

ToolTipWindow::ToolTipWindow(const QString& text, QWidget* parent)
	:QWidget(parent)
{
    QLabel* label = new QLabel(text, this);
    label->setStyleSheet(""
        "background-color: rgba(35, 35, 35, 200);"
        "color: #FFFFFF;"
        "border-radius: 7px;" // 设置圆角半径
        "padding: 5px;" // 设置内边距，即包边的宽度
    );
    label->setAlignment(Qt::AlignCenter);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(label);

    setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
}

ToolTipWindow::~ToolTipWindow()
{

}
