#include "MultiFunctionStackWidget.h"
#include "StackIconWidget.h"
#include "StackWidget.h"
#include <QHBoxLayout>

MultiFunctionStackWidget::MultiFunctionStackWidget(QWidget* parent)
{
    main_layout = new QHBoxLayout(this);

    stack_icon_widget = new StackIconWidget();
    stack_widget = new StackWidget();
    stack_widget->setObjectName("stack_widget");
    stack_widget->setContentsMargins(0, 0, 0, 0);
    main_layout->addWidget(stack_icon_widget);
    main_layout->addWidget(stack_widget);
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->setSpacing(0);
    stack_icon_widget->connectStackWidget(stack_widget);
}

MultiFunctionStackWidget::~MultiFunctionStackWidget()
{
	delete stack_widget;
	delete stack_icon_widget;
}

StackWidget* MultiFunctionStackWidget::getStackWidget() const
{
	return stack_widget;
}

StackIconWidget* MultiFunctionStackWidget::getStackIconWidget() const
{
	return stack_icon_widget;
}
