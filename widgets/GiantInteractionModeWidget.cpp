#include "GiantInteractionModeWidget.h"
#include "Graphs/Graphicsview.h"
#include "graphs/GraphicsScene.h"
#include "GraphicsItemMenu.h"
#include "GenericToolButton.h"
#include "CalculateModeMenu.h"
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QStyleOption>

GiantInteractionModeWidget::GiantInteractionModeWidget(QWidget *parent)
	: QWidget(parent)
{
    setMouseTracking(true);
    int btn_width = 40;int btn_height = 34;
    /*************************Mode Btn************************/
    main_layout = new QVBoxLayout(this);
    rubber_btn = new GenericToolButton(this);
    rubber_btn->setIcon(QIcon(":/res/qss/GenericStyle/background-image/rubber_mode.png"));
    rubber_btn->setCheckable(true);
    rubber_btn->setObjectName("interaction_mode_btn");
    rubber_btn->setIconSize(QSize(30, 25));

    observe_btn = new GenericToolButton(this);
    observe_btn->setIcon(QIcon(":/res/qss/GenericStyle/background-image/observe_mode.png"));
    observe_btn->setIconSize(QSize(34, 28));

    select_btn = new GenericToolButton(this);
    select_btn->setIcon(QIcon(":/res/qss/GenericStyle/background-image/select_mode.png"));
    select_btn->setIconSize(QSize(45, 40));

    edit_polygon_btn = new GenericToolButton(this);
    edit_polygon_btn->setIcon(QIcon(":/res/qss/GenericStyle/background-image/edit_polygon_mode.png"));
    edit_polygon_btn->setIconSize(QSize(26, 26));

    draw_btn = new GenericToolButton(this);
    draw_btn->setIcon(QIcon(":/res/qss/GenericStyle/background-image/draw_mode.png"));
    draw_btn->setIconSize(QSize(32, 27));

    calculate_btn = new GenericToolButton(this);
    calculate_btn->setIcon(QIcon(":/res/qss/GenericStyle/background-image/calculate_mode.png"));
    calculate_btn->setIconSize(QSize(24, 30));

    rubber_btn->setCustomTooltip("Rubber Mode");
    observe_btn->setCustomTooltip("Observe Mode");
    select_btn->setCustomTooltip("Select Mode");
    edit_polygon_btn->setCustomTooltip("Edit Polygon Mode");
    draw_btn->setCustomTooltip("Draw Mode");
    calculate_btn->setCustomTooltip("Calculate Mode");

    exclusive_button_group = new QButtonGroup(this);
    exclusive_button_group->addButton(rubber_btn);
    exclusive_button_group->addButton(observe_btn);
    exclusive_button_group->addButton(select_btn);
    exclusive_button_group->addButton(edit_polygon_btn);
    exclusive_button_group->addButton(draw_btn);
    exclusive_button_group->addButton(calculate_btn);
    exclusive_button_group->setExclusive(true);

    foreach (QAbstractButton* btn, exclusive_button_group->buttons())
    {
        btn->setFixedSize(btn_width, btn_height);
        btn->setCheckable(true);
        btn->setObjectName("interaction_mode_btn");
    }
    observe_btn->setChecked(true);

    connect(rubber_btn, &QPushButton::toggled, this, &GiantInteractionModeWidget::onRubberBtn);
    connect(observe_btn, &QPushButton::toggled, this, &GiantInteractionModeWidget::onObserveBtn);
    connect(select_btn, &QPushButton::toggled, this, &GiantInteractionModeWidget::onSelectBtn);
    connect(edit_polygon_btn, &QPushButton::toggled, this, &GiantInteractionModeWidget::onEditPolygonBtn);
    connect(draw_btn, &QPushButton::toggled, this, &GiantInteractionModeWidget::onDrawBtn);
    connect(calculate_btn, &QPushButton::toggled, this, &GiantInteractionModeWidget::onCalculateBtn);

    main_layout->addWidget(observe_btn);
    main_layout->addWidget(select_btn);
    main_layout->addSpacing(5);

    main_layout->addWidget(draw_btn);
    main_layout->addWidget(rubber_btn);
    main_layout->addWidget(calculate_btn);
    main_layout->addWidget(edit_polygon_btn);

    draw_btn->installEventFilter(this);
    calculate_btn->installEventFilter(this);

    /*************************Function Btn************************/
    clear_scene_btn = new GenericToolButton(this);
    clear_scene_btn->setIcon(QIcon(":/res/qss/GenericStyle/background-image/deleteall_btn.png"));
    clear_scene_btn->setCustomTooltip("Delete All Annotation");
    clear_scene_btn->setFixedSize(btn_width, btn_height);
    clear_scene_btn->setObjectName("interaction_mode_btn");
    clear_scene_btn->setIconSize(QSize(28, 28));

    clear_calculate_btn = new GenericToolButton(this);
    clear_calculate_btn->setIcon(QIcon(":/res/qss/GenericStyle/background-image/delete_cal_btn.png"));
    clear_calculate_btn->setCustomTooltip("Delete All Calculate Item");
    clear_calculate_btn->setFixedSize(btn_width, btn_height);
    clear_calculate_btn->setObjectName("interaction_mode_btn");
    clear_calculate_btn->setIconSize(QSize(28, 28));

    fit_screen_btn = new GenericToolButton(this);
    fit_screen_btn->setIcon(QIcon(":/res/qss/GenericStyle/background-image/fit_screen_btn.png"));
    fit_screen_btn->setCustomTooltip("Reset Camera");
    fit_screen_btn->setFixedSize(btn_width, btn_height);
    fit_screen_btn->setObjectName("interaction_mode_btn");
    fit_screen_btn->setIconSize(QSize(28, 28));

    connect(clear_scene_btn, &QPushButton::clicked, this, [this]() {
        m_view->getGraphicsScene()->clearSceneGraphicsItem(); });

    connect(clear_calculate_btn, &QPushButton::clicked, this, [this]() {
        m_view->deCalLine(); });

    connect(fit_screen_btn, &QPushButton::clicked, this, [this]() {
        m_view->getViewTransFormModel()->fitScreen();
        m_view->getViewTransFormModel()->originPositonReturn(); });

    main_layout->addSpacing(5);
    main_layout->addWidget(fit_screen_btn);
    main_layout->addWidget(clear_scene_btn);
    main_layout->addWidget(clear_calculate_btn);

    main_layout->addStretch();
    main_layout->setContentsMargins(5, 5, 0, 0);
    main_layout->setSpacing(1);
}

