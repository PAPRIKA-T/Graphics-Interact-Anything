#pragma once
#include <QPointF>
QPointF QpointFMultiplication(const QPointF& p1, const QPointF& p2); //QPointF相乘
QPointF QPointFDivision(const QPointF& p1, const QPointF& p2); //QPointF相除

//交互点的绘制类型
enum class PointPaintType {
    Cross = 0, //十字
    Round, //圆
    Rect, //矩形
};
//点类型
enum class PointType
{
    Start = 0, //起点
    Edge, //边缘点
    Center, //中心点
    RightTop, //右上点
    LeftBottom, //左下点
    StylePoint, //样式点
    Other,
};
//交互点
class ItemPointF : public QPointF
{
public:
    ItemPointF() { ; }
    ItemPointF(const QPoint& p) :QPointF(p) { ; }
    ItemPointF(const QPointF& p) :QPointF(p) { ; }
    ItemPointF(const QPointF& p, PointPaintType type) :QPointF(p) { paint_type = type; }
    ItemPointF(const QPointF& p, PointType type) :QPointF(p) { point_type = type; }

    ItemPointF(qreal xpos, qreal ypos) :QPointF(xpos, ypos) { ; }
    ItemPointF(qreal xpos, qreal ypos, PointPaintType type) :QPointF(xpos, ypos) { paint_type = type; }
    ItemPointF(qreal xpos, qreal ypos, PointType type) :QPointF(xpos, ypos) { point_type = type; }
    ItemPointF(qreal xpos, qreal ypos, PointType type0, PointPaintType type1) :QPointF(xpos, ypos) {
        point_type = type0;
        paint_type = type1;
    }

    ItemPointF& operator=(const QPointF& p) { setX(p.x()); setY(p.y()); return *this; }

    inline void setPaintType(PointPaintType type) { paint_type = type; }
    inline PointPaintType getPaintType() { return paint_type; }

    inline void setPointType(PointType type) { point_type = type; }
    inline PointType getPointType() { return point_type; }
private:
    PointPaintType paint_type = PointPaintType::Round;
    PointType point_type = PointType::Other;
};