#include "GraphicsItemWidget.h"
#include "GenericToolButton.h"
#include "Model/ViewListContainer.h"
#include "graphs/GraphicsRect.h"
#include <QStyleOption>
#include <QVBoxLayout>
#include <QButtonGroup>

GraphicsItemWidget::GraphicsItemWidget(QWidget *parent)
	: QWidget(parent)
{
    int btn_width = 28;int btn_height = 28;
    setFixedWidth(38);
    setMinimumHeight(400);
    //图形按钮垂直布局器
    shape_btn_layout = new QVBoxLayout(this);

    destory_btn = new GenericToolButton();
    destory_btn->setObjectName("graphicsitem_btn");
    destory_btn->setIcon(QIcon(":/res/background-image/destory.png"));
    destory_btn->setFixedSize(btn_width, btn_height);
    connect(destory_btn, &QPushButton::clicked, this, &GraphicsItemWidget::onDestoryBtn);
    //设置图形按钮
    point_btn = new GenericToolButton();
    point_btn->setObjectName("graphicsitem_btn");

    rect_btn = new GenericToolButton();
    rect_btn->setObjectName("graphicsitem_btn");

    line_btn = new GenericToolButton();
    line_btn->setObjectName("graphicsitem_btn");

    round_btn = new GenericToolButton();
    round_btn->setObjectName("graphicsitem_btn");

    pie_btn = new GenericToolButton();
    pie_btn->setObjectName("graphicsitem_btn");

    ellipse_btn = new GenericToolButton();
    ellipse_btn->setObjectName("graphicsitem_btn");

    line_segment_btn = new GenericToolButton();
    line_segment_btn->setObjectName("graphicsitem_btn");

    polygon_btn = new GenericToolButton();
    polygon_btn->setObjectName("graphicsitem_btn");

    angle_btn = new GenericToolButton();
    angle_btn->setObjectName("graphicsitem_btn");

    parallel_line_btn = new GenericToolButton();
    parallel_line_btn->setObjectName("graphicsitem_btn");

    draw_button_list.append(point_btn);
    draw_button_list.append(line_btn);
    draw_button_list.append(rect_btn);
    draw_button_list.append(ellipse_btn);
    draw_button_list.append(round_btn);
    draw_button_list.append(pie_btn);
    draw_button_list.append(line_segment_btn);
    draw_button_list.append(polygon_btn);
    draw_button_list.append(angle_btn);
    draw_button_list.append(parallel_line_btn);

    destory_btn->setCustomTooltip(tr("Destory"));//设置提示
    point_btn->setCustomTooltip(tr("Point"));
    rect_btn->setCustomTooltip(tr("Rect"));
    round_btn->setCustomTooltip(tr("Round"));
    pie_btn->setCustomTooltip(tr("Pie"));
    ellipse_btn->setCustomTooltip(tr("Ellipse"));
    line_btn->setCustomTooltip(tr("Line"));
    line_segment_btn->setCustomTooltip(tr("LineSegment"));
    polygon_btn->setCustomTooltip(tr("Polygon"));
    angle_btn->setCustomTooltip(tr("Angle"));
    parallel_line_btn->setCustomTooltip(tr("ParallelLine"));

    shape_btn_layout->addWidget(destory_btn);

    foreach(QPushButton* btn, draw_button_list)
    {
        shape_btn_layout->addWidget(btn);
        btn->setFixedSize(btn_width, btn_height);
        btn->setCheckable(true);
        btn->setIconSize(QSize(20, 20));
    }
    shape_btn_layout->setContentsMargins(5, 0, 5, 25);
    shape_btn_layout->setSpacing(3);
    setLayout(shape_btn_layout);

    point_btn->setIcon(QIcon(":/res/background-image/point.png"));
    line_btn->setIcon(QIcon(":/res/background-image/line.png"));
    round_btn->setIcon(QIcon(":/res/background-image/round.png"));
    ellipse_btn->setIcon(QIcon(":/res/background-image/ellipse.png"));
    rect_btn->setIcon(QIcon(":/res/background-image/rect.png"));
    pie_btn->setIcon(QIcon(":/res/background-image/pie.png"));
    polygon_btn->setIcon(QIcon(":/res/background-image/polygon.png"));
    angle_btn->setIcon(QIcon(":/res/background-image/angle.png"));
    parallel_line_btn->setIcon(QIcon(":/res/background-image/Parallel_line.png"));
    line_segment_btn->setIcon(QIcon(":/res/background-image/line_segment.png"));

    cal_center_btn = new GenericToolButton();
    cal_center_btn->setObjectName("graphicsitem_btn");
    cal_center_btn->setFixedSize(btn_width, btn_height);
    cal_center_btn->setCheckable(true);

    cal_vertical_btn = new GenericToolButton();
    cal_vertical_btn->setObjectName("graphicsitem_btn");
    cal_vertical_btn->setFixedSize(btn_width, btn_height);
    cal_vertical_btn->setCheckable(true);

    cal_angle_btn = new GenericToolButton();
    cal_angle_btn->setObjectName("graphicsitem_btn");
    cal_angle_btn->setFixedSize(btn_width, btn_height);
    cal_angle_btn->setCheckable(true);

    cal_center_btn->setIcon(QIcon(":/res/background-image/cal_center.png"));
    cal_vertical_btn->setIcon(QIcon(":/res/background-image/cal_verti.png"));
    cal_angle_btn->setIcon(QIcon(":/res/background-image/cal_angle.png"));

    cal_center_btn->setCustomTooltip("CalCenter");
    cal_vertical_btn->setCustomTooltip("CalVertical");
    cal_angle_btn->setCustomTooltip("CalAngle");

    exclusive_button_group = new QButtonGroup(this);
    exclusive_button_group->addButton(cal_center_btn);
    exclusive_button_group->addButton(cal_vertical_btn);
    exclusive_button_group->addButton(cal_angle_btn);
    exclusive_button_group->setExclusive(true);

    shape_btn_layout->addSpacing(14);
    shape_btn_layout->addWidget(cal_center_btn);
    shape_btn_layout->addWidget(cal_vertical_btn);
    shape_btn_layout->addWidget(cal_angle_btn);
    shape_btn_layout->addStretch();
    cal_center_btn->setChecked(true);
    connect(cal_center_btn, &QPushButton::toggled, this, &GraphicsItemWidget::onCalCenterBtnClicked);
    connect(cal_vertical_btn, &QPushButton::toggled, this, &GraphicsItemWidget::onCalVerticalBtnClicked);
    connect(cal_angle_btn, &QPushButton::toggled, this, &GraphicsItemWidget::onCalAngleBtnClicked);
}

