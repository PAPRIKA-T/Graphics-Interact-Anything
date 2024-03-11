#include "GraphicsPaintModel.h"
#include <QPainter>
GraphicsPaintModel::GraphicsPaintModel()
{
}

GraphicsPaintModel::~GraphicsPaintModel()
{
}

void GraphicsPaintModel::initColorSetting()
{
    m_color_item = DEFAULT_COLOR_ITEM;
    m_pen_no_selected.setColor(DEFAULT_COLOR_ITEM);
    m_pen_is_selected.setColor(Qt::green);
    fill_color_unselected = QColor(102, 0, 204);
    fill_color_unselected.setAlpha(100);
    fill_color_hover = DEFAULT_COLOR_ITEM;
    fill_color_hover.setAlpha(100);
    fill_color_selected = QColor(Qt::green);
    fill_color_selected.setAlpha(100);
    m_pen_is_selected.setWidth(pen_width);
    m_pen_no_selected.setWidth(pen_width);
}

QColor& GraphicsPaintModel::getRItemColor()
{
    return m_color_item;
}

void GraphicsPaintModel::setPenIsSelected(QPen pen)
{
    m_pen_is_selected = pen;
}

QPen GraphicsPaintModel::getPenIsSelected() const
{
    return m_pen_is_selected;
}

QPen& GraphicsPaintModel::getRPenIsSelected()
{
    return m_pen_is_selected;
}

QPen GraphicsPaintModel::getPenNoSelected() const
{
    return m_pen_no_selected;
}

QPen& GraphicsPaintModel::getRPenNoSelected()
{
    return m_pen_no_selected;   
}

void GraphicsPaintModel::setPenIsNoSelected(QPen pen)
{
    m_pen_no_selected = pen;
}

void GraphicsPaintModel::setCenterColor(const QColor& c)
{
    m_color_item = c;
}

QColor GraphicsPaintModel::getCenterColor() const
{
    return m_color_item;
}

void GraphicsPaintModel::setFillColorUnselected(const QColor& c)
{
    fill_color_unselected = c;
}

QColor GraphicsPaintModel::getFillColorUnselected() const
{
    return fill_color_unselected;
}

QColor& GraphicsPaintModel::getRFillColorUnselected()
{
    return fill_color_unselected;
}

void GraphicsPaintModel::setFillColorHover(const QColor& c)
{
    fill_color_hover = c;
}

QColor GraphicsPaintModel::getFillColorHover() const
{
    return fill_color_hover;
}

QColor& GraphicsPaintModel::getRFillColorHover()
{
    return fill_color_hover;
}

bool GraphicsPaintModel::getIsCloseItem() const
{
    return is_close_item;
}

void GraphicsPaintModel::setIsCloseItem(bool ok)
{
    is_close_item = ok;
}

bool GraphicsPaintModel::getIsPaintCenter() const
{
    return is_paint_center;
}

void GraphicsPaintModel::setIsPaintCenter(bool ok)
{
    is_paint_center = ok;
}

bool GraphicsPaintModel::getIsFillItem() const
{
	return is_fill;
}

void GraphicsPaintModel::setIsFillItem(bool ok)
{
	is_fill = ok;
}

const qreal GraphicsPaintModel::getAdjustSize() const
{
    return adjust_size;
}

bool GraphicsPaintModel::getIsPaintStrokeShape() const
{
    return is_paint_stroke_shape;
}

void GraphicsPaintModel::setIsPaintStrokeShape(bool ok)
{
    is_paint_stroke_shape = ok;
}

void GraphicsPaintModel::setAdjustSize(const qreal& size)
{
    adjust_size = size;
}

const qreal GraphicsPaintModel::getScaleFactor() const
{
    return scale_factor;
}

void GraphicsPaintModel::setScaleFactor(const qreal& s)
{
    scale_factor = s;
}

const qreal GraphicsPaintModel::getInteracitonRange() const
{
    return interaction_range;
}

const qreal GraphicsPaintModel::getLineLength() const
{
    return line_lengh;
}

void GraphicsPaintModel::setLineLength(const qreal& l)
{
    line_lengh = l;
}

const qreal GraphicsPaintModel::getPenWidth() const
{
    return pen_width;
}

void GraphicsPaintModel::setPenWidth(const qreal& p)
{
    pen_width = p;
}

void GraphicsPaintModel::setInteracitonRange(const qreal& r)
{
    interaction_range = r;
}

void GraphicsPaintModel::paintPoint(QPainter* painter, ItemPointF& p)
{
    if (p.getPaintType() == PointPaintType::Cross)
    {
        painter->drawLine(p - QPointF(line_lengh, 0), p + QPointF(line_lengh, 0));
        painter->drawLine(p - QPointF(0, line_lengh), p + QPointF(0, line_lengh));
    }
    else if (p.getPaintType() == PointPaintType::Round)
    {
        painter->drawEllipse(p, line_lengh / 2, line_lengh / 2);
    }
    else if (p.getPaintType() == PointPaintType::Rect)
    {
        QPointF lt = p - QPointF(line_lengh / 2, line_lengh / 2);
        painter->drawRect(lt.x(), lt.y(), line_lengh, line_lengh);
    }
}
