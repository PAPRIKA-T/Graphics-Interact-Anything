#pragma once

#include <QWidget>
class GraphicsView;
class QHBoxLayout;
class SceneToolWidget;

class ViewToolBar  : public QWidget
{
	Q_OBJECT

public:
	ViewToolBar(QWidget* parent = nullptr);
	~ViewToolBar();
	SceneToolWidget* getSceneToolWidget();
	void setGraphicsView(GraphicsView*);
	virtual void paintEvent(QPaintEvent* event) override;
private:
	QHBoxLayout* main_layout = nullptr;
	SceneToolWidget* scene_tool_widget = nullptr;
};
