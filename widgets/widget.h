#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "model/ViewListContainer.h"

/*****************************************Widget***************************************/
#define PADDING 5
class QTimer;
class QVBoxLayout;
class QHBoxLayout;
class QSplitter;
class GraphicsItemWidget;
class FileView;
class TitleWidget;
class GraphicsScene;
class ImageSceneWidget2D;
class SamWidget;
class ImageSceneWidget3D;
class ForePlayWidget;
class ViewToolBar;
class MagnifyingGlassWidget;
class LabelBoardWidget;
class LabelBoardToolWidget;
class ItemIndexView;
class StatusWidget;
class HeaderButtonWidget;
class QButtonGroup;
class StackWidget;
class StackIconWidget;

class Widget : public QWidget
{
    Q_OBJECT
public:
    enum class Direction
    {
        UP = 0,
        DOWN = 1,
        LEFT,
        RIGHT,
        LEFTTOP,
        LEFTBOTTOM,
        RIGHTBOTTOM,
        RIGHTTOP,
        NONE
    }; //枚举变量用于判断是否更改窗体大小
    Widget(QWidget *parent = nullptr); //构造函数
    ~Widget() override; //析构函数
    QVBoxLayout* getMainLayout(); //获取主界面布局器
/**************************************************一些相关函数********************************************************/
    void setStyle(QString fileName); //设置样式表

/**************************************************场景、视图指针的获取与设置********************************************************/
    ViewListContainer* getViewListContainer(); //获取视图容器
    FileView* getFileView(); //获取文件索引控件
    SamWidget* getSamWidget(); //获取sam控件

    void imageWidgetAdd(ImageSceneWidget2D*image_widget); //切换图像显示控件
    void DimensionTrans();//2D\3D布局转换

protected:
/**************************************************Qt虚函数重写********************************************************/
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void closeEvent (QCloseEvent * event) override;
    virtual void changeEvent(QEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;

public slots:
    void paintContinue(); //继续绘画

private:
    void mousePressChangeImageWidget(ImageSceneWidget2D*);
    void connectToolButton(ImageSceneWidget2D*);
    void disConnectToolButton(ImageSceneWidget2D*);

    void setWidgetSize(); //设置各个控件部分的大小
    void region(const QPoint&); //用于判断鼠标位置是否在界面边缘，设置是否更改窗体大小
    void initTimerSetting(); //初始化定时器
    void updateGeometry(); //定时器更新窗口大小位置
/**************************************************类具有的一些成数据********************************************************/
    bool is_left_mouse_pressed = false; // 判断左键是否按下
    Direction dir = Direction::NONE; // 窗口大小改变时，记录改变方向
    QList<QPushButton*> draw_button_list; //绘图按钮链表
    QList<QPushButton*> tool_button_list; //上方工具按钮链表
    QButtonGroup* exclusive_graphics_btn_box = nullptr; //图形按钮互斥组
    QPoint global_point;
    int update_main_window_geomtry_flag = 0; //定时器更新窗口大小位置的时间标志

/**************************************************界面所有布局器********************************************************/
    QVBoxLayout *main_layout = nullptr; //主界面垂直布局器
    QVBoxLayout *center_scene_widget_verti_layout = nullptr; //中间scene部分垂直的布局器
    QHBoxLayout* center_scene_widget_hori_layout = nullptr; //中间scene部分水平的布局器
    QVBoxLayout* label_board_widget_layout = nullptr; //labelBoard部分的布局器
    QHBoxLayout* stack_widget_hori_layout = nullptr; //右下堆栈控件的布局器

    QSplitter *right_widget_splitter = nullptr; // 右侧的splitter
    QSplitter* center_widget_splitter = nullptr; // 整体的splitter

/**************************************************界面布局器底层控件********************************************************/
    QWidget* main_under_widget = nullptr; //主界面底层控件
    TitleWidget* title_widget = nullptr; //标题栏

    QWidget* center_scene_verti_widget = nullptr; //中间垂直部分的底层控件
    QWidget* center_scene_hori_widget = nullptr; //中间水平部分的底层控件

    QWidget* label_board_under_widget = nullptr; //labelboard部分的底层控件

    QWidget* stack_under_widget = nullptr; //stack部分的底层控件
    StackWidget* rb_stack_widget = nullptr; //右下堆栈控件
    StackIconWidget* stack_icon_widget = nullptr; //右下堆栈的按钮控件

/**************************************************界面所有功能控件(不含布局底层控件)********************************************************/
    GraphicsItemWidget* graphicsitem_widget = nullptr; //图形绘制按钮控件

    ImageSceneWidget2D* image_widget_2d = nullptr; //2d显示控件
    ImageSceneWidget3D* image_widget_3d = nullptr; //3d显示控件
    ViewToolBar* view_tool_bar = nullptr; //view工具控件
    MagnifyingGlassWidget *mag_glass_widget = nullptr; //放大镜控件
    FileView *file_view = nullptr; //窗口右下角文件列表索引交互控件
    StatusWidget* status_widget = nullptr; //底部状态栏
    LabelBoardWidget* label_board_widget = nullptr; //标签面板
    LabelBoardToolWidget* label_board_tool_wodget = nullptr; //标签面板上方按钮
    ItemIndexView* item_index_view = nullptr; //item索引控件
    SamWidget* sam_widget = nullptr;
    ForePlayWidget* foreplay_widget = nullptr;

/**************************************************场景、视图指针********************************************************/
    ViewListContainer view_list_container; //视图容器
    QTimer* update_geometry_timer = nullptr;
};

#endif // WIDGET_H
