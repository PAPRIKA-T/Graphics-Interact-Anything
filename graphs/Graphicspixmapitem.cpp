#include "GraphicspixmapItem.h"
#include "utils/thread/ImageCvLoaderThread.h"

GraphicsPixmapItem::GraphicsPixmapItem(const QPixmap &pixmap)
    :QGraphicsPixmapItem (),pix(pixmap)
{
    setFlags(QGraphicsItem::ItemIsMovable);
    if (!pixmap.isNull()) setPixmap(pixmap);
    setData(0,"GraphicsPixmapItem");}

void GraphicsPixmapItem::setPixmap(const QPixmap &p)
{
    if (p.isNull()) pixmap_path = "";
    pix = p;
    origin_width = pix.width();
    origin_height = pix.height();

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

void GraphicsPixmapItem::setPixmap(const QString& path)
{
    QElapsedTimer timer;
    timer.start();
    if (!original_image.load(path)) {
        qDebug() << path <<"GraphicsPixmapItem::setPixmap(image load fail)";
        return;
    }
    //pix = QPixmap::fromImage(original_image);
    //origin_width = pix.width();
    //origin_height = pix.height();

    origin_width = original_image.width();
    origin_height = original_image.height();
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

void GraphicsPixmapItem::updatePixmap(const QPixmap& p)
{
    pix = p;
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
    if (!clone) return orgin_img;
    else return orgin_img.clone();
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

void GraphicsPixmapItem::showOriginalPixmap()
{
    pix = original_pixmap;
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
    painter->drawImage(img_rect,original_image);
    //painter->drawPixmap(img_rect,pix);
}

void GraphicsPixmapItem::LoadCvImageInNewThread(const QString& f)
{
    ImageCvLoaderThread* image_read_thread = new ImageCvLoaderThread();
    connect(image_read_thread, &ImageCvLoaderThread::imageLoaded, [=](const cv::Mat& image) {
        orgin_img = image;
        //original_pixmap = pix;
        //original_image = pix.toImage();
        });
    image_read_thread->setPixmapPath(f);
}
