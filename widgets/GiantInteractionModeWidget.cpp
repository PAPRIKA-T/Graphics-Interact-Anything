#include "GiantInteractionModeWidget.h"
#include "graphs/GraphicsScene.h"
#include "GraphicsItemMenu.h"
#include "GenericToolButton.h"
#include "CalculateModeMenu.h"
#include "GraphicsItemWidget.h"
#include "AiModelInteractWidget.h"
#include "InteractionModeStackWidget.h"
#include <QButtonGroup>
#include <QVBoxLayout>

GiantInteractionModeWidget::GiantInteractionModeWidget(QWidget *parent)
	: QWidget(parent)
{
    int btn_width = 38;int btn_height = 32;
    /*************************Mode Btn************************/
    main_layout = new QVBoxLayout(this);

    sam_model_btn = new GenericToolButton(this);
    sam_model_btn->setIcon(QIcon(":/res/qss/GenericStyle/background-image/sam_mode.png"));
    sam_model_btn->setIconSize(QSize(28, 22));

    rubber_btn = new GenericToolButton(this);
    rubber_btn->setIcon(QIcon(":/res/qss/GenericStyle/background-image/rubber_mode.png"));
    rubber_btn->setIconSize(QSize(30, 25));

    observe_btn = new GenericToolButton(this);
    observe_btn->setIcon(QIcon(":/res/qss/GenericStyle/background-image/observe_mode.png"));
    observe_btn->setIconSize(QSize(26, 22));

    select_btn = new GenericToolButton(this);
    select_btn->setIcon(QIcon(":/res/qss/GenericStyle/background-image/select_mode.png"));
    select_btn->setIconSize(QSize(30, 25));

    edit_polygon_btn = new GenericToolButton(this);
    edit_polygon_btn->setIcon(QIcon(":/res/qss/GenericStyle/background-image/edit_polygon_mode.png"));
    edit_polygon_btn->setIconSize(QSize(22, 22));

    draw_btn = new GenericToolButton(this);
    draw_btn->setIcon(QIcon(":/res/qss/GenericStyle/background-image/draw_mode.png"));
    draw_btn->setIconSize(QSize(34, 31));

    calculate_btn = new GenericToolButton(this);
    calculate_btn->setIcon(QIcon(":/res/qss/GenericStyle/background-image/calculate_mode.png"));
    calculate_btn->setIconSize(QSize(20, 26));

    rubber_btn->setCustomTooltip("Rubber Mode");
    observe_btn->setCustomTooltip("Observe Mode");
    select_btn->setCustomTooltip("Select Mode");

    sam_model_btn->setCustomTooltip("Sam Mode");
    draw_btn->setCustomTooltip("Draw Mode");
    calculate_btn->setCustomTooltip("Calculate Mode");
    edit_polygon_btn->setCustomTooltip("Edit Polygon Mode");

    exclusive_button_group = new QButtonGroup(this);
    exclusive_button_group->addButton(rubber_btn);
    exclusive_button_group->addButton(observe_btn);
    exclusive_button_group->addButton(select_btn);
    exclusive_button_group->addButton(edit_polygon_btn);
    exclusive_button_group->addButton(draw_btn);
    exclusive_button_group->addButton(sam_model_btn);
    exclusive_button_group->addButton(calculate_btn);
    exclusive_button_group->setExclusive(true);

    foreach (QAbstractButton* btn, exclusive_button_group->buttons())
    {
        btn->setFixedSize(btn_width, btn_height);
        btn->setCheckable(true);
        btn->setObjectName("interaction_mode_btn");
    }
    observe_btn->setChecked(true);

    setFixedHeight((exclusive_button_group->buttons().size()+1)* (btn_height+2) -2 + 8 + 5 +5 +20);

    connect(rubber_btn, &QPushButton::toggled, this, &GiantInteractionModeWidget::onRubberBtn);
    connect(observe_btn, &QPushButton::toggled, this, &GiantInteractionModeWidget::onObserveBtn);
    connect(select_btn, &QPushButton::toggled, this, &GiantInteractionModeWidget::onSelectBtn);
    connect(edit_polygon_btn, &QPushButton::toggled, this, &GiantInteractionModeWidget::onEditPolygonBtn);
    connect(draw_btn, &QPushButton::toggled, this, &GiantInteractionModeWidget::onDrawBtn);
    connect(calculate_btn, &QPushButton::toggled, this, &GiantInteractionModeWidget::onCalculateBtn);
    connect(sam_model_btn, &QPushButton::toggled, this, &GiantInteractionModeWidget::onSamModelBtn);

    main_layout->addWidget(draw_btn);
    main_layout->addWidget(rubber_btn);
    main_layout->addWidget(calculate_btn);
    main_layout->addSpacing(5);

    main_layout->addWidget(observe_btn);
    main_layout->addWidget(select_btn);

    main_layout->addSpacing(5);
    main_layout->addWidget(sam_model_btn);
    main_layout->addWidget(edit_polygon_btn);

    draw_btn->installEventFilter(this);
    calculate_btn->installEventFilter(this);

    /*************************Function Btn************************/
    fit_screen_btn = new GenericToolButton(this);
    fit_screen_btn->setIcon(QIcon(":/res/qss/GenericStyle/background-image/fit_screen_btn.png"));
    fit_screen_btn->setCustomTooltip("Reset Camera");
    fit_screen_btn->setFixedSize(btn_width, btn_height);
    fit_screen_btn->setObjectName("interaction_mode_btn");
    fit_screen_btn->setIconSize(QSize(24, 24));

    connect(fit_screen_btn, &QPushButton::clicked, this, [this]() {
        m_view->getViewTransFormModel()->fitScreen();
        m_view->getViewTransFormModel()->originPositonReturn(); });

    main_layout->addSpacing(20);
    main_layout->addWidget(fit_screen_btn);
    main_layout->addStretch();
    main_layout->setContentsMargins(4, 4, 4, 4);
    main_layout->setSpacing(2);
}

