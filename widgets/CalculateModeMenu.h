#pragma once
#include <QMenu>
#include "core/GiantMacros.h"
class GraphicsView;
class QActionGroup;
class QAction;

class CalculateModeMenu : public QMenu
{
	
public:
	CalculateModeMenu(QWidget* parent = nullptr);
	~CalculateModeMenu();
	void setGraphicsView(GraphicsView*);

	G_Menu_StayOpenAfterPress
private slots:
	void onCalCenterBtnClicked(int checked);
	void onCalVerticalBtnClicked(int checked);
	void onCalAngleBtnClicked(int checked);

private:
	GraphicsView* m_view = nullptr; //视图指针
	//测量功能模块
	QActionGroup* exclusive_button_group = nullptr;
	QAction* cal_center_btn = nullptr;
	QAction* cal_vertical_btn = nullptr;
	QAction* cal_angle_btn = nullptr;
};

