#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QMessageBox>

#include "Model/GenericInteractionModel.h"
#include "Model/ViewTransFormModel.h"
#include "Model/GraphicsCalculateModel.h"
#include "model/GraphicsViewPaintModel.h"

class MagnifyingGlassWidget;
class StatusWidget;
class ForePlayWidget;
class GraphicsTextItem;
class GraphicsScene;
class GiantImageItem;
class ImageSceneWidget2D;
class QVBoxLayout;
class QPushButton;
class QButtonGroup;
class ViewToolBar;
class QHBoxLayout;
class GiantInteractionModeWidget;
class InteractionModeStackWidget;
class SamSegmentRealTimeThread;

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(QWidget *parent = nullptr);
    ~GraphicsView();
    void initSamSegmentRealTimeThread(bool); //初始化sam线程
    GraphicsScene* getGraphicsScene() const;
    ViewToolBar* getViewToolBar() const;
    GiantInteractionModeWidget* getGiantInteractionModeWidget() const;
    GenericInteractionModel* getGenericInteractionModel();
    ViewTransFormModel* getViewTransFormModel();
    GraphicsCalculateModel* getGraphicsCalculateModel();
    InteractionModeStackWidget* getInteractionModeStackWidget()const ;
    const QPoint& getMouseCoordinate();
    inline QPoint& getMousePresentPos();

    void setPaintCrossStyle(bool ok);
    void setPaintRectStyle(bool);

    void setMagImage(const QPointF& p); //设置放大镜控件的图像
    void setActived(bool ok);
    void setEnterView(bool);

    QPixmap& getMagPixmap() {return grab_map;} //返回grabMap

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;

    virtual void enterEvent(QEnterEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;

signals:
    void sliceChangeOneByOne(bool); //3D数据切片显示切换，true为向上翻，false为向下翻

    void mouseEnter(ImageSceneWidget2D*);
    void mouseLeave(ImageSceneWidget2D*);
    void mousePressed(ImageSceneWidget2D*);
    void mouseEnterPixmapItem(bool);
    void startSamSegmentRealTime();

public slots:
    void hideAllText(); //全部文本隐藏
    void showAllText(); //全部文本显示
    void showAllItemMessage(); //显示全部item的extra_text
    void hideAllItemMessage(); //隐藏全部item的extra_text
    void deCalLine(); //删除全部测量线段
    void saveViewImage();// 保存
    void setSprayRectSize(int);

private:
    void initGraphicsScene(); //初始化scene
    void initLayout(); //初始化布局
    void paintContinue();
    void promptContinue();

    void grabItemForCalculate(const QPoint&); //抓取坐标处图元传递给测量模型
    void keyDelete(); //删除函数

    void startPaintMode(QMouseEvent* event); //绘画模式启动！
    void startSamMode(QMouseEvent* event); //sam模式启动！
    void moveAtSamMode(QMouseEvent* event); //sam模式移动！
    void moveAtSprayMode(QMouseEvent* event); //喷涂模式移动！

    void showMenuAfterMouseRelease(QMouseEvent* event); //鼠标释放后显示菜单

    enum class MOUSE_PRESS_STATUS {
        LEFT_BUTTON_PRESSED,
        RIGHT_BUTTON_PRESSED,
        MIDDLE_BUTTON_PRESSED,
        NO_BUTTON_PRESSED,
    };
    enum class MOUSE_INTERACTION {
        HOVERING,
        PRESSED_NO_MOVE,
        PRESSED_AND_MOVE,
    };

    /*************Layout Setting*******************/
    QVBoxLayout* main_layout{ nullptr };
    QHBoxLayout* horizontal_layout{ nullptr };
    GiantInteractionModeWidget* interaction_mode_widget{ nullptr };
    ViewToolBar* view_tool_bar{ nullptr }; //view工具控件
    InteractionModeStackWidget* mode_stack_widget{ nullptr }; //模式切换控件

    /********************************/
    class GraphicsScene* m_scene{ nullptr }; //实例对象pView的scene
    GiantImageItem* pixmap_item{ nullptr }; //图像指针
    GenericInteractionModel generic_interaction_model{};
    ViewTransFormModel m_transform_model{};
    GraphicsCalculateModel m_graphics_calculate_model{};
    GraphicsViewPaintModel m_graphics_view_paint_model{};

    SamSegmentRealTimeThread* sam_segment_real_time_thread{ nullptr }; //sam线程

    QPixmap grab_map{};//截取的图像

    QPoint m_press_pos{}; //鼠标点击时pos
    QPoint m_present_pos{}; //鼠标当前位置
    QPoint m_present_pos_on_origin_image{}; //鼠标当前位置映射到图像上的坐标

    MOUSE_PRESS_STATUS mouse_press_status{ MOUSE_PRESS_STATUS::NO_BUTTON_PRESSED };//鼠标按钮点击状态
    MOUSE_INTERACTION mouse_interaction{ MOUSE_INTERACTION::HOVERING };//鼠标操作状态
    bool is_zoom_right_mouse{ true }; //是否鼠标右键缩放
    bool is_context_menu{ false }; //是否显示菜单栏

    bool is_enter_view{ false }; //鼠标是否在view移动
    bool is_actived{ false }; //是否激活(3D视图下具有焦点)
    bool is_mouse_enter_pixmap_item{ false }; //鼠标是否进入图像项
};
inline QPoint& GraphicsView::getMousePresentPos() {
    return m_present_pos;
}
#endif // GRAPHICSVIEW_H