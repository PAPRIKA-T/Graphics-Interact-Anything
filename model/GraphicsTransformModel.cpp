#include "GraphicsTransformModel.h"

GraphicsTransformModel::GraphicsTransformModel()
{

}

GraphicsTransformModel::~GraphicsTransformModel()
{

}

void GraphicsTransformModel::setImageScale(qreal s)
{
	image_scale = s;
}

qreal GraphicsTransformModel::getImageScale()
{
    return image_scale;
}

QGraphicsItem* GraphicsTransformModel::getMeasureObject()
{
    return measure_object;
}

void GraphicsTransformModel::setMeasureObject(QGraphicsItem* object)
{
    measure_object = object;
}

qreal GraphicsTransformModel::getRotateAngle()
{
    return rotate_angle;
}

void GraphicsTransformModel::setRotateAngle(const qreal& angle)
{
    rotate_angle = angle;
}

void GraphicsTransformModel::setScenePressPos(const QPointF& p)
{
    scene_press_pos = p;
}

QPointF GraphicsTransformModel::getScenePressPos()
{
    return scene_press_pos;
}