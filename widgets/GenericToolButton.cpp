#include "GenericToolButton.h"
#include <QStyleOptionButton>
#include <QPainter>

GenericToolButton::GenericToolButton(QWidget* parent)
	: GiantPushButton(parent)
{
    setFixedSize(28, 28);
}

GenericToolButton::GenericToolButton(const QString& text, QWidget* parent)
    :GiantPushButton(text, parent)
{
}

GenericToolButton::~GenericToolButton()
{

}

void GenericToolButton::paintEvent(QPaintEvent* event)
{
    QStyleOptionButton  styleOpt;
    styleOpt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &styleOpt, &painter, this);
    QPushButton::paintEvent(event);
}
