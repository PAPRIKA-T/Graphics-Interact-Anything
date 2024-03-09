#pragma once
#include "GraphicsPoint.h"

/***************************************Round Class**************************************************/
//Base Round Class
class BRound : public GraphicsItem
{
public:
    BRound(qreal x, qreal y, qreal r);
    BRound();
    void initItem();
    void setRadius(qreal r);
    qreal getRadius();
    virtual void onUpdatePointMessage() override;
    virtual void paint(QPainter* painter,
        const QStyleOptionGraphicsItem* option,
        QWidget* widget) override;
    virtual QPainterPath shape() const override;
    virtual QPainterPath getStrokePath() override;
    virtual QPainterPath getFillPath()override;
    virtual QRectF boundingRect() const override;
    virtual int type() const override
    {
        return itemType::BRoundType;
    }
protected:
    QRectF bounding_rect;
    qreal radius;
};

//InteractionRound Class
class InteractionRound : public BRound
{
    Q_OBJECT
public:
    InteractionRound(qreal x, qreal y, qreal r);
    InteractionRound();
    EPoint* getEPointItem() { return edge_bpoint; }
    CPoint* getCPointItem() { return center_bpoint; }
    virtual void setEdge(const QPointF&) override;
    virtual void setCenter(const QPointF&) override;
    virtual void setSE(const QPointF& , const QPointF&) override;
    void initItem();
    virtual void onActionRemoveSelf() override;
    virtual int type() const override
    {
        return itemType::InteractionRoundType;
    }
protected:
    EPoint* edge_bpoint = nullptr;
    CPoint* center_bpoint = nullptr;
};
