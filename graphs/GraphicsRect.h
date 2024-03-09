#pragma once
#include "GraphicsPoint.h"
class QPainter;
/***************************************Rect Class**************************************************/
//矩形基类
class BRect : public GraphicsItem
{
    Q_OBJECT
public:
    BRect(qreal x, qreal y, qreal width, qreal height);
    BRect();
    virtual void onUpdatePointMessage() override;
    virtual void paint(QPainter* painter,
        const QStyleOptionGraphicsItem* option,
        QWidget* widget) override;
    virtual QPainterPath shape() const override;
    virtual QPainterPath getStrokePath() override;
    virtual QPainterPath getFillPath()override;
    virtual int type() const override
    {
        return itemType::BRectType;
    }

protected:
    QRectF vision_rect; //可视化的矩形（区别于shape矩形范围）
    ItemPointF right_top; //矩形右上顶点
    ItemPointF left_buttom; //矩形左下顶点
    qreal width = 0.0; //宽
    qreal height = 0.0; //高
};

//端点边线可交互矩形类
class InteractionRect : public BRect
{
    Q_OBJECT
public:
    InteractionRect(qreal x, qreal y, qreal width, qreal height);
    InteractionRect();
    void initItem();
    SPoint* getSPointItem() { return start_bpoint; }
    EPoint* getEPointItem() { return edge_bpoint; }
    CPoint* getRtPointItem() { return rt_bpoint; }
    CPoint* getLbPointItem() { return lb_bpoint; }
    virtual void setStart(const QPointF& s) override;
    virtual void setEdge(const QPointF& e) override;
    virtual void setSE(const QPointF& s, const QPointF& e) override;
    virtual void onActionRemoveSelf() override;
    virtual void paint(QPainter* painter,
        const QStyleOptionGraphicsItem* option,
        QWidget* widget) override;
    virtual void onUpdatePointMessage() override;
    virtual int type() const override
    {
        return itemType::InteractionRectType;
    }
public slots:
    virtual void onPointSelected() override;
    virtual void receptItemPointF(ItemPointF& p, GraphicsItem* item = nullptr) override; //接收item坐标系的一个点
protected:
    SPoint* start_bpoint = nullptr; //起点
    EPoint* edge_bpoint = nullptr; //边缘点
    CPoint* rt_bpoint = nullptr; //右上点
    CPoint* lb_bpoint = nullptr; //左下点
};

//rubberRect
class RubberRect : public BRect
{
    Q_OBJECT
public:
    RubberRect(qreal x, qreal y, qreal width, qreal height);
    virtual void paint(QPainter* painter,
        const QStyleOptionGraphicsItem* option,
        QWidget* widget) override;
    virtual int type() const override
    {
        return itemType::RubberRectType;
    }
};

/***********************Prompt Rect Class**************************/
class PromptRect : public BRect
{
    Q_OBJECT
public:
    PromptRect();
};
