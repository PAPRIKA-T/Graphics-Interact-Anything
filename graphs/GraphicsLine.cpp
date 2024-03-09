#include "GraphicsLine.h"
#include <QKeyEvent>
#define EPS (1e-5) //除数最小量
/***************************************Line Class**************************************************/
/***********************Line Base Class**************************/
BLine::BLine(QPointF start, QPointF edge)
    : GraphicsItem(start, edge)
{
    initItem();
}

BLine::BLine()
    : GraphicsItem()
{
    initItem();
}

void BLine::initItem()
{
    graphics_paint_model.setIsCloseItem(false);
    graphics_paint_model.setIsPaintCenter(false);
    setData(0, "BLine");
    graphics_text_model.setItemType("Line");
    set_point_fp_list.push_back(std::make_pair(this, &GraphicsItem::setStart));
    set_point_fp_list.push_back(std::make_pair(this, &GraphicsItem::setEdge));
}

void BLine::onUpdatePointMessage()
{
    GraphicsItem::onUpdatePointMessage();
    k = (m_edge.y() - m_start.y()) / (m_edge.x() - m_start.x() + EPS);
    graphics_text_model.setExtraMessage("length:" + QString::number(CoordinateCalculation::calculatePointToPoint(m_start_map_to_measure_ob,
        m_edge_map_to_measure_ob), 'f', 2) + "px");
}

void BLine::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->setPen(pen());
    painter->drawLine(m_start, m_edge);
    GraphicsItem::paint(painter, option, widget);
}

QPainterPath BLine::shape() const
{
    QPainterPath path;
    if (isnan(k) || isinf(k))
    {
        qDebug() << "error!";
        return path;
    }
    QPolygonF polygon;
    path.setFillRule(Qt::WindingFill);
    qreal interaction_range = graphics_paint_model.getInteracitonRange();

    qreal length = CoordinateCalculation::calculatePointToPoint(m_start, m_edge);
    qreal deltaY = sqrt(interaction_range * interaction_range / (k * k + 1));
    qreal deltaX = k * deltaY;
    QPointF p1(m_start.x() + deltaX, m_start.y() - deltaY);
    QPointF p2(m_start.x() - deltaX, m_start.y() + deltaY);
    qreal width = sqrt(length * length / (k * k + 1));
    if (m_edge.x() - m_start.x() < 0)
        width = -width;
    polygon << p1
        << QPointF(p1.x() + width, p1.y() + width * k)
        << QPointF(p2.x() + width, p2.y() + width * k)
        << p2;
    path.addPolygon(polygon);
    path.closeSubpath();
    return path;
}

QPainterPath BLine::getStrokePath()
{
    return shape();
}

/***********************DoubleEndPointLine Class**************************/
DoubleEndPointLine::DoubleEndPointLine(QPointF start, QPointF edge)
    : BLine(start, edge)
{
    initItem();
}

DoubleEndPointLine::DoubleEndPointLine()
    : BLine()
{
    initItem();
}

void DoubleEndPointLine::setStart(const QPointF& s)
{
    GraphicsItem::setStart(s);
    start_bpoint->setStart(s);
}

void DoubleEndPointLine::setEdge(const QPointF& e)
{
    GraphicsItem::setEdge(e);
    edge_bpoint->setEdge(e);
}

void DoubleEndPointLine::setSE(const QPointF& s, const QPointF& e)
{
    GraphicsItem::setSE(s, e);
    start_bpoint->setStart(s);
    edge_bpoint->setEdge(e);
}

void DoubleEndPointLine::initItem()
{
    graphics_paint_model.setIsCloseItem(false);
    graphics_paint_model.setIsPaintCenter(false);
    setData(1, "DoubleEndPointLine");

    start_bpoint = new SPoint();
    edge_bpoint = new EPoint();
    start_bpoint->setParentGraphicsItem(this, true);
    edge_bpoint->setParentGraphicsItem(this, true);
}

void DoubleEndPointLine::onActionRemoveSelf()
{
    removeChildItem(&start_bpoint);
    removeChildItem(&edge_bpoint);
    GraphicsItem::onActionRemoveSelf();
}

/***********************SingleEndPointLine Class**************************/
SingleEndPointLine::SingleEndPointLine(QPointF start, QPointF edge)
    : BLine(start, edge)
{
    initItem();
}

SingleEndPointLine::SingleEndPointLine()
    : BLine()
{
    initItem();
}

void SingleEndPointLine::setEdge(const QPointF& e)
{
    GraphicsItem::setEdge(e);
    edge_bpoint->setEdge(e);
}

void SingleEndPointLine::setSE(const QPointF& s, const QPointF& e)
{
    GraphicsItem::setSE(s, e);
    edge_bpoint->setEdge(e);
    setStart(s);
}

