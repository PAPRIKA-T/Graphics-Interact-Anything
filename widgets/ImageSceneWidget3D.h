#pragma once
#include <QWidget>
#include <QGridLayout>

class ImageSceneWidget2D;
class VTKWidget;
/*************************ImageSceneWidget3D************************/
class ImageSceneWidget3D :public QWidget
{
    Q_OBJECT
public:
    ImageSceneWidget3D(QWidget* parent = nullptr);
    ~ImageSceneWidget3D();
    VTKWidget* getVTKWidget() { return vtk_widget; }
    ImageSceneWidget2D* getSceneWidgetSagittal();
    ImageSceneWidget2D* getSceneWidgetTransverse();
    ImageSceneWidget2D* getSceneWidgetCoronal();
    virtual void mousePressEvent(QMouseEvent* event) override;
public slots:
    void zoomWidgetTransverse(bool);
    void zoomWidgetSagittal(bool);
    void zoomWidgetCoronal(bool);
    void zoomWidgetGL(bool);

private:
    QGridLayout* main_layout = nullptr;
    ImageSceneWidget2D* scene_widget_Sagittal = nullptr;
    ImageSceneWidget2D* scene_widget_Coronal = nullptr;
    ImageSceneWidget2D* scene_widget_Transverse = nullptr;
    VTKWidget* vtk_widget = nullptr;
};
