#include "ViewToolBar.h"
#include <QHBoxLayout>
#include "SceneToolWidget.h"
#include "GiantInteractionModeWidget.h"

ViewToolBar::ViewToolBar(QWidget* parent)
	: QWidget(parent)
{
	main_layout = new QHBoxLayout(this);
	main_layout->setContentsMargins(0, 0, 0, 0);
	main_layout->setSpacing(0);
	setFixedHeight(32);
	setObjectName("view_tool_bar");

	scene_tool_widget = new SceneToolWidget(this);
	interaction_mode_widget = new GiantInteractionModeWidget(this);

	main_layout->addWidget(interaction_mode_widget);
	main_layout->addStretch();
	main_layout->addWidget(scene_tool_widget);
}

ViewToolBar::~ViewToolBar()
{
	delete main_layout;
	delete interaction_mode_widget;
	delete scene_tool_widget;
}

void ViewToolBar::setViewListContainer(ViewListContainer* vlc)
{
	view_list_container = vlc;
	interaction_mode_widget->setViewListContainer(vlc);
	scene_tool_widget->setViewListContainer(vlc);
}

SceneToolWidget* ViewToolBar::getSceneToolWidget()
{
	return scene_tool_widget;
}

GiantInteractionModeWidget* ViewToolBar::getInteractionModeWidget()
{
	return interaction_mode_widget;
}
