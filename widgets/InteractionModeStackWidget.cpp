#include "InteractionModeStackWidget.h"
#include "GraphicsItemWidget.h"
#include "CalculateModeWidget.h"

InteractionModeStackWidget::InteractionModeStackWidget(QWidget* parent)
{
    setFixedHeight(26);
    setObjectName("interaction_mode_stack_widget");

    none_widget = new QWidget();
    none_widget->setFixedWidth(0);

    draw_mode_widget = new GraphicsItemWidget();
    calculate_mode_widget = new CalculateModeWidget();

    addWidget(none_widget);
    addWidget(draw_mode_widget);
    addWidget(calculate_mode_widget);
    setCurrentIndex(InteractionModeStackWidget::StackWidgetType::NoneStackWidget);
}

InteractionModeStackWidget::~InteractionModeStackWidget()
{
    delete none_widget;
    delete draw_mode_widget;
    delete calculate_mode_widget;
}

void InteractionModeStackWidget::setGraphicsView(GraphicsView* view)
{
    m_view = view;
	draw_mode_widget->setGraphicsView(m_view);
    calculate_mode_widget->setGraphicsView(m_view);
}

GraphicsItemWidget* InteractionModeStackWidget::getGraphicsItemWidget() const
{
    return draw_mode_widget;
}

void InteractionModeStackWidget::setCurrentIndex(StackWidgetType index)
{
    StackWidget::setCurrentIndex((int)index);
	setFixedWidth(currentWidget()->width());
    setContentsMargins(8, 0, 8, 0);
}
