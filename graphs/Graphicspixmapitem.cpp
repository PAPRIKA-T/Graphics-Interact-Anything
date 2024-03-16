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
    if (!pix.load(path)) {
        qDebug() << path <<"GraphicsPixmapItem::setPixmap(image load fail)";
        return;
    }
    origin_width = pix.width();
    origin_height = pix.height();

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

// 绘画事件
void GraphicsPixmapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QRect img_rect(0,0,
                   static_cast<int>(fScaleW),static_cast<int>(fScaleH));
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->drawPixmap(img_rect,pix);
}

void GraphicsPixmapItem::LoadCvImageInNewThread(const QString& f)
{
    ImageCvLoaderThread* image_read_thread = new ImageCvLoaderThread();
    connect(image_read_thread, &ImageCvLoaderThread::imageLoaded, [=](const cv::Mat& image) {
        orgin_img = image;
        });
    image_read_thread->setPixmapPath(f);
}
