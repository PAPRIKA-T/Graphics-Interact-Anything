#include "GiantInteractionModeWidget.h"
#include "Graphs/Graphicsview.h"
#include "GraphicsItemMenu.h"
#include "GenericToolButton.h"
#include "CalculateModeMenu.h"
#include <QButtonGroup>
#include <QVBoxLayout>

GiantInteractionModeWidget::GiantInteractionModeWidget(QWidget *parent)
	: QWidget(parent)
{
    setFixedWidth(47);
    int btn_width = 40;int btn_height = 35;
    main_layout = new QVBoxLayout(this);
    rubber_btn = new GenericToolButton(this);
    rubber_btn->setIcon(QIcon(":/res/background-image/rubber.png"));
    rubber_btn->setCheckable(true);
    rubber_btn->setObjectName("interaction_mode_btn");

    observe_btn = new GenericToolButton(this);
    observe_btn->setIcon(QIcon(":/res/background-image/observe.png"));
    observe_btn->setCheckable(true);
    observe_btn->setObjectName("interaction_mode_btn");

    select_btn = new GenericToolButton(this);
    select_btn->setIcon(QIcon(":/res/background-image/arrow_cursor.png"));
    select_btn->setCheckable(true);
    select_btn->setObjectName("interaction_mode_btn");

    edit_polygon_btn = new GenericToolButton(this);
    edit_polygon_btn->setIcon(QIcon(":/res/background-image/edit_polygon.png"));
    edit_polygon_btn->setCheckable(true);
    edit_polygon_btn->setObjectName("interaction_mode_btn");

    draw_btn = new GenericToolButton(this);
    draw_btn->setIcon(QIcon(":/res/background-image/draw_pen.png"));
    draw_btn->setCheckable(true);
    draw_btn->setObjectName("interaction_mode_btn");

    calculate_btn = new GenericToolButton(this);
    calculate_btn->setIcon(QIcon(":/res/background-image/calculate_mode.png"));
    calculate_btn->setCheckable(true);
    calculate_btn->setObjectName("interaction_mode_btn");

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
    observe_btn->setChecked(true);

    foreach (QAbstractButton* btn, exclusive_button_group->buttons())
    {
        btn->setFixedSize(btn_width, btn_height);
    }

    connect(rubber_btn, &QPushButton::toggled, this, &GiantInteractionModeWidget::onRubberBtn);
    connect(observe_btn, &QPushButton::toggled, this, &GiantInteractionModeWidget::onObserveBtn);
    connect(select_btn, &QPushButton::toggled, this, &GiantInteractionModeWidget::onSelectBtn);
    connect(edit_polygon_btn, &QPushButton::toggled, this, &GiantInteractionModeWidget::onEditPolygonBtn);
    connect(draw_btn, &QPushButton::toggled, this, &GiantInteractionModeWidget::onDrawBtn);
    connect(calculate_btn, &QPushButton::toggled, this, &GiantInteractionModeWidget::onCalculateBtn);

    main_layout->addWidget(draw_btn);
    main_layout->addWidget(calculate_btn);
    main_layout->addWidget(edit_polygon_btn);
    main_layout->addWidget(rubber_btn);

    main_layout->addSpacing(3);
    main_layout->addWidget(observe_btn);
    main_layout->addWidget(select_btn);

    main_layout->addStretch();
    main_layout->setContentsMargins(8, 5, 5, 5);
    main_layout->setSpacing(2);

    draw_btn->installEventFilter(this);
    calculate_btn->installEventFilter(this);
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
