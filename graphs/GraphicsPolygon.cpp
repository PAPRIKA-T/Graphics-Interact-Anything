#include "GraphicsPolygon.h"

/***************************************Polygon Class**************************************************/

/***********************PolygonPoint Class**************************/
PolygonPoint::PolygonPoint()
    :CPoint{}
{
    initItem();
}

PolygonPoint::PolygonPoint(QPointF p)
    :CPoint{ p }
{
    initItem();
}

void PolygonPoint::initItem()
{
    graphics_paint_model.setIsPaintCenter(true);
    setData(1, "PolygonPoint");
    graphics_text_model.setItemType("PolygonPoint");
    graphics_paint_model.getRPenNoSelected().setColor(graphics_paint_model.getRItemColor());
    graphics_text_model.setIsHideText(true);
    m_center.setPointType(PointType::Other);
}

void PolygonPoint::onActionRemoveSelf()
{
    if (delete_judge && pol->point_list.size() <= 3)
    {
        pol->onActionRemoveSelf();
        return;
    }
    setParentItem(nullptr);
    int index = pol->getPointItemList().indexOf(this);
    if (index == -1) return;
    pol->point_list.removeAt(index);
    pol->point_item_list.removeAt(index);
    pol->graphics_relation_model.removeOneOfChildList(this);
    GraphicsItem::onActionRemoveSelf();
    if (pol->line_list.size() > 0) {
        if (index > 0)pol->line_list.at(index - 1)->onActionRemoveSelf();
        else pol->line_list.last()->onActionRemoveSelf();
    }
    if (pol->point_list.size() > 0) {
        if (index == 0) {
            index = pol->line_list.size();
            pol->line_list.move(0, index - 1);
        }
        PolygonPoint* point = pol->point_item_list[index - 1];
        emit point->sentItemPointF(point->getRCenter(), point);
    }
}

/***********************PolygonLine Class**************************/
PolygonLine::PolygonLine(QPointF start, QPointF edge)
    :BLine(start, edge)
{
    is_rotate_action = false;
    graphics_paint_model.setIsPaintStrokeShape(true);
    graphics_text_model.setIsHideText(true);
    setData(1, "PolygonLine");
    graphics_text_model.setItemType("PolygonLine");
    setSE(start, edge);
}

void PolygonLine::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        if (pol->is_edit_polygon) {
            index = pol->getLineList().indexOf(this);
            if (index >= 0 && index < pol->point_list.size() - 1) {
                pol->point_list.insert(index + 1, event->pos());
                pol->insertPoint(index + 1, event->pos());
                PolygonLine* line = pol->insertLine(index + 1, pol->point_list.at(index + 1), pol->point_list.at(index + 2));
                PolygonPoint* point1 = pol->point_item_list[index + 1];
                PolygonPoint* point2 = pol->point_item_list[index + 2];
                emit point1->sentItemPointF(point1->getRCenter(), point1);
                emit point2->sentItemPointF(point2->getRCenter(), point2);
                line->setPolygon(pol);
            }
            else if (index == pol->point_list.size() - 1) {
                pol->point_list.push_back(event->pos());
                pol->insertPoint(index + 1, event->pos());
                PolygonLine* line = pol->insertLine(index + 1, event->pos(), pol->point_list.at(0));
                PolygonPoint* point1 = pol->point_item_list[index + 1];
                PolygonPoint* point2 = pol->point_item_list[0];
                emit point1->sentItemPointF(point1->getRCenter(), point1);
                emit point2->sentItemPointF(point2->getRCenter(), point2);
                line->setPolygon(pol);
            }
            edit_move = true;
        }
    }
    BLine::mousePressEvent(event);
}

void PolygonLine::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (edit_move){
        pol->point_item_list.at(index + 1)->setCenter(event->pos());
    }
    else {
        BLine::mouseMoveEvent(event);
    }
}

void PolygonLine::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (edit_move)edit_move = false;
    BLine::mouseReleaseEvent(event);
}

