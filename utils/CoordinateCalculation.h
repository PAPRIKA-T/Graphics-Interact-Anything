#pragma once
#include <QPointF>

class ItemPointF;
class QRectF;
namespace CoordinateCalculation {
	double calculatePointToPoint(const QPointF&, const QPointF&); //计算点到点距离
	QPointF getPointToLineVerticalpoint(const QPointF& pt, const QPointF& LineStart, const QPointF& LineEdge); //计算点到直线垂点
	bool verticalPointIsOnLine(const QPointF& np, const QPointF& LineStart, const QPointF& LineEdge); //判断垂点是否在线段上
	double pointToLineDis(const QPointF& pt, const QPointF& LineStart, const QPointF& LineEdge); //获取点到线段最短距离
	QPointF findRoundLinePos(const QPointF& LineStart, const QPointF& LineEdge, const QPointF& agvPos, int radius); //计算直线与圆的交点
	QPointF getCentreFromList(const QList<ItemPointF>& list); //根据参数链表获取中心点
	QRectF getListRect(const QList<ItemPointF>& pointList); //根据参数链表获取合适的boundingrect参数
}