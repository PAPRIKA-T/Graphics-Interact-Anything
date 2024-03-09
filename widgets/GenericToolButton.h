#pragma once

#include "GiantPushButton.h"

class GenericToolButton  : public GiantPushButton
{
	Q_OBJECT

public:
	GenericToolButton(QWidget* parent = nullptr);
	GenericToolButton(const QString& text, QWidget* parent = nullptr); //构造函数重载
	~GenericToolButton();
	virtual void paintEvent(QPaintEvent* event) override; 
};
