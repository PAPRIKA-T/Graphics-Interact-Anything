#pragma once

#include <QWidget>
#include "utils/GiantMacros.h"
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

protected:
	G_Widget_PaintEventOverride
	virtual void enterEvent(QEnterEvent* event) override;
	virtual void leaveEvent(QEvent* event) override;
private:
	QHBoxLayout* main_layout = nullptr;
	GraphicsView* m_view = nullptr;
	SceneToolWidget* scene_tool_widget = nullptr;
};
