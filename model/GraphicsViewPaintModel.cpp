#include "GraphicsViewPaintModel.h"
#include "graphs/GraphicsView.h"
#include <QPainter>

void GraphicsViewPaintModel::paintCrossStyle(QPainter* painter)
{
    QPen pen;
    pen.setColor(QColor(91, 220, 47));
    pen.setWidth(1);
    painter->setPen(pen);
    QPoint& p = m_view->getMousePresentPos();
    //绘制横向线
    painter->drawLine(QPoint(0, p.ry()), QPoint(m_view->width(), p.ry()));
    pen.setColor(QColor(220, 60, 74));
    painter->setPen(pen);
    //绘制纵向线
    painter->drawLine(QPoint(p.rx(), 0), QPoint(p.rx(), m_view->height()));
}

void GraphicsViewPaintModel::paintRectStyle(QPainter* painter)
{
    QPen pen;
    pen.setColor(QColor(220, 60, 74));
    pen.setWidth(1);
    painter->setPen(pen);
    QPoint& p = m_view->getMousePresentPos();
    spray_rect = QRect(p.rx() - (spray_rect_size >> 1), p.ry() - (spray_rect_size >> 1),
		spray_rect_size, spray_rect_size);
    painter->drawRect(spray_rect);
}

const QRect& GraphicsViewPaintModel::getSprayRect()
{
    return spray_rect;
}
