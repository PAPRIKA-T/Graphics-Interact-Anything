#pragma once
#include <QAbstractGraphicsShapeItem>
#include <opencv2/opencv.hpp>
#include "utils/GiantMacros.h"
class QColor;

class GiantMaskItem : public QAbstractGraphicsShapeItem
{
public:
    static int count; //用于计数
    explicit GiantMaskItem(QGraphicsItem* parent = nullptr);
    explicit GiantMaskItem(const QPixmap&, QGraphicsItem* parent = nullptr);
    const QPixmap& getOriginalPixmap();
    ~GiantMaskItem();
    void setColor(const QColor&);
    QColor getColor() const;
    void setImageShowSize(const QSize&);
    void setMask(const cv::Mat&);
    void setMaskOpacity(qreal opacity);
    void acceptMask();
    void resetMask();
protected:
    virtual QRectF boundingRect() const override;
/****************************************************事件函数*********************************************************/
    virtual void paint(QPainter* painter,
        const QStyleOptionGraphicsItem* option,
        QWidget* widget) override;
private:
    QPixmap original_pixmap{}; //原始图像
    QColor m_color = DEFAULT_COLOR_ITEM;

    qreal fScaleW = 1;
    qreal fScaleH = 1;

    qreal mask_opacity = 0.5;
    cv::Mat original_mask{};
};

