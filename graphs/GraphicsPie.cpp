#include "GraphicsPie.h"

/***************************************Pie Class**************************************************/

/***********************Pie Base Class**************************/
Pie::Pie()
    :BRound()
{
    is_cal_start_by_ce = false;
    is_cal_center_by_se = false;
    graphics_paint_model.setIsCloseItem(true);
    setData(1, "Pie");
    graphics_text_model.setItemType("Pie");
    set_point_fp_list.clear();
    set_point_fp_list.push_back(std::make_pair(this, &GraphicsItem::setCenter));
    set_point_fp_list.push_back(std::make_pair(this, &GraphicsItem::setStart));
    set_point_fp_list.push_back(std::make_pair(this, &GraphicsItem::setEdge));
}

Pie::Pie(qreal x, qreal y, qreal r)
    :BRound(x, y, r)
{
    is_cal_start_by_ce = false;
    is_cal_center_by_se = false;
    graphics_paint_model.setIsCloseItem(true);
    m_center = QPointF(x, y);
    m_start = m_center + QPointF(0, -r);
    m_edge = m_center + QPointF(r, 0);
    setData(1, "Pie");
    graphics_text_model.setItemType("Pie");
    set_point_fp_list.clear();
    set_point_fp_list.push_back(std::make_pair(this, &GraphicsItem::setCenter));
    set_point_fp_list.push_back(std::make_pair(this, &GraphicsItem::setStart));
    set_point_fp_list.push_back(std::make_pair(this, &GraphicsItem::setEdge));
}

void Pie::onUpdatePointMessage()
{
    radius = CoordinateCalculation::calculatePointToPoint(m_center, m_start);
    bounding_rect = QRectF(m_center.x() - radius, m_center.y() - radius, 2 * radius, 2 * radius);
    GraphicsItem::onUpdatePointMessage();

    QLineF horizontalLine(QPointF(0, 0), QPointF(1, 0));
    QLineF base_line(m_center, m_start);
    QLineF Wandering(m_center, m_edge);
    angle_s = horizontalLine.angleTo(base_line);
    angle_e = horizontalLine.angleTo(Wandering);
    angle = base_line.angleTo(Wandering);
    qreal delta_y = radius * qSin(qDegreesToRadians(angle_e));
    qreal delta_x = radius * qCos(qDegreesToRadians(angle_e));
    m_edge.setX(m_center.x() + delta_x);
    m_edge.setY(m_center.y() - delta_y);
    if (!angle_to_arc)graphics_text_model.setExtraMessage("angle " + QString::number(abs(angle), 'f', 2) + "°" + "\nradius " +
        QString::number(abs(radius), 'f', 2) + "px");
    else {
        qreal radian = qDegreesToRadians(angle);
        graphics_text_model.setExtraMessage("radian " + QString::number(abs(radian), 'f', 2) + "\nradius " +
            QString::number(abs(radius), 'f', 2) + "px");
    }
}

void Pie::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->setPen(pen());
    if (!pie_to_arc)painter->drawPie(bounding_rect, static_cast<int>(angle_s * 16), static_cast<int>(angle * 16));
    else painter->drawArc(bounding_rect, static_cast<int>(angle_s * 16), static_cast<int>(angle * 16));
    GraphicsItem::paint(painter, option, widget); // clazy:exclude=skipped-base-method
}

QPainterPath Pie::getFillPath()
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.moveTo(m_center);
    path.arcTo(bounding_rect, angle_s, angle);
    path.closeSubpath();
    return path;
}

QRectF Pie::boundingRect() const
{
    return bounding_rect.adjusted(-graphics_paint_model.getAdjustSize(), -graphics_paint_model.getAdjustSize(),
        graphics_paint_model.getAdjustSize(), graphics_paint_model.getAdjustSize());
}

void Pie::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    QAction* angleToArc = menu.addAction(QStringLiteral("Unit Conversion"), this, &Pie::angleToArc);
    QAction* pieToArc = menu.addAction(QStringLiteral("ArcPie Conversion"), this, &Pie::pieToArc);
    GraphicsItem::contextMenuEvent(event);
    Q_UNUSED(angleToArc);
    Q_UNUSED(pieToArc);
    menu.clear();
}

