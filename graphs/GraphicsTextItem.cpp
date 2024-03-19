#include "GraphicsTextItem.h"

// shape
QPainterPath GraphicsTextItem::shape() const
{
    QPainterPath path;
    path = path.subtracted(path);
    return path;
    //return QGraphicsTextItem::shape();
}

void GraphicsTextItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->setPen(back_ground_color);
    painter->setBrush(back_ground_color);
    painter->drawRect(boundingRect());
    painter->setBrush(Qt::NoBrush);
    QGraphicsTextItem::paint(painter, option, widget);
}

void GraphicsTextItem::setBackGroundColor(const QColor& c)
{
    back_ground_color = c; 
    //back_ground_color.setAlpha(100);
}

