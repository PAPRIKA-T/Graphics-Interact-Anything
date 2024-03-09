#pragma once
#include <QDateTime>
#include "GraphicsTextItem.h"
// 时间显示item
class TimeTextItem : public GraphicsTextItem
{
    Q_OBJECT
public:
    TimeTextItem(QString message) :GraphicsTextItem(message) {}
    TimeTextItem(QPointF p) :GraphicsTextItem(p) {}
    virtual void paint(QPainter* painter,
        const QStyleOptionGraphicsItem* option,
        QWidget* widget) override;
};