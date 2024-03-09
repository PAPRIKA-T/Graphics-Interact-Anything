#include "ItemPointF.h"
#define EPS (1e-5) //除数最小量
//QPointF * QPointF
QPointF QpointFMultiplication(const QPointF& p1, const QPointF& p2)
{
    QPointF result;
    result.setX(p1.x() * p2.x());
    result.setY(p1.y() * p2.y());
    return result;
}

//QPointF / QPointF
QPointF QPointFDivision(const QPointF& p1, const QPointF& p2)
{
    QPointF result;
    result.setX(p1.x() / (p2.x() + EPS));
    result.setY(p1.y() / (p2.y() + EPS));
    return result;
}
