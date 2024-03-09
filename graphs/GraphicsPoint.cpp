#include "GraphicsPoint.h"
#include <QKeyEvent>
/***************************************Point Class**************************************************/

/***********************Point Base Class**************************/
BPoint::BPoint()
    : GraphicsItem()
{
    m_center = QPointF(0, 0);
    initItem();
}

BPoint::BPoint(QPointF p)
    : GraphicsItem()
{
    m_center = p;
    initItem();
}

void BPoint::initItem()
{
    setData(0,"BPoint");
    graphics_text_model.setItemType("BPoint");
    is_change_cener_aciton = false;
    is_rotate_action = false;
    graphics_paint_model.setIsCloseItem(false);
    is_cal_center_by_se = false;
    is_cal_start_by_ce = false;
    graphics_paint_model.setIsPaintCenter(true);
    graphics_paint_model.setIsPaintStrokeShape(true);
    set_point_fp_list.push_back(std::make_pair(this, &GraphicsItem::setCenter));

    point_paint_type.append(PointPaintType::Round);
    point_paint_type.append(PointPaintType::Cross);
    point_paint_type.append(PointPaintType::Rect);
}

void BPoint::setParentGraphicsItem(GraphicsItem *parent, bool is_depend_with_parent)
{
    if(!parent){
        qDebug()<<"BPoint::*parent not valid";
        return;
    }
    setParentItem(parent);
    graphics_relation_model.setIsDependWithparent(is_depend_with_parent);
    connect(this, &BPoint::sentItemPointF, parent, &GraphicsItem::receptItemPointF);
    parent->setHandlesChildEvents(false);
}

void BPoint::setIsGrab(bool ok)
{
    is_grab_point = ok;
}

bool BPoint::getIsGrab()
{
    return is_grab_point;
}

void BPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (isSelected())
    {
        emit pointSelected();
        if (graphics_paint_model.getIsPaintStrokeShape())
        {
            QPen pen(QColor(255, 255, 255), graphics_paint_model.getRPenIsSelected().width());
            pen.setStyle(Qt::DashLine);
            painter->setPen(pen);
            painter->drawPath(getStrokePath());
        }
        painter->setPen(graphics_paint_model.getRPenIsSelected());
        painter->setBrush(graphics_paint_model.getRPenIsSelected().color());
    }
    else
    {
        painter->setPen(graphics_paint_model.getRPenNoSelected());
        painter->setBrush(graphics_paint_model.getRPenNoSelected().color());
    }
    graphics_paint_model.paintPoint(painter, m_center);
    painter->setBrush(Qt::NoBrush);
}

void BPoint::onUpdatePointMessage()
{
    m_start = m_center;
    m_edge = m_center;
    GraphicsItem::onUpdatePointMessage();
    graphics_text_model.setExtraMessage("(" + QString::number(m_center_map_to_measure_ob.rx(), 'f', 0) + ","
        + QString::number(m_center_map_to_measure_ob.ry(), 'f', 0) + ")");
}

void BPoint::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    GraphicsItem::mouseMoveEvent(event);
    emit updatePointMessage();
}

void BPoint::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    m_center.setPaintType(point_paint_type[1]);
    m_start.setPaintType(point_paint_type[1]);
    m_edge.setPaintType(point_paint_type[1]);
    Q_UNUSED(event);
}

void BPoint::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    m_center.setPaintType(point_paint_type[0]);
    m_start.setPaintType(point_paint_type[0]);
    m_edge.setPaintType(point_paint_type[0]);
    Q_UNUSED(event);
}

QRectF BPoint::boundingRect() const
{
    if (!is_grab_point)return QRectF();
    qreal interaction_range = graphics_paint_model.getInteracitonRange();
    return QRectF(m_center.x()- interaction_range, m_center.y()-
        interaction_range, interaction_range*2,
        interaction_range*2);
}


/***********************Start Point Class**************************/
SPoint::SPoint()
    :BPoint{}
{
    initItem();
}

SPoint::SPoint(QPointF p)
    :BPoint(p)
{
    initItem();
}

void SPoint:: initItem()
{
    graphics_paint_model.setIsPaintCenter(false);
    setData(1,"SPoint");
    graphics_text_model.setIsHideText(true);
    graphics_text_model.setExtraMessage("SP");
    graphics_paint_model.getRPenIsSelected().setColor(DEFAULT_COLOR_POINT_SELECTED);
    setGraphicsColor(DEFAULT_COLOR_POINT_NOSELECTED);
}

void SPoint::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    if (isSelected())
    {
        emit pointSelected();
        if (graphics_paint_model.getIsPaintStrokeShape())
        {
            QPen pen(QColor(255, 255, 255), graphics_paint_model.getRPenIsSelected().width());
            pen.setStyle(Qt::DashLine);
            painter->setPen(pen);
            painter->drawPath(getStrokePath());
        }
        painter->setPen(graphics_paint_model.getRPenIsSelected());
        painter->setBrush(graphics_paint_model.getRPenIsSelected().color());
    }
    else
    {
        painter->setPen(graphics_paint_model.getRPenNoSelected());
        painter->setBrush(graphics_paint_model.getRPenNoSelected().color());
    }
    graphics_paint_model.paintPoint(painter, m_start);
    painter->setBrush(Qt::NoBrush);
}

void SPoint::setStart(const QPointF& p)
{
    if (m_start == p) return;
    BPoint::setStart(p);
    if (parentItem()) emit sentItemPointF(m_start, this);
}

