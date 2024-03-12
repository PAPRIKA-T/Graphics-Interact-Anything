#ifndef GRAPHICSITEM_H
#define GRAPHICSITEM_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QMenu>
#include <opencv2/opencv.hpp>
#include "utils/CoordinateCalculation.h"
#include "Model/GraphicsPaintModel.h"
#include "Model/GraphicsTextModel.h"
#include "Model/GraphicsRelationModel.h"
#include "Model/GraphicsTransformModel.h"

enum itemType
{
    BPointType = 1,
    BLineType,
    DoubleEndPointLineType,
    SingleEndPointLineType,
    BRectType,
    InteractionRectType,
    BEllipseType,
    InteractionEllipseType,
    BRoundType,
    InteractionRoundType,
    PolygonType,
    LineSegmentType,
    TextPointType,
    PolygonLineType,
    CalculatePointType,
    CalculateLineType,
    CalVerticalLineType,
    CalLineAngleLineType,
    PieType,
    InteractionPieType,
    AngleType,
    RubberRectType,
    ParallelLineType,
};
class GraphicsItem : public QObject, public QAbstractGraphicsShapeItem
{
    Q_OBJECT
public:
/********************构造、析构、初始化********************/
    enum class MouseSelectedStatus
    {
        SELECTED_ROTATE,
        SELECTED_NODE,
    };
    static int count; //use for count the total items added to the scene
    GraphicsItem();
    GraphicsItem(QPointF start, QPointF edge);
    ~GraphicsItem();
    void initGraphicItem();
    
/********************模型使用相关********************/
    GraphicsPaintModel& getGraphicsPaintModel();
    GraphicsTextModel& getGraphicsTextModel();
    GraphicsRelationModel& getGraphicsRelationModel();
    GraphicsTransformModel& getGraphicsTransformModel();

/********************基本属性相关********************/
    void setName(int n); //m_name is an ID of each item
    int getName();
    void setIsAcceptOthersSetting(bool ok);
    bool getIsAcceptOthersSetting();

    void setDeleteDirectly(bool ok);

/********************坐标信息相关********************/
    ItemPointF getCenter() const;
    ItemPointF& getRCenter();
    virtual void setCenter(const QPointF& p);
    ItemPointF getStart() const;
    ItemPointF& getRStart();
    virtual void setStart(const QPointF& p);
    ItemPointF getEdge() const;
    ItemPointF& getREdge();
    virtual void setEdge(const QPointF& p);
    virtual void setSE(const QPointF& s, const QPointF& e);
    const QPointF& getStartMeasurePos();
    const QPointF& getEdgeMeasurePos();
    const QPointF& getCenterMeasurePos();

/********************颜色信息相关********************/
    void setGraphicsColor(const QColor&);
    void initColorSetting();

/********************映射变换相关********************/
    void setRotateAngle(qreal r); //旋转参数
    void setViewScale(qreal s);

/********************父子关系相关********************/
    QList<std::pair<GraphicsItem*, void (GraphicsItem::*)(const QPointF&)>>& getSetPointFunctionList();
    virtual void generateOtherItems(const QPointF& pos); //释放该item相关其他item
    template<typename T> void removeChildItem(T** ptr) {
        //all child item call when delete（!!!important）
        if (!(*ptr)) return;
        QGraphicsItem *item = (*ptr)->parentItem();
        GraphicsItem* m_item = dynamic_cast<GraphicsItem*>(item);
        m_item->graphics_relation_model.removeOneOfChildList((*ptr));
        (*ptr)->setParentItem(nullptr);
        (*ptr)->onActionRemoveSelf();
        *ptr = nullptr;
    }
    void findAllGraphicsChildItems(QList<GraphicsItem*>& child_item_list);

/********************派生类根据需要需要重写********************/
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

/********************信号与槽********************/
signals:
    void prepareToRemove(); //准备清除
    void sentUnselectedColor(GraphicsItem* item, const QColor& c); //信号发射非选中颜色
    void sentItemPointF(ItemPointF &p, GraphicsItem* item = nullptr); //发送item坐标系的一个点
    void updatePointMessage(); //从新计算各个点相关距离、映射等信息

public slots:
    void onActionRotate(); //旋转
    virtual void onActionRemoveSelf(); //item的delete按钮绑定槽
    virtual void onPointSelected();
    virtual void receptItemPointF(ItemPointF &p, GraphicsItem* item = nullptr); //接收item坐标系的一个点
    virtual void onUpdatePointMessage(); //从新计算各个点相关距离、映射等信息

protected:
    /********************功能型函数********************/
    void rotate(const QPointF& mousePos); //旋转
    QPointF getCenterFromTwoPoint(QPointF& p, QPointF& q);//根据两个参数点获取中心点
    QPointF getStartFromTwoPoint(QPointF& center, QPointF& edge); //根据两个参数点获取矩形左上定点

    /********************派生类需要重写********************/
    virtual QPainterPath getStrokePath(); //获取描边path
    virtual QPainterPath getFillPath();//获取填充path
    virtual void paint(QPainter* painter,
        const QStyleOptionGraphicsItem* option,
        QWidget* widget) override;
    virtual QRectF boundingRect() const override; //粗略碰撞体积

    /********************派生类根据需要需要重写********************/
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

/********************基本属性********************/
    int m_name = 0; //用于索引item（item的名字编号)
    QList<std::pair<GraphicsItem*, void (GraphicsItem::*)(const QPointF&)>>set_point_fp_list; 
    //scene点击时会根据链表的函数指针顺序依次对点赋值
    QMenu menu; //右键菜单

    bool is_accept_others_setting = true; //是否接收labelBoard统一设置属性
    bool is_rotate_action = true; //是否右键菜单具有旋转功能
    bool is_change_cener_aciton = true; //是否右键菜单具有显示、隐藏中心点功能
    bool is_cal_start_by_ce = false; //是否根据中心点、边缘点计算起点
    bool is_cal_center_by_se = true; //是否根据起点、边缘点计算中心点
    bool is_show_others_no_selected = false; //在未选中时是否绘制起点、边缘点、描边等
    bool is_generate_context_menu = true; //是否生成菜单栏（item自身设置）

/********************一些标志********************/
    bool delete_direct = false; //释放内存时直接delete
    bool is_hover_enter = false; //鼠标悬浮是否进入
    bool move_parent_item = false; //移动父图元
    bool is_double_clicked = false; //是否双击
    bool is_accept_hover_event = true; //是否接收hover事件
    
/********************坐标变量********************/
    ItemPointF m_center; //中心点
    ItemPointF m_start; //起始点
    ItemPointF m_edge; //边缘点
    QPointF m_center_map_to_measure_ob; //中心点映射到measure_object的新坐标
    QPointF m_start_map_to_measure_ob; //起点映射到measure_object的新坐标
    QPointF m_edge_map_to_measure_ob; //边缘点映射到measure_object的新坐标
    QPointF m_press_pos; //鼠标点击时pos

/********************管理模型********************/
    MouseSelectedStatus mouse_selected_status = MouseSelectedStatus::SELECTED_NODE; //鼠标选中状态
    GraphicsPaintModel graphics_paint_model{}; //绘制参数模型
    GraphicsTextModel graphics_text_model{this}; //文本参数模型
    GraphicsRelationModel graphics_relation_model{}; //父子关系管理模型
    GraphicsTransformModel graphics_transform_model{}; //映射参数模型
};

#endif // GRAPHICSITEM_H
