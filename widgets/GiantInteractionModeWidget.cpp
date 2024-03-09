#include "GiantInteractionModeWidget.h"
#include "graphs/Graphicsview.h"
#include "model/ViewListContainer.h"
#include "GenericToolButton.h"
#include <QButtonGroup>
#include <QHBoxLayout>

GiantInteractionModeWidget::GiantInteractionModeWidget(QWidget *parent)
	: QWidget(parent)
{
    setFixedHeight(32);
    main_layout = new QHBoxLayout(this);
    rubber_btn = new GenericToolButton(this);
    rubber_btn->setIcon(QIcon(":/res/background-image/rubber.png"));
    rubber_btn->setCheckable(true);
    observe_btn = new GenericToolButton(this);
    observe_btn->setIcon(QIcon(":/res/background-image/observe.png"));
    observe_btn->setCheckable(true);
    select_btn = new GenericToolButton(this);
    select_btn->setIcon(QIcon(":/res/background-image/arrow_cursor.png"));
    select_btn->setCheckable(true);
    edit_polygon_btn = new GenericToolButton(this);
    edit_polygon_btn->setIcon(QIcon(":/res/background-image/edit_polygon.png"));
    edit_polygon_btn->setCheckable(true);
    draw_btn = new GenericToolButton(this);
    draw_btn->setIcon(QIcon(":/res/background-image/draw_pen.png"));
    draw_btn->setCheckable(true);
    calculate_btn = new GenericToolButton(this);
    calculate_btn->setIcon(QIcon(":/res/background-image/calculate_mode.png"));
    calculate_btn->setCheckable(true);

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

    connect(rubber_btn, &QPushButton::toggled, this, &GiantInteractionModeWidget::onRubberBtn);
    connect(observe_btn, &QPushButton::toggled, this, &GiantInteractionModeWidget::onObserveBtn);
    connect(select_btn, &QPushButton::toggled, this, &GiantInteractionModeWidget::onSelectBtn);
    connect(edit_polygon_btn, &QPushButton::toggled, this, &GiantInteractionModeWidget::onEditPolygonBtn);
    connect(draw_btn, &QPushButton::toggled, this, &GiantInteractionModeWidget::onDrawBtn);
    connect(calculate_btn, &QPushButton::toggled, this, &GiantInteractionModeWidget::onCalculateBtn);

    main_layout->addWidget(draw_btn);
    main_layout->addWidget(calculate_btn);

    QFrame* line_for = new QFrame(this);
    line_for->setFrameShape(QFrame::VLine);
    line_for->setFrameShadow(QFrame::Plain);
    line_for->setLineWidth(1);
    line_for->setContentsMargins(2, 5, 2, 5);
    QPalette palette = line_for->palette();
    palette.setColor(QPalette::WindowText, QColor(100, 100, 100));
    line_for->setPalette(palette);

    main_layout->addWidget(line_for);

    main_layout->addWidget(observe_btn);
    main_layout->addWidget(select_btn);
    main_layout->addWidget(edit_polygon_btn);
    main_layout->addWidget(rubber_btn);

    main_layout->addStretch();
    main_layout->setContentsMargins(2, 2, 2, 2);
    main_layout->setSpacing(6);
}

GiantInteractionModeWidget::~GiantInteractionModeWidget()
{
    delete main_layout;
}

void GiantInteractionModeWidget::setViewListContainer(ViewListContainer* vlc)
{
    view_list_container = vlc;
}

void GiantInteractionModeWidget::returnToDefaultMode()
{
    observe_btn->setChecked(true);
}

void GiantInteractionModeWidget::onRubberBtn(int checked)
{
    for (GraphicsView* v : view_list_container->getViewList())
        v->
        getGenericInteractionModel()->setRubberInteraction(checked);
}

void GiantInteractionModeWidget::onObserveBtn(int checked)
{
    for (GraphicsView* v : view_list_container->getViewList())
        v->
        getGenericInteractionModel()->setObserveInteraction(checked);
}

void GiantInteractionModeWidget::onSelectBtn(int checked)
{
    for (GraphicsView* v : view_list_container->getViewList())
        v->
        getGenericInteractionModel()->setSelectInteraction(checked);
}

void GiantInteractionModeWidget::onDrawBtn(int checked)
{
    for (GraphicsView* v : view_list_container->getViewList())
        v->
        getGenericInteractionModel()->setPaintInteraction(checked);
}

void GiantInteractionModeWidget::onEditPolygonBtn(int checked)
{
    for (GraphicsView* v : view_list_container->getViewList())
        v->
        getGenericInteractionModel()->setEditPolygonInteraction(checked);
}

void GiantInteractionModeWidget::onCalculateBtn(int checked)
{
    for (GraphicsView* v : view_list_container->getViewList())
        v->
        getGenericInteractionModel()->setCalculateInteraction(checked);
}