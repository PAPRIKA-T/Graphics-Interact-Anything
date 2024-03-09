#pragma once
#include "GraphicsPoint.h"

/***************************************Line Class**************************************************/
/***********************Line Base Class**************************/
class BLine : public GraphicsItem
{
    Q_OBJECT
public:
    BLine(QPointF start, QPointF edge);
    BLine();
    void initItem();
    void setK(qreal gradient) { k = gradient; } //设置k
    qreal& getK() { return k; } //获取k
    virtual void onUpdatePointMessage() override;
    virtual void paint(QPainter* painter,
        const QStyleOptionGraphicsItem* option,
        QWidget* widget) override;
    virtual QPainterPath shape() const override;
    virtual QPainterPath getStrokePath() override;
    virtual int type() const override
    {
        return itemType::BLineType;
    }

protected:
    qreal k = 0; //斜率
};

/***********************DoubleEndPointLine Class**************************/
class DoubleEndPointLine : public BLine
{
    Q_OBJECT
public:
    DoubleEndPointLine(QPointF start, QPointF edge);
    DoubleEndPointLine();
    SPoint* getSPointItem() { return start_bpoint; }
    EPoint* getEPointItem() { return edge_bpoint; }
    virtual void setStart(const QPointF& s) override;
    virtual void setEdge(const QPointF& e) override;
    virtual void setSE(const QPointF&, const QPointF&) override;
    void initItem();
    virtual void onActionRemoveSelf() override;
    virtual int type() const override
    {
        return itemType::DoubleEndPointLineType;
    }
protected:
    SPoint* start_bpoint = nullptr;
    EPoint* edge_bpoint = nullptr;
};

/***********************SingleEndPointLine Class**************************/
class SingleEndPointLine : public BLine
{
    Q_OBJECT
public:
    SingleEndPointLine(QPointF start, QPointF edge);
    SingleEndPointLine();
    EPoint* getEPointItem() { return edge_bpoint; }
    virtual void setEdge(const QPointF& s) override;
    virtual void setSE(const QPointF&, const QPointF&) override;
    void initItem();
    virtual int type() const override
    {
        return itemType::SingleEndPointLineType;
    }
    virtual void onActionRemoveSelf() override;
protected:
    EPoint* edge_bpoint = nullptr; //起点
};

/***********************ParallelLine Class**************************/
class ParallelLine : public DoubleEndPointLine
{
    Q_OBJECT
public:
    ParallelLine();
    ParallelLine(QPointF start, QPointF edge);
    ~ParallelLine() override;
    void initItem();
    virtual void generateOtherItems(const QPointF& pos);
    virtual int type() const override
    {
        return itemType::ParallelLineType;
    }
    DoubleEndPointLine* getAuxiliaryLine() { return auxiliary_line; }
    virtual void onPointSelected() override;
    virtual void onActionRemoveSelf() override;
private:
    DoubleEndPointLine* auxiliary_line = nullptr;
};

