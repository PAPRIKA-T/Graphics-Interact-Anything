#pragma once

#include <QPushButton>
#include <QPointer>
class ToolTipWindow;

class GiantPushButton  : public QPushButton
{
public:
	GiantPushButton(QWidget* parent = nullptr);
	GiantPushButton(const QString& text, QWidget* parent = nullptr); //构造函数重载
	~GiantPushButton();
	void setCustomTooltip(const QString& tooltip);//设置提示文字的函数
	virtual void enterEvent(QEnterEvent* event); //鼠标进入事件
	virtual void leaveEvent(QEvent* event) override; //鼠标离开事件
private:
	ToolTipWindow* tooltip_window = nullptr;
	QString tooltip_text;
	bool is_set_custom_tooltip = false;
};
