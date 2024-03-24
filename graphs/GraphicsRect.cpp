#include "GraphicsRect.h"
#include "utils/ColorOperation.h"
/***************************************Rect Class**************************************************/
/***********************Rect Base Class**************************/
BRect::BRect(qreal x, qreal y, qreal width, qreal height)
    : GraphicsItem(QPointF(x, y), QPointF((x + width), (y + height)))
{
    right_top = QPointF(x + width, y);
    left_buttom = QPointF(x, y + height);
    setData(0, "BRect");
    graphics_text_model.setItemType("Bndbox");
    set_point_fp_list.push_back(std::make_pair(this, &GraphicsItem::setStart));
    set_point_fp_list.push_back(std::make_pair(this, &GraphicsItem::setEdge));
}

BRect::BRect()
    : GraphicsItem()
{
    graphics_paint_model.setIsCloseItem(true);
    right_top = QPointF(m_start.x() + width, m_start.y());
    left_buttom = QPointF(m_start.x(), m_start.y() + height);
    setData(0, "BRect");
    graphics_text_model.setItemType("Bndbox");
    set_point_fp_list.push_back(std::make_pair(this, &GraphicsItem::setStart));
    set_point_fp_list.push_back(std::make_pair(this, &GraphicsItem::setEdge));
}

void BRect::onUpdatePointMessage()
{
    QPointF left_top, right_bottom;
    if (m_start.x() < m_edge.x()) {
        left_top.setX(m_start.x());
        right_bottom.setX(m_edge.x());
    }
    else {
        left_top.setX(m_edge.x());
        right_bottom.setX(m_start.x());
    }
    if (m_start.y() < m_edge.y()) {
        left_top.setY(m_start.y());
        right_bottom.setY(m_edge.y());
    }
    else {
        left_top.setY(m_edge.y());
        right_bottom.setY(m_start.y());
    }
    vision_rect = QRectF(left_top, right_bottom);

    GraphicsItem::onUpdatePointMessage();
    width = abs((m_edge_map_to_measure_ob).rx() - m_start_map_to_measure_ob.rx());
    height = abs(m_edge_map_to_measure_ob.ry() - m_start_map_to_measure_ob.ry());
    QString str1 = "width:" + QString::number(width, 'f', 0) + "px ";
    QString str2 = "height:" + QString::number(height, 'f', 0) + "px ";
    graphics_text_model.setExtraMessage(str1 + "\n" + str2);
}

void BRect::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->setPen(pen());
    painter->drawRect(vision_rect);
    GraphicsItem::paint(painter, option, widget);
}

QPainterPath BRect::shape() const
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    const qreal adjust_size = graphics_paint_model.getAdjustSize();
    path.addRect(vision_rect.adjusted(-adjust_size, -adjust_size, 
        adjust_size, adjust_size));
    return path;
}

QPainterPath BRect::getStrokePath()
{
    QPainterPath path;
    const qreal adjust_size = graphics_paint_model.getAdjustSize();
    path.addRect(vision_rect
        .adjusted(-adjust_size, -adjust_size, 
           adjust_size, adjust_size));
    return path;
}

QPainterPath BRect::getFillPath()
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(vision_rect);
    return path;
}

/***********************InteractionRect Class**************************/
InteractionRect::InteractionRect(qreal x, qreal y, qreal width, qreal height)
    : BRect(x, y, width, height)
{
    initItem();
    start_bpoint->setStart(QPoint(x, y));
    edge_bpoint->setEdge(QPoint(x + width, y + height));
}

InteractionRect::InteractionRect()
    : BRect()
{
    initItem();
}

void InteractionRect::initItem()
{
    setData(1, "InteractionRect");
    start_bpoint = new SPoint();
    edge_bpoint = new EPoint();
    rt_bpoint = new CPoint();
    lb_bpoint = new CPoint();
    rt_bpoint->setGraphicsColor(DEFAULT_COLOR_POINT_NOSELECTED);
    lb_bpoint->setGraphicsColor(DEFAULT_COLOR_POINT_NOSELECTED);
    rt_bpoint->getRCenter().setPointType(PointType::RightTop);
    lb_bpoint->getRCenter().setPointType(PointType::LeftBottom);
    start_bpoint->setParentGraphicsItem(this, true);
    edge_bpoint->setParentGraphicsItem(this, true);
    rt_bpoint->setParentGraphicsItem(this, true);
    lb_bpoint->setParentGraphicsItem(this, true);
    connect(start_bpoint, &BPoint::pointSelected, this, &InteractionRect::onPointSelected);
    connect(edge_bpoint, &BPoint::pointSelected, this, &InteractionRect::onPointSelected);
    connect(rt_bpoint, &BPoint::pointSelected, this, &InteractionRect::onPointSelected);
    connect(lb_bpoint, &BPoint::pointSelected, this, &InteractionRect::onPointSelected);

    graphics_relation_model.pushBackChildList(start_bpoint);
    graphics_relation_model.pushBackChildList(edge_bpoint);
    graphics_relation_model.pushBackChildList(rt_bpoint);
    graphics_relation_model.pushBackChildList(lb_bpoint);
    graphics_relation_model.setIsHideChildList(true);

}

