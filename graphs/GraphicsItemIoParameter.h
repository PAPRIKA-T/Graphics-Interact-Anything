#pragma once
#include <QString>
#include <QPointF>
class GraphicsItem;

struct GraphicsItemIoParameter
{
	void setGraphicsItemIoParameter(GraphicsItem*);

	QString text{};
	QString ID{};
	int name = 0;
	QString color{};
	QPointF rotate_point{};
	qreal rotate_angle = 0;  //接受item旋转角度
};
