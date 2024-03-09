#pragma once

#include <QWidget>

class FileView;
class ViewListContainer;
class QHBoxLayout;
class GenericToolButton;
class GiantInteractionModeWidget;

/*************************SceneToolWidget************************/
class SceneToolWidget : public QWidget
{
    Q_OBJECT
public:
    SceneToolWidget(QWidget* parent = nullptr);
    ~SceneToolWidget();
    virtual void paintEvent(QPaintEvent* event) override;
    void setViewListContainer(ViewListContainer*);
    void setFileView(FileView*);
private slots:
    void onMaskToGraphicsItemBtn();
    void onCameraBtn(); //截图
    void onZoomSceneBtn(int checked); //放大某一个scene
    void onColorReverseBtn(); //反色
    void onFixScreenBtn(); //使图像适应屏幕
    void onCenterOnBtn(); //图像居中
    void onRotateRBtn(); //顺时针旋转
    void onRotateLBtn(); //逆时针旋转
    void onClearSceneBtn(); //清空scene图元
    void onTurnLeftBtn(); //向下切换图像
    void onTurnRightBtn(); //向上切换图像
private:
    QHBoxLayout* main_layout = nullptr;
    FileView* file_view = nullptr;
    ViewListContainer* view_list_container = nullptr;
    GenericToolButton* mask_to_graphicsitem = nullptr;
    GenericToolButton* camera_btn = nullptr;
    GenericToolButton* zoom_btn = nullptr;
    GenericToolButton* color_reverse_btn = nullptr;
    GenericToolButton* fix_screen_btn = nullptr;
    GenericToolButton* center_on_btn = nullptr;
    GenericToolButton* rotateR_btn = nullptr;
    GenericToolButton* rotateL_btn = nullptr;
    GenericToolButton* clear_scene_btn = nullptr;
    GenericToolButton* turn_left_btn = nullptr;
    GenericToolButton* turn_right_btn = nullptr;
};
