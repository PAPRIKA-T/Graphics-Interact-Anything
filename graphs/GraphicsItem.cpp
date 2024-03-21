#include <QGraphicsScene>
#include <QGraphicsView>
#include <QKeyEvent>
#include "GraphicsItem.h"
#include "model/StyleSheetConfigModel.h"

#define EPS (1e-5) //除数最小量

GraphicsItem::GraphicsItem()
{
    initGraphicItem();
}

GraphicsItem::GraphicsItem(QPointF start, QPointF edge)
    : m_start(mapFromScene(start)), m_edge(mapFromScene(edge))
{
    initGraphicItem();
}

void GraphicsItem::initGraphicItem()
{
    // called in the construction
    ++count;
    this->setFlags(QGraphicsItem::ItemIsSelectable |
                   QGraphicsItem::ItemIsMovable |
                   QGraphicsItem::ItemIsFocusable);
    setAcceptHoverEvents(true);
    m_center = ItemPointF(0,0,PointType::Center);
    m_start = ItemPointF(0,0,PointType::Start);
    m_edge = ItemPointF(0,0,PointType::Edge);
    m_press_pos = QPointF(0,0);
    initColorSetting();
    connect(this, &GraphicsItem::updatePointMessage, this, &GraphicsItem::onUpdatePointMessage);
}

void GraphicsItem::initColorSetting()
{
    // 颜色初始化
    graphics_paint_model.initColorSetting();
    graphics_text_model.initTextColorSetting();
}

GraphicsPaintModel& GraphicsItem::getGraphicsPaintModel()
{
    return graphics_paint_model;
}

GraphicsTextModel& GraphicsItem::getGraphicsTextModel()
{
    return graphics_text_model;
}

GraphicsRelationModel& GraphicsItem::getGraphicsRelationModel()
{
    return graphics_relation_model;
}

GraphicsTransformModel& GraphicsItem::getGraphicsTransformModel()
{
    return graphics_transform_model;
}

void GraphicsItem::setRotateAngle(qreal r)
{
    graphics_transform_model.setRotateAngle(r);
    setRotation(r);
}

GraphicsItem::~GraphicsItem()
{
    count--;
    qDebug()<<"ItemCount: "<<count;
}

void GraphicsItem::setDeleteDirectly(bool ok)
{
    delete_direct = ok;
}

void GraphicsItem::setViewScale(qreal s)
{
    graphics_paint_model.setScaleFactor(s);
    // 设置画笔线宽
    graphics_paint_model.getRPenNoSelected().setWidthF(graphics_paint_model.getPenWidth() / s);
    graphics_paint_model.getRPenIsSelected().setWidthF(graphics_paint_model.getPenWidth() / s);
    graphics_paint_model.setLineLength(ITEM_LINE_LENGTH / s);
    graphics_paint_model.setInteracitonRange(INTERACTION_RANGE / s);
    graphics_paint_model.setAdjustSize(INTERACTION_RANGE / s);
}

QList<std::pair<GraphicsItem*, void(GraphicsItem::*)(const QPointF&)>>& GraphicsItem::getSetPointFunctionList()
{
    return set_point_fp_list;
}

void GraphicsItem::generateOtherItems(const QPointF& pos)
{
    Q_UNUSED(pos);
}

ItemPointF GraphicsItem::getCenter() const
{
    return m_center;
}

ItemPointF& GraphicsItem::getRCenter()
{
    return m_center;
}

void GraphicsItem::setCenter(const QPointF& p)
{
    m_center = p;
    emit updatePointMessage();
}

ItemPointF GraphicsItem::getStart() const
{
    return  m_start;
}

ItemPointF& GraphicsItem::getRStart()
{
    return  m_start;
}

void GraphicsItem::setStart(const QPointF& p)
{
    m_start = p; 
    emit updatePointMessage();
}

ItemPointF GraphicsItem::getEdge() const
{
    return m_edge;
}

ItemPointF& GraphicsItem::getREdge()
{
    return m_edge;
}

