#include "GraphicsRound.h"

/***************************************Round Class**************************************************/

/***********************Round Base Class**************************/
BRound::BRound(qreal x, qreal y, qreal r)
    : GraphicsItem()
{
    m_center = QPointF(x, y);
    radius = r;
    initItem();
}

BRound::BRound()
    : GraphicsItem()
{
    initItem();
}

void BRound::initItem()
{
    is_cal_start_by_ce = true;
    is_cal_center_by_se = false;
    graphics_paint_model.setIsFillItem(true);
    setData(0, "BRound");
    m_center = QPointF(0, 0);
    radius = 100;
    graphics_text_model.setItemType("Round");
    set_point_fp_list.push_back(std::make_pair(this, &GraphicsItem::setCenter));
    set_point_fp_list.push_back(std::make_pair(this, &GraphicsItem::setEdge));
}

void BRound::setRadius(qreal r)
{
    radius = r;
}

qreal BRound::getRadius()
{
    return  radius;
}


void BRound::onUpdatePointMessage()
{
    radius = CoordinateCalculation::calculatePointToPoint(m_center, m_edge);
    bounding_rect = QRectF(m_center.x() - radius, m_center.y() - radius, 2 * radius, 2 * radius);
    GraphicsItem::onUpdatePointMessage();
    graphics_text_model.setExtraMessage("radius:" + 
        QString::number(CoordinateCalculation::
            calculatePointToPoint(m_center_map_to_measure_ob, m_edge_map_to_measure_ob), 'f', 2) + "px");
}

void BRound::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->setPen(pen());
    painter->drawEllipse(m_center, radius, radius);
    GraphicsItem::paint(painter, option, widget);
}

QPainterPath BRound::shape() const
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    const qreal adjust_size = graphics_paint_model.getAdjustSize();
    path.addEllipse(bounding_rect.adjusted(-adjust_size, -adjust_size, 
        adjust_size, adjust_size));
    return path;
}

QPainterPath BRound::getStrokePath()
{
    QPainterPath path;
    const qreal adjust_size = graphics_paint_model.getAdjustSize();
    path.addEllipse(bounding_rect.adjusted(-adjust_size, -adjust_size, 
        adjust_size, adjust_size));
    return path;
}

QPainterPath BRound::getFillPath()
{
    QPainterPath path;
    path.addEllipse(m_center.x() - radius, m_center.y() - radius, 2 * radius, 2 * radius);
    return path;
}

QRectF BRound::boundingRect() const
{
    const qreal adjust_size = graphics_paint_model.getAdjustSize();
    return QRectF(m_center.x() - radius, m_center.y() - radius, 2 * radius, 2 * radius)
        .adjusted(-adjust_size, -adjust_size, 
            adjust_size, adjust_size);
}

/***********************InteractionRound Class**************************/
InteractionRound::InteractionRound(qreal x, qreal y, qreal r)
    :BRound(x, y, r)
{
    initItem();
}

InteractionRound::InteractionRound()
{
    initItem();
}

void InteractionRound::initItem()
{
    graphics_paint_model.setIsCloseItem(true);
    graphics_paint_model.setIsPaintCenter(false);
    setData(1, "InteractionRound");

    edge_bpoint = new EPoint();
    edge_bpoint->setParentGraphicsItem(this, true);
    center_bpoint = new CPoint();
    center_bpoint->setParentGraphicsItem(this, false);
}

void InteractionRound::onActionRemoveSelf()
{
    removeChildItem(&edge_bpoint);
    removeChildItem(&center_bpoint);
    GraphicsItem::onActionRemoveSelf();
}

void InteractionRound::setEdge(const QPointF& e)
{
    GraphicsItem::setEdge(e);
    edge_bpoint->setEdge(e);
}

void InteractionRound::setCenter(const QPointF& c)
{
    GraphicsItem::setCenter(c);
    center_bpoint->setCenter(c);
}

void InteractionRound::setSE(const QPointF& s, const QPointF& e)
{
    m_start = s;
    m_edge = e;
    edge_bpoint->setEdge(e);
}