void Pie::setAngleS(qreal s)
{
    angle_s = s;
}

void Pie::setAngleE(qreal e)
{
    angle_e = e;
}

qreal Pie::getAngleS()
{
    return angle_s;
}

qreal Pie::getAngleE()
{
    return angle_e;
}

void Pie::setAngleToArc(bool ok)
{
    angle_to_arc = ok;
}

void Pie::setPieToArc(bool ok)
{
    pie_to_arc = ok;
}

bool Pie::getAngleToArc()
{
    return angle_to_arc;
}

bool Pie::getPieToArc()
{
    return pie_to_arc;
}

void Pie::angleToArc()
{
    angle_to_arc = !angle_to_arc;
    onUpdatePointMessage();
}

QPainterPath Pie::shape() const
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.moveTo(m_center);
    path.arcTo(bounding_rect, angle_s, angle);
    path.closeSubpath();
    return path;
}

void Pie::pieToArc()
{
    pie_to_arc = !pie_to_arc;
}

/***********************InteractionPie Class**************************/
InteractionPie::InteractionPie(qreal x, qreal y, qreal r)
    :Pie(x, y, r)
{
    initItem();
}

InteractionPie::InteractionPie()
    :Pie()
{
    initItem();
}

void InteractionPie::setStart(const QPointF& s)
{

    start_bpoint->setStart(s);
    GraphicsItem::setStart(s);
}

void InteractionPie::setEdge(const QPointF& e)
{
    edge_bpoint->setEdge(e);
    GraphicsItem::setEdge(e);
}

void InteractionPie::setCenter(const QPointF& c)
{
    center_bpoint->setCenter(c);
    GraphicsItem::setCenter(c);
}

void InteractionPie::setSE(const QPointF& s, const QPointF& e)
{
    GraphicsItem::setSE(s, e);
    start_bpoint->setStart(s);
    edge_bpoint->setEdge(e);
}

void InteractionPie::onUpdatePointMessage()
{
    radius = CoordinateCalculation::calculatePointToPoint(m_center, m_start);
    bounding_rect = QRectF(m_center.x() - radius, m_center.y() - radius, 2 * radius, 2 * radius);
    GraphicsItem::onUpdatePointMessage();
    QLineF horizontalLine(QPointF(0, 0), QPointF(1, 0));
    QLineF base_line(m_center, m_start);
    QLineF Wandering(m_center, m_edge);
    angle_s = horizontalLine.angleTo(base_line);
    angle_e = horizontalLine.angleTo(Wandering);
    qreal delta_y = radius * qSin(qDegreesToRadians(angle_e));
    qreal delta_x = radius * qCos(qDegreesToRadians(angle_e));

    edge_bpoint->getREdge().setX(m_center.x() + delta_x);
    edge_bpoint->getREdge().setY(m_center.y() - delta_y);

    angle = base_line.angleTo(Wandering);
    if (!angle_to_arc)graphics_text_model.setExtraMessage("angle " + QString::number(abs(angle), 'f', 2) + "°" +
        "\nradius " + QString::number(abs(radius), 'f', 2) + "px");
    else {
        qreal radian = qDegreesToRadians(angle);
        graphics_text_model.setExtraMessage("radian " + QString::number(abs(radian), 'f', 2) +
            "\nradius " + QString::number(abs(radius), 'f', 2) + "px");
    }
}

void InteractionPie::initItem()
{
    graphics_paint_model.setIsPaintCenter(false);
    setData(1, "InteractionPie");

    start_bpoint = new SPoint();
    edge_bpoint = new EPoint();
    center_bpoint = new CPoint();
    start_bpoint->getGraphicsTextModel().setExtraMessage("angle point");
    start_bpoint->setGraphicsColor(QColor(71, 75, 220));
    start_bpoint->setParentGraphicsItem(this, true);
    edge_bpoint->setParentGraphicsItem(this, true);
    center_bpoint->setParentGraphicsItem(this, false);
}

void InteractionPie::onActionRemoveSelf()
{
    removeChildItem(&start_bpoint);
    removeChildItem(&edge_bpoint);
    removeChildItem(&center_bpoint);
    GraphicsItem::onActionRemoveSelf();
}