void InteractionRect::receptItemPointF(ItemPointF& p, GraphicsItem* item)
{
    if (p.getPointType() == PointType::RightTop)
    {
        if (item) right_top = mapFromItem(item, p);
        else right_top = p;
    }
    else if (p.getPointType() == PointType::LeftBottom)
    {
        if (item) left_buttom = mapFromItem(item, p);
        else left_buttom = p;
    }
    GraphicsItem::receptItemPointF(p, item);
}

void InteractionRect::setStart(const QPointF& s)
{
    GraphicsItem::setStart(s);
    start_bpoint->setStart(s);
}

void InteractionRect::setEdge(const QPointF& e)
{
    GraphicsItem::setEdge(e);
    edge_bpoint->setEdge(e);
}

void InteractionRect::setSE(const QPointF& s, const QPointF& e)
{
    GraphicsItem::setSE(s, e);
    start_bpoint->setStart(s);
    edge_bpoint->setEdge(e);
}

void InteractionRect::onActionRemoveSelf()
{
    removeChildItem(&start_bpoint);
    removeChildItem(&start_bpoint);
    removeChildItem(&rt_bpoint);
    removeChildItem(&lb_bpoint);
    GraphicsItem::onActionRemoveSelf();
}

void InteractionRect::onPointSelected()
{
    if (rt_bpoint->isSelected())
    {
        start_bpoint->setStart(mapToItem(start_bpoint, QPointF(left_buttom.x(), right_top.y())));
        edge_bpoint->setEdge((mapToItem(edge_bpoint, QPointF(right_top.x(), left_buttom.y()))));
    }
    else if (lb_bpoint->isSelected())
    {
        start_bpoint->setStart(mapToItem(start_bpoint, QPointF(left_buttom.x(), right_top.y())));
        edge_bpoint->setEdge((mapToItem(edge_bpoint, QPointF(right_top.x(), left_buttom.y()))));
    }
    else
    {
        rt_bpoint->setCenter(mapToItem(rt_bpoint, QPointF(m_edge.x(), m_start.y())));
        lb_bpoint->setCenter(mapToItem(lb_bpoint, QPointF(m_start.x(), m_edge.y())));
    }
}

void InteractionRect::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->setPen(pen());
    painter->drawRect(vision_rect);
    GraphicsItem::paint(painter, option, widget); // clazy:exclude=skipped-base-method
}

void InteractionRect::onUpdatePointMessage()
{
    QPointF left_top, right_bottom;
    if (m_start.x() < m_edge.x()) {
        left_top.setX(m_start.x());
        right_bottom.setX(m_edge.x());
    }
    else {
        left_top.setX(m_edge.x());
        right_bottom.setX(m_start.x());
    }
    if (m_start.y() < m_edge.y()) {
        left_top.setY(m_start.y());
        right_bottom.setY(m_edge.y());
    }
    else {
        left_top.setY(m_edge.y());
        right_bottom.setY(m_start.y());
    }
    vision_rect = QRectF(left_top, right_bottom);
    GraphicsItem::onUpdatePointMessage(); // clazy:exclude=skipped-base-method
    width = abs(m_edge_map_to_measure_ob.rx() - m_start_map_to_measure_ob.rx());
    height = abs(m_edge_map_to_measure_ob.ry() - m_start_map_to_measure_ob.ry());
    QString str1 = "width:" + QString::number(width, 'f', 0) + "px ";
    QString str2 = "height:" + QString::number(height, 'f', 0) + "px ";
    graphics_text_model.setExtraMessage(str1 + "\n" + str2);
}

/***********************RubberRect**************************/
RubberRect::RubberRect(qreal x, qreal y, qreal width, qreal height)
    :BRect(x, y, width, height)
{
    graphics_text_model.setIsHideText(true);
    QPen pen1(Qt::cyan, 1);
    pen1.setStyle(Qt::DashLine);
    graphics_paint_model.getRPenIsSelected() = pen1;
    QPen pen2(Qt::yellow, 1);
    pen2.setStyle(Qt::DashLine);
    graphics_paint_model.getRPenNoSelected() = pen2;
}

void RubberRect::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    BRect::paint(painter, option, widget);
}

/***********************Prompt Rect Class**************************/
PromptRect::PromptRect()
    :BRect()
{
    getGraphicsTextModel().setLabelText("Prompt Rect");
    setData(1, "PromptRect");
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
    is_accept_others_setting = false;
    is_generate_context_menu = false;
    is_accept_hover_event = false;
    setGraphicsColor(ColorOperation::GenerateColorByText("Prompt Rect"));
}
