#include "AiModelInteractWidget.h"
#include "Graphs/GraphicsView.h"
#include "GenericToolButton.h"
#include <QHBoxLayout>
#include <QButtonGroup>

AiModelInteractWidget::AiModelInteractWidget(QWidget* parent)
	:QWidget(parent)
{
	main_layout = new QHBoxLayout(this);
	positive_point = new GenericToolButton(this);
	positive_point->setIcon(QIcon(":/res/qss/GenericStyle/background-image/positive_point.png"));
	positive_point->setIconSize(QSize(32, 27));

	negative_point = new GenericToolButton(this);
	negative_point->setIcon(QIcon(":/res/qss/GenericStyle/background-image/negative_point.png"));
	negative_point->setIconSize(QSize(32, 27));

	box_prompt = new GenericToolButton(this);
	box_prompt->setIcon(QIcon(":/res/qss/GenericStyle/background-image/box_prompt.png"));
	box_prompt->setIconSize(QSize(32, 27));

	exclusive_button_group = new QButtonGroup(this);
	exclusive_button_group->addButton(positive_point);
	exclusive_button_group->addButton(negative_point);
	exclusive_button_group->addButton(box_prompt);

	main_layout->addWidget(positive_point);
	main_layout->addWidget(negative_point);
	main_layout->addWidget(box_prompt);
	main_layout->setContentsMargins(0, 0, 0, 0);
	main_layout->addStretch();
}

AiModelInteractWidget::~AiModelInteractWidget()
{
	delete main_layout;
	delete exclusive_button_group;
}

void AiModelInteractWidget::setGraphicsView(GraphicsView* view)
{
	m_view = view;
}
