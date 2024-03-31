#pragma once
#include <QAbstractGraphicsShapeItem>
#include <opencv2/opencv.hpp>
#include "core/GiantMacros.h"
class QColor;

class GiantMaskItem : public QObject, public QAbstractGraphicsShapeItem
{
    Q_OBJECT
public:
    static int count; //用于计数
    explicit GiantMaskItem(QGraphicsItem* parent = nullptr);
    explicit GiantMaskItem(const QPixmap&, QGraphicsItem* parent = nullptr);
    const QPixmap& getOriginalPixmap();
    ~GiantMaskItem();
    void onDeleteSelf();
    void setColor(const QColor&);
    QColor getColor() const;
    void setImageShowSize(const QSize&, const QSize&);
    void setMaskOpacity(qreal opacity);
    void resetMask();
    void setMask(const cv::Mat&, bool ok = true);//ok=1上色，ok=0清除
    void applyMaskRangeToLabel(const cv::Mat&, bool ok = true);//ok=1上色，ok=0清除

    void getRectMask(const QRect&,cv::Mat&);
    void update();
    const cv::Mat& getScaledMask();
    cv::Mat getOriginalMask();
signals:
    void preparedToDelete();

protected:
    virtual QRectF boundingRect() const override;
/****************************************************事件函数*********************************************************/
    virtual void paint(QPainter* painter,
        const QStyleOptionGraphicsItem* option,
        QWidget* widget) override;
private:
    QPixmap original_pixmap{}; //原始图像
    QColor m_color = Qt::transparent;

    qreal fScaleW = 1;
    qreal fScaleH = 1;
    qreal original_scaleW = 1;
    qreal original_scaleH = 1;

    qreal mask_opacity = 0.8;
    cv::Mat scaled_mask{};
};

