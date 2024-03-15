#pragma once
#include "StackWidget.h"
#include "utils/GiantMacros.h"
class GraphicsView;
class GraphicsItemWidget;
class CalculateModeWidget;
class AiModelInteractWidget;

class InteractionModeStackWidget : public StackWidget
{
	Q_OBJECT
public:
	enum StackWidgetType {
		NoneStackWidget,
		DrawModeStackWidget,
		CalculateModeStackWidget,
		SamModeStackWidget,
	};
	InteractionModeStackWidget(QWidget *parent = nullptr);
	~InteractionModeStackWidget();
	void setGraphicsView(GraphicsView* view);
	GraphicsItemWidget* getGraphicsItemWidget() const;
	AiModelInteractWidget* getAiModelInteractWidget() const;
	void setCurrentIndex(StackWidgetType index);
	G_Widget_EnterLeaveView
private:
	QWidget* none_widget = nullptr;
	GraphicsItemWidget* draw_mode_widget = nullptr;
	CalculateModeWidget* calculate_mode_widget = nullptr;
	AiModelInteractWidget* ai_model_interact_widget = nullptr;
	GraphicsView* m_view = nullptr;
};
