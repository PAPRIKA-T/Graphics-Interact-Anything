#ifndef BASICSHAPE_H
#define BASICSHAPE_H

#include "GraphicsItem.h"
#include <QGraphicsScene>

/***************************************Point Class**************************************************/
/***********************Point Base Class**************************/
class BPoint : public GraphicsItem
{
    Q_OBJECT
public:
    BPoint();
    BPoint(QPointF p);
    void initItem();
    void setParentGraphicsItem(GraphicsItem* parent, bool is_depend_with_parent);
    void setIsGrab(bool ok); //是否能选中端点
    bool getIsGrab(); 
    virtual void onUpdatePointMessage() override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
protected:
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
    virtual QRectF boundingRect() const override;
    virtual int type() const override
    {
        return itemType::BPointType;
    }
signals:
    void pointSelected();

public:
    bool is_grab_point = true; //是否能选中端点
    QList<PointPaintType> point_paint_type{}; //交互点绘制类型
};

/***********************Start Point Class**************************/
class SPoint : public BPoint
{
public:
    SPoint();
    SPoint(QPointF p);
    void initItem();
    virtual void paint(QPainter* painter,
        const QStyleOptionGraphicsItem* option,
        QWidget* widget) override;
    virtual void setStart(const QPointF& p) override;
    virtual QRectF boundingRect() const override;
    virtual void onUpdatePointMessage() override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
};

/***********************Edge Point Class**************************/
class EPoint : public BPoint
{
public:
    EPoint();
    EPoint(QPointF p);
    void initItem();
    virtual void setEdge(const QPointF& p) override;
    virtual void onUpdatePointMessage() override;
    virtual QRectF boundingRect() const override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void paint(QPainter* painter,
        const QStyleOptionGraphicsItem* option,
        QWidget* widget) override;
};

/***********************Center Point Class**************************/
class CPoint : public BPoint
{
public:
    CPoint();
    CPoint(QPointF p);
    void initItem();
    virtual void setCenter(const QPointF& p) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
};

/***********************Text Point Class**************************/
class TextPoint : public BPoint
{
    Q_OBJECT
public:
    TextPoint(QPointF p);
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
    virtual int type() const override
    {
        return itemType::TextPointType;
    }
};

/***********************Positive Point Class**************************/
//positive point for segment anything
class PositivePoint : public BPoint
{
public:
    PositivePoint();
};

/***********************Negative Point Class**************************/
class NegativePoint : public BPoint
{
public:
    NegativePoint();
};

#endif // BASICSHAPE_H