GiantInteractionModeWidget::~GiantInteractionModeWidget()
{
    delete main_layout;
    delete item_menu;
    delete calculate_menu;
}

bool GiantInteractionModeWidget::eventFilter(QObject* object, QEvent* event)
{
    if (object == draw_btn) {
        if (event->type() == QEvent::MouseButtonPress) {
            if (draw_btn->isChecked()) {
                item_menu->exec(draw_btn->mapToGlobal(QPoint(draw_btn->width()+5, 5)));
            }
        }
    }
    else if (object == item_menu) {
        if (event->type() == QEvent::Leave) {
            if (draw_btn->isChecked()) {
                item_menu->close();
            }
        }
    }

    else if (object == calculate_btn)
    {
        if (event->type() == QEvent::MouseButtonPress){
            if (calculate_btn->isChecked()){
				calculate_menu->exec(calculate_btn->mapToGlobal(QPoint(calculate_btn->width()+5, 5)));
			}
		}
	}
    else if (object == calculate_menu) {
        if (event->type() == QEvent::Leave) {
            if (calculate_btn->isChecked()) {
				calculate_menu->close();
			}
		}
    }
    return QObject::eventFilter(object, event);
}

void GiantInteractionModeWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QStyleOption styleOpt;
    styleOpt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &styleOpt, &painter, this);
}

void GiantInteractionModeWidget::enterEvent(QEnterEvent* event)
{
    Q_UNUSED(event);
    m_view->setEnterView(false);
}

void GiantInteractionModeWidget::leaveEvent(QEvent* event)
{
    Q_UNUSED(event);
    m_view->setEnterView(true);
}

void GiantInteractionModeWidget::returnToDefaultMode()
{
    observe_btn->setChecked(true);
}

void GiantInteractionModeWidget::setGraphicsView(GraphicsView* view)
{
    m_view = view;
    initModeMenu();
}

GraphicsItemMenu* GiantInteractionModeWidget::getGraphicsItemMenu() const
{
    return item_menu;
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
}

void GiantInteractionModeWidget::onEditPolygonBtn(int checked)
{
    m_view->getGenericInteractionModel()->setEditPolygonInteraction(checked);
}

void GiantInteractionModeWidget::onCalculateBtn(int checked)
{
    m_view->getGenericInteractionModel()->setCalculateInteraction(checked);
}

void GiantInteractionModeWidget::initModeMenu()
{
    item_menu = new GraphicsItemMenu();
    item_menu->installEventFilter(this);
    item_menu->setGraphicsView(m_view);

    calculate_menu = new CalculateModeMenu();
    calculate_menu->installEventFilter(this);
    calculate_menu->setGraphicsView(m_view);
}
