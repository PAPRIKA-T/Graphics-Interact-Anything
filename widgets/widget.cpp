#include <QDebug>
#include <QLineEdit>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QSplitter>
#include <QHBoxLayout>
#include <QTimer>
#include <QGuiApplication>
#include "widget.h"
#include "GraphicsItemWidget.h"
#include "LabelBoardWidget.h"
#include "LabelBoardToolWidget.h"
#include "ItemIndexView.h"
#include "StatusWidget.h"
#include "ImageSceneWidget3D.h"
#include "ViewToolBar.h"
#include "FileView.h"
#include "ForePlayWidget.h"
#include "TitleWidget.h"
#include "MagnifyingGlassWidget.h"
#include "SAM/samwidget.h"
#include "graphs/Graphicsscene.h"
#include "graphs/Graphicsview.h"
#include "graphs/Graphicsitem.h"
#include "graphs/Graphicspixmapitem.h"
#include "Model/ScenePromptItemModel.h"
#include "ChosePathWidget.h"
#include "StackWidget.h"
#include "StackIconWidget.h"
#include"ImageSceneWidget2D.h"
#include "SceneToolWidget.h"
#include "GiantInteractionModeWidget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setObjectName("this_window");
    setWindowIcon(QIcon(":/res/background-image/DRAW_ICON.png"));
    setMouseTracking(true);

    title_widget = new TitleWidget(this);
    title_widget->setObjectName("title_widget");
    title_widget->setTitleName("");
    title_widget->setTitleIcon(QPixmap(":/res/background-image/DRAW_ICON.png").scaled(20, 20));

    /*****************设置界面控件*****************/
    {
        //设置图像窗口控件2d
        image_widget_2d = new ImageSceneWidget2D(this);
        image_widget_2d->setObjectName("image_widget_2d");

        //设置图形窗口指针
        view_list_container.setActivatdView(image_widget_2d->getGraphicsView());
        view_list_container.pushBackView(image_widget_2d->getGraphicsView());

        //设置TreeView
        file_view = new FileView();
        file_view->setObjectName("file_view");
        file_view->setViewListContainer(&view_list_container);
        image_widget_2d->getViewToolBar()->getSceneToolWidget()->setFileView(file_view);
        
        //LabelBoardWidget
        label_board_widget = new LabelBoardWidget(label_board_under_widget);
        label_board_under_widget = new QWidget(this);
        label_board_under_widget->setObjectName("label_board_under_widget");
        label_board_widget->setObjectName("label_board_widget");
        view_list_container.getActivedView()->getGraphicsScene()->setLabelBoardWidget(label_board_widget);
        label_board_widget->setViewListContainer(&view_list_container);
        label_board_widget_layout = new QVBoxLayout(label_board_under_widget);
        label_board_tool_wodget = new LabelBoardToolWidget(label_board_under_widget);
        label_board_tool_wodget->setObjectName("label_board_tool_wodget");
        label_board_tool_wodget->setLabelBoardWidget(label_board_widget);
        label_board_widget_layout->addWidget(label_board_tool_wodget);
        label_board_widget_layout->addWidget(label_board_widget);
        label_board_widget_layout->setSpacing(0);
        label_board_widget_layout->setContentsMargins(0, 0, 0, 0);
        label_board_under_widget->setLayout(label_board_widget_layout);
        
        //设置图元索引控件
        item_index_view = new ItemIndexView(this);
        item_index_view->setObjectName("item_index_widget");
        item_index_view->setLabelBoardWidget(label_board_widget);
        view_list_container.getActivedView()->getGraphicsScene()->setItemIndexView(item_index_view);

        //设置底部状态栏
        status_widget = new StatusWidget(this);
        status_widget->setObjectName("status_widget");
        status_widget->setLeftLabelText("Version 2.2.0");
        file_view->setStatusWidget(status_widget);

        //Init foreplayWidget
        foreplay_widget = new ForePlayWidget(this);
        foreplay_widget->setObjectName("foreplay_widget");
        foreplay_widget->setFileView(file_view);
        foreplay_widget->setViewListContainer(&view_list_container);
        file_view->setForeplayWidget(foreplay_widget);

        //设置SamWidget
        sam_widget = new SamWidget(this);
        sam_widget->setObjectName("sam_widget");
        view_list_container.getActivedView()->getGraphicsScene()
            ->getScenePromptItemModel()->setSamWidget(sam_widget);

        //设置右下堆栈控件
        stack_under_widget = new QWidget(this);
        stack_widget_hori_layout = new QHBoxLayout(stack_under_widget);

        stack_icon_widget = new StackIconWidget(stack_under_widget);
        rb_stack_widget = new StackWidget(stack_under_widget);
        rb_stack_widget->setObjectName("rb_stack_widget");
        rb_stack_widget->addWidget(file_view);
        rb_stack_widget->addWidget(foreplay_widget);
        rb_stack_widget->addWidget(item_index_view);
        rb_stack_widget->addWidget(sam_widget);
        rb_stack_widget->setContentsMargins(0, 0, 0, 0);
        stack_widget_hori_layout->addWidget(stack_icon_widget);
        stack_widget_hori_layout->addWidget(rb_stack_widget);
        stack_widget_hori_layout->setContentsMargins(0, 0, 0, 0);
        stack_widget_hori_layout->setSpacing(0);
        stack_icon_widget->connectStackWidget(rb_stack_widget);
    }

    title_widget->setParentWidget(this);

    /*****************连接信号*****************/
    {
        connect(view_list_container.getActivedView()->getGraphicsScene(),
            &GraphicsScene::createItemIndex,item_index_view,&ItemIndexView::addItemInitAfterPaint);
        connect(file_view, &FileView::sceneClear, view_list_container.getActivedView()->getGraphicsScene(), &GraphicsScene::resetScene);
        connect(file_view, &FileView::viewClear, foreplay_widget, &ForePlayWidget::onPathClear);
    }

