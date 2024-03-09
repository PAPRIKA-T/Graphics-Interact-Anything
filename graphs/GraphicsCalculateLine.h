#pragma once
#include "GraphicsLine.h"

/***************************************测量线段类**************************************************/
//测量线段基类
class CalculateLine : public DoubleEndPointLine
{
    Q_OBJECT
public:
    CalculateLine(QPointF start, QPointF edge);
    CalculateLine();
    void initItem();
    void setCalItemStart(GraphicsItem* cal) {
        cal_disStart = cal;
        if (cal_disStart->data(0) == "BPoint")start_bpoint->setVisible(false);
    } //设置指向计算距离图形项start
    void setCalItemEdge(GraphicsItem* cal) {
        cal_disEdge = cal;
        if (cal_disEdge->data(0) == "BPoint")edge_bpoint->setVisible(false);
    } //设置指向计算距离图形项edge
    GraphicsItem* getCalItemStart() { return cal_disStart; }
    GraphicsItem* getCalItemEdge() { return cal_disEdge; }
    void setStartName(int n) { start_name = n; }
    void setEdgeName(int n) { edge_name = n; }
    int getStartName() { return start_name; }
    int getEdgeName() { return edge_name; }
    virtual QPainterPath shape() const override;
    virtual void onUpdatePointMessage() override;
    virtual void paint(QPainter* painter,
        const QStyleOptionGraphicsItem* option,
        QWidget* widget) override;
    virtual int type() const override
    {
        return itemType::CalculateLineType;
    }

protected:
    GraphicsItem* cal_disStart = nullptr; //指向计算距离图形项start
    GraphicsItem* cal_disEdge = nullptr; //指向计算距离图形项edge
    int start_name = 0;
    int edge_name = 0;
};

//垂直线段类
class CalVerticalLine : public CalculateLine
{
public:
    CalVerticalLine(QPointF start, QPointF edge);
    CalVerticalLine();
    void initItem();
    virtual void paint(QPainter* painter,
        const QStyleOptionGraphicsItem* option,
        QWidget* widget) override;
    virtual void onUpdatePointMessage() override;
    virtual int type() const override
    {
        return itemType::CalVerticalLineType;
    }
    QPointF Verticalpoint = QPointF(0, 0);
    bool vertical_point_is_on_line = true;
};

//线线角度线段类
class CalLineAngleLine : public CalculateLine
{
    Q_OBJECT
public:
    CalLineAngleLine(QPointF start, QPointF edge);
    CalLineAngleLine() :CalculateLine() {
        graphics_text_model.setTextColorNoSelected(QColor(Qt::darkMagenta));
        QPen pen(Qt::darkMagenta, 1);
        pen.setStyle(Qt::DashDotLine);
        graphics_paint_model.getRPenNoSelected() = QPen(pen);
        setData(2, "CalLineAngleLine");
        graphics_text_model.setItemType("AngleLine");
    }
protected:
    virtual void onUpdatePointMessage() override;
    virtual void paint(QPainter* painter,
        const QStyleOptionGraphicsItem* option,
        QWidget* widget) override;
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;
    virtual int type() const override
    {
        return itemType::CalLineAngleLineType;
    }
public slots:
    void angleToArc() {
        if (is_angleToArc)is_angleToArc = false;
        else is_angleToArc = true;
    }
protected:
    bool is_angleToArc = false;
    qreal angle = 0.0;
};