QRectF SPoint::boundingRect() const
{
    if (!is_grab_point)return QRectF();
    qreal interaction_range = graphics_paint_model.getInteracitonRange();
    return QRectF(m_start.x() - interaction_range, m_start.y() -
        interaction_range, interaction_range * 2,
        interaction_range * 2);
}


void SPoint::onUpdatePointMessage()
{
    m_edge = m_start;
    m_center = m_start;
    GraphicsItem::onUpdatePointMessage();
    graphics_text_model.setExtraMessage("(" + QString::number(m_start_map_to_measure_ob.rx(), 'f', 0) + ","
        + QString::number(m_center_map_to_measure_ob.ry(), 'f', 0) + ")");
}

void SPoint::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    m_start = event->pos();
    emit updatePointMessage();
    if (parentItem()) emit sentItemPointF(m_start, this);
}

/***********************Edge Point Class**************************/
EPoint::EPoint()
    :BPoint{}
{
    initItem();
}

EPoint::EPoint(QPointF p)
    :BPoint(p)
{
    initItem();
}

void EPoint:: initItem()
{
    graphics_paint_model.setIsPaintCenter(false);
    setData(1,"EPoint");
    graphics_text_model.setIsHideText(true);
    graphics_text_model.setExtraMessage("EP");
    graphics_paint_model.getRPenIsSelected().setColor(DEFAULT_COLOR_POINT_SELECTED);
    setGraphicsColor(DEFAULT_COLOR_POINT_NOSELECTED);
}

void EPoint::setEdge(const QPointF& p)
{
    if (m_edge == p) return;
    BPoint::setEdge(p);
    if (parentItem()) emit sentItemPointF(m_edge, this);
}

void EPoint::onUpdatePointMessage()
{
    m_start = m_edge;
    m_center = m_edge;
    GraphicsItem::onUpdatePointMessage();
    graphics_text_model.setExtraMessage("(" + QString::number(m_edge_map_to_measure_ob.rx(), 'f', 0) + ","
        + QString::number(m_center_map_to_measure_ob.ry(), 'f', 0) + ")");
}

QRectF EPoint::boundingRect() const
{
    if (!is_grab_point)return QRectF();
    qreal interaction_range = graphics_paint_model.getInteracitonRange();
    return QRectF(m_edge.x() - interaction_range, m_edge.y() -
        interaction_range, interaction_range * 2,
        interaction_range * 2);
}

void EPoint::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    m_edge = event->pos();
    emit updatePointMessage();
    if (parentItem()) emit sentItemPointF(m_edge, this);
}

void EPoint::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    if (isSelected())
    {
        emit pointSelected();
        if (graphics_paint_model.getIsPaintStrokeShape())
        {
            QPen pen(QColor(255, 255, 255), graphics_paint_model.getRPenIsSelected().width());
            pen.setStyle(Qt::DashLine);
            painter->setPen(pen);
            painter->drawPath(getStrokePath());
        }
        painter->setPen(graphics_paint_model.getRPenIsSelected());
        painter->setBrush(graphics_paint_model.getRPenIsSelected().color());
    }
    else
    {
        painter->setPen(graphics_paint_model.getRPenNoSelected());
        painter->setBrush(graphics_paint_model.getRPenNoSelected().color());
    }
    graphics_paint_model.paintPoint(painter, m_edge);
    painter->setBrush(Qt::NoBrush);
}

/***********************Center Point Class**************************/
CPoint::CPoint()
    :BPoint{}
{
    initItem();
}

CPoint::CPoint(QPointF p)
    :BPoint{p}
{
    initItem();
}

void CPoint:: initItem()
{
    setData(1,"CPoint");
    graphics_text_model.setIsHideText(true);
    graphics_text_model.setExtraMessage("CP");
    graphics_paint_model.getRPenIsSelected().setColor(DEFAULT_COLOR_POINT_SELECTED);
    setGraphicsColor(DEFAULT_COLOR_UNIQUE_POINT);
}

void CPoint::setCenter(const QPointF& p)
{
    if (m_center == p) return;
    BPoint::setCenter(p);
    if (parentItem()) emit sentItemPointF(m_center, this);
}


void CPoint::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    BPoint::mouseMoveEvent(event);
    if (parentItem()) emit sentItemPointF(m_center, this);
}

/***********************Text Point Class**************************/
TextPoint::TextPoint(QPointF p)
    :BPoint(p)
{
    graphics_paint_model.getRItemColor() = QColor(Qt::red);
    setData(1,"TextPoint");
    is_change_cener_aciton = false;
    is_rotate_action = false;
    graphics_paint_model.setIsCloseItem(false);
    is_cal_center_by_se = false;
    is_grab_point = false;
    setFlags(QGraphicsItem::ItemIgnoresTransformations);
}

void TextPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    setPos(mapToScene(0, 0));
    BPoint::paint(painter,option,widget);
}

#include "utils/ColorOperation.h"
/***********************Positive Point Class**************************/
PositivePoint::PositivePoint()
    :BPoint{}
{
    getGraphicsTextModel().setLabelText("positiveP");
    setData(1, "PositivePoint");
    setFlag(QGraphicsItem::ItemIsMovable, false);
    is_accept_others_setting = false;
    setGraphicsColor(ColorOperation::generate_color_by_text("positiveP"));
    is_generate_context_menu = false;
}

/***********************Negative Point Class**************************/
NegativePoint::NegativePoint()
    :BPoint{}
{
    getGraphicsTextModel().setLabelText("negtiveP");
    setData(1, "NegativePoint");
    setFlag(QGraphicsItem::ItemIsMovable, false);
    is_accept_others_setting = false;
    setGraphicsColor(ColorOperation::generate_color_by_text("negtiveP"));
    is_generate_context_menu = false;
}