/**************************************************设置布局器********************************************************/
    {
        //界面右侧部分控件垂直
        right_widget_splitter = new QSplitter(this);
        right_widget_splitter->setObjectName("right_tab_widget_splitter");
        right_widget_splitter->setOrientation(Qt::Vertical);
        right_widget_splitter->addWidget(label_board_under_widget);
        right_widget_splitter->addWidget(stack_under_widget);
        right_widget_splitter->setCollapsible(0, false);
        right_widget_splitter->setCollapsible(1, false);
        right_widget_splitter->setStretchFactor(0, 6);
        right_widget_splitter->setStretchFactor(1, 4);
        right_widget_splitter->setHandleWidth(4);
        right_widget_splitter->setContentsMargins(0, 2, 2, 0);

        center_widget_splitter = new QSplitter(this);
        center_widget_splitter->setObjectName("right_widget_splitter");
        center_widget_splitter->addWidget(image_widget_2d);
        center_widget_splitter->addWidget(right_widget_splitter);
        center_widget_splitter->setStretchFactor(0, 6);
        center_widget_splitter->setStretchFactor(1, 4);
        center_widget_splitter->setCollapsible(0, false);
        center_widget_splitter->setHandleWidth(2);
        center_widget_splitter->setContentsMargins(0, 0, 0, 0);

        //窗口总体布局器
        main_layout = new QVBoxLayout(this);
        main_layout->addWidget(title_widget);
        main_layout->addWidget(center_widget_splitter);
        main_layout->addWidget(status_widget);
        main_layout->setContentsMargins(0, 0, 0, 0);
        main_layout->setSpacing(0);
    }
    setWidgetSize();
}

Widget::~Widget()
{
}

QVBoxLayout* Widget::getMainLayout()
{
	return main_layout;
}

/*****************其他函数*********************/
ViewListContainer* Widget::getViewListContainer()
{
    return &view_list_container;
}

FileView* Widget::getFileView()
{
    return file_view;
}

SamWidget* Widget::getSamWidget()
{
    return sam_widget;
}

//设置各个控件部分的大小
void Widget::setWidgetSize()
{
    resize(1000, 700);
    setMinimumSize(520, 220);
    image_widget_2d->resize(650, 600);
    label_board_under_widget->setMinimumWidth(200);
    right_widget_splitter->setMinimumWidth(200);
    center_widget_splitter->setMinimumSize(750, 570);
    sam_widget->setMinimumHeight(100);
    //DimensionTrans();
}

//切换图像显示控件
void Widget::imageWidgetAdd(ImageSceneWidget2D* image_widget)
{
    view_list_container.pushBackView(image_widget->getGraphicsView());
    GraphicsScene* m_scene = image_widget->getGraphicsScene();
    m_scene->getScenePromptItemModel()->setSamWidget(sam_widget);
    m_scene->setLabelBoardWidget(label_board_widget);
    m_scene->setItemIndexView(item_index_view);
    connect(m_scene, &GraphicsScene::createItemIndex, item_index_view, 
        &ItemIndexView::addItemInitAfterPaint);
}

void Widget::mousePressChangeImageWidget(ImageSceneWidget2D* image_widget)
{
    if (image_widget_2d) {
        GraphicsScene* scene = image_widget_2d->getGraphicsScene();
        image_widget_2d->getGraphicsView()->setActived(false);
        image_widget_2d->setStyleSheet(QString::fromUtf8("border: 1px solid rgb(102, 0, 204);"));
        disConnectToolButton(image_widget_2d);
        foreach(QGraphicsItem * item, scene->items()) {
            item->setSelected(false);
        }
    }
    image_widget_2d = image_widget;
    view_list_container.setActivatdView(image_widget->getGraphicsView());
    image_widget->getGraphicsView()->setActived(true);
    image_widget->setStyleSheet(QString::fromUtf8("border:1px solid #fe5820;"));
    GraphicsScene* m_scene = image_widget->getGraphicsScene();
    connectToolButton(image_widget_2d);
    QString pix_path = m_scene->getPixmapItem()->getPixmapPath();
    file_view->setFilePath(pix_path);
    file_view->setItemSelected(pix_path);
    status_widget->setRightLabelText(pix_path);
}

