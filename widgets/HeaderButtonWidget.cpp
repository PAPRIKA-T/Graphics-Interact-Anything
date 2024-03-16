#include "HeaderButtonWidget.h"
#include "GenericToolButton.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QStyleOption>
#include <QMenu>
#include "widget.h"
#include "graphs/Graphicsview.h"

HeaderButtonWidget::HeaderButtonWidget(QWidget*parent)
	: QWidget(parent)
{
    setFixedHeight(50);

    header_btn_layout = new QHBoxLayout();

    header_btn_layout->addStretch();
    foreach(GenericToolButton * btn, header_btn_list)
    {
        header_btn_layout->addWidget(btn);
        btn->setIconSize(QSize(20, 20));
    }
    header_btn_layout->setSpacing(8);
    header_btn_layout->setContentsMargins(6, 5, 5, 6);

    setLayout(header_btn_layout);
}

HeaderButtonWidget::~HeaderButtonWidget()
{
    delete header_btn_layout;
}

void HeaderButtonWidget::paintEvent(QPaintEvent* event)
{
    QStyleOption styleOpt;
    styleOpt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &styleOpt, &painter, this);
}

void HeaderButtonWidget::setWindowWidget(Widget* widget)
{
    window_widget = widget;
}
