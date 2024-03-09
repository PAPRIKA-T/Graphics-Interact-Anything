#include "ThumbnailPixmapItem.h"
#include "Graphicsscene.h"
#include "Graphicsview.h"
#include "graphs/Graphicspixmapitem.h"

ThumbnailPixmapItem::ThumbnailPixmapItem(const QPixmap& pixmap)
	:QGraphicsPixmapItem()
{
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
    setFlag(QGraphicsItem::ItemIsFocusable, false);
    setFlag(QGraphicsItem::ItemIgnoresTransformations);
    if (!pixmap.isNull())setPixmap(pixmap);
    setData(0, "ThumbnailPixmapItem");
    setVisible(false);
}

ThumbnailPixmapItem::~ThumbnailPixmapItem()
{
}

void ThumbnailPixmapItem::setPixmap(const QPixmap& pixmap)
{
    if (pixmap.isNull()) {
        qDebug() << "ThumbnailPixmapItem::setPixmap: pixmap is null!";
        fScaleH = 0;
        fScaleW = 0;
        QGraphicsPixmapItem::setPixmap(pixmap);
        return;
    }
    qreal width = pixmap.width();
    qreal height = pixmap.height();
    m_fScale = width / height;
    if (width > height)
    {
        fScaleW = scene()->width();
        fScaleH = fScaleW / m_fScale;
    }
    else
    {
        fScaleH = scene()->height();
        fScaleW = fScaleH * m_fScale;
    }
    origin_size.setWidth(fScaleW);
    origin_size.setHeight(fScaleH);
    scene_window.setWidth(m_scene->width());
    scene_window.setHeight(m_scene->height());
    resetScaleFactor();
    fScaleH *= scale_factor;
    fScaleW *= scale_factor;
    QGraphicsPixmapItem::setPixmap(pixmap);
}

void ThumbnailPixmapItem::updatePixmap(const QPixmap& p)
{
    QGraphicsPixmapItem::setPixmap(p);
}

void ThumbnailPixmapItem::updateSize()
{
    qreal delta = m_scene->width() / scene_window.width();
    if (delta < 0)return;
    scale_factor *= delta;
    scene_window.setWidth(m_scene->width());
    scene_window.setHeight(m_scene->height());
    fScaleH = origin_size.height()*scale_factor;
    fScaleW = origin_size.width()*scale_factor;
}

void ThumbnailPixmapItem::updateDecorator()
{
    calculateBoxPrompt();
}

void ThumbnailPixmapItem::calculateBoxPrompt()
{
    QPoint scene_lt_pos_on_origin_image = (scale_factor *m_scene->getPixmapItem()->mapFromScene(m_scene->getGraphicsView()->mapToScene(0, 0))).toPoint();
    qreal box_width = scale_factor * m_scene->width() / m_scene->getGraphicsView()->getViewTransFormModel()->getViewScale();
    qreal box_height = scale_factor * m_scene->height() / m_scene->getGraphicsView()->getViewTransFormModel()->getViewScale();

    switch (box_rotate_mode)
    {
    case ThumbnailPixmapItem::RotateMode::OneFourRotate:
        box_prompt.setTopLeft(scene_lt_pos_on_origin_image + QPoint(-box_height, 0));
        box_prompt.setWidth(box_height);
        box_prompt.setHeight(box_width);
        break;
    case ThumbnailPixmapItem::RotateMode::TwoFourRotate:
        box_prompt.setTopLeft(scene_lt_pos_on_origin_image + QPoint(-box_width, -box_height));
        box_prompt.setWidth(box_width);
        box_prompt.setHeight(box_height);
        break;
    case ThumbnailPixmapItem::RotateMode::ThreeFourRotate:
        box_prompt.setTopLeft(scene_lt_pos_on_origin_image + QPoint(0, -box_width));
        box_prompt.setWidth(box_height);
        box_prompt.setHeight(box_width);
        break;
    case ThumbnailPixmapItem::RotateMode::NoRotate:
        box_prompt.setTopLeft(scene_lt_pos_on_origin_image);
        box_prompt.setWidth(box_width);
        box_prompt.setHeight(box_height);
        break;
    default:
        break;
    }
}

void ThumbnailPixmapItem::setBoxRotateMode(const RotateMode& r)
{
    box_rotate_mode = r;
}

void ThumbnailPixmapItem::resetScaleFactor()
{
    scale_factor = 0.3;
}

QRectF ThumbnailPixmapItem::boundingRect() const
{
    QRectF rect(0, 0,
        static_cast<int>(fScaleW), static_cast<int>(fScaleH));
    return rect;
}

void ThumbnailPixmapItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QRect img_rect(0, 0,
        static_cast<int>(fScaleW), static_cast<int>(fScaleH));
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->drawPixmap(img_rect, pixmap());
    painter->setPen(QPen(QColor(255, 0, 0)));
    painter->drawRect(box_prompt);
    painter->setPen(QPen(QColor(255, 255, 0)));
    painter->setBrush(QBrush(QColor(255, 255, 0)));
    painter->setOpacity(0.25);
    painter->drawRect(boundingRect());
}

void ThumbnailPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_press_pos = event->pos();
        m_scene->pixmapItemMoveStart();
        event->accept();
    }
    else {
        event->ignore();
    }
}

void ThumbnailPixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    QPointF delta = event->pos() - m_press_pos;
    delta = delta / scale_factor;
    m_scene->pixmapItemMoveBy(-delta);
}

void ThumbnailPixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsPixmapItem::mouseReleaseEvent(event);
}
