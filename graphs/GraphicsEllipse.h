#pragma once
#include "GraphicsRect.h"

/***************************************Ellipse Class**************************************************/
//Base Ellipse Class
class BEllipse : public BRect
{
public:
    BEllipse(qreal x, qreal y, qreal width, qreal height);
    BEllipse();
    virtual void paint(QPainter* painter,
        const QStyleOptionGraphicsItem* option,
        QWidget* widget) override;
    virtual QPainterPath shape() const override;
    virtual QPainterPath getStrokePath() override;
    virtual QPainterPath getFillPath()override;
    virtual int type() const override
    {
        return itemType::BEllipseType;
    }
};

class InteractionEllipse : public InteractionRect
{
public:
    InteractionEllipse();
    InteractionEllipse(qreal x, qreal y, qreal width, qreal height);
    virtual void paint(QPainter* painter,
        const QStyleOptionGraphicsItem* option,
        QWidget* widget) override;
    virtual void onUpdatePointMessage() override;
    virtual QPainterPath getStrokePath() override;
    virtual QPainterPath getFillPath()override;
    virtual void onPointSelected() override;
    virtual int type() const override
    {
        return itemType::InteractionEllipseType;
    }
};