QPainterPath PolygonLine::getStrokePath()
{
    return GraphicsItem::getStrokePath();
}

QRectF PolygonLine::boundingRect() const
{
    QPointF lt{};
    lt.setX(m_start.x() < m_edge.x() ? m_start.x() : m_edge.x());
    lt.setY(m_start.y() < m_edge.y() ? m_start.y() : m_edge.y());
    return QRectF(lt, lt + QPointF(abs(m_start.x() - m_edge.x()), abs(m_start.y() - m_edge.y())));
}

void PolygonLine::onActionRemoveSelf()
{
    setParentItem(nullptr);
    if (pol->point_list.size() < pol->getLineList().size())
    {
        pol->getLineList().removeOne(this);
        pol->graphics_relation_model.removeOneOfChildList(this);
        GraphicsItem::onActionRemoveSelf();
    }
    else
    {
        int index = pol->getLineList().indexOf(this);
        if (index < 0)return;
        if (index < pol->getLineList().size() - 1)pol->point_item_list.at(index + 1)->onActionRemoveSelf();
        else pol->point_item_list.at(0)->onActionRemoveSelf();
    }
}

/***********************Polygon Class**************************/
InteractionPolygon::InteractionPolygon()
    :GraphicsItem()
{
    initItem();
}

InteractionPolygon::~InteractionPolygon()
{

}

InteractionPolygon::InteractionPolygon(QList<QPointF> pointList, QGraphicsItem* item, qreal item_scale)
{
    initItem();
    QPointF point_scale(item_scale, item_scale);
    foreach(QPointF p, pointList) {
        ItemPointF m_p(p, PointType::Other);
        m_p = QpointFMultiplication(m_p, point_scale);
        m_p = mapFromItem(item, m_p);
        point_list.push_back(m_p);
    }
    setIsCreateFinished(true);
    graphics_relation_model.setIsHideChildList(true);
    graphics_paint_model.setIsFillItem(true);
    replaceWithItem();
}

InteractionPolygon::InteractionPolygon(std::vector<cv::Point> pointList, QGraphicsItem* item, qreal item_scale)
{
    initItem();
    QPointF point_scale(item_scale, item_scale);
    for (const cv::Point& p : pointList) {
        ItemPointF m_p(p.x, p.y, PointType::Other);
        m_p = QpointFMultiplication(m_p, point_scale);
        m_p = mapFromItem(item, m_p);
        point_list.push_back(m_p);
    }
    setIsCreateFinished(true);
    graphics_relation_model.setIsHideChildList(true);
    replaceWithItem();
}

void InteractionPolygon::initItem()
{
    graphics_paint_model.setIsCloseItem(false);
    setData(0, "polygon");
    is_cal_center_by_se = false;
    graphics_text_model.setItemType("Polygon");
    graphics_text_model.setExtraMessage("polygon");
    graphics_relation_model.setIsHideChildList(false);
    graphics_relation_model.setIsChildConsistentColor(true);
    is_accept_hover_event = false;
    graphics_paint_model.setIsFillItem(false);
}

PolygonLine* InteractionPolygon::createPolygonLine(QPointF start, QPointF edge)
{
    m_line = new PolygonLine(start, edge);
    m_line->setViewScale(graphics_paint_model.getScaleFactor());
    line_list.push_back(m_line);
    m_line->setParentItem(this);
    m_line->setPolygon(this);
    m_line->setGraphicsColor(graphics_paint_model.getRPenNoSelected().color());
    graphics_relation_model.pushBackChildList(m_line);
    m_line->setZValue(1);
    return m_line;
}