void SingleEndPointLine::initItem()
{
    graphics_paint_model.setIsCloseItem(false);
    graphics_paint_model.setIsPaintCenter(false);
    setData(1, "SingleEndPointLine");

    edge_bpoint = new EPoint();
    edge_bpoint->setParentGraphicsItem(this, true);
}

void SingleEndPointLine::onActionRemoveSelf()
{
    removeChildItem(&edge_bpoint);
    GraphicsItem::onActionRemoveSelf();
}

/***********************ParallelLine Class**************************/
ParallelLine::ParallelLine() :DoubleEndPointLine()
{
    initItem();
}

ParallelLine::ParallelLine(QPointF start, QPointF edge)
    :DoubleEndPointLine(start, edge)
{
    initItem();
}

ParallelLine::~ParallelLine()
{
}

void ParallelLine::initItem()
{
    auxiliary_line = new DoubleEndPointLine();
    auxiliary_line->setVisible(false);
    auxiliary_line->setGraphicsColor(Qt::blue);
    auxiliary_line->setData(1, "Parallel_line");
    auxiliary_line->getGraphicsPaintModel().setPenIsSelected(QPen(Qt::yellow));
    setData(2, "ParallelLine");
    graphics_paint_model.setIsCloseItem(false);
    auxiliary_line->setParentItem(this);
    auxiliary_line->getGraphicsRelationModel().setIsDependWithparent(true);
    graphics_text_model.setItemType("ParaLine");
    set_point_fp_list.clear();
    set_point_fp_list.push_back(std::make_pair(this, &GraphicsItem::setStart));
    set_point_fp_list.push_back(std::make_pair(this, &GraphicsItem::setEdge));
    set_point_fp_list.push_back(std::make_pair(this, &GraphicsItem::generateOtherItems));
    connect(start_bpoint, &BPoint::pointSelected, this, &ParallelLine::onPointSelected);
    connect(edge_bpoint, &BPoint::pointSelected, this, &ParallelLine::onPointSelected);
    connect(auxiliary_line->getSPointItem(), &BPoint::pointSelected, this, &ParallelLine::onPointSelected);
    connect(auxiliary_line->getEPointItem(), &BPoint::pointSelected, this, &ParallelLine::onPointSelected);
    disconnect(auxiliary_line->getSPointItem(), &BPoint::sentItemPointF, auxiliary_line, &GraphicsItem::receptItemPointF);
    disconnect(auxiliary_line->getEPointItem(), &BPoint::sentItemPointF, auxiliary_line, &GraphicsItem::receptItemPointF);
}

void ParallelLine::generateOtherItems(const QPointF& pos)
{
    auxiliary_line->setStart(pos);
    auxiliary_line->setEdge(pos);
    auxiliary_line->setVisible(true);
    set_point_fp_list.push_back(std::make_pair(auxiliary_line, &GraphicsItem::setStart));
    set_point_fp_list.push_back(std::make_pair(auxiliary_line, &GraphicsItem::setEdge));
}

void ParallelLine::onPointSelected()
{
    if (start_bpoint->isSelected() || edge_bpoint->isSelected())
    {
        k = (m_edge.y() - m_start.y()) / (m_edge.x() - m_start.x() + EPS);
        if (start_bpoint->isSelected())
        {
            qreal line1_len = CoordinateCalculation::calculatePointToPoint(auxiliary_line->getStart(), auxiliary_line->getEdge());
            qreal delX = sqrt(line1_len * line1_len / (1 + k * k));
            if (m_edge.x() >= m_start.x())auxiliary_line->setStart(auxiliary_line->getEdge() - QPointF(delX, delX * k));
            else auxiliary_line->setStart(auxiliary_line->getEdge() + QPointF(delX, delX * k));
        }
        else
        {
            qreal line1_len = CoordinateCalculation::calculatePointToPoint(auxiliary_line->getStart(), auxiliary_line->getEdge());
            qreal delX = sqrt(line1_len * line1_len / (1 + k * k));
            if (m_edge.x() >= m_start.x()) {
                auxiliary_line->setEdge(auxiliary_line->getStart() + QPointF(delX, delX * k));
            }
            else auxiliary_line->setEdge(auxiliary_line->getStart() - QPointF(delX, delX * k));
        }
    }
    else
    {
        if (auxiliary_line->getSPointItem()->isSelected())
        {
            qreal delX = auxiliary_line->getSPointItem()->getStart().rx() - auxiliary_line->getEdge().rx();
            qreal delY = delX * k;
            auxiliary_line->setStart(auxiliary_line->getEdge() + QPointF(delX, delY));
        }
        else
        {
            qreal delX = auxiliary_line->getEPointItem()->getEdge().rx() - auxiliary_line->getStart().rx();
            qreal delY = delX * k;
            auxiliary_line->setEdge(auxiliary_line->getStart() + QPointF(delX, delY));
        }
    }
}

void ParallelLine::onActionRemoveSelf()
{
    removeChildItem(&auxiliary_line);
    DoubleEndPointLine::onActionRemoveSelf();
}
