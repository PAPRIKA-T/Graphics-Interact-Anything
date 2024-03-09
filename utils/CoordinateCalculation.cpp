#include "CoordinateCalculation.h"
#include "graphs/ItemPointF.h"
#include <QtMath>
#include <QList>
#include <QRect>

#define EPS (1e-5) //除数最小量

double CoordinateCalculation::calculatePointToPoint(const QPointF& p, const QPointF& q)
{
    double dis = 0;
    double width = p.x() - q.x();
    double height = p.y() - q.y();
    if (0.0 == width && 0.0 == height) return 0;
    dis = qSqrt(width * width + height * height);
    return dis;
}

QPointF CoordinateCalculation::getPointToLineVerticalpoint
(const QPointF& pt, const QPointF& LineStart, const QPointF& LineEdge)
{
    QPointF np; //垂点
    double x_se = LineStart.x() - LineEdge.x();
    double y_se = LineStart.y() - LineEdge.y();
    // 表示线段的开始顶点与结束顶点重合，因此直接返回其中一点即可
    if (0.0 == x_se && 0.0 == y_se)
    {
        np.setX(LineStart.x());
        np.setY(LineStart.y());
        return np;
    }
    double x_se_2 = x_se * x_se; // x平方
    double y_se_2 = y_se * y_se; // y平方

    double x = (x_se_2 * pt.x() + (pt.y() - LineStart.y()) * y_se * x_se +
        LineStart.x() * y_se_2) /
        (x_se_2 + y_se_2 + EPS);

    double y;
    if (y_se != 0)
        y = pt.y() + x_se * (pt.x() - x) / (y_se + EPS);
    else
        y = LineStart.y();

    np.setX(x);
    np.setY(y);
    return np;
}

bool CoordinateCalculation::verticalPointIsOnLine
(const QPointF& np, const QPointF& LineStart, const QPointF& LineEdge)
{
    bool isOnX = false;  // 垂点的x坐标是否在线段上
    bool isOnY = false;  // 垂点的y坐标是否在线段上
    // 1.判断x坐标
    // 1.1 判断线段的起始位置。从左向右还是从右向左
    // 1.2 判断目录的x坐标是否在线段的区间内。
    if (LineStart.x() > LineEdge.x()) {
        if (np.x() <= LineStart.x() && np.x() >= LineEdge.x()) {
            isOnX = true;
        }
    }
    else {
        if (np.x() <= LineEdge.x() && np.x() >= LineStart.x()) {
            isOnX = true;
        }
    }
    // 2.判断y坐标
    // 2.1 判断线段的起始位置。从上向下还是从下向上
    // 2.2 判断目录的y坐标是否在线段的区间内。
    if (LineStart.y() > LineEdge.y()) {
        if (np.y() <= LineStart.y() && np.y() >= LineEdge.y()) {
            isOnY = true;
        }
    }
    else {
        if (np.y() <= LineEdge.y() && np.y() >= LineStart.y()) {
            isOnY = true;
        }
    }
    // 如果x，y坐标都在线段区间内那么认为坐标在线段上。
    if (isOnX && isOnY) {
        return true;
    }
    return false;
}

double CoordinateCalculation::pointToLineDis
(const QPointF& pt, const QPointF& LineStart, const QPointF& LineEdge)
{
    // 1. 获取垂点
    QPointF Verticalpoint;
    Verticalpoint = CoordinateCalculation::getPointToLineVerticalpoint(pt, LineStart, LineEdge);
    // 2. 判断垂点是否在线段上
    bool isOnSeg = CoordinateCalculation::verticalPointIsOnLine(Verticalpoint, LineStart, LineEdge);
    if (isOnSeg) {
        // 如果在线段上那么垂点到点的距离最短
        return CoordinateCalculation::calculatePointToPoint(pt, Verticalpoint);
    }
    else {
        // 如果不在点段上那么到两点的距离最短。
        double startDistance = CoordinateCalculation::calculatePointToPoint(pt, LineStart);
        double endDistance = CoordinateCalculation::calculatePointToPoint(pt, LineEdge);
        return startDistance < endDistance ? startDistance : endDistance;
    }
}

QPointF CoordinateCalculation::findRoundLinePos
(const QPointF& LineStart, const QPointF& LineEdge, const QPointF& agvPos, int radius)
{
    QPoint point1, point2;
    double m = 0;
    double k, b;
    //计算分子
    m = LineStart.x() - LineEdge.x();
    //求直线的方程
    if (0.0 == m) {
        k = 100000;
        b = LineStart.y() - k * LineStart.x();
    }
    else {
        k = (LineEdge.y() - LineStart.y()) / (LineEdge.x() - LineStart.x());
        b = LineStart.y() - k * LineStart.x();
    }
    //求直线与圆的交点 前提是圆需要与直线有交点
    if (static_cast<int>(fabs(k * agvPos.x() - agvPos.y() + b) / sqrt(k * k + b * b)))
    {
        point1.setX(static_cast<int>((2 * agvPos.x() - 2 * k * (b - agvPos.y()) + sqrt(pow((2 * k * (b - agvPos.y()) - 2 * agvPos.x()), 2) - 4 * (k * k + 1) * ((b - agvPos.y()) * (b - agvPos.y()) + agvPos.x() * agvPos.x() - radius * radius))) / (2 * k * k + 2)));
        point2.setX(static_cast<int>((2 * agvPos.x() - 2 * k * (b - agvPos.y()) - sqrt(pow((2 * k * (b - agvPos.y()) - 2 * agvPos.x()), 2) - 4 * (k * k + 1) * ((b - agvPos.y()) * (b - agvPos.y()) + agvPos.x() * agvPos.x() - radius * radius))) / (2 * k * k + 2)));
        point1.setY(static_cast<int>(k * point1.x() + b));
        point2.setY(static_cast<int>(k * point2.x() + b));
    }
    //第一个点到终点的距离大于第二个点到终点坐标的距离，则返回的是第二个点坐标
    if ((pow(point1.x() - LineEdge.x(), 2) + pow(point1.y() - LineEdge.y(), 2)) >
        ((pow(point2.x() - LineEdge.x(), 2) + pow(point2.y() - LineEdge.y(), 2))))
        return point2;
    else return point1;
}

QPointF CoordinateCalculation::getCentreFromList(const QList<ItemPointF>& list)
{
    qreal x = 0;
    qreal y = 0;
    for (auto& temp : list)
    {
        x += temp.x();
        y += temp.y();
    }
    x = x / list.size();
    y = y / list.size();
    return QPointF(x, y);
}

QRectF CoordinateCalculation::getListRect(const QList<ItemPointF>& pointList)
{
    QVector<qreal> X;
    QVector<qreal> Y;
    qreal maxX = 0;
    qreal maxY = 0;
    qreal minX = 0;
    qreal minY = 0;
    int i = 0;
    int point_size = pointList.size();
    for (i = 0; i < point_size; i++)
    {
        X.push_back(pointList[i].x());
        Y.push_back(pointList[i].y());
    }
    i = 0;
    foreach(qreal x, X)
    {
        if (i == 0)
        {
            minX = pointList[0].x();
            maxX = pointList[0].x();
            i++;
        }
        else
        {
            if (x < minX) minX = x;
            else if (x > maxX) maxX = x;
        }
    }
    i = 0;
    foreach(qreal y, Y)
    {
        if (i == 0)
        {
            minY = pointList[0].y();
            maxY = pointList[0].y();
            i++;
        }
        else
        {
            if (y < minY) minY = y;
            else if (y > maxY) maxY = y;
        }
    }
    return QRectF(minX, minY, maxX - minX, maxY - minY);
}
