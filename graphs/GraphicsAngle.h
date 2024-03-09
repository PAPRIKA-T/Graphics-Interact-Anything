#pragma once
#include "GraphicsLine.h"

/***************************************角度类**************************************************/
class Angle : public GraphicsItem
{
    Q_OBJECT
public:
    Angle();
    Angle(QPointF c);
    ~Angle() override;
    void initItem();
    virtual QPainterPath shape() const override;
    virtual QRectF boundingRect() const override;
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;
    virtual void onActionRemoveSelf() override;
    virtual void onUpdatePointMessage() override;
    virtual int type() const override
    {
        return itemType::AngleType;
    }
    void setAngleToArc(bool ok) { is_angleToArc = ok; }
    bool getAngleToArc() { return is_angleToArc; }
    SingleEndPointLine* getLine1() { return line1; }
    SingleEndPointLine* getLine2() { return line2; }
    CPoint* getCPointItem() { return center_bpoint; }
    virtual void setCenter(const QPointF& e) override;
public slots:
    void angleToArc() {
        if (is_angleToArc)is_angleToArc = false;
        else is_angleToArc = true; graphics_text_model.updateText();
    }
private:
    SingleEndPointLine* line1 = nullptr;
    SingleEndPointLine* line2 = nullptr;
    CPoint* center_bpoint = nullptr; //中心点
    bool is_angleToArc = false;
    qreal angle = 0.0;
};