GiantInteractionModeWidget::~GiantInteractionModeWidget()
{
    delete main_layout;
    delete exclusive_button_group;
}

bool GiantInteractionModeWidget::eventFilter(QObject* object, QEvent* event)
{
    return QObject::eventFilter(object, event);
}

void GiantInteractionModeWidget::returnToDefaultMode()
{
    observe_btn->setChecked(true);
}

void GiantInteractionModeWidget::setInteractionModeStackWidget(InteractionModeStackWidget* w)
{
    mode_stack_widget = w;
}

void GiantInteractionModeWidget::onRubberBtn(int checked)
{
    m_view->getGenericInteractionModel()->setRubberInteraction(checked);
}

void GiantInteractionModeWidget::onObserveBtn(int checked)
{
    m_view->getGenericInteractionModel()->setObserveInteraction(checked);
}

void GiantInteractionModeWidget::onSelectBtn(int checked)
{
    m_view->getGenericInteractionModel()->setSelectInteraction(checked);
}

void GiantInteractionModeWidget::onDrawBtn(int checked)
{
    m_view->getGenericInteractionModel()->setPaintInteraction(checked);
    if (checked) {
        mode_stack_widget->getGraphicsItemWidget()->paintItemLoad();
        mode_stack_widget->setCurrentIndex(InteractionModeStackWidget::StackWidgetType::DrawModeStackWidget);
    }
    else {
        mode_stack_widget->setCurrentIndex(InteractionModeStackWidget::StackWidgetType::NoneStackWidget);
    }
}

void GiantInteractionModeWidget::onEditPolygonBtn(int checked)
{
    m_view->getGenericInteractionModel()->setEditPolygonInteraction(checked);
}

void GiantInteractionModeWidget::onCalculateBtn(int checked)
{
    m_view->getGenericInteractionModel()->setCalculateInteraction(checked);
    if (checked) {
        mode_stack_widget->setCurrentIndex(InteractionModeStackWidget::StackWidgetType::CalculateModeStackWidget);
    }
    else {
        mode_stack_widget->setCurrentIndex(InteractionModeStackWidget::StackWidgetType::NoneStackWidget);
    }
}

void GiantInteractionModeWidget::onSamModelBtn(int checked)
{
    m_view->getGenericInteractionModel()->setSamModelInteraction(checked);
    if (checked) {
        mode_stack_widget->getAiModelInteractWidget()->promptItemLoad();
        mode_stack_widget->setCurrentIndex(InteractionModeStackWidget::StackWidgetType::SamModeStackWidget);
    }
    else {
        mode_stack_widget->setCurrentIndex(InteractionModeStackWidget::StackWidgetType::NoneStackWidget);
    }
}
