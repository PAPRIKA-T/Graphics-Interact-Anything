#include "GraphicsCalculateLine.h"
#define EPS (1e-5) //除数最小量
/***************************************测量线段类**************************************************/
/***********************测量线段基类**************************/
CalculateLine::CalculateLine(QPointF start, QPointF edge)
    :DoubleEndPointLine(start, edge)
{
    initItem();
}

CalculateLine::CalculateLine()
    :DoubleEndPointLine()
{
    initItem();
}

void CalculateLine::initItem()
{
    QPen pen(1);
    pen.setStyle(Qt::DashLine);
    graphics_paint_model.getRPenNoSelected() = pen;
    setGraphicsColor(Qt::red);
    graphics_paint_model.setIsCloseItem(false);
    graphics_paint_model.setIsPaintCenter(false);
    is_rotate_action = false;
    setData(1, "CalculateLine");
    graphics_text_model.setItemType("CenterLine");
    delete_direct = true;
    start_bpoint->setGraphicsColor(DEFAULT_COLOR_POINT_SELECTED);
    edge_bpoint->setGraphicsColor(DEFAULT_COLOR_POINT_SELECTED);
}

QPainterPath CalculateLine::shape() const
{
    QPainterPath path{}, path_point{};
    path = BLine::shape();
    qreal interaction_range = graphics_paint_model.getInteracitonRange();
    path_point.addEllipse(m_start, interaction_range, interaction_range);
    path_point.addEllipse(m_edge, interaction_range, interaction_range);
    path = path.subtracted(path_point);
    return path;
}

void CalculateLine::onUpdatePointMessage()
{
    GraphicsItem::onUpdatePointMessage();
    k = (m_edge.y() - m_start.y()) / (m_edge.x() - m_start.x() + EPS);
    graphics_text_model.setExtraMessage("CalLength:" + QString::number(CoordinateCalculation::calculatePointToPoint
    (m_start_map_to_measure_ob,m_edge_map_to_measure_ob), 'f', 2) + "px");
}

void CalculateLine::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    start_bpoint->setStart(start_bpoint->mapFromItem(cal_disStart, cal_disStart->getRCenter()));
    edge_bpoint->setEdge(edge_bpoint->mapFromItem(cal_disEdge, cal_disEdge->getRCenter()));
    DoubleEndPointLine::paint(painter, option, widget);
}

/***********************垂直线段类**************************/


CalVerticalLine::CalVerticalLine(QPointF start, QPointF edge)
    :CalculateLine(start, edge)
{
    initItem();
}

CalVerticalLine::CalVerticalLine()
    :CalculateLine()
{
    initItem();
}

void CalVerticalLine::initItem()
{
    QPen pen(QColor(255, 94, 20), 1);
    pen.setStyle(Qt::DashLine);
    graphics_paint_model.getRPenNoSelected() = pen;
    setGraphicsColor(QColor(255, 94, 20));
    setData(2, "CalVerticalLine");
    graphics_text_model.setItemType("VerticalLine");
}

void CalVerticalLine::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Verticalpoint = CoordinateCalculation::getPointToLineVerticalpoint
    (cal_disStart->mapToScene(cal_disStart->getCenter()),
        cal_disEdge->mapToScene(cal_disEdge->getStart()), cal_disEdge->mapToScene(cal_disEdge->getEdge()));
    start_bpoint->setStart(start_bpoint->mapFromItem(cal_disStart, cal_disStart->getCenter()));
    edge_bpoint->setEdge(edge_bpoint->mapFromScene(Verticalpoint));
    if (!vertical_point_is_on_line)
    {
        QLineF extend;
        if (CoordinateCalculation::calculatePointToPoint(mapToScene(m_edge), 
            cal_disEdge->mapToScene(cal_disEdge->getStart())) <
            CoordinateCalculation::calculatePointToPoint(mapToScene(m_edge), 
                cal_disEdge->mapToScene(cal_disEdge->getEdge())))
        {
            extend = QLineF(m_edge, mapFromItem(cal_disEdge, cal_disEdge->getStart()));
        }
        else extend = QLineF(m_edge, mapFromItem(cal_disEdge, cal_disEdge->getEdge()));
        QPen pen(Qt::yellow, graphics_paint_model.getRPenNoSelected().widthF());
        pen.setStyle(Qt::DashLine);
        painter->setPen(pen);
        painter->drawLine(extend);
        painter->setPen(Qt::NoPen);
    }
    DoubleEndPointLine::paint(painter, option, widget);
}

void CalVerticalLine::onUpdatePointMessage()
{
    vertical_point_is_on_line = CoordinateCalculation::verticalPointIsOnLine(mapToScene(m_edge),
        cal_disEdge->mapToScene(cal_disEdge->getStart()), cal_disEdge->mapToScene(cal_disEdge->getEdge()));
    GraphicsItem::onUpdatePointMessage();
    k = (m_edge.y() - m_start.y()) / (m_edge.x() - m_start.x() + EPS);
    graphics_text_model.setExtraMessage("CalDistance:" + QString::number(CoordinateCalculation::calculatePointToPoint(m_start_map_to_measure_ob,
        m_edge_map_to_measure_ob), 'f', 2) + "px");
}

/***********************线线角度线段类**************************/
CalLineAngleLine::CalLineAngleLine(QPointF start, QPointF edge)
    :CalculateLine(start, edge)
{
    QPen pen(Qt::darkMagenta, 1);
    pen.setStyle(Qt::DashDotLine);
    setGraphicsColor(Qt::darkMagenta);
    graphics_paint_model.getRPenNoSelected() = QPen(pen);
    setData(2, "CalAngleLine");
    graphics_text_model.setItemType("AngleLine");
}

void CalLineAngleLine::onUpdatePointMessage()
{
    GraphicsItem::onUpdatePointMessage();
    k = (m_edge.y() - m_start.y()) / (m_edge.x() - m_start.x() + EPS);
    if (!is_angleToArc)graphics_text_model.setExtraMessage("angle " + QString::number(abs(angle), 'f', 2) + "°");
    else {
        qreal radian = qDegreesToRadians(angle);
        graphics_text_model.setExtraMessage("radian " + QString::number(abs(radian), 'f', 2));
    }
}

void CalLineAngleLine::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    if (cal_disStart != nullptr && cal_disEdge != nullptr) {
        QLineF l1(cal_disStart->getStart(), cal_disStart->getEdge());
        QLineF l2(cal_disEdge->getStart(), cal_disEdge->getEdge());
        angle = l1.angleTo(l2);
        if (angle >= 180) angle -= 360;
        CalculateLine::paint(painter, option, widget);
    }
    else qDebug() << "测量对象空指针";
}

void CalLineAngleLine::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    QAction* angleToArc = menu.addAction(QStringLiteral("Unit Conversion"), this, &CalLineAngleLine::angleToArc);
    CalculateLine::contextMenuEvent(event);
    Q_UNUSED(angleToArc);
}

