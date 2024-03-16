#include "AiModelInteractWidget.h"
#include "Graphs/GraphicsView.h"
#include "Graphs/GraphicsScene.h"
#include "GenericToolButton.h"
#include "ComboBoxWidget.h"
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QComboBox>

AiModelInteractWidget::AiModelInteractWidget(QWidget* parent)
	:QWidget(parent)
{
	int btn_width = 20; int btn_height = 20;

	main_layout = new QHBoxLayout(this);
	embedding_btn = new GenericToolButton(this);
	embedding_btn->setText("Embed");
	embedding_btn->setObjectName("graphics_item_btn");
	embedding_btn->setFixedSize(40, 20);

	positive_point = new GenericToolButton(this);
	positive_point->setIcon(QIcon(":/res/qss/GenericStyle/background-image/positive_point.png"));

	negative_point = new GenericToolButton(this);
	negative_point->setIcon(QIcon(":/res/qss/GenericStyle/background-image/negative_point.png"));

	box_prompt = new GenericToolButton(this);
	box_prompt->setIcon(QIcon(":/res/qss/GenericStyle/background-image/prompt_rect.png"));

	positive_point->setCustomTooltip("Positive Point");
	negative_point->setCustomTooltip("Negative Point");
	box_prompt->setCustomTooltip("Box Prompt");

	exclusive_button_group = new QButtonGroup(this);
	exclusive_button_group->addButton(positive_point);
	exclusive_button_group->addButton(negative_point);
	exclusive_button_group->addButton(box_prompt);

	main_layout->addWidget(embedding_btn);
	main_layout->addSpacing(8);

	foreach(QAbstractButton * btn, exclusive_button_group->buttons())
	{
		main_layout->addWidget(btn);
		btn->setFixedSize(btn_width, btn_height);
		btn->setCheckable(true);
		btn->setIconSize(QSize(15, 15));
		btn->setObjectName("graphics_item_btn");
	}
	positive_point->setChecked(true);

	output_shape_widget = new ComboBoxHWidget(this);
	output_shape_widget->setLabelText("Output:");
	output_shape_widget->getComboBox()->addItem("Mask");
	output_shape_widget->getComboBox()->addItem("Polygon");
	output_shape_widget->getComboBox()->addItem("Rect");

	output_shape_widget->setFixedHeight(26);
	output_shape_widget->getComboBox()->setMinimumWidth(75);
	output_shape_widget->getComboBox()->setFixedHeight(21);

	main_layout->addSpacing(12);
	main_layout->addWidget(output_shape_widget);
	main_layout->setContentsMargins(0, 0, 0, 0);
	main_layout->setSpacing(1);
	main_layout->addStretch();
	setFixedWidth(300);
}

AiModelInteractWidget::~AiModelInteractWidget()
{
	delete main_layout;
	delete embedding_btn;
	delete exclusive_button_group;
	delete output_shape_widget;
}

void AiModelInteractWidget::setGraphicsView(GraphicsView* view)
{
	m_view = view;
	connectSceneSignal(m_view->getGraphicsScene());
	m_view->getGraphicsScene()->getScenePromptItemModel()->setSamInteractWidget(this);
}

ComboBoxHWidget* AiModelInteractWidget::getOutputShapeWidget()
{
	return output_shape_widget;
}

QButtonGroup* AiModelInteractWidget::getExclusiveButtonGroup() const
{
	return exclusive_button_group;
}

void AiModelInteractWidget::promptItemLoad()
{
	foreach(QAbstractButton * btn, exclusive_button_group->buttons()) {
		if (btn->isChecked()) {
			if (!btn->isEnabled())return;
			emit btn->toggled(true);
			break;
		}
	}
}

void AiModelInteractWidget::connectSceneSignal(GraphicsScene* s)
{
	connect(positive_point, &QPushButton::toggled,
		s, &GraphicsScene::positivePointClicked);
	connect(negative_point, &QPushButton::toggled,
		s, &GraphicsScene::negativePointClicked);
	connect(box_prompt, &QPushButton::toggled,
		s, &GraphicsScene::promptRectClicked);
}
