#include "TimeTextItem.h"

// paint
void TimeTextItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QDateTime dateTime;
    dateTime = QDateTime::currentDateTime();
    setPlainText(dateTime.toString("yyyy-MM-dd hh:mm:ss ddd"));
    GraphicsTextItem::paint(painter, option, widget);
}