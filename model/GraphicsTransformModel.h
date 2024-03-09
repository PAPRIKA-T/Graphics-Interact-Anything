#pragma once
#include <QGraphicsItem>

class GraphicsTransformModel
{
public:
	GraphicsTransformModel();
	~GraphicsTransformModel();

    void setImageScale(qreal s);//设置图像缩放比
    qreal getImageScale(); //获取图像缩放比
    QGraphicsItem* getMeasureObject(); //获取坐标映射对象
    void setMeasureObject(QGraphicsItem* object); //设置坐标映射对象
    qreal getRotateAngle(); //旋转角度
    void setRotateAngle(const qreal& angle); //设置旋转角度
    void setScenePressPos(const QPointF& p);//设置鼠标点击scene时记录的item位置
    QPointF getScenePressPos();//获取鼠标点击scene时记录的item位置

private:
    qreal image_scale = 1.0; //用于获取item添加进scene时图像的缩放比，使图像距离测量准确
    QGraphicsItem* measure_object = nullptr; //用于点坐标系映射 使测量准确（默认都是底层图像）
    qreal rotate_angle = 0; //旋转角度
    QPointF scene_press_pos; //鼠标点击scene时item的位置
};
