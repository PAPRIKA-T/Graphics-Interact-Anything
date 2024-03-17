#include "GraphicspixmapItem.h"
#include "utils/thread/ImageCvLoaderThread.h"

GraphicsPixmapItem::GraphicsPixmapItem(const QImage &img)
    :QGraphicsPixmapItem (),show_image(img)
{
    setFlags(QGraphicsItem::ItemIsMovable);
    if (!img.isNull()) setShowImage(img);
    setData(0,"GraphicsPixmapItem");}

void GraphicsPixmapItem::setShowImage(const QImage &i)
{
    if (i.isNull()) pixmap_path = "";
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
    scene_compare_origin_scale = fScaleW / origin_width;
}

void GraphicsPixmapItem::setShowImage(const QString& path)
{
    QElapsedTimer timer;
    timer.start();
    if (!show_image.load(path)) {
        qDebug() << path <<"GraphicsPixmapItem::setPixmap(image load fail)";
        return;
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
    scene_compare_origin_scale = fScaleW / origin_width;
    setPixmapPath(path);
    qDebug() << "GraphicsPixmapItem::setPixmap " << timer.elapsed();
}

void GraphicsPixmapItem::updateShowImage(const QImage& i)
{
    show_image = i;
    update();
}

void GraphicsPixmapItem::setPixmapPath(const QString& f)
{
    if (f.isEmpty()) {
        qDebug() << "pixmapPath is null!\nimage load fail"; 
        pixmap_path = "";
        return;
    }
    pixmap_path = f;
    LoadCvImageInNewThread(f);
}

cv::Mat GraphicsPixmapItem::getOrignImageMat(bool clone)
{
    if (!clone) return orgin_mat;
    else return orgin_mat.clone();
}

// 碰撞矩形
QRectF GraphicsPixmapItem::boundingRect() const
{
    QRectF rect;
    rect.setTopLeft(QPointF(0, 0));
    rect.setWidth(fScaleW);
    rect.setHeight(fScaleH);
    return rect;
}

void GraphicsPixmapItem::showOriginalImage()
{
    show_image = original_image;
	update();
}

// 绘画事件
void GraphicsPixmapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QRect img_rect(0,0,
                   static_cast<int>(fScaleW),static_cast<int>(fScaleH));
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->drawImage(img_rect, show_image);
}

void GraphicsPixmapItem::LoadCvImageInNewThread(const QString& f)
{
    ImageCvLoaderThread* image_read_thread = new ImageCvLoaderThread();
    connect(image_read_thread, &ImageCvLoaderThread::imageLoaded, [=](const cv::Mat& image) {
        orgin_mat = image;
        original_pixmap = QPixmap::fromImage(show_image);
        original_image = show_image;

        });
    image_read_thread->setPixmapPath(f);
}
