#include "GraphicsItemMenu.h"
#include "graphs/Graphicsscene.h"
#include "graphs/GraphicsRect.h"
#include "graphs/Graphicsview.h"
#include "model/StyleSheetConfigModel.h"
#include <QAction>
#include <QActionGroup>

GraphicsItemMenu::GraphicsItemMenu(QWidget* parent)
{
    destory_btn = new QAction();
    destory_btn->setObjectName("graphicsitem_btn");
    destory_btn->setIcon(QIcon(":/res/background-image/destory.png"));
    connect(destory_btn, &QAction::triggered, this, &GraphicsItemMenu::onDestoryBtn);
    //设置图形按钮
    point_btn = new QAction();
    point_btn->setObjectName("graphicsitem_btn");
    rect_btn = new QAction();
    rect_btn->setObjectName("graphicsitem_btn");
    line_btn = new QAction();
    line_btn->setObjectName("graphicsitem_btn");
    round_btn = new QAction();
    round_btn->setObjectName("graphicsitem_btn");
    pie_btn = new QAction();
    pie_btn->setObjectName("graphicsitem_btn");
    ellipse_btn = new QAction();
    ellipse_btn->setObjectName("graphicsitem_btn");
    line_segment_btn = new QAction();
    line_segment_btn->setObjectName("graphicsitem_btn");
    polygon_btn = new QAction();
    polygon_btn->setObjectName("graphicsitem_btn");
    angle_btn = new QAction();
    angle_btn->setObjectName("graphicsitem_btn");
    parallel_line_btn = new QAction();
    parallel_line_btn->setObjectName("graphicsitem_btn");

    draw_action_list.append(point_btn);
    draw_action_list.append(line_btn);
    draw_action_list.append(rect_btn);
    draw_action_list.append(ellipse_btn);
    draw_action_list.append(round_btn);
    draw_action_list.append(pie_btn);
    draw_action_list.append(line_segment_btn);
    draw_action_list.append(polygon_btn);
    draw_action_list.append(angle_btn);
    draw_action_list.append(parallel_line_btn);

    destory_btn->setText(tr("Destory"));//设置提示
    point_btn->setText(tr("Point"));
    rect_btn->setText(tr("Rect"));
    round_btn->setText(tr("Round"));
    pie_btn->setText(tr("Pie"));
    ellipse_btn->setText(tr("Ellipse"));
    line_btn->setText(tr("Line"));
    line_segment_btn->setText(tr("LineSegment"));
    polygon_btn->setText(tr("Polygon"));
    angle_btn->setText(tr("Angle"));
    parallel_line_btn->setText(tr("ParallelLine"));

    exclusive_action_group = new QActionGroup(this);

    foreach(QAction * btn, draw_action_list)
    {
        btn->setCheckable(true);
        exclusive_action_group->addAction(btn);
    }
    draw_action_list[0]->setChecked(true);
    exclusive_action_group->setExclusive(true);

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

    addActions(draw_action_list);
    StyleSheetConfigModel* style_model = new StyleSheetConfigModel();
    style_model->setMenuStyle(this);
}

GraphicsItemMenu::~GraphicsItemMenu()
{
    foreach(QAction * btn, draw_action_list)
    {
        delete btn;
    }
}

QList<QAction*> GraphicsItemMenu::getDrawActionList()
{
    return draw_action_list;
}

void GraphicsItemMenu::setGraphicsView(GraphicsView* v)
{
    m_view = v;
    connectSceneSignal(m_view->getGraphicsScene());
}

void GraphicsItemMenu::connectSceneSignal(GraphicsScene* s)
{
    connect(point_btn, &QAction::toggled, s, &GraphicsScene::pointClicked);
    connect(ellipse_btn, &QAction::toggled, s, &GraphicsScene::ellipseClicked);
    connect(round_btn, &QAction::toggled, s, &GraphicsScene::roundClicked);
    connect(pie_btn, &QAction::toggled, s, &GraphicsScene::pieClicked);
    connect(line_btn, &QAction::toggled, s, &GraphicsScene::lineClicked);
    connect(rect_btn, &QAction::toggled, s, &GraphicsScene::rectClicked);
    connect(polygon_btn, &QAction::toggled, s, &GraphicsScene::polygonClicked);
    connect(line_segment_btn, &QAction::toggled, s, &GraphicsScene::lineSegmentClicked);
    connect(angle_btn, &QAction::toggled, s, &GraphicsScene::angleClicked);
    connect(parallel_line_btn, &QAction::toggled, s, &GraphicsScene::parallelLineClicked);
    s->pointClicked(true);
}

void GraphicsItemMenu::onDestoryBtn()
{
    QList<GraphicsItem*> item_list;
    for (int i = 0; i < 1000; i++) {
        InteractionRect* rect = new InteractionRect(100, 100, 100, 100);
        item_list.append(rect);
    }
    for (int i = 0; i < 1000; i++) {
        m_view->getGraphicsScene()->addItemInit(item_list[i]);
    }
}