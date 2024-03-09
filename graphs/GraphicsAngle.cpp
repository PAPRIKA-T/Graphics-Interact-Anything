#include "GraphicsAngle.h"

/********************************角度类***********************************/
/***********************直角角度**************************/
Angle::Angle()
{
    line1 = new SingleEndPointLine(m_center, m_center + QPointF(100, 0));
    line2 = new SingleEndPointLine(m_center, m_center - QPointF(0, 100));
    initItem();
}

Angle::Angle(QPointF c)
{
    line1 = new SingleEndPointLine(c, c + QPointF(100, 0));
    line2 = new SingleEndPointLine(c, c - QPointF(0, 100));
    initItem();
}

Angle::~Angle()
{
}

void Angle::initItem()
{
    line1->setData(2, "AngleLine");
    line2->setData(2, "AngleLine");
    setData(0, "Angle");
    graphics_text_model.setItemType("Angle");
    graphics_paint_model.setIsPaintCenter(true);
    graphics_paint_model.setIsPaintStrokeShape(true);
    is_cal_center_by_se = false;
    graphics_paint_model.setIsCloseItem(false);
    center_bpoint = new CPoint();
    line1->setParentItem(this);
    line2->setParentItem(this);
    line1->getEPointItem()->getGraphicsTextModel().setIsHideText(true);
    line2->getEPointItem()->getGraphicsTextModel().setIsHideText(true);
    line1->getGraphicsTextModel().setIsHideIdText(true);
    line2->getGraphicsTextModel().setIsHideIdText(true);
    center_bpoint->setParentGraphicsItem(this, true);
    center_bpoint->getGraphicsTextModel().setIsHideText(true);
    graphics_paint_model.setIsPaintCenter(false);
    graphics_relation_model.pushBackChildList(line1);
    graphics_relation_model.pushBackChildList(line2);
    connect(line1->getEPointItem(), &GraphicsItem::updatePointMessage, this, &Angle::onUpdatePointMessage);
    connect(line2->getEPointItem(), &GraphicsItem::updatePointMessage, this, &Angle::onUpdatePointMessage);
    set_point_fp_list.push_back(std::make_pair(line1, &GraphicsItem::setEdge));
    set_point_fp_list.push_back(std::make_pair(this, &GraphicsItem::setCenter));
    set_point_fp_list.push_back(std::make_pair(line2, &GraphicsItem::setEdge));
    graphics_relation_model.setIsChildConsistentColor(true);

}

QPainterPath Angle::shape() const
{
    QPainterPath path;
    qreal scale_factor = graphics_paint_model.getScaleFactor();
    path.addEllipse(QRectF(m_center - QPointF(20 / scale_factor, 20 / scale_factor),
        m_center + QPointF(20 / scale_factor, 20 / scale_factor)));
    return path;
}

QRectF Angle::boundingRect() const
{
    qreal scale_factor = graphics_paint_model.getScaleFactor();
    return QRectF(m_center - QPointF(20 / scale_factor, 20 / scale_factor),
        m_center + QPointF(20 / scale_factor, 20 / scale_factor));
}

void Angle::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    QAction* angleToArc = menu.addAction(QStringLiteral("Unit Conversion"), this, &Angle::angleToArc);
    Q_UNUSED(angleToArc);
    GraphicsItem::contextMenuEvent(event);
}

void Angle::onActionRemoveSelf()
{
    removeChildItem(&line1);
    removeChildItem(&line2);
    removeChildItem(&center_bpoint);
    GraphicsItem::onActionRemoveSelf();
}

void Angle::onUpdatePointMessage()
{
    GraphicsItem::onUpdatePointMessage();
    QLineF l1(line1->getStart(), line1->getEdge());
    QLineF l2(line2->getStart(), line2->getEdge());
    angle = l1.angleTo(l2);
    if (angle >= 180) angle -= 360;
    line1->setStart(m_center);
    line2->setStart(m_center);
    if (!is_angleToArc) {
        graphics_text_model.setExtraMessage("angle " + QString::number(abs(angle), 'f', 2) + "°");
    }
    else {
        qreal radian = qDegreesToRadians(angle);
        graphics_text_model.setExtraMessage("radian " + QString::number(abs(radian), 'f', 2));
    }
}

void Angle::setCenter(const QPointF& c)
{
    GraphicsItem::setStart(c);
    line1->setStart(c);
    line2->setStart(c);
    center_bpoint->setCenter(c);
}

