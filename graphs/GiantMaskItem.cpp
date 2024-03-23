#include "GiantMaskItem.h"
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

void GiantMaskItem::onDeleteSelf()
{
	emit preparedToDelete();
	deleteLater();
}

void GiantMaskItem::setColor(const QColor& color)
{
	m_color = color;
}

QColor GiantMaskItem::getColor() const
{
	return m_color;
}

void GiantMaskItem::setImageShowSize(const QSize& s, const QSize& o)
{
	fScaleW = s.width();
	fScaleH = s.height();
	original_scaleW = o.width();
	original_scaleH = o.height();
	scaled_mask = cv::Mat::zeros(s.height(), s.width(), CV_8UC1);
	original_pixmap = QPixmap(s);
	original_pixmap.fill(Qt::transparent);
}

void GiantMaskItem::setMask(const cv::Mat& m)
{
	original_pixmap.fill(m_color);
	scaled_mask = m.clone();
	cv::bitwise_not(scaled_mask, m);
	original_pixmap.setMask(QBitmap::fromImage(CVOperation::cvMat2QImage(m)));
}

void GiantMaskItem::setMaskOpacity(qreal opacity)
{
	mask_opacity = opacity;
}

void GiantMaskItem::resetMask()
{
	if (original_pixmap.isNull())return;
	scaled_mask = cv::Mat::zeros(fScaleH, fScaleW, CV_8UC1);
	original_pixmap.fill(Qt::transparent);
}

void GiantMaskItem::applyMaskRangeToLabel(const cv::Mat& m)
{
	setMask(scaled_mask | m);
}

void GiantMaskItem::getRectMask(const QRect& r, cv::Mat& m)
{
	QPointF convert_p = mapFromScene(r.topLeft());
	m = cv::Mat::zeros(fScaleH, fScaleW, CV_8UC1);
	cv::rectangle(m, cv::Rect(convert_p.rx(), convert_p.ry(), r.width(), r.height()),
		cv::Scalar(255), -1);
}

void GiantMaskItem::update()
{
	if (!cv::countNonZero(scaled_mask))return;
	std::cout << scaled_mask.size();
	original_pixmap.fill(m_color);
	cv::Mat m{};
	cv::bitwise_not(scaled_mask, m);
	original_pixmap.setMask(QBitmap::fromImage(CVOperation::cvMat2QImage(m)));
}

const cv::Mat& GiantMaskItem::getScaledMask()
{
	return scaled_mask;
}

cv::Mat GiantMaskItem::getOriginalMask()
{
	cv::Mat original_mask{};
	cv::Size original_size(original_scaleW, original_scaleH);
	cv::resize(scaled_mask, original_mask, original_size);
	return original_mask.clone();
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
	painter->setOpacity(mask_opacity);
	painter->drawPixmap(img_rect, original_pixmap);
}
