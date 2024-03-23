#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "Model/ViewListContainer.h"

/*****************************************Widget***************************************/
#define PADDING 5
class QTimer;
class QVBoxLayout;
class QHBoxLayout;
class QSplitter;
class FileView;
class TitleWidget;
class ImageSceneWidget2D;
class ImageSceneWidget3D;
class ForePlayWidget;
class ViewToolBar;
class MagnifyingGlassWidget;
class LabelBoardWithTool;
class ItemIndexView;
class StatusWidget;
class MultiFunctionStackWidget;
class Sam;

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

/**************************************************场景、视图指针的获取与设置********************************************************/
    ViewListContainer* getViewListContainer(); //获取视图容器
    FileView* getFileView(); //获取文件索引控件
    Sam* getSam() const; //获取sam
    LabelBoardWithTool* getLabelBoardWithTool() const;

    void imageWidgetAdd(ImageSceneWidget2D*image_widget); //切换图像显示控件
    void DimensionTrans();//2D\3D布局转换

protected:
/**************************************************Qt虚函数重写********************************************************/
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void closeEvent (QCloseEvent * event) override;
    virtual void leaveEvent(QEvent* event) override;

private:
    void mousePressChangeImageWidget(ImageSceneWidget2D*);
    void connectToolButton(ImageSceneWidget2D*);
    void disConnectToolButton(ImageSceneWidget2D*);

    void setWidgetSize(); //设置各个控件部分的大小
    void initSamModel();//初始化sam模型

/**************************************************界面所有布局器********************************************************/
    QVBoxLayout *main_layout = nullptr; //主界面垂直布局器
    QSplitter *right_widget_splitter = nullptr; // 右侧的splitter
    QSplitter* center_widget_splitter = nullptr; // 整体的splitter

/*************************************界面所有功能控件**********************************************/
    TitleWidget* title_widget = nullptr; //标题栏
    MultiFunctionStackWidget* rb_stack_widget = nullptr; //右下堆栈控件
    ImageSceneWidget2D* image_widget_2d = nullptr; //2d显示控件
    ImageSceneWidget3D* image_widget_3d = nullptr; //3d显示控件
    //MagnifyingGlassWidget *mag_glass_widget = nullptr; //放大镜控件
    FileView *file_view = nullptr; //窗口右下角文件列表索引交互控件
    StatusWidget* status_widget = nullptr; //底部状态栏
    LabelBoardWithTool* label_board_widget = nullptr; //标签面板
    ItemIndexView* item_index_view = nullptr; //item索引控件
    ForePlayWidget* foreplay_widget = nullptr;

/**************************************************场景、视图指针********************************************************/
    ViewListContainer view_list_container; //视图容器

    /**********************内置模型***************************/
    Sam* sam = nullptr;
};

#endif // WIDGET_H
