#pragma once

#include <QObject>
#include <QTransform>
class GraphicsView;
class GraphicsScene;
class GraphicsPixmapItem;

class ViewTransFormModel
{
public:
	ViewTransFormModel();
	~ViewTransFormModel();
	void setGraphicsView(GraphicsView* view);

	void updateRotateParameter(); //更新旋转参数
	void resetRotateParameter(); //重置旋转参数

	void setOriginPosition(const QPoint& p); //设置图像初始位置(居中)
	QPoint getOriginPosition(); //获取图像初始位置(居中)
	void originPositonReturn(); //回到初始位置（居中）
	void fitScreen(); //使图像适应屏幕

	void zoomIn(); //放大
	void zoomOut(); //缩小
	void zoom(const qreal& scaleFactor); //缩放
	void setZoomDelta(const qreal& delta); //设置缩放增量
	qreal getZoomDelta() const; //获取缩放增量
	void setViewScale(const qreal& s);//设置缩放值
	qreal getViewScale(); //获取缩放值
	void setImageScaleTotal(const qreal& s); //设置图像缩放值
	qreal getImageScaleTotal(); //获取图像总体缩放值

	void rotateR(); //顺时针90
	void rotateL(); //逆时针90
	void rotateX(); //水平翻转
	void rotateY(); //垂直翻转
	void resetTransform(); //变换还原

	void setActived(bool ok);
	bool isZoomIn();
	void setZoomIn(bool ok);

private:
	GraphicsView* m_view = nullptr;
	GraphicsScene* m_scene = nullptr;
	GraphicsPixmapItem* pixmap_item = nullptr;
	QPoint origin_position; //图像初始位置
	qreal m_zoom_delta; //缩放的增量
	qreal view_scale = 1.0; //view缩放值
	qreal image_scale_total = 1.0;//图像与原始尺寸对比缩放值
	qreal rotate_angle = 0; //当前view旋转的角度
	QTransform transform_rotate_total; //transform总旋转矩阵
	QTransform transform_total; //transform总操作

	bool is_actived = false; //视图焦点是否在这个scene上
	bool is_zoom_in = false; //判断是否放大显示这个scene所在控件(用于3D视图框架)
};
