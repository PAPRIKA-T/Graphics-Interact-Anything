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
#include "LabelBoardWithTool.h"
#include "ItemIndexView.h"
#include "StatusWidget.h"
#include "ImageSceneWidget3D.h"
#include "ViewToolBar.h"
#include "FileView.h"
#include "ForePlayWidget.h"
#include "TitleWidget.h"
#include "MagnifyingGlassWidget.h"
#include "graphs/Graphicsscene.h"
#include "graphs/Graphicsview.h"
#include "graphs/Graphicsitem.h"
#include "graphs/Graphicspixmapitem.h"
#include "Model/ScenePromptItemModel.h"
#include "ChosePathWidget.h"
#include "MultiFunctionStackWidget.h"
#include "StackWidget.h"
#include"ImageSceneWidget2D.h"
#include "SceneToolWidget.h"
#include "GiantInteractionModeWidget.h"
#include "LabelBoard.h"
#include "SAM/sam.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setObjectName("this_window");
    setWindowIcon(QPixmap(":/res/qss/GenericStyle/background-image/DRAW_ICON.png").scaled(15, 15));
    setWindowTitle("Graphics-Interact-Anything");
    title_widget = new TitleWidget(this);
    title_widget->setObjectName("title_widget");
    title_widget->setTitleName("");
    title_widget->setTitleIcon(QPixmap(":/res/qss/GenericStyle/background-image/DRAW_ICON.png").scaled(20, 20));

    /*****************设置界面控件*****************/
    {
        //设置图像窗口控件2d
        image_widget_2d = new ImageSceneWidget2D(this);
        image_widget_2d->setObjectName("image_widget_2d");

        //设置图形窗口指针
        view_list_container.setActivatdView(image_widget_2d->getGraphicsView());
        view_list_container.pushBackView(image_widget_2d->getGraphicsView());

        //设置TreeView
        file_view = new FileView(this);
        file_view->setObjectName("file_view");
        file_view->setViewListContainer(&view_list_container);
        image_widget_2d->getViewToolBar()->getSceneToolWidget()->setFileView(file_view);
        
        //LabelBoardWidget
        label_board_widget = new LabelBoardWithTool(this);
        label_board_widget->setObjectName("label_board_widget");
        view_list_container.getActivedView()->getGraphicsScene()->setLabelBoardWidget(label_board_widget->getLabelBoardWidget());
        label_board_widget->getLabelBoardWidget()->setViewListContainer(&view_list_container);
        
        //设置图元索引控件
        item_index_view = new ItemIndexView(this);
        item_index_view->setObjectName("item_index_widget");
        item_index_view->setLabelBoardWidget(label_board_widget->getLabelBoardWidget());
        view_list_container.getActivedView()->getGraphicsScene()->setItemIndexView(item_index_view);

        //设置底部状态栏
        status_widget = new StatusWidget(this);
        status_widget->setObjectName("status_widget");
        status_widget->setLeftLabelText("Version 2.3.0");
        file_view->setStatusWidget(status_widget);

        //Init foreplayWidget
        foreplay_widget = new ForePlayWidget(this);
        foreplay_widget->setObjectName("foreplay_widget");
        foreplay_widget->setFileView(file_view);
        foreplay_widget->setViewListContainer(&view_list_container);
        file_view->setForeplayWidget(foreplay_widget);

        //设置右下堆栈控件
        rb_stack_widget = new MultiFunctionStackWidget(this);
        rb_stack_widget->getStackWidget()->addWidget(file_view);
        rb_stack_widget->getStackWidget()->addWidget(foreplay_widget);
        rb_stack_widget->getStackWidget()->addWidget(item_index_view);
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
        right_widget_splitter->addWidget(label_board_widget);
        right_widget_splitter->addWidget(rb_stack_widget);
        right_widget_splitter->setCollapsible(0, false);
        right_widget_splitter->setCollapsible(1, false);
        right_widget_splitter->setStretchFactor(0, 4);
        right_widget_splitter->setStretchFactor(1, 6);
        right_widget_splitter->setHandleWidth(3);
        right_widget_splitter->setContentsMargins(0, 0, 0, 0);

        center_widget_splitter = new QSplitter(this);
        center_widget_splitter->setObjectName("center_widget_splitter");
        center_widget_splitter->addWidget(image_widget_2d);
        center_widget_splitter->addWidget(right_widget_splitter);
        center_widget_splitter->setStretchFactor(0, 6);
        center_widget_splitter->setStretchFactor(1, 4);
        center_widget_splitter->setCollapsible(0, false);
        center_widget_splitter->setHandleWidth(4);
        center_widget_splitter->setContentsMargins(0, 0, 0, 2);

        //窗口总体布局器
        main_layout = new QVBoxLayout(this);
        main_layout->addWidget(title_widget);
        main_layout->addWidget(center_widget_splitter);
        main_layout->addWidget(status_widget);
        main_layout->setContentsMargins(1, 0, 1, 0);
        main_layout->setSpacing(0);
    }
    setWidgetSize();
    initSamModel();
}

