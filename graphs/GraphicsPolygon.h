#pragma once
#include "GraphicsLine.h"

/****************************多边形类**************************************************/
class InteractionPolygon;
//PolygonPoint Class
class PolygonPoint : public CPoint
{
public:
    PolygonPoint();
    PolygonPoint(QPointF p);
    void initItem();
    void setPolygon(InteractionPolygon* p) { pol = p; }
    InteractionPolygon* getPolygon() { return pol; }
    virtual void onActionRemoveSelf() override;
    void setDeleteJudge(bool ok) { delete_judge = ok; }
protected:
    InteractionPolygon* pol = nullptr;
    bool delete_judge = true;
};

//PolygonLine Class
class PolygonLine : public BLine
{
    Q_OBJECT
public:
    PolygonLine(QPointF start, QPointF edge);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    virtual QPainterPath getStrokePath() override;
    virtual QRectF boundingRect() const override;
    virtual int type() const override
    {
        return itemType::PolygonLineType;
    }
    void setPolygon(InteractionPolygon* p) { pol = p; }
    InteractionPolygon* getPolygon() { return pol; }
    virtual void onActionRemoveSelf() override;
protected:
    bool edit_move = false;
    InteractionPolygon* pol = nullptr;
    int index = 0;
};

//Polygon Class
class InteractionPolygon : public GraphicsItem
{
    Q_OBJECT
    friend class PolygonPoint;
    friend class PolygonLine;
public:
    InteractionPolygon();
    ~InteractionPolygon() override;
    InteractionPolygon(QList<QPointF> pointList, QGraphicsItem* item, qreal item_scale);
    InteractionPolygon(std::vector<cv::Point> pointList, QGraphicsItem* item, qreal item_scale);
    void initItem();
    PolygonLine* createPolygonLine(QPointF start, QPointF edge);
    PolygonPoint* createPolygonPoint(QPointF p);
    PolygonPoint* insertPoint(int index, QPointF p);
    PolygonLine* insertLine(int index, QPointF start, QPointF edge);
    virtual void replaceWithItem();
    void updateLineAfterPointMove(int index);
    void updateLine(int index);
    void setEdit(bool ok);
    QList<PolygonLine*>& getLineList() { return line_list; }
    void setLineList(QList<PolygonLine*> l) { line_list = l; }
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter* painter,
        const QStyleOptionGraphicsItem* option,
        QWidget* widget) override;
    virtual void onUpdatePointMessage() override;
    virtual void updateLineAll();
    virtual QPainterPath shape() const override;
    virtual QPainterPath getFillPath()override;
    virtual int type() const override
    {
        return itemType::PolygonType;
    }
    QList<ItemPointF>& getPointList() { return point_list; }
    void setPointList(QList<ItemPointF> l) { point_list = l; }
    bool getIsCreateFinished() { return is_create_finished; }
    void setIsCreateFinished(bool ok);
    void setBodRect(QRectF r) { bod_rect = r; }
    QList<PolygonPoint*>& getPointItemList() { return point_item_list; }
    void setPointItemList(QList<PolygonPoint*> p) { point_item_list = p; }
    bool getIsDrawContinuously() { return draw_continuously; }
    void setIsDrawContinuously(bool ok) { draw_continuously = ok; }
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;

public slots:
    virtual void pullPoint(const QPointF& p, bool isFinished);
    virtual void onActionRemoveSelf() override;
    virtual void receptItemPointF(ItemPointF& p, GraphicsItem* item = nullptr) override; //接收item坐标系的一个点

protected:
    QList<ItemPointF> point_list; // 多边形点链表
    QList<PolygonLine*> line_list; // 线段链表
    QList<PolygonPoint*> point_item_list; //点图形项列表

    PolygonLine* m_line = nullptr; //线段指针
    PolygonPoint* m_point = nullptr; //点指针
    bool is_create_finished = false; //完成绘制
    int point_index = 0; //点索引
    QRectF bod_rect; //bounding矩形
    bool is_edit_polygon = false; //编辑多边形
    bool draw_continuously = true; //按住鼠标连续绘制
};