#pragma once
#include <QPen>
#include <QColor>
#include "graphs/ItemPointF.h"
#define PEN_WIDTHF 1 //画笔宽度
#define DEFAULT_COLOR_ITEM Qt::red //item color
#define DEFAULT_COLOR_POINT_NOSELECTED QColor(255, 249, 57)
#define DEFAULT_COLOR_POINT_SELECTED QColor(255, 20, 147)
#define DEFAULT_COLOR_UNIQUE_POINT QColor(237, 153, 74)
#define ITEM_LINE_LENGTH 6 //item交互十字（点）的大小（px）
#define INTERACTION_RANGE 6 //item交互十字接收事件的范围

class GraphicsPaintModel
{
public:
	GraphicsPaintModel();
    ~GraphicsPaintModel();
    void initColorSetting();

    QColor& getRItemColor(); //获取item颜色引用

    void setPenIsSelected(QPen pen); //设置选中pen
    QPen getPenIsSelected() const;//获取选中pen
    QPen& getRPenIsSelected();//获取选中pen引用
    QPen getPenNoSelected() const;//获取未选中pen
    QPen& getRPenNoSelected();//获取未选中pen引用
    void setPenIsNoSelected(QPen pen); //设置非选中pen

    void setCenterColor(const QColor& c); //设置中心点颜色
    QColor getCenterColor() const; //获取中心点颜色
    void setFillColorUnselected(const QColor& c); //设置填充颜色
    QColor getFillColorUnselected() const; //获取填充颜色
    QColor& getRFillColorUnselected(); //获取填充颜色引用
    void setFillColorHover(const QColor& c); //设置悬浮颜色
    QColor getFillColorHover() const; //获取悬浮颜色
    QColor& getRFillColorHover(); //获取悬浮颜色引用

    inline bool getIsCloseItem() const; //获取是否是封闭图形
    inline void setIsCloseItem(bool ok); //设置是否是封闭图形

    inline bool getIsPaintCenter() const;
    inline void setIsPaintCenter(bool ok);

    inline bool getIsFillItem() const; //是否处于填充状态
    inline void setIsFillItem(bool); //设置填充状态

    inline const qreal getAdjustSize() const;//获取边缘矩形调整大小
    inline void setAdjustSize(const qreal&); //设置边缘矩形调整大小

    inline bool getIsPaintStrokeShape() const; //获取是否绘制描边shape
    inline void setIsPaintStrokeShape(bool);

    inline qreal getScaleFactor() const;
    inline void setScaleFactor(const qreal&);

    inline qreal getInteracitonRange() const;
    inline void setInteracitonRange(const qreal&);

    inline qreal getLineLength() const;
    inline void setLineLength(const qreal&);

    inline qreal getPenWidth() const;
    inline void setPenWidth(const qreal&);

    inline qreal getHoverFillOpacity() const;
    inline void setHoverFillOpacity(const qreal&);

    void paintPoint(QPainter* painter, ItemPointF& p); //绘制交互点

private:
    qreal pen_width = PEN_WIDTHF; //item线条宽度
    qreal scale_factor = PEN_WIDTHF; //视图的缩放比例
    qreal interaction_range = INTERACTION_RANGE; //交互点的范围
    qreal line_lengh = ITEM_LINE_LENGTH; //标记线长度
    qreal adjust_size = INTERACTION_RANGE; //边缘矩形调整大小
    bool is_close_item = false; //是否是封闭图形（是否右键菜单具有填充功能）
    bool is_paint_stroke_shape = false; //是否绘制描边shape
    bool is_fill = false; //是否填充item
    bool is_paint_center = false; //是否绘制中心点
    QPen m_pen_is_selected{}; //item选中时pen
    QPen m_pen_no_selected{}; //item未选中时pen
    QColor m_color_item{}; //item的颜色
    QColor fill_color_unselected{}; //未选中时填充颜色
    QColor fill_color_hover{}; //悬浮填充颜色
    QColor fill_color_selected{}; //选中时填充颜色
    qreal hover_fill_opacity = 0.8; //item透明度
};

inline qreal GraphicsPaintModel::getPenWidth() const 
{ return pen_width; }
inline void GraphicsPaintModel::setPenWidth(const qreal& p)
{
    pen_width = p;
}

inline void GraphicsPaintModel::setScaleFactor(const qreal& s)
{
    scale_factor = s;
}
inline qreal GraphicsPaintModel::getScaleFactor() const
{
    return scale_factor;
}

inline qreal GraphicsPaintModel::getInteracitonRange() const
{
    return interaction_range;
}
inline void GraphicsPaintModel::setInteracitonRange(const qreal& r)
{
    interaction_range = r;
}

inline qreal GraphicsPaintModel::getLineLength() const
{
    return line_lengh;
}
inline void GraphicsPaintModel::setLineLength(const qreal& l)
{
    line_lengh = l;
}

inline bool GraphicsPaintModel::getIsCloseItem() const
{
    return is_close_item;
}
inline void GraphicsPaintModel::setIsCloseItem(bool ok)
{
    is_close_item = ok;
}

inline bool GraphicsPaintModel::getIsPaintCenter() const
{
    return is_paint_center;
}
inline void GraphicsPaintModel::setIsPaintCenter(bool ok)
{
    is_paint_center = ok;
}

inline bool GraphicsPaintModel::getIsFillItem() const
{
    return is_fill;
}
inline void GraphicsPaintModel::setIsFillItem(bool ok)
{
    is_fill = ok;
}

inline const qreal GraphicsPaintModel::getAdjustSize() const
{
    return adjust_size;
}
inline void GraphicsPaintModel::setAdjustSize(const qreal& size)
{
    adjust_size = size;
}

inline bool GraphicsPaintModel::getIsPaintStrokeShape() const
{
    return is_paint_stroke_shape;
}
inline void GraphicsPaintModel::setIsPaintStrokeShape(bool ok)
{
    is_paint_stroke_shape = ok;
}

inline qreal GraphicsPaintModel::getHoverFillOpacity() const
{
    return hover_fill_opacity;
}
inline void GraphicsPaintModel::setHoverFillOpacity(const qreal& o)
{
    hover_fill_opacity = o;
}
