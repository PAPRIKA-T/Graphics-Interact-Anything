#pragma once
#include "GenericAbstractModel.h"
#include <QRect>
class QPainter;

class GraphicsViewPaintModel : public GenericAbstractModel
{
	friend class GraphicsView;
private:
	enum class PaintStyleType {
		PAINT_NONE,
		PAINT_CROSS,
		PAINT_RECT,
		PAINT_ROUND,
	};
	void paintCrossStyle(QPainter*);
	void paintRectStyle(QPainter*);
	const QRect& getSprayRect();
	PaintStyleType paint_style_type{ PaintStyleType::PAINT_NONE };
	int spray_rect_size{20}; //px
	QRect spray_rect{};
};