GraphicsItemWidget::~GraphicsItemWidget()
{
    delete shape_btn_layout;
}

void GraphicsItemWidget::setViewListContainer(ViewListContainer* vlc)
{
    view_list_container = vlc;
}

QList<QPushButton*> GraphicsItemWidget::getDrawButtonList()
{
    return draw_button_list;
}

void GraphicsItemWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QStyleOption styleOpt;
    styleOpt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &styleOpt, &painter, this);
}

void GraphicsItemWidget::connectSceneSignal(GraphicsScene* s)
{
    connect(point_btn, &QPushButton::toggled, s, &GraphicsScene::pointClicked);
    connect(ellipse_btn, &QPushButton::toggled, s, &GraphicsScene::ellipseClicked);
    connect(round_btn, &QPushButton::toggled, s, &GraphicsScene::roundClicked);
    connect(pie_btn, &QPushButton::toggled, s, &GraphicsScene::pieClicked);
    connect(line_btn, &QPushButton::toggled, s, &GraphicsScene::lineClicked);
    connect(rect_btn, &QPushButton::toggled, s, &GraphicsScene::rectClicked);
    connect(polygon_btn, &QPushButton::toggled, s, &GraphicsScene::polygonClicked);
    connect(line_segment_btn, &QPushButton::toggled, s, &GraphicsScene::lineSegmentClicked);
    connect(angle_btn, &QPushButton::toggled, s, &GraphicsScene::angleClicked);
    connect(parallel_line_btn, &QPushButton::toggled, s, &GraphicsScene::parallelLineClicked);
    s->pointClicked(true);
}

void GraphicsItemWidget::disconnectSceneSignal(GraphicsScene* s)
{
    disconnect(point_btn, &QPushButton::toggled, s, &GraphicsScene::pointClicked);
    disconnect(ellipse_btn, &QPushButton::toggled, s, &GraphicsScene::ellipseClicked);
    disconnect(round_btn, &QPushButton::toggled, s, &GraphicsScene::roundClicked);
    disconnect(pie_btn, &QPushButton::toggled, s, &GraphicsScene::pieClicked);
    disconnect(line_btn, &QPushButton::toggled, s, &GraphicsScene::lineClicked);
    disconnect(rect_btn, &QPushButton::toggled, s, &GraphicsScene::rectClicked);
    disconnect(polygon_btn, &QPushButton::toggled, s, &GraphicsScene::polygonClicked);
    disconnect(line_segment_btn, &QPushButton::toggled, s, &GraphicsScene::lineSegmentClicked);
    disconnect(angle_btn, &QPushButton::toggled, s, &GraphicsScene::angleClicked);
    disconnect(parallel_line_btn, &QPushButton::toggled, s, &GraphicsScene::parallelLineClicked);
}

void GraphicsItemWidget::onCalCenterBtnClicked(int checked)
{
    if (checked) {
        for (GraphicsView* v : view_list_container->getViewList())
            v->getGraphicsCalculateModel()->setCalMode(
            GraphicsCalculateModel::CalModeType::CalCenterDis);
    }
    else {

    }
}

void GraphicsItemWidget::onCalVerticalBtnClicked(int checked)
{
    if (checked) {
        for (GraphicsView* v : view_list_container->getViewList())
            v->getGraphicsCalculateModel()->setCalMode(
            GraphicsCalculateModel::CalModeType::CalVertiDis);
    }
    else {

    }
}

void GraphicsItemWidget::onCalAngleBtnClicked(int checked)
{
    if (checked) {
        for (GraphicsView* v : view_list_container->getViewList())
            v->getGraphicsCalculateModel()->setCalMode(
            GraphicsCalculateModel::CalModeType::CalLineAngle);
    }
    else {

    }
}

void GraphicsItemWidget::onDestoryBtn()
{
    QList<GraphicsItem*> item_list;
    for (int i = 0; i < 1000; i++) {
        InteractionRect* rect = new InteractionRect(100, 100, 100, 100);
        item_list.append(rect);
    }
    for (int i = 0; i < 1000; i++) {
        view_list_container->getActivedView()->getGraphicsScene()->addItemInit(item_list[i]);
    }
}