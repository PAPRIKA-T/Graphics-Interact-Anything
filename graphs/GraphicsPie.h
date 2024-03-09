#pragma once
#include "GraphicsRound.h"

/***************************************圆饼类**************************************************/
//圆饼基类
class Pie : public BRound
{
    Q_OBJECT
public:
    Pie();
    Pie(qreal x, qreal y, qreal r);
    virtual void onUpdatePointMessage() override;
    virtual void paint(QPainter* painter,
        const QStyleOptionGraphicsItem* option,
        QWidget* widget) override;
    virtual QPainterPath shape() const override;
    virtual QPainterPath getFillPath() override;
    virtual QRectF boundingRect() const override;
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;
    virtual int type() const override
    {
        return itemType::PieType;
    }
    void setAngleS(qreal s);
    void setAngleE(qreal e);
    qreal getAngleS();
    qreal getAngleE();
    void setAngleToArc(bool ok);
    void setPieToArc(bool ok);
    bool getAngleToArc();
    bool getPieToArc();

public slots:
    void pieToArc();
    void angleToArc();
protected:
    qreal angle_s{};
    qreal angle_e{};
    qreal angle{};
    bool angle_to_arc = false;
    bool pie_to_arc = false;
};

//点可交互圆饼类
class InteractionPie : public Pie
{
    Q_OBJECT
public:
    InteractionPie(qreal x, qreal y, qreal r);
    InteractionPie();
    SPoint* getSPointItem() { return start_bpoint; }
    EPoint* getEPointItem() { return edge_bpoint; }
    CPoint* getCPointItem() { return center_bpoint; }
    virtual void setStart(const QPointF& s) override;
    virtual void setEdge(const QPointF& e) override;
    virtual void setCenter(const QPointF& c) override;
    virtual void setSE(const QPointF& s, const QPointF& e) override;
    virtual void onUpdatePointMessage() override;
    void initItem();
    virtual void onActionRemoveSelf() override;
    virtual int type() const override
    {
        return itemType::InteractionPieType;
    }
protected:
    SPoint* start_bpoint = nullptr; 
    EPoint* edge_bpoint = nullptr; 
    CPoint* center_bpoint = nullptr; 
};