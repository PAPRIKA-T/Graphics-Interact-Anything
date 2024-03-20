#include "GiantMaskItem.h"
#include <QPainter>
#include <QBitmap>

GiantMaskItem::GiantMaskItem(QGraphicsItem* parent)
	: QAbstractGraphicsShapeItem(parent)
{
}

GiantMaskItem::GiantMaskItem(const QPixmap& i, QGraphicsItem* parent)
	:original_pixmap(i), QAbstractGraphicsShapeItem(parent)
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

void GiantMaskItem::setImageSize(const QSize& s, const QSize& o)
{
	fScaleW = s.width();
	fScaleH = s.height();

	originW = o.width();
	originH = o.height();

	original_pixmap = QPixmap(s);
	original_pixmap.fill(Qt::transparent);
}

void GiantMaskItem::setMask(const QBitmap& m)
{
	original_pixmap.fill(m_color);
	original_pixmap.setMask(m);
}

void GiantMaskItem::setMaskOpacity(qreal opacity)
{
	mask_opacity = opacity;
}

void GiantMaskItem::acceptMask()
{
	if (original_pixmap.isNull())return;
	setMaskOpacity(0.8);
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
	painter->setOpacity(mask_opacity);
	painter->drawPixmap(img_rect, original_pixmap);
}

