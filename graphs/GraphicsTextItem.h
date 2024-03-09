#pragma once

#include <QGraphicsTextItem>
#include <QPainter>

//自定义textItem类
class GraphicsTextItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    GraphicsTextItem(const QString message) :QGraphicsTextItem(message) {
        setFlags(QGraphicsItem::ItemIgnoresTransformations);
        setData(0, "GraphicsTextItem");
        back_ground_color = QColor(0, 0, 0, 0);
    }
    GraphicsTextItem(QPointF p) {
        setPos(p);
        setFlags(QGraphicsItem::ItemIgnoresTransformations);
        setData(0, "GraphicsTextItem");
        back_ground_color = QColor(0, 0, 0, 0);
    }
    ~GraphicsTextItem() override {
	}
    virtual QPainterPath shape() const override;
    virtual void paint(QPainter* painter,
        const QStyleOptionGraphicsItem* option,
        QWidget* widget) override;
    void setBackGroundColor(const QColor& c);
private:
    QColor back_ground_color;
};