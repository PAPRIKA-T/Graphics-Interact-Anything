#ifndef GRAPHICSPIXMAPITEM_H
#define GRAPHICSPIXMAPITEM_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QDebug>
#include <opencv2/opencv.hpp>

class GraphicsPixmapItem:public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit GraphicsPixmapItem(const QPixmap &pixmap); //构造函数
    void setPixmap(const QPixmap &p);//设置显示图像
    void setPixmap(const QString& path);
    void updatePixmap(const QPixmap &p);//更新图像，只更新内容
    QPixmap getPixmap(){return pix;} //获取图像
    const QImage& getOriginalImage(){return original_image;} //获取原始图像
    const QPixmap& getOriginalPixmap(){return original_pixmap;} //获取原始图像
    void setPixSize(double w,double h){
        fScaleW = w;fScaleH = h;
        scene_compare_origin_scale = fScaleW / origin_width;
    } //设置图像宽高
    const qreal& getFscaleH(){return fScaleH;} //获取图像高度
    const qreal& getFscaleW(){return fScaleW;} //获取图像宽度
    const qreal& getOriginWidth(){return origin_width;} //获取图像原始宽度
    const qreal& getOriginHeight(){return origin_height;} //获取图像原始高度
    const qreal& getScale(){return m_fScale;} //获取宽高比
    void setPixmapPath(const QString& f);//设置图像路径
    QString& getPixmapPath() { return pixmap_path; }//获取图像路径
    const qreal& getSceneCompareOriginScale(){return scene_compare_origin_scale;}//获取图像原始尺寸与导入scene尺寸缩放比
    void setSceneCompareOriginScale(qreal s){scene_compare_origin_scale = s;}//设置图像原始尺寸与导入scene尺寸缩放比
    cv::Mat getOrignImageMat(bool clone);
    virtual QRectF boundingRect() const override;

    void showOriginalPixmap();
protected:
/****************************************************事件函数*********************************************************/
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

private:
    void LoadCvImageInNewThread(const QString&);
    QPixmap pix; //显示图像
    QPixmap original_pixmap; //原始图像
    QImage original_image; //原始图像
    QString pixmap_path = ""; //图像路径
    qreal m_fScale = 1; //宽高比
    qreal fScaleH = 1; //图像高度
    qreal fScaleW = 1;  //图像宽度
    qreal origin_width = 1; //图像原始宽度
    qreal origin_height = 1; //图像原始高度
    qreal scene_compare_origin_scale = 1; //图像原始尺寸与导入scene尺寸缩放比
    cv::Mat orgin_img = {}; //原始图像矩阵
};

#endif // GRAPHICSPIXMAPITEM_H