void GraphicsItem::setEdge(const QPointF& p)
{
    m_edge = p; 
    emit updatePointMessage();
}

void GraphicsItem::setSE(const QPointF& s, const QPointF& e)
{
    m_start = s; 
    m_edge = e; 
    emit updatePointMessage();
}

const QPointF& GraphicsItem::getStartMeasurePos()
{
    if (graphics_transform_model.getMeasureObject())return m_start_map_to_measure_ob;
    else return m_start;
}

const QPointF& GraphicsItem::getEdgeMeasurePos()
{
    if (graphics_transform_model.getMeasureObject())return m_edge_map_to_measure_ob;
    else return m_edge;
}

const QPointF& GraphicsItem::getCenterMeasurePos()
{
    if (graphics_transform_model.getMeasureObject()) {
        return m_center_map_to_measure_ob;
    }
    else return m_center;
}

void GraphicsItem::setGraphicsColor(const QColor& color)
{
    graphics_paint_model.getRPenNoSelected().setColor(color);
    graphics_text_model.getTextItem()->setBackGroundColor(color);

    graphics_paint_model.getRFillColorHover() = color;
    graphics_paint_model.getRFillColorHover().setAlpha(100);
    graphics_paint_model.getRFillColorUnselected() = color;
    graphics_paint_model.getRFillColorUnselected().setAlpha(100);
    if (graphics_relation_model.getIsChildConsistentColor()) {
        foreach(GraphicsItem * item, graphics_relation_model.getChildItemList()) {
            item->setGraphicsColor(color);
        }
    }
    emit sentUnselectedColor(this, color);
}

void GraphicsItem::setName(int n)
{
    m_name = n;
}

int GraphicsItem::getName()
{
    return m_name;
}

void GraphicsItem::setIsAcceptOthersSetting(bool ok)
{
    is_accept_others_setting = ok;
}

bool GraphicsItem::getIsAcceptOthersSetting()
{
    return is_accept_others_setting;
}

void GraphicsItem::receptItemPointF(ItemPointF &p, GraphicsItem *item)
{
    if(p.getPointType() == PointType::Start)
    {
        if(item) m_start = mapFromItem(item, p);
        else m_start = p;
        emit updatePointMessage();
    }
    else if(p.getPointType() == PointType::Edge)
    {
        if(item) m_edge = mapFromItem(item, p);
        else m_edge = p;
        emit updatePointMessage();
    }
    else if(p.getPointType() == PointType::Center)
    {
        if(item) m_center = mapFromItem(item, p);
        else m_center = p;
        emit updatePointMessage();
    }
    else if(p.getPointType() == PointType::Other)
    {
    }
}

void GraphicsItem::onUpdatePointMessage()
{
    if (is_cal_center_by_se) m_center = getCenterFromTwoPoint(m_start, m_edge);
    else if (is_cal_start_by_ce) m_start = getStartFromTwoPoint(m_center, m_edge);

    if (graphics_transform_model.getMeasureObject() != nullptr)
    {
        m_start_map_to_measure_ob = graphics_transform_model.getImageScale() *
            mapToItem(graphics_transform_model.getMeasureObject(), m_start);
        m_edge_map_to_measure_ob = graphics_transform_model.getImageScale() *
            mapToItem(graphics_transform_model.getMeasureObject(), m_edge);
        m_center_map_to_measure_ob = graphics_transform_model.getImageScale() *
            mapToItem(graphics_transform_model.getMeasureObject(), m_center);
    }
    else
    {
        m_start_map_to_measure_ob = mapToScene(m_start);
        m_edge_map_to_measure_ob = mapToScene(m_edge);
        m_center_map_to_measure_ob = mapToScene(m_center);
    }
    // 设置text
    graphics_text_model.getTextItem()->setPos(m_center);
}

QPainterPath GraphicsItem::getStrokePath()
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

QPainterPath GraphicsItem::getFillPath()
{
    return shape();
}

void GraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if(graphics_paint_model.getIsCloseItem())
    {
        if(isSelected())
        {
            if(graphics_paint_model.getIsFillItem())
            {
                painter->setBrush(graphics_paint_model.getRFillColorUnselected());
                painter->drawPath(getFillPath());
                painter->setBrush(Qt::NoBrush);
            }
            painter->setBrush(graphics_paint_model.getRFillColorHover());
            painter->drawPath(getFillPath());
            painter->setBrush(Qt::NoBrush);
        }
        else
        {
            if(graphics_paint_model.getIsFillItem())
            {
                painter->setBrush(graphics_paint_model.getRFillColorUnselected());
                painter->drawPath(getFillPath());
                painter->setBrush(Qt::NoBrush);
            }
            else if (is_hover_enter)
            {
                painter->setBrush(graphics_paint_model.getRFillColorHover());
                painter->drawPath(getFillPath());
                painter->setBrush(Qt::NoBrush);
            }
        }
    }
    if(isSelected())
    {
        if(graphics_paint_model.getIsPaintStrokeShape())
        {
            QPen pen(QColor(255,255,255), graphics_paint_model.getRPenIsSelected().width());
            pen.setStyle(Qt::DashLine);
            painter->setPen(pen);
            painter->drawPath(getStrokePath());
            painter->setPen(Qt::NoPen);
        }
        if(graphics_paint_model.getIsPaintCenter())
        {
            painter->setPen(QPen(DEFAULT_COLOR_UNIQUE_POINT, graphics_paint_model.getRPenIsSelected().width()));
            painter->setBrush(DEFAULT_COLOR_UNIQUE_POINT);
            graphics_paint_model.paintPoint(painter, m_center);
            painter->setBrush(Qt::NoBrush);
        }
        setPen(graphics_paint_model.getRPenIsSelected());
    }
    else
    {
        if(is_show_others_no_selected)
        {
            if(graphics_paint_model.getIsPaintStrokeShape())
            {
                QPen pen(Qt::black,2);
                pen.setStyle(Qt::DashLine);
                painter->setPen(pen);
                painter->drawPath(getStrokePath());
                painter->setPen(Qt::NoPen);
            }
        }
        if(graphics_paint_model.getIsPaintCenter())
        {
            painter->setPen(QPen(DEFAULT_COLOR_UNIQUE_POINT, graphics_paint_model.getRPenNoSelected().width()));
            painter->setBrush(DEFAULT_COLOR_UNIQUE_POINT);
            graphics_paint_model.paintPoint(painter, m_center);
            painter->setBrush(Qt::NoBrush);
        }
        setPen(graphics_paint_model.getRPenNoSelected());
    }
    if(parentItem()!=nullptr)
    {
        if(graphics_relation_model.getUnselectedWhileParentSelected() && parentItem()->isSelected()) {
            setSelected(false);
            setPen(graphics_paint_model.getRPenIsSelected());
        }
    }
}

QRectF GraphicsItem::boundingRect() const
{
    const qreal adjust_size = graphics_paint_model.getAdjustSize();
    return QRectF(m_center.x()-abs(m_edge.x()-m_center.x())*2,
                  m_center.y()-abs(m_edge.y()-m_center.y())*2,
                  abs(m_edge.x()-m_center.x())*4,
                  abs(m_edge.y()-m_center.y())*4).
            adjusted(-adjust_size, -adjust_size, 
                adjust_size, adjust_size);
}

void GraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (scene()->selectedItems().size() > 0)
        scene()->clearSelection();
    QGraphicsItem::mousePressEvent(event);
    if (event->button() == Qt::LeftButton){
        m_press_pos = event->pos();
        if (mouse_selected_status == MouseSelectedStatus::SELECTED_ROTATE) return;
        if(parentItem()){
            if(!graphics_relation_model.getIsDependWithparent()){
                move_parent_item = true;
                graphics_relation_model.findOriginParentItem(this)->setSelected(true);
            }
        }
    }
}

void GraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (mouse_selected_status == MouseSelectedStatus::SELECTED_NODE) {
        QGraphicsItem::mouseMoveEvent(event);
    }
    else if (mouse_selected_status == MouseSelectedStatus::SELECTED_ROTATE) {
        rotate(event->pos());
    }
}

void GraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    if (mouse_selected_status == MouseSelectedStatus::SELECTED_ROTATE)
        unsetCursor();
    if (parentItem()) {
        if (!is_double_clicked) {
            parentItem()->setSelected(false);
            setSelected(true);
        }
    }
    mouse_selected_status = MouseSelectedStatus::SELECTED_NODE;
    move_parent_item = false;
    is_double_clicked = false;
}

QVariant GraphicsItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged) {
        if (value.toBool()) {
            graphics_text_model.getTextItem()->setDefaultTextColor(graphics_text_model.getTextColorSelected());
        }
        else {
            graphics_text_model.getTextItem()->setDefaultTextColor(graphics_text_model.getTextColorNoSelected());
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

void GraphicsItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
    Q_UNUSED(event);
}

void GraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    if (is_accept_hover_event) {
        if (graphics_relation_model.getIsHideChildItemList()) {
            for (GraphicsItem* item : graphics_relation_model.getChildItemList()) {
                item->setVisible(true);
            }
        }
        is_hover_enter = true;
    }
    QGraphicsItem::hoverEnterEvent(event);
}

void GraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if (graphics_relation_model.getIsHideChildItemList()) {
        for (GraphicsItem* item : graphics_relation_model.getChildItemList()) {
            item->setVisible(false);
        }
    }
    is_hover_enter = false;
    QGraphicsItem::hoverLeaveEvent(event);
}

void GraphicsItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if (is_generate_context_menu)
    {
        setSelected(true);
        StyleSheetConfigModel style_model;
        style_model.setMenuStyle(&menu);

        QAction* textEditAction = menu.addAction(QStringLiteral("Edit Text"), &graphics_text_model, &GraphicsTextModel::onActionEditText);
        QAction* text_status = menu.addAction(QStringLiteral("Text Status"), [=]() {
            graphics_text_model.setIsHideText(!graphics_text_model.getIsHideText());
            });
        QAction* extra_text_status = menu.addAction(QStringLiteral("Message Status"), [=]() {
            graphics_text_model.setIsHideExtraText(!graphics_text_model.getIsHideExtraText());
            });
        QAction* removeAction = menu.addAction(QStringLiteral("Delete"), this, &GraphicsItem::onActionRemoveSelf);
        Q_UNUSED(removeAction);
        if (is_rotate_action)
        {
            QAction* rotateAction = menu.addAction(QStringLiteral("Rotate"), this, &GraphicsItem::onActionRotate);
            Q_UNUSED(rotateAction);
        }
        if (graphics_paint_model.getIsCloseItem())
        {
            QAction* fillAction = menu.addAction(QStringLiteral("Fill"), [=]() {
                graphics_paint_model.setIsFillItem(!graphics_paint_model.getIsFillItem());
                });
            Q_UNUSED(fillAction);
        }
        if (is_change_cener_aciton)
        {
            QAction* hideCener = menu.addAction(QStringLiteral("Center Status"), [=]() {
                graphics_paint_model.setIsPaintCenter(!graphics_paint_model.getIsPaintCenter());
                });
            Q_UNUSED(hideCener);
        }
        Q_UNUSED(textEditAction);
        Q_UNUSED(text_status);
        Q_UNUSED(extra_text_status);
        Q_UNUSED(event);
        menu.exec(QCursor::pos());
        menu.clear();
    }
    else menu.clear();
}

