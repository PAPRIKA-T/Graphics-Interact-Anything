#include "GraphicspixmapItem.h"
#include <QPainter>
#include <QGraphicsScene>
#include "utils/thread/ImageCvLoaderThread.h"
#include "utils/CVOperation.h"

GiantImageItem::GiantImageItem(const QImage &img)
    :QAbstractGraphicsShapeItem(),show_image(img)
{
    setFlags(QGraphicsItem::ItemIsMovable);
    if (!img.isNull()) setShowImage(img);
    setData(0,"GiantImageItem");}

GiantImageItem::~GiantImageItem()
{

}

bool GiantImageItem::setShowImage(const QImage &i)
{
    resetImageLoadStatus();
    if (i.isNull()) {
        pixmap_path = "";
        return false;
    }
    show_image = i;
    origin_width = show_image.width();
    origin_height = show_image.height();

    m_fScale = origin_width/origin_height;
    if (origin_width > origin_height)
    {
        fScaleW = scene()->width();
        fScaleH = fScaleW / m_fScale;
    }
    else
    {
        fScaleH = scene()->height();
        fScaleW = fScaleH * m_fScale;
    }
    show_compare_origin_size_scale = fScaleW / origin_width;
    LoadCvImageInNewThread();
    return true;
}

bool GiantImageItem::setShowImage(const QString& path)
{
    resetImageLoadStatus();
    if (!show_image.load(path)) {
        qDebug() << path <<"GiantImageItem::setPixmap(image load fail)";
        return false;
    }
    origin_width = show_image.width();
    origin_height = show_image.height();
    m_fScale = origin_width / origin_height;
    if (origin_width > origin_height)
    {
        fScaleW = scene()->width();
        fScaleH = fScaleW / m_fScale;
    }
    else
    {
        fScaleH = scene()->height();
        fScaleW = fScaleH * m_fScale;
    }
    show_compare_origin_size_scale = fScaleW / origin_width;
    setImagePath(path);
    LoadCvImageInNewThread();
    return true;
}

void GiantImageItem::updateShowImage(const QImage& i)
{
    show_image = i;
    update();
}

void GiantImageItem::setImagePath(const QString& f)
{
    if (f.isEmpty()) {
        qDebug() << "pixmapPath is null!\nimage load fail"; 
        pixmap_path = "";
        return;
    }
    pixmap_path = f;
}

cv::Mat GiantImageItem::getOrignImageMat(bool clone)
{
    if (!clone) return orgin_image_mat;
    else return orgin_image_mat.clone();
}

// 碰撞矩形
QRectF GiantImageItem::boundingRect() const
{
    QRectF rect;
    rect.setTopLeft(QPointF(0, 0));
    rect.setWidth(fScaleW);
    rect.setHeight(fScaleH);
    return rect;
}

void GiantImageItem::showOriginalImage()
{
    if (!is_load_image_all_data)return;
    show_image = original_image;
	update();
}

bool GiantImageItem::getIsLoadImageAllData() const
{
    return is_load_image_all_data;
}

// 绘画事件
void GiantImageItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QRect img_rect(0, 0, static_cast<int>(fScaleW), static_cast<int>(fScaleH));
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->drawImage(img_rect, show_image);
}

void GiantImageItem::resetImageLoadStatus()
{
    is_load_image_all_data = false;
}

void GiantImageItem::LoadCvImageInNewThread()
{
    ImageCvLoaderThread* image_read_thread = new ImageCvLoaderThread();
    connect(image_read_thread, &ImageCvLoaderThread::imageLoaded, [=](const cv::Mat& m) {
        if (m.empty()) {
			qDebug() << "Empty Image";
            resetAllImageData();
			return;
		}
        orgin_image_mat = m;
        original_image = show_image;
        is_load_image_all_data = true;
        });
    image_read_thread->translateQImage2cvMat(show_image);
}

void GiantImageItem::resetAllImageData()
{
    orgin_image_mat = cv::Mat{};
    original_image = QImage();
    show_image = original_image;
    pixmap_path = "";
    is_load_image_all_data = false;
}
