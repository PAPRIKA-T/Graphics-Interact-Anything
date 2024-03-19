#pragma once
#include <QAbstractGraphicsShapeItem>
class QColor;
class QImage;

class GiantMaskItem : public QAbstractGraphicsShapeItem
{
public:
    explicit GiantMaskItem(QGraphicsItem* parent = nullptr);
    explicit GiantMaskItem(const QImage&, QGraphicsItem* parent = nullptr);
    ~GiantMaskItem();
    void setColor(QColor color);
    QColor getColor() const;
    void setImageSize(const QSize&);
    void setImage(const QImage&);

protected:
    virtual QRectF boundingRect() const override;
/****************************************************事件函数*********************************************************/
    virtual void paint(QPainter* painter,
        const QStyleOptionGraphicsItem* option,
        QWidget* widget) override;
private:
    QImage original_image; //原始图像
    QColor m_color = nullptr;
    qreal fScaleH = 1; //图像高度
    qreal fScaleW = 1;  //图像宽度
};

