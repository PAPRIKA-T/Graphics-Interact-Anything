#pragma once
#include <QStackedWidget>

class StackWidget  : public QStackedWidget
{
	Q_OBJECT

public:
	StackWidget(QWidget *parent = nullptr);
	~StackWidget();
};
