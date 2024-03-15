#pragma once
#include <QWidget>
class GenericToolButton;
class QVBoxLayout;
class QButtonGroup;
class StackWidget;

class StackIconWidget : public QWidget
{
	Q_OBJECT

public:
	StackIconWidget(QWidget *parent = nullptr);
	~StackIconWidget();
	void connectStackWidget(StackWidget*);

private:
	QVBoxLayout* stack_btn_layout = nullptr; //绘图按钮垂直布局器
	QButtonGroup* exclusive_button_group = nullptr;
	GenericToolButton* file_view_btn = nullptr;
	GenericToolButton* file_setting_btn = nullptr;
	GenericToolButton* item_list_btn = nullptr;
	GenericToolButton* color_adjust_btn = nullptr;
};
