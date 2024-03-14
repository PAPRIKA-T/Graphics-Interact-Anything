#pragma once
#include <QWidget>
class QHBoxLayout;
class StackIconWidget;
class StackWidget;

class MultiFunctionStackWidget : public QWidget
{
	Q_OBJECT
public:
	explicit MultiFunctionStackWidget(QWidget *parent = nullptr);
	~MultiFunctionStackWidget() override;

	StackWidget* getStackWidget() const;
	StackIconWidget* getStackIconWidget() const;
private:
	QHBoxLayout* main_layout = nullptr; //主界面布局器
	StackWidget* stack_widget = nullptr; //右下堆栈控件
	StackIconWidget* stack_icon_widget = nullptr; //右下堆栈的按钮控件
};
