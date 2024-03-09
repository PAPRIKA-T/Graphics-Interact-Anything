#pragma once
#include <QWidget>
#include <QtOpenGLWidgets/QOpenGLWidget>
class GraphicsView;
/****************************************************放大镜控件*********************************************************/
class MagnifyingGlassWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    MagnifyingGlassWidget(QWidget* parent = nullptr);
    ~MagnifyingGlassWidget() override;
    void setView(GraphicsView* view) { m_view = view; }
    void setPaintPos(QPoint p) { paint_pos = p; }
    virtual void paintEvent(QPaintEvent* event) override;
    void setZoomFactor(qreal f) { zoom_factor = f; }
    qreal getZoomFactor() { return zoom_factor; }
    void zoomFactorReset() { zoom_factor = 2.0; }
private:
    GraphicsView* m_view;
    QPoint paint_pos;
    qreal zoom_factor = 2.0;
};