#pragma once
#include <QWidget>

class QHBoxLayout;
class GenericToolButton;
class QButtonGroup;
class Widget;

class HeaderButtonWidget  : public QWidget
{
	Q_OBJECT
public:
	HeaderButtonWidget(QWidget *parent = nullptr);
	~HeaderButtonWidget();
	virtual void paintEvent(QPaintEvent* event) override;
	void setWindowWidget(Widget* widget);

private slots:

private:
	Widget* window_widget = nullptr;
	QList<GenericToolButton*> header_btn_list;
	QHBoxLayout* header_btn_layout = nullptr;
};
