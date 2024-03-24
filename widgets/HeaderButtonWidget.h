#pragma once
#include <QWidget>
#include "utils/GiantMacros.h"
class QHBoxLayout;
class GenericToolButton;
class QButtonGroup;
class Widget;

class HeaderButtonWidget  : public QWidget
{
	Q_OBJECT
	G_Widget_PaintEventOverride
public:
	HeaderButtonWidget(QWidget *parent = nullptr);
	~HeaderButtonWidget();
	void setWindowWidget(Widget* widget);

private slots:

private:
	Widget* window_widget = nullptr;
	QList<GenericToolButton*> header_btn_list;
	QHBoxLayout* header_btn_layout = nullptr;
};
