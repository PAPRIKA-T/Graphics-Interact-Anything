#ifndef GRAPHICSPIXMAPITEM_H
#define GRAPHICSPIXMAPITEM_H
#include <QAbstractGraphicsShapeItem>
#include <opencv2/opencv.hpp>

class GiantImageItem:public QObject,public QAbstractGraphicsShapeItem
{
    Q_OBJECT
public:
    explicit GiantImageItem(const QImage &); //构造函数
    ~GiantImageItem();
    bool setShowImage(const QImage &);//设置显示图像
    bool setShowImage(const QString&);
    void updateShowImage(const QImage &);//更新图像，只更新内容
    void showOriginalImage();

    cv::Mat getOrignImageMat(bool clone);
    const QImage& getShowImage(){return show_image;} //获取图像
    const QImage& getOriginalImage(){return original_image;} //获取原始图像

    bool getIsLoadImageAllData() const;

    const QSize getFscaleSize(){return QSize(fScaleW,fScaleH);} //获取图像显示尺寸
    const QSize getOriginSize(){return QSize(origin_width,origin_height);} //获取图像原始尺寸
    const qreal& getFscaleH(){return fScaleH;} //获取图像高度
    const qreal& getFscaleW(){return fScaleW;} //获取图像宽度
    const qreal& getOriginWidth(){return origin_width;} //获取图像原始宽度
    const qreal& getOriginHeight(){return origin_height;} //获取图像原始高度
    const qreal& getScale(){return m_fScale;} //获取宽高比
    const qreal& getSceneCompareOriginScale() { return show_compare_origin_size_scale; }//获取图像原始尺寸与导入scene尺寸缩放比
    const int& getImageDepth(){return iamge_depth;} //获取图像深度
    void setImagePath(const QString& f);//设置图像路径
    QString& getImagePath() { return pixmap_path; }//获取图像路径
protected:
    virtual QRectF boundingRect() const override;
/****************************************************事件函数*********************************************************/
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

private:
    void resetImageLoadStatus();
    void LoadCvImageInNewThread();
    void resetAllImageData();

    QImage original_image{}; //原始图像
    QImage show_image{}; //显示图像

    QString pixmap_path = ""; //图像路径
    qreal m_fScale = 1; //宽高比
    qreal fScaleH = 1; //图像高度
    qreal fScaleW = 1;  //图像宽度
    qreal origin_width = 1; //图像原始宽度
    qreal origin_height = 1; //图像原始高度

    int iamge_depth{ 0 }; //图像深度
    qreal show_compare_origin_size_scale = 1; //图像原始尺寸与导入scene尺寸缩放比
    cv::Mat orgin_image_mat = {}; //原始图像矩阵

    bool is_load_image_all_data = false; //是否加载完整图像数据
};

#endif // GRAPHICSPIXMAPITEM_H
