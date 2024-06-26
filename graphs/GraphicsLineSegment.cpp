#include "GraphicsLineSegment.h"

/***********************LineSegmentPoint Class**************************/
void LineSegmentPoint::onActionRemoveSelf()
{
    LineSegment* line_seg = dynamic_cast<LineSegment*>(pol);
    if (delete_judge && line_seg->point_list.size() <= 3) {
        line_seg->onActionRemoveSelf();
        return;
    }
    setParentItem(nullptr);
    int index = line_seg->getPointItemList().indexOf(this);
    if (index == -1)return;
    line_seg->point_list.removeAt(index);
    line_seg->point_item_list.removeAt(index);
    line_seg->graphics_relation_model.removeOneOfChildList(this);
    GraphicsItem::onActionRemoveSelf();
    if (line_seg->getLineList().size() > 0) {
        if (index > 0)line_seg->getLineList().at(index - 1)->onActionRemoveSelf();
        else line_seg->getLineList().first()->onActionRemoveSelf();
    }
    if (line_seg->point_list.size() > 0) {
        if (index == 0 || index == line_seg->line_list.size() || line_seg->line_list.size() == 0) return;
        PolygonPoint* point = line_seg->point_item_list[index - 1];
        emit point->sentItemPointF(point->getRCenter(), point);
    }
}

/***********************LineSegmentLine Class**************************/
void LineSegmentLine::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        LineSegment* line_s = dynamic_cast<LineSegment*>(pol);
        if (line_s->is_edit_polygon) {
            index = line_s->getLineList().indexOf(this);
            if (index >= 0 && index < line_s->point_list.size() - 1) {
                line_s->point_list.insert(index + 1, event->pos());
                line_s->insertPoint(index + 1, event->pos());
                LineSegmentLine* line = line_s->insertLine(index + 1, line_s->point_list.at(index + 1), line_s->point_list.at(index + 2));
                LineSegmentPoint* point1 = dynamic_cast<LineSegmentPoint*>(line_s->point_item_list[index + 1]);
                LineSegmentPoint* point2 = dynamic_cast<LineSegmentPoint*>(line_s->point_item_list[index + 2]);
                emit point1->sentItemPointF(point1->getRCenter(), point1);
                emit point2->sentItemPointF(point2->getRCenter(), point2);
                line->setPolygon(line_s);
            }
            edit_move = true;
        }
    }
    BLine::mousePressEvent(event);
}

void LineSegmentLine::onActionRemoveSelf()
{
    LineSegment* line_seg = dynamic_cast<LineSegment*>(pol);
    setParentItem(nullptr);
    if (line_seg->point_list.size() < line_seg->getLineList().size() + 1)
    {
        line_seg->getLineList().removeOne(this);
        line_seg->graphics_relation_model.removeOneOfChildList(this);
        GraphicsItem::onActionRemoveSelf();
    }
    else
    {
        int index = line_seg->getLineList().indexOf(this);
        line_seg->point_item_list.at(index + 1)->onActionRemoveSelf();
    }
}

/***********************LineSegment Class**************************/
LineSegment::LineSegment()
    :InteractionPolygon()
{
    initItem();
}

LineSegment::LineSegment(QList<QPointF> pointList, QGraphicsItem* item, qreal item_scale)
    :InteractionPolygon(pointList, item, item_scale)
{
    initItem();
}

LineSegment::LineSegment(std::vector<cv::Point> pointList, QGraphicsItem* item, qreal item_scale)
    :InteractionPolygon(pointList, item, item_scale)
{
    initItem();
}

void LineSegment::initItem()
{
    graphics_paint_model.setIsCloseItem(false);
    graphics_paint_model.setIsFillItem(false);
    setData(1, "LineSegment");
    graphics_text_model.setItemType("LineSeg");
    graphics_text_model.setExtraMessage("LineSeg");
    graphics_relation_model.setIsHideChildList(false);
    graphics_relation_model.setIsChildConsistentColor(true);
}

