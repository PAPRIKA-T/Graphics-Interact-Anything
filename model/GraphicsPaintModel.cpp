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
    fill_color_hover = DEFAULT_COLOR_ITEM;
    fill_color_selected = QColor(Qt::green);
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
