#pragma once

#include <QWidget>
class QButtonGroup;
class ViewListContainer;
class QVBoxLayout;
class GenericToolButton;
class GiantInteractionModeWidget  : public QWidget
{
	Q_OBJECT

public:
	GiantInteractionModeWidget(QWidget *parent = nullptr);
	~GiantInteractionModeWidget();
    void setViewListContainer(ViewListContainer*);
    void returnToDefaultMode();

private slots:
    void onRubberBtn(int checked); //橡皮擦模式
    void onObserveBtn(int checked); //观察模式
    void onSelectBtn(int checked); //选择模式
    void onDrawBtn(int checked); //绘画模式
    void onEditPolygonBtn(int checked); //编辑多边形模式
    void onCalculateBtn(int checked); //测量模式
private:
    ViewListContainer* view_list_container = nullptr;
    QButtonGroup* exclusive_button_group = nullptr;
    QVBoxLayout* main_layout = nullptr;

    GenericToolButton* rubber_btn = nullptr;
    GenericToolButton* observe_btn = nullptr;
    GenericToolButton* select_btn = nullptr;
    GenericToolButton* draw_btn = nullptr;
    GenericToolButton* edit_polygon_btn = nullptr;
    GenericToolButton* calculate_btn = nullptr;
};