LineSegmentPoint* LineSegment::createLineSegmentPoint(QPointF p)
{
    m_point = new LineSegmentPoint(p);
    m_point->setParentGraphicsItem(this, true);
    point_item_list.push_back(m_point);
    m_point->setPolygon(this);
    graphics_relation_model.pushBackChildList(m_point);
    m_point->setGraphicsColor(graphics_paint_model.getRPenNoSelected().color());
    m_point->setViewScale(graphics_paint_model.getScaleFactor());
    m_point->setZValue(2);
    m_point->setCenter(p);
    m_point->onUpdatePointMessage();
    return dynamic_cast<LineSegmentPoint*>(m_point);
}

LineSegmentLine* LineSegment::createLineSegmentLine(QPointF start, QPointF edge)
{
    m_line = new LineSegmentLine(start, edge);
    m_line->setViewScale(graphics_paint_model.getScaleFactor());
    line_list.push_back(m_line);
    graphics_relation_model.pushBackChildList(m_line);
    m_line->setParentItem(this);
    m_line->setPolygon(this);
    m_line->setGraphicsColor(graphics_paint_model.getRPenNoSelected().color());
    m_line->setZValue(1);
    return dynamic_cast<LineSegmentLine*>(m_line);
}

LineSegmentPoint* LineSegment::insertPoint(int index, QPointF p)
{
    if (index >= 0 && index <= point_item_list.size())
    {
        m_point = new LineSegmentPoint(p);
        m_point->setParentGraphicsItem(this, true);
        point_item_list.insert(index, m_point);
        m_point->setPolygon(this);
        graphics_relation_model.pushBackChildList(m_point);
        m_point->setGraphicsColor(graphics_paint_model.getRPenNoSelected().color());
        m_point->setViewScale(graphics_paint_model.getScaleFactor());
        m_point->setZValue(2);
        m_point->onUpdatePointMessage();
        return dynamic_cast<LineSegmentPoint*>(m_point);
    }
    else
    {
        qDebug() << "LineSegmentPoint::index wrong!";
        return nullptr;
    }
}

LineSegmentLine* LineSegment::insertLine(int index, QPointF start, QPointF edge)
{
    if (index >= 0 && index <= line_list.size())
    {
        m_line = new LineSegmentLine(start, edge);
        m_line->setViewScale(graphics_paint_model.getScaleFactor());
        line_list.insert(index, m_line);
        graphics_relation_model.pushBackChildList(m_line);
        m_line->setParentItem(this);
        m_line->setPolygon(this);
        m_line->setGraphicsColor(graphics_paint_model.getRPenNoSelected().color());
        m_line->setZValue(1);
        return dynamic_cast<LineSegmentLine*>(m_line);
    }
    else
    {
        qDebug() << "LineSegmentLine::index wrong!";
        return nullptr;
    }
}

void LineSegment::replaceWithItem()
{
    for (int i = 0; i < point_list.size() - 1; i++) {
        m_line = createLineSegmentLine(point_list.at(i), point_list.at(i + 1));
        m_point = createLineSegmentPoint(point_list.at(i));
    }
    m_point = createLineSegmentPoint(point_list.at(point_list.size() - 1));
}

void LineSegment::pullPoint(const QPointF& p, bool isFinished)
{
    if (isFinished) {
        is_create_finished = true;
    }
    else
    {
        m_edge = mapFromScene(p);
        if (point_list.size() > 0) m_line = createLineSegmentLine(point_list.last(), m_edge);
        point_list.push_back(m_edge);
        m_point = createLineSegmentPoint(m_edge);
    }
    emit updatePointMessage();
}

void LineSegment::receptItemPointF(ItemPointF& p, GraphicsItem* item)
{
    LineSegmentPoint* pol_point = dynamic_cast<LineSegmentPoint*>(item);
    if(pol_point){
        int point_index = point_item_list.indexOf(pol_point);
        point_list.replace(point_index, mapFromItem(item, p));//更新点
        //更新线
        if (point_index == point_list.size() - 1) {
            line_list.last()->setEdge(mapFromItem(item, p));
            return;
        }
        line_list.at(point_index)->setStart(mapFromItem(item, p));
        if (point_index == 0)return;
        line_list.at(point_index - 1)->setEdge(mapFromItem(item, p));
        emit updatePointMessage();
    }
}

QPainterPath LineSegment::shape() const
{
    return InteractionPolygon::shape();
}

void LineSegment::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(painter);
    Q_UNUSED(widget);
}
