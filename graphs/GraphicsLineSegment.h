#pragma once
#include "GraphicsPolygon.h"

//LineSegmentPoint Class
class LineSegmentPoint :public PolygonPoint
{
public:
    LineSegmentPoint(QPointF p) : PolygonPoint(p) {
        setData(2, "LineSegmentPoint");
        graphics_text_model.setItemType("LineSegmentPoint");
    }
    virtual void onActionRemoveSelf() override;
};

//LineSegmentLine Class
class LineSegmentLine :public PolygonLine
{
public:
    LineSegmentLine(QPointF start, QPointF edge) : PolygonLine(start, edge) {
        setData(2, "LineSegmentLine");
        graphics_text_model.setItemType("LineSegmentLine");
    }
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void onActionRemoveSelf() override;
};

//LineSegment Class
class LineSegment :public InteractionPolygon
{
    Q_OBJECT
    friend class LineSegmentPoint;
    friend class LineSegmentLine;
public:
    LineSegment();
    LineSegment(QList<QPointF> pointList, QGraphicsItem* item, qreal item_scale);
    LineSegment(std::vector<cv::Point> pointList, QGraphicsItem* item, qreal item_scale);
    void initItem();
    LineSegmentPoint* createLineSegmentPoint(QPointF p);
    LineSegmentLine* createLineSegmentLine(QPointF start, QPointF edge);
    LineSegmentPoint* insertPoint(int index, QPointF p);
    LineSegmentLine* insertLine(int index, QPointF start, QPointF edge);
    virtual void replaceWithItem() override;
    virtual void pullPoint(const QPointF& p, bool isFinished) override;
    virtual void receptItemPointF(ItemPointF& p, GraphicsItem* item = nullptr) override;
    virtual QPainterPath shape() const override;
    virtual int type() const override
    {
        return itemType::LineSegmentType;
    }
    virtual void paint(QPainter* painter,
        const QStyleOptionGraphicsItem* option,
        QWidget* widget) override;
};