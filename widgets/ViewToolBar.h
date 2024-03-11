#pragma once

#include <QWidget>
class ViewListContainer;
class QHBoxLayout;
class SceneToolWidget;

class ViewToolBar  : public QWidget
{
	Q_OBJECT

public:
	ViewToolBar(QWidget* parent = nullptr);
	~ViewToolBar();
	void setViewListContainer(ViewListContainer*);
	SceneToolWidget* getSceneToolWidget();
	virtual void paintEvent(QPaintEvent* event) override;
private:
	QHBoxLayout* main_layout = nullptr;
	ViewListContainer* view_list_container = nullptr;
	SceneToolWidget* scene_tool_widget = nullptr;
};