void GraphicsItem::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_F:
        if(graphics_paint_model.getIsCloseItem()) 
            graphics_paint_model.setIsFillItem(!graphics_paint_model.getIsFillItem());
        break;
    case Qt::Key_R:
        if(is_rotate_action) onActionRotate();
        break;
    case Qt::Key_T:
        graphics_text_model.setIsHideText(!graphics_text_model.getIsHideText());
        break;
    case Qt::Key_Down:
        if(isSelected()&&parentItem()==nullptr)moveBy(0, 10);
        break;
    case Qt::Key_Up:
        if(isSelected()&&parentItem()==nullptr)moveBy(0, -10);
        break;
    case Qt::Key_Right:
        if(isSelected()&&parentItem()==nullptr)moveBy(10, 0);
        break;
    case Qt::Key_Left:
        if(isSelected()&&parentItem()==nullptr)moveBy(-10, 0);
        break;
    default:
        break;
    }
}

void GraphicsItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    is_double_clicked = true;

    if (parentItem() && !graphics_relation_model.getIsDependWithparent()){
        move_parent_item = true;
        graphics_relation_model.findOriginParentItem(this)->setSelected(true);
    }
    Q_UNUSED(event);
}

void GraphicsItem::onActionRemoveSelf()
{
    //slot connect with contextMenuEvent::delete Action
    if (parentItem() == nullptr)
    {
        setVisible(false);
        //qDebug() << "null" << data(1);
        emit prepareToRemove();
        if (!delete_direct){
            //qDebug() << "deletelater";
            deleteLater();
        }
        else delete this;
        //qDebug() << "delete";
        //qDebug() << "------------------------------------";
        return;
    }
    else
    {
        //qDebug()<<"par"<<data(1);
        QGraphicsItem *origin_parent = parentItem();
        while(origin_parent->parentItem()!=nullptr) origin_parent = origin_parent->parentItem();
        GraphicsItem *item = dynamic_cast<GraphicsItem*>(origin_parent);
        //qDebug() << "called parent slotRemoveSelf method";
        //qDebug() << "------------------------------------";
        if(item)item->onActionRemoveSelf();
    }
}

void GraphicsItem::onPointSelected()
{
}

void GraphicsItem::onActionRotate()
{
    if (!is_rotate_action)return;
    mouse_selected_status = MouseSelectedStatus::SELECTED_ROTATE;
    setCursor(QCursor(QPixmap(":/res/background-image/rotate_view.png").scaled(QSize(25, 25))));
}

QPointF GraphicsItem::getCenterFromTwoPoint(QPointF& p, QPointF& q)
{
    return QPointF((p.x() + q.x()) / 2, (p.y() + q.y()) / 2);
}

QPointF GraphicsItem::getStartFromTwoPoint(QPointF &center, QPointF &edge)
{
    return QPointF(center.x()-abs(edge.x()-center.x()),
                   center.y()-abs(edge.y()-center.y()));
}

void GraphicsItem::rotate(const QPointF& mousePos)
{
    QPointF pos_before = mapToScene(m_center);
    // 设置中心点为原点
    QPointF originPos = m_center;
    // 从原点延伸出去两条线，鼠标按下时的点和当前鼠标位置所在点的连线
    QLineF p1 = QLineF(originPos, m_press_pos);
    QLineF p2 = QLineF(originPos, mousePos);
    // 旋转角度
    qreal dRotateAngle = p2.angleTo(p1);
    // 设置旋转中心
    setTransformOriginPoint(m_center);
    // 计算当前旋转的角度
    qreal dCurAngle = this->rotation() + dRotateAngle;
    while (dCurAngle > 360.0) {
        dCurAngle -= 360.0;
    }
    // 设置旋转角度
    prepareGeometryChange();
    setRotation(dCurAngle);
    graphics_transform_model.setRotateAngle(dCurAngle);
    QPointF pos_after = mapToScene(m_center);
    QPointF delta = pos_after - pos_before;
    moveBy(-delta.rx(), -delta.ry());
}

void GraphicsItem::findAllGraphicsChildItems(QList<GraphicsItem*>& child_item_list)
{
    for (QGraphicsItem* childItem : childItems()) {
        GraphicsItem* m_item = dynamic_cast<GraphicsItem*>(childItem);
        if (m_item) {
            child_item_list.push_back(m_item);
            m_item->findAllGraphicsChildItems(child_item_list);
        }
    }
}
