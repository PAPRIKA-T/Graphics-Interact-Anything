#include "ViewToolBar.h"
#include "Graphs/GraphicsView.h"
#include <QHBoxLayout>
#include "SceneToolWidget.h"
#include <QStyleOption>
#include <QPainter>

ViewToolBar::ViewToolBar(QWidget* parent)
	: QWidget(parent)
{
	main_layout = new QHBoxLayout(this);
	main_layout->addStretch();
	main_layout->setContentsMargins(0, 0, 0, 0);
	main_layout->setSpacing(0);
	setFixedHeight(32);
	setMinimumWidth(600);
	setObjectName("view_tool_bar");

	scene_tool_widget = new SceneToolWidget(this);
	main_layout->addWidget(scene_tool_widget);
}

ViewToolBar::~ViewToolBar()
{
	delete main_layout;
	delete scene_tool_widget;
}

SceneToolWidget* ViewToolBar::getSceneToolWidget()
{
	return scene_tool_widget;
}

void ViewToolBar::setGraphicsView(GraphicsView*v)
{
	m_view = v;
	scene_tool_widget->setGraphicsView(v);
}

void ViewToolBar::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);
	QStyleOption styleOpt;
	styleOpt.initFrom(this);
	QPainter painter(this);
	style()->drawPrimitive(QStyle::PE_Widget, &styleOpt, &painter, this);
}

void ViewToolBar::enterEvent(QEnterEvent* event)
{
	Q_UNUSED(event);
	m_view->setEnterView(false);
}

void ViewToolBar::leaveEvent(QEvent* event)
{
	Q_UNUSED(event);
	m_view->setEnterView(true);
}