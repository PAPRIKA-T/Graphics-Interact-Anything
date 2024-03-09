#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QMessageBox>

#include "Model/GenericInteractionModel.h"
#include "Model/ViewTransFormModel.h"
#include "Model/GraphicsCalculateModel.h"

#include "UndoCommandModel.h"


class MagnifyingGlassWidget;
class StatusWidget;
class ForePlayWidget;
class GraphicsTextItem;
class GraphicsScene;
class GraphicsPixmapItem;
class ImageSceneWidget2D;

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(QWidget* parent = nullptr);
    ~GraphicsView();
    GraphicsScene* getGraphicsScene();
    void initGraphicsScene();
    QPointF getPresentPosOnOriginImage();

    GenericInteractionModel* getGenericInteractionModel();
    ViewTransFormModel* getViewTransFormModel();
    GraphicsCalculateModel* getGraphicsCalculateModel();
    void updateLtText(); //更新左上文本

    void setMagImage(const QPointF& p); //设置放大镜控件的图像

    void setPaintCross(bool ok);
    void setActived(bool ok);

    QPixmap& getMagPixmap() { return grab_map; } //返回grabMap

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void contextMenuEvent(QContextMenuEvent* event) override;
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;

    virtual void enterEvent(QEnterEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;

signals:
    void sliceChangeOneByOne(bool); //3D数据切片显示切换，true为向上翻，false为向下翻

    void mouseEnter(ImageSceneWidget2D*);
    void mouseLeave(ImageSceneWidget2D*);
    void mousePressed(ImageSceneWidget2D*);

public slots:
    void hideAllText(); //全部文本隐藏
    void showAllText(); //全部文本显示
    void showAllItemMessage(); //显示全部item的extra_text
    void hideAllItemMessage(); //隐藏全部item的extra_text
    void deCalLine(); //删除全部测量线段
    void saveViewImage();// 保存



    //撤回与重做堆栈设置
    static void setUndoStack(UndoStack* undoStack)
    {
        m2_undoStack = undoStack;
    }



private:
    void grabItemForCalculate(const QPoint&); //抓取坐标处图元传递给测量模型
    void keyDelete(); //删除函数
    void startPaintMode(QMouseEvent* event); //绘画模式启动！

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
    class GraphicsScene* m_scene = nullptr; //实例对象pView的scene
    GraphicsPixmapItem* pixmap_item = nullptr; //图像指针
    GenericInteractionModel generic_interaction_model;
    ViewTransFormModel m_transform_model;
    GraphicsCalculateModel m_graphics_calculate_model;

    QPixmap grab_map;//截取的图像

    QPoint m_press_pos; //鼠标点击时pos
    QPoint m_present_pos; //鼠标当前位置
    QPoint m_present_pos_on_origin_image; //鼠标当前位置映射到图像上的坐标

    rubberCommand* command = nullptr;

    MOUSE_PRESS_STATUS mouse_press_status = MOUSE_PRESS_STATUS::NO_BUTTON_PRESSED;//鼠标按钮点击状态
    MOUSE_INTERACTION mouse_interaction = MOUSE_INTERACTION::HOVERING;//鼠标操作状态
    bool is_zoom_right_mouse = true; //是否鼠标右键缩放
    bool is_context_menu = false; //是否显示菜单栏

    bool paint_cross = false; //绘制十字线
    bool is_enter_view = false; //鼠标是否在view移动
    bool is_actived = false; //是否激活(3D视图下具有焦点)

    static UndoStack* m2_undoStack;
};

#endif // GRAPHICSVIEW_H
