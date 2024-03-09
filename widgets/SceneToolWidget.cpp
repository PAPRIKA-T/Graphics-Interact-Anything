#include "SceneToolWidget.h"
#include "Model/ViewListContainer.h"
#include "graphs/Graphicspixmapitem.h"
#include "Model/ScenePromptItemModel.h"
#include "GenericToolButton.h"
#include <QButtonGroup>
#include <QHBoxLayout>
#include "GiantInteractionModeWidget.h"
#include "FileView.h"

/*************************SceneToolWidget************************/
SceneToolWidget::SceneToolWidget(QWidget* parent)
    :QWidget(parent)
{
    setFixedHeight(32);
    main_layout = new QHBoxLayout(this);
    zoom_btn = new GenericToolButton(this);
    zoom_btn->setCheckable(true);
    zoom_btn->setIcon(QIcon(":/res/background-image/zoom_in.png"));
    camera_btn = new GenericToolButton(this);
    camera_btn->setIcon(QIcon(":/res/background-image/camera.png"));
    color_reverse_btn = new GenericToolButton(this);
    color_reverse_btn->setIcon(QIcon(":/res/background-image/reverse.png"));
    //color_reverse_btn->setCheckable(true);
    fix_screen_btn = new GenericToolButton(this);
    fix_screen_btn->setIcon(QIcon(":/res/background-image/fix_screen.png"));
    center_on_btn = new GenericToolButton(this);
    center_on_btn->setIcon(QIcon(":/res/background-image/center.png"));
    rotateR_btn = new GenericToolButton(this);
    rotateR_btn->setIcon(QIcon(":/res/background-image/rotate_R.png"));
    rotateL_btn = new GenericToolButton(this);
    rotateL_btn->setIcon(QIcon(":/res/background-image/rotate_L.png"));
    clear_scene_btn = new GenericToolButton(this);
    clear_scene_btn->setIcon(QIcon(":/res/background-image/delete.png"));
    turn_left_btn = new GenericToolButton(this);
    turn_left_btn->setIcon(QIcon(":/res/background-image/left.png"));
    turn_right_btn = new GenericToolButton(this);
    turn_right_btn->setIcon(QIcon(":/res/background-image/right.png"));

    mask_to_graphicsitem = new GenericToolButton(this);
    mask_to_graphicsitem->setIcon(QIcon(":/res/background-image/mask_to_item.png"));

    mask_to_graphicsitem->setCustomTooltip("Mask To Item");
    zoom_btn->setCustomTooltip("Zoom In");
    camera_btn->setCustomTooltip("Camera");
    color_reverse_btn->setCustomTooltip("Color Reverse");
    fix_screen_btn->setCustomTooltip("Fit Screen");
    center_on_btn->setCustomTooltip("Center On");
    rotateR_btn->setCustomTooltip("Rotate Right");
    rotateL_btn->setCustomTooltip("Rotate Left");
    clear_scene_btn->setCustomTooltip("Clear");
    turn_left_btn->setCustomTooltip("Previous");
    turn_right_btn->setCustomTooltip("Next");

    connect(mask_to_graphicsitem, &QPushButton::clicked, this, &SceneToolWidget::onMaskToGraphicsItemBtn);
    connect(camera_btn, &QPushButton::clicked, this, &SceneToolWidget::onCameraBtn);
    connect(zoom_btn, &QPushButton::clicked, this, &SceneToolWidget::onZoomSceneBtn);
    connect(color_reverse_btn, &QPushButton::clicked, this, &SceneToolWidget::onColorReverseBtn);
    connect(fix_screen_btn, &QPushButton::clicked, this, &SceneToolWidget::onFixScreenBtn);
    connect(center_on_btn, &QPushButton::clicked, this, &SceneToolWidget::onCenterOnBtn);
    connect(rotateR_btn, &QPushButton::clicked, this, &SceneToolWidget::onRotateRBtn);
    connect(rotateL_btn, &QPushButton::clicked, this, &SceneToolWidget::onRotateLBtn);
    connect(clear_scene_btn, &QPushButton::clicked, this, &SceneToolWidget::onClearSceneBtn);
    connect(turn_left_btn, &QPushButton::clicked, this, &SceneToolWidget::onTurnLeftBtn);
    connect(turn_right_btn, &QPushButton::clicked, this, &SceneToolWidget::onTurnRightBtn);

    main_layout->addStretch();
    main_layout->addWidget(clear_scene_btn);
    main_layout->addWidget(turn_left_btn);
    main_layout->addWidget(turn_right_btn);

    QFrame* line_thr = new QFrame(this);
    line_thr->setFrameShape(QFrame::VLine);
    line_thr->setFrameShadow(QFrame::Plain);
    line_thr->setLineWidth(1);
    line_thr->setContentsMargins(2, 5, 2, 5);
    QPalette palette = line_thr->palette();
    palette.setColor(QPalette::WindowText, QColor(100, 100, 100));
    line_thr->setPalette(palette);

    main_layout->addWidget(line_thr);

    main_layout->addWidget(fix_screen_btn);
    main_layout->addWidget(center_on_btn);
    main_layout->addWidget(color_reverse_btn);
    main_layout->addWidget(rotateL_btn);
    main_layout->addWidget(rotateR_btn);

    QFrame* line_second = new QFrame(this);
    line_second->setFrameShape(QFrame::VLine);
    line_second->setFrameShadow(QFrame::Plain);
    line_second->setLineWidth(1);
    line_second->setContentsMargins(2, 5, 2, 5);
    palette = line_second->palette();
    palette.setColor(QPalette::WindowText, QColor(100, 100, 100));
    line_second->setPalette(palette);

    main_layout->addWidget(line_second);

    main_layout->addWidget(camera_btn);
    main_layout->addWidget(zoom_btn);
    main_layout->addWidget(mask_to_graphicsitem);
    main_layout->setContentsMargins(2, 2, 2, 2);
    main_layout->setSpacing(6);
    setLayout(main_layout);
}