Widget::~Widget()
{
	delete title_widget;
	delete image_widget_2d;
	delete image_widget_3d;
	//delete mag_glass_widget;
    delete label_board_widget;
    delete status_widget;
    delete sam;
    delete file_view;
    delete item_index_view;
    delete foreplay_widget;
    delete rb_stack_widget;
    delete right_widget_splitter;
    delete center_widget_splitter;
    delete main_layout;
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

Sam* Widget::getSam() const 
{
    return sam;
}

LabelBoardWithTool* Widget::getLabelBoardWithTool() const
{
    return label_board_widget;
}

//设置各个控件部分的大小
void Widget::setWidgetSize()
{
    resize(1000, 650);
    image_widget_2d->resize(800, 550);
    right_widget_splitter->setMinimumWidth(250);
    //DimensionTrans();
}

//初始化sam模型
void Widget::initSamModel()
{
    if (sam) return;
    std::string pre_model_path{};
    std::string model_path{};

    pre_model_path = "ai_models/sam/mobile_sam_preprocess.onnx";
    model_path = "ai_models/sam/mobile_sam.onnx";
    if (!QFile::exists(QString::fromStdString(pre_model_path)) || 
        !QFile::exists(QString::fromStdString(model_path))) {
        status_widget->setRightLabelText("model file not exist!");
        qDebug() << "Sam model file not exist!";
		return;
    }
    Sam::Parameter param(pre_model_path, model_path, std::thread::hardware_concurrency());
    param.providers[0].deviceType = 1; // cpu for preprocess
    param.providers[1].deviceType = 1; // CUDA for sam
    sam = new Sam(param);
    if (!sam) {
        status_widget->setRightLabelText("load Sam model fail!");
        qDebug() << "load model fail!";
        return;
    }
    status_widget->setRightLabelText("Sam Model Load Success!");
    image_widget_2d->getGraphicsScene()->getScenePromptItemModel()->setSam(sam);
}

//切换图像显示控件
void Widget::imageWidgetAdd(ImageSceneWidget2D* image_widget)
{
    view_list_container.pushBackView(image_widget->getGraphicsView());
    GraphicsScene* m_scene = image_widget->getGraphicsScene();
    m_scene->setLabelBoardWidget(label_board_widget->getLabelBoardWidget());
    m_scene->setItemIndexView(item_index_view);
    connect(m_scene, &GraphicsScene::createItemIndex, item_index_view, 
        &ItemIndexView::addItemInitAfterPaint);
    image_widget->getGraphicsScene()->getScenePromptItemModel()->setSam(sam);
}

void Widget::mousePressChangeImageWidget(ImageSceneWidget2D* image_widget)
{
    if (image_widget_2d) {
        GraphicsScene* scene = image_widget_2d->getGraphicsScene();
        image_widget_2d->getGraphicsView()->setActived(false);
        image_widget_2d->setStyleSheet(QString::fromUtf8(""));
        disConnectToolButton(image_widget_2d);
        foreach(QGraphicsItem * item, scene->items()) {
            item->setSelected(false);
        }
    }
    image_widget_2d = image_widget;
    view_list_container.setActivatdView(image_widget->getGraphicsView());
    image_widget->getGraphicsView()->setActived(true);
    image_widget->setStyleSheet("border:1px solid #fe5820;");
    GraphicsScene* m_scene = image_widget->getGraphicsScene();
    connectToolButton(image_widget_2d);
    QString pix_path = m_scene->getPixmapItem()->getImagePath();
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

        //mousePressChangeImageWidget(image_widget_3d->getSceneWidgetSagittal());
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

void Widget::leaveEvent(QEvent* event)
{
    this->unsetCursor();
}
