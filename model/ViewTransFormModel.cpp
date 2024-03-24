#include "ViewTransFormModel.h"
#include "graphs/GraphicsView.h"
#include "graphs/GraphicsScene.h"
#include "graphs/Graphicspixmapitem.h"
#include "graphs/ThumbnailPixmapItem.h"
#include "widgets/MagnifyingGlassWidget.h"

ViewTransFormModel::ViewTransFormModel()
    : m_zoom_delta(0.1), view_scale(1.0)
{

}

ViewTransFormModel::~ViewTransFormModel()
{

}

void ViewTransFormModel::setGraphicsView(GraphicsView * view)
{
    m_view = view;
    m_scene = m_view->getGraphicsScene();
    pixmap_item = m_scene->getPixmapItem();
}

void ViewTransFormModel::rotateR()
{
    transform_rotate_total.rotate(90);
    transform_total.rotate(90);
    rotate_angle += 90;
    updateRotateParameter();
    m_view->setTransform(transform_total);
    originPositonReturn();
}

qreal ViewTransFormModel::getZoomDelta() const
{
    return m_zoom_delta;
}

void ViewTransFormModel::setViewScale(const qreal& s)
{
    transform_total.scale(1.0, 1.0);
    // 防止过小或过大
    qreal factor = m_view->transform().scale(s, s).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.01 || factor > 10)
        return;
    view_scale = s;
    transform_total.scale(s, s);
    m_view->setTransform(transform_total);
    foreach(QGraphicsItem * item, m_view->items()) {
        GraphicsItem* new_item = dynamic_cast<GraphicsItem*>(item);
        if (new_item) new_item->setViewScale(view_scale);
    }
    m_scene->updateRtText();
}

qreal ViewTransFormModel::getViewScale()
{
    return view_scale;
}

void ViewTransFormModel::setImageScaleTotal(const qreal& s)
{
    image_scale_total = s;
}

qreal ViewTransFormModel::getImageScaleTotal()
{
    return image_scale_total;
}

void ViewTransFormModel::rotateL()
{
    transform_rotate_total.rotate(-90);
    transform_total.rotate(-90);
    rotate_angle -= 90;
    updateRotateParameter();
    m_view->setTransform(transform_total);
    originPositonReturn();
}

void ViewTransFormModel::rotateX()
{
    transform_rotate_total.rotate(180, Qt::YAxis);
    transform_total.rotate(180, Qt::YAxis);
    m_view->setTransform(transform_total);
    originPositonReturn();
}

void ViewTransFormModel::rotateY()
{
    transform_rotate_total.rotate(180, Qt::XAxis);
    transform_total.rotate(180, Qt::XAxis);
    m_view->setTransform(transform_total);
    originPositonReturn();
}

void ViewTransFormModel::resetTransform()
{
    transform_total.reset();
    view_scale = 1.0;
    m_view->setTransform(transform_total);
    resetRotateParameter();
    originPositonReturn();
    m_scene->updateRtText();
    m_scene->updateTextPos();
}

void ViewTransFormModel::setActived(bool ok)
{
    is_actived = ok;
}

bool ViewTransFormModel::isZoomIn()
{
    return is_zoom_in;
}

void ViewTransFormModel::setZoomIn(bool ok)
{
    is_zoom_in = ok;
}

void ViewTransFormModel::updateRotateParameter()
{
    if (rotate_angle < 0) rotate_angle += 360;
    else if (rotate_angle >= 360)rotate_angle -= 360;
    if (rotate_angle == 0) m_scene->getThumbnailItem()->
        setBoxRotateMode(ThumbnailPixmapItem::RotateMode::NoRotate);
    else if (rotate_angle == 90) m_scene->getThumbnailItem()->
        setBoxRotateMode(ThumbnailPixmapItem::RotateMode::ThreeFourRotate);
    else if (rotate_angle == 180) m_scene->getThumbnailItem()->
        setBoxRotateMode(ThumbnailPixmapItem::RotateMode::TwoFourRotate);
    else if (rotate_angle == 270) m_scene->getThumbnailItem()->
        setBoxRotateMode(ThumbnailPixmapItem::RotateMode::OneFourRotate);
}

void ViewTransFormModel::resetRotateParameter()
{
    rotate_angle = 0;
    updateRotateParameter();
}

void ViewTransFormModel::setOriginPosition(const QPoint& p)
{
    origin_position = p;
}

QPoint ViewTransFormModel::getOriginPosition()
{
    return origin_position;
}

void ViewTransFormModel::originPositonReturn()
{
    qreal dx = pixmap_item->pos().x() - origin_position.x();
    qreal dy = pixmap_item->pos().y() - origin_position.y();
    pixmap_item->setPos(origin_position);
    foreach(QGraphicsItem * item, m_view->items()) {
        if (m_scene->isPaintItem(item) && item->data(1) != "CalculateLine") item->moveBy(-dx, -dy);
    }
    m_scene->updateThumbnailBox();
}

void ViewTransFormModel::fitScreen()
{
    if (pixmap_item->getShowImage().isNull())return;
    qreal scaleFactor{};
    if (pixmap_item->getOriginWidth() > pixmap_item->getOriginHeight())
        scaleFactor = m_view->width() / (pixmap_item->getOriginWidth() * image_scale_total);
    else
        scaleFactor = m_view->height() / (pixmap_item->getOriginHeight() * image_scale_total);
    m_view->centerOn(pixmap_item);
    zoom(scaleFactor);
    m_scene->updateThumbnailBox();
}

void ViewTransFormModel::zoomIn()
{
    zoom(1 + m_zoom_delta);
}

void ViewTransFormModel::zoomOut()
{
    zoom(1 - m_zoom_delta);
}

void ViewTransFormModel::zoom(const qreal& scaleFactor)
{
    // 防止过小或过大
    qreal factor = m_view->transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.05 || factor > 20)
        return;
    view_scale *= scaleFactor;
    transform_total.scale(scaleFactor, scaleFactor);
    m_view->setTransform(transform_total);
    foreach(QGraphicsItem * item, m_view->items()) {
        GraphicsItem* new_item = dynamic_cast<GraphicsItem*>(item);
        if (new_item) new_item->setViewScale(view_scale);
    }
    m_scene->updateRtText();
    m_scene->updateTextPos();
}

void ViewTransFormModel::setZoomDelta(const qreal& delta)
{
    // 建议增量范围
    Q_ASSERT_X(delta >= 0.0 && delta <= 1.0,
        "InteractiveView::setZoomDelta", "Delta should be in range [0.0, 1.0].");
    m_zoom_delta = delta;
}
