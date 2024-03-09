#pragma once

#include <QWidget>
class ViewListContainer;
class QHBoxLayout;
class GiantInteractionModeWidget;
class SceneToolWidget;

class ViewToolBar  : public QWidget
{
	Q_OBJECT

public:
	ViewToolBar(QWidget* parent = nullptr);
	~ViewToolBar();
	void setViewListContainer(ViewListContainer*);
	SceneToolWidget* getSceneToolWidget();
	GiantInteractionModeWidget* getInteractionModeWidget();
private:
	QHBoxLayout* main_layout = nullptr;
	ViewListContainer* view_list_container = nullptr;
	GiantInteractionModeWidget* interaction_mode_widget = nullptr;
	SceneToolWidget* scene_tool_widget = nullptr;
};
