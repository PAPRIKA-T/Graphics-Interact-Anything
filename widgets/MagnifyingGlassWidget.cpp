#include "MagnifyingGlassWidget.h"
#include "graphs/GraphicsView.h"
#include <QStyleOption>

/****************************************************放大镜控件*********************************************************/

MagnifyingGlassWidget::MagnifyingGlassWidget(QWidget* parent)
    :QOpenGLWidget(parent)
{
    paint_pos = QPoint(0, 0);
}

MagnifyingGlassWidget::~MagnifyingGlassWidget()
{

}

void MagnifyingGlassWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QStyleOption styleOpt;
    styleOpt.initFrom(this);
    style()->drawPrimitive(QStyle::PE_Widget, &styleOpt, &painter, this);
    Q_UNUSED(event);
    if (!m_view->getMagPixmap().isNull()) {
        QPixmap& pix = m_view->getMagPixmap();
        QSize origin_size = pix.size();
        pix = pix.scaled(pix.size() * zoom_factor, Qt::KeepAspectRatio, Qt::FastTransformation);
        qreal x = (zoom_factor - 1) * origin_size.width() / 2;
        qreal y = (zoom_factor - 1) * origin_size.height() / 2;
        QRect cropRect(x, y, origin_size.width(), origin_size.height());
        pix = pix.copy(cropRect);
        painter.drawPixmap(paint_pos, pix);

        QPen pen;
        pen.setColor(QColor(0, 255, 0));
        pen.setWidth(1);
        painter.setPen(pen);
        //绘制横向线
        painter.drawLine(QPointF(0, height() / 2), QPointF(width(), height() / 2));
        pen.setColor(QColor(255, 0, 0));
        painter.setPen(pen);
        //绘制纵向线
        painter.drawLine(QPointF(width() / 2, 0), QPointF(width() / 2, height()));
    }
}

