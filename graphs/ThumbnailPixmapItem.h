#pragma once
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QDebug>

class GraphicsScene;
class ThumbnailPixmapItem  : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT
public:
	enum class RotateMode
	{
		OneFourRotate,
		TwoFourRotate,
		ThreeFourRotate,
		NoRotate,
	};
	explicit ThumbnailPixmapItem(const QPixmap& pixmap); //构造函数
	~ThumbnailPixmapItem();
	void setPixmap(const QPixmap&);//设置显示图像
	void updatePixmap(const QPixmap& p);//更新图像，只更新内容
	void updateSize(); //根据scene大小调整size
	void updateDecorator(); //更新
	qreal getFscaleH() { return fScaleH; } //获取图像高度
	qreal getFscaleW() { return fScaleW; } //获取图像宽度
	void setGraphicsScene(GraphicsScene* s) { m_scene = s; }
	void setBoxRotateMode(const RotateMode&);
	void resetScaleFactor();
/****************************************************事件函数*********************************************************/
	virtual QRectF boundingRect() const override;
protected:
	virtual void paint(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		QWidget* widget) override;
	virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
	void calculateBoxPrompt();//计算矩形框区域大小位置
	GraphicsScene* m_scene = nullptr;
	qreal m_fScale = 1; //宽高比
	QSize origin_size{}; //保存图像原始大小
	QSize scene_window{}; //保存scene大小
	qreal fScaleH = 1; //图像高度
	qreal fScaleW = 1;  //图像宽度
	qreal scale_factor = 0.3; //图像缩放
	QRect box_prompt{}; //矩形框区域
	QPointF m_press_pos; //鼠标点击时pos
	RotateMode box_rotate_mode = RotateMode::NoRotate; //矩形框区域旋转模式
};
