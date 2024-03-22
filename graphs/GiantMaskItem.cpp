#include "GiantMaskItem.h"
#include "utils/CVOperation.h"
#include <QPainter>
#include <QBitmap>

GiantMaskItem::GiantMaskItem(QGraphicsItem* parent)
	: QAbstractGraphicsShapeItem(parent)
{
	++count;
}

GiantMaskItem::GiantMaskItem(const QPixmap& i, QGraphicsItem* parent)
	:original_pixmap(i), QAbstractGraphicsShapeItem(parent)
{
	++count;
}

const QPixmap& GiantMaskItem::getOriginalPixmap()
{
	return original_pixmap;
}

GiantMaskItem::~GiantMaskItem()
{
	count--;
	qDebug()<<"GiantMaskItem::count "<<count;
}

void GiantMaskItem::setColor(const QColor& color)
{
	m_color = color;
}

QColor GiantMaskItem::getColor() const
{
	return m_color;
}

void GiantMaskItem::setImageShowSize(const QSize& s)
{
	fScaleW = s.width();
	fScaleH = s.height();

	original_pixmap = QPixmap(s);
	original_pixmap.fill(Qt::transparent);
}

void GiantMaskItem::setMask(const cv::Mat& m)
{
	original_pixmap.fill(m_color);
	original_mask = m;
	cv::bitwise_not(m, m);
	original_pixmap.setMask(QBitmap::fromImage(CVOperation::cvMat2QImage(m)));
}

void GiantMaskItem::setMaskOpacity(qreal opacity)
{
	mask_opacity = opacity;
	qDebug()<<"mask_opacity "<<mask_opacity;
}

void GiantMaskItem::acceptMask()
{
	if (original_pixmap.isNull())return;
	setMaskOpacity(0.8);
}

void GiantMaskItem::resetMask()
{
	if (original_pixmap.isNull())return;
	original_pixmap.fill(Qt::transparent);
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