PolygonPoint* InteractionPolygon::createPolygonPoint(QPointF p)
{
    m_point = new PolygonPoint(p);
    m_point->setParentGraphicsItem(this, true);
    point_item_list.push_back(m_point);
    m_point->setPolygon(this);
    m_point->setGraphicsColor(graphics_paint_model.getRPenNoSelected().color());
    graphics_relation_model.pushBackChildList(m_point);
    m_point->setViewScale(graphics_paint_model.getScaleFactor());
    m_point->setZValue(2);
    m_point->setCenter(p);
    m_point->onUpdatePointMessage();
    return m_point;
}

PolygonPoint* InteractionPolygon::insertPoint(int index, QPointF p)
{
    if (index >= 0 && index <= point_item_list.size()) {
        m_point = new PolygonPoint(p);
        m_point->setParentGraphicsItem(this, true);
        point_item_list.insert(index, m_point);
        m_point->setPolygon(this);
        m_point->setGraphicsColor(graphics_paint_model.getPenNoSelected().color());
        graphics_relation_model.pushBackChildList(m_point);
        m_point->setViewScale(graphics_paint_model.getScaleFactor());
        m_point->setZValue(2);
        m_point->onUpdatePointMessage();
        return m_point;
    }
    else {
        qDebug() << "Polygon::index wrong!";
        return nullptr;
    }
}

PolygonLine* InteractionPolygon::insertLine(int index, QPointF start, QPointF edge)
{
    if (index >= 0 && index <= line_list.size())
    {
        m_line = new PolygonLine(start, edge);
        m_line->setViewScale(graphics_paint_model.getScaleFactor());
        line_list.insert(index, m_line);
        graphics_relation_model.pushBackChildList(m_line);
        m_line->setParentItem(this);
        m_line->setPolygon(this);
        m_line->setGraphicsColor(graphics_paint_model.getRPenNoSelected().color());
        m_line->setZValue(1);
        return m_line;
    }
    else {
        qDebug() << "LineSegmentLine::index wrong!";
        return nullptr;
    }
}

void InteractionPolygon::replaceWithItem()
{
    for (int i = 0; i < point_list.size() - 1; i++) {
        m_line = createPolygonLine(point_list.at(i), point_list.at(i + 1));
        m_point = createPolygonPoint(point_list.at(i));
    }
    m_line = createPolygonLine(point_list.at(point_list.size() - 1), point_list.at(0));
    m_point = createPolygonPoint(point_list.at(point_list.size() - 1));
    PolygonPoint* point = point_item_list.at(0);
    point->sentItemPointF(point->getRCenter(), point);
}

void InteractionPolygon::updateLineAfterPointMove(int index)
{
    if (index == 0)line_list.back()->setEdge(point_list.at(0));
    else line_list.at(index - 1)->setEdge(point_list.at(index));
}

void InteractionPolygon::updateLineAll()
{
    int k = 0;
    for (int i = 1; i < point_list.size(); i++)
    {
        line_list.at(k)->setSE(point_list.at(i - 1), point_list.at(i));
        k++;
    }
    line_list.at(k)->setSE(point_list.at(point_list.size() - 1), point_list.at(0));
}

void InteractionPolygon::updateLine(int index)
{
    if (index < line_list.size() - 1)
        line_list.at(index)->setSE(point_list.at(index), point_list.at(index + 1));
    else
        line_list.at(index)->setSE(point_list.at(index), point_list.at(0));
}

void InteractionPolygon::setEdit(bool ok)
{
    is_edit_polygon = ok;
}

void InteractionPolygon::receptItemPointF(ItemPointF& p, GraphicsItem* item)
{
    PolygonPoint* pol_point = dynamic_cast<PolygonPoint*>(item);
    int point_index = point_item_list.indexOf(pol_point);
    point_list.replace(point_index, mapFromItem(item, p));//更新点
    //更新线

    if (!is_create_finished) {
        if (point_item_list.size() > 1) {
            if (point_index > 0)
                line_list.at(point_index - 1)->setEdge(mapFromItem(item, p));
            if (point_index < point_item_list.size() - 1)
                line_list.at(point_index)->setStart(mapFromItem(item, p));
        }
    }
    else {
        if (point_index == 0) {
            line_list.last()->setEdge(mapFromItem(item, p));
        }
        else line_list.at(point_index - 1)->setEdge(mapFromItem(item, p));
        line_list.at(point_index)->setStart(mapFromItem(item, p));
    }
    emit updatePointMessage();

}