SceneToolWidget::~SceneToolWidget()
{
    delete main_layout;
}

void SceneToolWidget::onMaskToGraphicsItemBtn()
{
    view_list_container->getActivedView()->getGraphicsScene()->getScenePromptItemModel()->Mask2Item();
}

void SceneToolWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QStyleOption styleOpt;
    styleOpt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &styleOpt, &painter, this);
}

void SceneToolWidget::setViewListContainer(ViewListContainer* vlc)
{
    view_list_container = vlc;
}

void SceneToolWidget::setFileView(FileView* f)
{
    file_view = f;
}

void SceneToolWidget::onCameraBtn()
{
    view_list_container->getActivedView()->saveViewImage();
}

void SceneToolWidget::onZoomSceneBtn(int checked)
{
    emit view_list_container->getActivedView()->getGraphicsScene()->zoom3DLayout(checked);
    view_list_container->getActivedView()->getViewTransFormModel()->setZoomIn(checked);
    view_list_container->getActivedView()->getViewTransFormModel()->fitScreen();
}

void SceneToolWidget::onColorReverseBtn()
{
    QImage org_image = view_list_container->getActivedView()->getGraphicsScene()->
        getPixmapItem()->getPixmap().toImage();
    if (org_image.isNull()) return;
    // 循环获得每个像素的rgb，循环减，进行反转
    for (int w = 0; w < org_image.width(); ++w)
    {
        for (int h = 0; h < org_image.height(); ++h)
        {
            QColor rgb = org_image.pixel(w, h);
            rgb.setRgb(255 - rgb.red(), 255 - rgb.green(), 255 - rgb.blue());
            //替换颜色
            org_image.setPixel(w, h, rgb.rgba());
        }
    }

    view_list_container->getActivedView()->getGraphicsScene()->
        getPixmapItem()->updatePixmap(QPixmap::fromImage(org_image));
}

void SceneToolWidget::onFixScreenBtn()
{
    view_list_container->getActivedView()->getViewTransFormModel()->fitScreen();
}

void SceneToolWidget::onCenterOnBtn()
{
    view_list_container->getActivedView()->getViewTransFormModel()->originPositonReturn();
}

void SceneToolWidget::onRotateRBtn()
{
    view_list_container->getActivedView()->getViewTransFormModel()->rotateR();
    view_list_container->getActivedView()->getGraphicsScene()->updateTextPos();
}

void SceneToolWidget::onRotateLBtn()
{
    view_list_container->getActivedView()->getViewTransFormModel()->rotateL();
    view_list_container->getActivedView()->getGraphicsScene()->updateTextPos();
}

void SceneToolWidget::onClearSceneBtn()
{
    view_list_container->getActivedView()->
        getGraphicsScene()->clearSceneGraphicsItem();
}

void SceneToolWidget::onTurnLeftBtn()
{
    file_view->PreviousIndex();
}

void SceneToolWidget::onTurnRightBtn()
{
    file_view->nextIndex();
}

