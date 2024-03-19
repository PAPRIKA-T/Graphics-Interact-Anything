#pragma once
#include <QAbstractGraphicsShapeItem>
class QColor;


class GiantMaskItem : public QAbstractGraphicsShapeItem
{
public:
    explicit GiantMaskItem(QGraphicsItem* parent = nullptr);
    explicit GiantMaskItem(const QPixmap&, QGraphicsItem* parent = nullptr);
    ~GiantMaskItem();
    void setColor(QColor color);
    QColor getColor() const;
    void setImageSize(const QSize&, const QSize&);
    void setMask(const QBitmap&);
    void setMaskOpacity(qreal opacity);
protected:
    virtual QRectF boundingRect() const override;
/****************************************************事件函数*********************************************************/
    virtual void paint(QPainter* painter,
        const QStyleOptionGraphicsItem* option,
        QWidget* widget) override;
private:
    QPixmap original_pixmap{}; //原始图像
    QColor m_color = Qt::transparent;
    qreal originW = 1;
    qreal originH = 1;

    qreal fScaleW = 1;
    qreal fScaleH = 1;

    qreal mask_opacity = 0.5; 
};

