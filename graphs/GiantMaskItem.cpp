﻿#include "GiantMaskItem.h"
#include "utils/CVOperation.h"
#include <QPainter>
#include <QBitmap>
#include <QElapsedTimer>

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
	qDebug() << "GiantMaskItem::count " << count;
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
	original_mask = cv::Mat(s.height(), s.width(), CV_8UC1);
	original_pixmap = QPixmap(s);
	original_pixmap.fill(Qt::transparent);
}

void GiantMaskItem::setMask(const cv::Mat& m)
{
	original_pixmap.fill(m_color);
	original_mask = m.clone();
	cv::bitwise_not(original_mask, m);
	original_pixmap.setMask(QBitmap::fromImage(CVOperation::cvMat2QImage(m)));
}

void GiantMaskItem::setMaskOpacity(qreal opacity)
{
	mask_opacity = opacity;
}

void GiantMaskItem::resetMask()
{
	if (original_pixmap.isNull())return;
	original_mask = cv::Mat::zeros(fScaleH, fScaleW, CV_8UC1);
	original_pixmap.fill(Qt::transparent);
}

void GiantMaskItem::addMaskRange(const cv::Mat& m)
{
	setMask(original_mask | m);
}

void GiantMaskItem::addRectRange(const QRect& r)
{
	QPointF convert_p = mapFromScene(r.topLeft());
	cv::Mat m = cv::Mat::zeros(fScaleH, fScaleW, CV_8UC1);
	cv::rectangle(m, cv::Rect(convert_p.rx(), convert_p.ry(), r.width(), r.height()), 
		cv::Scalar(255), -1);
	addMaskRange(m);
}

const cv::Mat& GiantMaskItem::getOriginalMask()
{
	return original_mask;
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
