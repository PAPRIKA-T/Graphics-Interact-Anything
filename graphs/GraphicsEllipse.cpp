#include "GraphicsEllipse.h"

/***************************************Ellipse Class**************************************************/
/***********************Ellipse Base Class**************************/
BEllipse::BEllipse(qreal x, qreal y, qreal width, qreal height)
    : BRect(x, y, width, height)
{
    setData(0, "BEllipse");
    graphics_text_model.setItemType("Ellipse");
}

BEllipse::BEllipse()
    : BRect()
{
    setData(0, "BEllipse");
    graphics_text_model.setItemType("Ellipse");
}

void BEllipse::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->setPen(pen());
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
    painter->drawEllipse(vision_rect);

    width = abs(m_edge_map_to_measure_ob.rx() - m_start_map_to_measure_ob.rx());
    height = abs(m_edge_map_to_measure_ob.ry() - m_start_map_to_measure_ob.ry());
    if (!graphics_text_model.getIsHideExtraText())
    {
        QString str1 = "width:" + QString::number(width, 'f', 0) + "px ";
        QString str2 = "height:" + QString::number(height, 'f', 0) + "px ";
        QString str3 = "eccentricity:" + QString::number(abs(m_edge.rx() - m_center.rx()) / abs(m_edge.ry() - m_center.ry()), 'f', 1);
        QString str = str1 + "\n" + str2 + "\n" + str3;
        graphics_text_model.setExtraMessage(str);
    }
    GraphicsItem::paint(painter, option, widget);// clazy:exclude=skipped-base-method
}

QPainterPath BEllipse::shape() const
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addEllipse(vision_rect.adjusted(-graphics_paint_model.getAdjustSize(),
        -graphics_paint_model.getAdjustSize(),
        graphics_paint_model.getAdjustSize(),
        graphics_paint_model.getAdjustSize()));
    return path;
}

QPainterPath BEllipse::getStrokePath()
{
    QPainterPath path;
    path.addEllipse(vision_rect
        .adjusted(-graphics_paint_model.getAdjustSize(), -graphics_paint_model.getAdjustSize(),
            graphics_paint_model.getAdjustSize(), graphics_paint_model.getAdjustSize()));
    return path;
}

QPainterPath BEllipse::getFillPath()
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addEllipse(vision_rect);
    return path;
}

/***********************InteractionEllipse Base Class**************************/
InteractionEllipse::InteractionEllipse()
    :InteractionRect()
{
    graphics_text_model.setItemType("Ellipse");
    graphics_relation_model.pushBackChildList(start_bpoint);
    graphics_relation_model.pushBackChildList(edge_bpoint);
    graphics_relation_model.pushBackChildList(rt_bpoint);
    graphics_relation_model.pushBackChildList(lb_bpoint);
    graphics_relation_model.setIsHideChildList(true);
}

InteractionEllipse::InteractionEllipse(qreal x, qreal y, qreal width, qreal height)
    :InteractionRect(x, y, width, height)
{
    graphics_text_model.setItemType("Ellipse");
    graphics_relation_model.pushBackChildList(start_bpoint);
    graphics_relation_model.pushBackChildList(edge_bpoint);
    graphics_relation_model.pushBackChildList(rt_bpoint);
    graphics_relation_model.pushBackChildList(lb_bpoint);
    graphics_relation_model.setIsHideChildList(true);
}

void InteractionEllipse::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->setPen(pen());
    painter->drawEllipse(vision_rect);
    GraphicsItem::paint(painter, option, widget); // clazy:exclude=skipped-base-method
}

void InteractionEllipse::onUpdatePointMessage()
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
    width = abs(m_edge_map_to_measure_ob.rx() - m_start_map_to_measure_ob.rx());
    height = abs(m_edge_map_to_measure_ob.ry() - m_start_map_to_measure_ob.ry());
    QString str1 = "width:" + QString::number(width, 'f', 0) + "px ";
    QString str2 = "height:" + QString::number(height, 'f', 0) + "px ";
    QString str3 = "eccentricity:" + QString::number(abs(m_edge.rx() - m_center.rx()) / abs(m_edge.ry() - m_center.ry()), 'f', 1);
    QString str = str1 + "\n" + str2 + "\n" + str3;
    graphics_text_model.setExtraMessage(str);
}


QPainterPath InteractionEllipse::getStrokePath()
{
    QPainterPath path;
    path.addEllipse(vision_rect
        .adjusted(-graphics_paint_model.getAdjustSize(), -graphics_paint_model.getAdjustSize(),
            graphics_paint_model.getAdjustSize(), graphics_paint_model.getAdjustSize()));
    return path;
}

QPainterPath InteractionEllipse::getFillPath()
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addEllipse(vision_rect);
    return path;
}

void InteractionEllipse::onPointSelected()
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