QRectF InteractionPolygon::boundingRect() const
{
    const qreal adjust_size = graphics_paint_model.getAdjustSize();
    return bod_rect.adjusted(-adjust_size, -adjust_size, 
        adjust_size, adjust_size);
}

void InteractionPolygon::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->setPen(pen());
    if (!is_create_finished) {
        QPolygonF polygon;
        for (int i = 0; i < point_list.size(); i++) polygon << point_list.at(i);
        painter->drawPolyline(polygon);
    }
    else if (!graphics_paint_model.getIsFillItem() && !is_hover_enter) {
        QPolygonF polygon;
        for (int i = 0; i < point_list.size(); i++) polygon << point_list.at(i);
        painter->drawPolygon(polygon);
    }
    GraphicsItem::paint(painter, option, widget);
}

void InteractionPolygon::onUpdatePointMessage()
{
    bod_rect = CoordinateCalculation::getListRect(point_list);
    m_center = CoordinateCalculation::getCentreFromList(point_list);
    m_start.rx() = bod_rect.left();
    m_start.ry() = bod_rect.top();
    GraphicsItem::onUpdatePointMessage();
}

void InteractionPolygon::setIsCreateFinished(bool ok)
{
    is_create_finished = ok;     
    is_accept_hover_event = ok;
    graphics_paint_model.setIsCloseItem(ok);
    graphics_paint_model.setIsFillItem(ok);
}

void InteractionPolygon::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    if (is_create_finished) GraphicsItem::contextMenuEvent(event);
}

QPainterPath InteractionPolygon::shape() const
{
    QPainterPath path;
    QPainterPath path_point;
    path.setFillRule(Qt::WindingFill);
    QPolygonF polygon;
    qreal interaction_range = graphics_paint_model.getInteracitonRange();
    for (int i = 0; i < point_list.size(); i++)
    {
        polygon << point_list.at(i);
        path_point.addEllipse(point_list.at(i), interaction_range, 
            interaction_range);
    }
    path.addPolygon(polygon);
    if (!is_create_finished) path.addPath(path_point);
    path.closeSubpath();
    if (graphics_paint_model.getIsPaintCenter())
        path.addEllipse(m_center, interaction_range, interaction_range);
    return path;
}

QPainterPath InteractionPolygon::getFillPath()
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    QPolygonF polygon;
    for (int i = 0; i < point_list.size(); i++) polygon << point_list.at(i);
    path.addPolygon(polygon);
    path.closeSubpath();
    return path;
}

void InteractionPolygon::pullPoint(const QPointF& p, bool isFinished)
{
    if (isFinished) {
        is_create_finished = true;
        graphics_paint_model.setIsCloseItem(true);
        graphics_relation_model.setIsHideChildList(true);
        graphics_paint_model.setIsFillItem(true);
        is_accept_hover_event = true;
        if (point_list.size() > 1) {
            m_line = createPolygonLine(point_list.last(), point_list.at(0));
        }
    }
    else
    {
        m_edge = mapFromScene(p);
        m_point = createPolygonPoint(m_edge);
        if (point_list.size() > 0) {
            m_line = createPolygonLine(point_list.last(), m_edge);
        }
        point_list.push_back(m_edge);
    }
    emit updatePointMessage();
}

void InteractionPolygon::onActionRemoveSelf()
{
    int size = point_item_list.size();
    for (int i = size - 1; i >= 0; --i) {
        point_item_list[i]->setParentItem(nullptr);
        point_item_list[i]->setDeleteJudge(false);
        point_item_list[i]->onActionRemoveSelf();
    }
    GraphicsItem::onActionRemoveSelf();
}
