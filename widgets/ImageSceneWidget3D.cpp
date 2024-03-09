#include "ImageSceneWidget3D.h"
#include "ImageSceneWidget2D.h"
#include "graphs/Graphicsscene.h"
#include "VTKWidget.h"

/*****************ImageSceneWidget3D**********************/
ImageSceneWidget3D::ImageSceneWidget3D(QWidget* parent)
    :QWidget(parent)
{
    main_layout = new QGridLayout(this);
    scene_widget_Sagittal = new ImageSceneWidget2D(this);
    scene_widget_Transverse = new ImageSceneWidget2D(this);
    scene_widget_Coronal = new ImageSceneWidget2D(this);
    vtk_widget = new VTKWidget(this);
    main_layout->addWidget(scene_widget_Sagittal, 0, 0, 1, 1);
    main_layout->addWidget(scene_widget_Transverse, 0, 1, 1, 1);
    main_layout->addWidget(vtk_widget, 1, 0, 1, 1);
    main_layout->addWidget(scene_widget_Coronal, 1, 1, 1, 1);
    main_layout->setContentsMargins(1, 1, 1, 1);
    main_layout->setSpacing(2);
    setLayout(main_layout);
    connect(scene_widget_Sagittal->getGraphicsScene(), &GraphicsScene::zoom3DLayout, 
        this, &ImageSceneWidget3D::zoomWidgetSagittal);
    connect(scene_widget_Transverse->getGraphicsScene(), &GraphicsScene::zoom3DLayout, 
        this, &ImageSceneWidget3D::zoomWidgetTransverse);
    connect(scene_widget_Coronal->getGraphicsScene(), &GraphicsScene::zoom3DLayout, 
        this, &ImageSceneWidget3D::zoomWidgetCoronal);
}

ImageSceneWidget3D::~ImageSceneWidget3D()
{
    delete scene_widget_Sagittal;
    delete scene_widget_Transverse;
    delete scene_widget_Coronal;
    delete vtk_widget;
    delete main_layout;
}

ImageSceneWidget2D* ImageSceneWidget3D::getSceneWidgetSagittal()
{
    return scene_widget_Sagittal;
}

ImageSceneWidget2D* ImageSceneWidget3D::getSceneWidgetTransverse()
{
    return scene_widget_Transverse;
}

ImageSceneWidget2D* ImageSceneWidget3D::getSceneWidgetCoronal()
{
    return scene_widget_Coronal;
}

void ImageSceneWidget3D::zoomWidgetTransverse(bool zoom)
{
    if (zoom) {
        scene_widget_Sagittal->setVisible(false);
        scene_widget_Coronal->setVisible(false);
        vtk_widget->setVisible(false);
    }
    else {
        scene_widget_Coronal->setVisible(true);
        scene_widget_Sagittal->setVisible(true);
        vtk_widget->setVisible(true);
    }
}

void ImageSceneWidget3D::zoomWidgetSagittal(bool zoom)
{
    if (zoom) {
        scene_widget_Transverse->setVisible(false);
        scene_widget_Coronal->setVisible(false);
        vtk_widget->setVisible(false);
    }
    else {
        scene_widget_Transverse->setVisible(true);
        scene_widget_Coronal->setVisible(true);
        vtk_widget->setVisible(true);
    }
}

void ImageSceneWidget3D::zoomWidgetCoronal(bool zoom)
{
    if (zoom) {
        scene_widget_Transverse->setVisible(false);
        scene_widget_Sagittal->setVisible(false);
        vtk_widget->setVisible(false);
    }
    else {
        scene_widget_Transverse->setVisible(true);
        scene_widget_Sagittal->setVisible(true);
        vtk_widget->setVisible(true);
    }
}

void ImageSceneWidget3D::zoomWidgetGL(bool zoom)
{
    if (zoom) {
        scene_widget_Transverse->setVisible(false);
        scene_widget_Coronal->setVisible(false);
        scene_widget_Sagittal->setVisible(false);
    }
    else {
        scene_widget_Transverse->setVisible(true);
        scene_widget_Sagittal->setVisible(true);
        scene_widget_Coronal->setVisible(true);
    }
}

void ImageSceneWidget3D::mousePressEvent(QMouseEvent* event)
{
    QWidget::mousePressEvent(event);
}
