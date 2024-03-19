#include "GiantMaskItem.h"
#include <QPainter>

GiantMaskItem::GiantMaskItem(QGraphicsItem* parent)
	: QAbstractGraphicsShapeItem(parent)
{
}

GiantMaskItem::GiantMaskItem(const QImage& i, QGraphicsItem* parent)
	:original_image(i), QAbstractGraphicsShapeItem(parent)
{
}

GiantMaskItem::~GiantMaskItem()
{
}

void GiantMaskItem::setColor(QColor color)
{
	m_color = color;
}

QColor GiantMaskItem::getColor() const
{
	return m_color;
}

void GiantMaskItem::setImageSize(const QSize& s)
{
	fScaleW = s.width();
	fScaleH = s.height();
}

void GiantMaskItem::setImage(const QImage& i)
{
	original_image = i;
}

QRectF GiantMaskItem::boundingRect() const
{

	QRectF rect;
	rect.setTopLeft(QPointF(0, 0));
	rect.setWidth(fScaleW);
	rect.setHeight(fScaleH);
	return rect;
}

void GiantMaskItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	QRect img_rect(0, 0, static_cast<int>(fScaleW), static_cast<int>(fScaleH));
	painter->setRenderHint(QPainter::SmoothPixmapTransform);
	painter->setPen(m_color);
	painter->drawImage(img_rect, original_image);
}

