#include "StackIconWidget.h"
#include "GenericToolButton.h"
#include "StackWidget.h"
#include <QVBoxLayout>
#include <QButtonGroup>

StackIconWidget::StackIconWidget(QWidget *parent)
	: QWidget(parent)
{
	setFixedWidth(30);
    stack_btn_layout = new QVBoxLayout(this);

    file_view_btn = new GenericToolButton(this);
    file_view_btn->setObjectName("stack_btn");
    file_view_btn->setIcon(QIcon(":/res/qss/GenericStyle/background-image/folder_stack_icon.png"));

    file_setting_btn = new GenericToolButton(this);
    file_setting_btn->setObjectName("stack_btn");
    file_setting_btn->setIcon(QIcon(":/res/qss/GenericStyle/background-image/setting_btn.png"));

    item_list_btn = new GenericToolButton(this);
    item_list_btn->setObjectName("stack_btn");
    item_list_btn->setIcon(QIcon(":/res/qss/GenericStyle/background-image/item_list_btn.png"));

    sam_model_btn = new GenericToolButton(this);
    sam_model_btn->setObjectName("stack_btn");
    sam_model_btn->setIcon(QIcon(":/res/qss/GenericStyle/background-image/sam_model_btn.png"));

    color_adjust_btn = new GenericToolButton(this);
    color_adjust_btn->setObjectName("stack_btn");
    color_adjust_btn->setIcon(QIcon(":/res/qss/GenericStyle/background-image/color_adjust_btn.png"));

    stack_btn_layout->addWidget(file_view_btn);
    stack_btn_layout->addWidget(file_setting_btn);
    stack_btn_layout->addWidget(item_list_btn);
    stack_btn_layout->addWidget(sam_model_btn);
    stack_btn_layout->addWidget(color_adjust_btn);
    stack_btn_layout->addStretch();
    stack_btn_layout->setContentsMargins(2, 0, 0, 2);
    stack_btn_layout->setSpacing(6);

    file_view_btn->setCustomTooltip(tr("FileView"));
    file_setting_btn->setCustomTooltip(tr("FileSetting"));
    item_list_btn->setCustomTooltip(tr("ItemList"));
    sam_model_btn->setCustomTooltip(tr("SamModel"));
    color_adjust_btn->setCustomTooltip(tr("ColorAujust"));

    file_view_btn->setCheckable(true);
    file_setting_btn->setCheckable(true);
    item_list_btn->setCheckable(true);
    sam_model_btn->setCheckable(true);
    color_adjust_btn->setCheckable(true);

    exclusive_button_group = new QButtonGroup(this);
    exclusive_button_group->addButton(file_view_btn);
    exclusive_button_group->addButton(file_setting_btn);
    exclusive_button_group->addButton(item_list_btn);
    exclusive_button_group->addButton(sam_model_btn);
    exclusive_button_group->addButton(color_adjust_btn);
    exclusive_button_group->setExclusive(true);
    file_view_btn->setChecked(true);
}

StackIconWidget::~StackIconWidget()
{

}

void StackIconWidget::connectStackWidget(StackWidget* s)
{
    if (!s)return;
    connect(file_view_btn, &QPushButton::clicked, [=]() {
        s->setCurrentIndex(0);
        });
    connect(file_setting_btn, &QPushButton::clicked, [=]() {
        s->setCurrentIndex(1);
        });
    connect(item_list_btn, &QPushButton::clicked, [=]() {
        s->setCurrentIndex(2);
        });
    connect(sam_model_btn, &QPushButton::clicked, [=]() {
        s->setCurrentIndex(3);
        });
}