void Widget::connectToolButton(ImageSceneWidget2D* image_widget)
{
    GraphicsScene* scene = image_widget->getGraphicsScene();
    connect(file_view, &FileView::sceneClear, 
        scene, &GraphicsScene::resetScene);
}

void Widget::disConnectToolButton(ImageSceneWidget2D* image_widget)
{
    GraphicsScene* scene = image_widget->getGraphicsScene();
    disconnect(file_view, &FileView::sceneClear, 
        scene, &GraphicsScene::resetScene);
}

void Widget::DimensionTrans()
{
    foreach(GraphicsView * view, view_list_container.getViewList()) {
        view->getGraphicsScene()->getScenePromptItemModel()->
            removeAllPromptsItems();
    }
    if (foreplay_widget->getAutoSave())
        foreplay_widget->saveItemToPathAllFormAllScene();

    if (image_widget_2d) {
        image_widget_2d->deleteLater();
        image_widget_2d = nullptr;
        image_widget_3d = new ImageSceneWidget3D(this);
        image_widget_3d->setObjectName("image_widget_3d");
        center_widget_splitter->insertWidget(0, image_widget_3d);
        view_list_container.clearViewList();

        file_view->setVTKWidget(image_widget_3d->getVTKWidget());
        imageWidgetAdd(image_widget_3d->getSceneWidgetTransverse());
        imageWidgetAdd(image_widget_3d->getSceneWidgetSagittal());
        imageWidgetAdd(image_widget_3d->getSceneWidgetCoronal());

        connect(image_widget_3d->getSceneWidgetSagittal()->getGraphicsView(), &GraphicsView::mousePressed,
            this, &Widget::mousePressChangeImageWidget);
        connect(image_widget_3d->getSceneWidgetTransverse()->getGraphicsView(), &GraphicsView::mousePressed,
            this, &Widget::mousePressChangeImageWidget);
        connect(image_widget_3d->getSceneWidgetCoronal()->getGraphicsView(), &GraphicsView::mousePressed,
            this, &Widget::mousePressChangeImageWidget);

        connect(image_widget_3d->getSceneWidgetSagittal()->getGraphicsView(), &GraphicsView::sliceChangeOneByOne,
            file_view, &FileView::onSliceChangeOneByOne);
        connect(image_widget_3d->getSceneWidgetTransverse()->getGraphicsView(), &GraphicsView::sliceChangeOneByOne,
            file_view, &FileView::onSliceChangeOneByOne);
        connect(image_widget_3d->getSceneWidgetCoronal()->getGraphicsView(), &GraphicsView::sliceChangeOneByOne,
            file_view, &FileView::onSliceChangeOneByOne);

        mousePressChangeImageWidget(image_widget_3d->getSceneWidgetSagittal());
    }
    else if (image_widget_3d) {
        image_widget_3d->deleteLater();
        image_widget_3d = nullptr;
        image_widget_2d = new ImageSceneWidget2D(this);
        image_widget_2d->setObjectName("image_widget_2d");
        center_widget_splitter->insertWidget(0, image_widget_2d);
        view_list_container.clearViewList();
        file_view->setVTKWidget(nullptr);
        imageWidgetAdd(image_widget_2d);
        connectToolButton(image_widget_2d);
    }
    //view_tool_bar->getInteractionModeWidget()->returnToDefaultMode();
}

/*******************override virtual function********************/

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QStyleOption opt;
    opt.initFrom(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
    Q_UNUSED(event);
    painter.setBrush(QColor(40, 40, 40));
    painter.drawRect(rect().adjusted(15, 15, -15, -15));
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Q:
        file_view->PreviousIndex();
        break;
    case Qt::Key_E:
        file_view->nextIndex();
        break;
    default:
        return;
    }
}

void Widget::closeEvent(QCloseEvent *event)
{
    if (foreplay_widget->getAutoSave())
        foreplay_widget->saveItemToPathAllFormAllScene();
    QWidget::closeEvent(event);
}

void Widget::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::WindowStateChange){
        if (isMaximized()){
            // 窗口被最大化时的处理
            status_widget->setStyleSheet("StatusWidget{border-bottom-left-radius: 0px;border-bottom-right-radius: 0px;}");
            title_widget->setStyleSheet("QWidget#title_widget{border-top-left-radius: 0px;border-top-right-radius: 0px;}");
        }
        else if(isMinimized()){
			// 窗口被最小化时的处理
        }
        else {
            // 窗口被还原时的处理
            status_widget->setStyleSheet("StatusWidget{border-bottom-left-radius: 8px;border-bottom-right-radius: 8px;}");
            title_widget->setStyleSheet("QWidget#title_widget{border-top-left-radius: 8px;border-top-right-radius: 8px;}");
        }
    }
    QWidget::changeEvent(event);
}

void Widget::leaveEvent(QEvent* event)
{
    this->unsetCursor();
}
