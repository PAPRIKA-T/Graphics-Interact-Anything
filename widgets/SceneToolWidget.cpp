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
    main_layout = new QHBoxLayout(this);
    zoom_btn = new GenericToolButton(this);
    zoom_btn->setCheckable(true);
    zoom_btn->setIcon(QIcon(":/res/qss/GenericStyle/background-image/zoom_in.png"));
    camera_btn = new GenericToolButton(this);
    camera_btn->setIcon(QIcon(":/res/qss/GenericStyle/background-image/camera.png"));
    color_reverse_btn = new GenericToolButton(this);
    color_reverse_btn->setIcon(QIcon(":/res/qss/GenericStyle/background-image/reverse.png"));
    fix_screen_btn = new GenericToolButton(this);
    fix_screen_btn->setIcon(QIcon(":/res/qss/GenericStyle/background-image/fix_screen.png"));
    center_on_btn = new GenericToolButton(this);
    center_on_btn->setIcon(QIcon(":/res/qss/GenericStyle/background-image/center.png"));
    rotateR_btn = new GenericToolButton(this);
    rotateR_btn->setIcon(QIcon(":/res/qss/GenericStyle/background-image/rotate_R.png"));
    rotateL_btn = new GenericToolButton(this);
    rotateL_btn->setIcon(QIcon(":/res/qss/GenericStyle/background-image/rotate_L.png"));

    turn_left_btn = new GenericToolButton(this);
    turn_left_btn->setIcon(QIcon(":/res/qss/GenericStyle/background-image/left.png"));
    turn_right_btn = new GenericToolButton(this);
    turn_right_btn->setIcon(QIcon(":/res/qss/GenericStyle/background-image/right.png"));

    mask_to_graphicsitem = new GenericToolButton(this);
    mask_to_graphicsitem->setIcon(QIcon(":/res/qss/GenericStyle/background-image/mask_to_item.png"));

    clear_scene_btn = new GenericToolButton(this);
    clear_scene_btn->setIcon(QIcon(":/res/qss/GenericStyle/background-image/deleteall_btn.png"));
    clear_scene_btn->setCustomTooltip("Delete All Annotation");
    clear_scene_btn->setIconSize(QSize(20, 20));

    clear_calculate_btn = new GenericToolButton(this);
    clear_calculate_btn->setIcon(QIcon(":/res/qss/GenericStyle/background-image/delete_cal_btn.png"));
    clear_calculate_btn->setCustomTooltip("Delete All Calculate Item");
    clear_calculate_btn->setIconSize(QSize(20, 20));

    mask_to_graphicsitem->setCustomTooltip("Mask To Item");
    zoom_btn->setCustomTooltip("Zoom In");
    camera_btn->setCustomTooltip("Camera");
    color_reverse_btn->setCustomTooltip("Color Reverse");
    fix_screen_btn->setCustomTooltip("Fit Screen");
    center_on_btn->setCustomTooltip("Center On");
    rotateR_btn->setCustomTooltip("Rotate Right");
    rotateL_btn->setCustomTooltip("Rotate Left");
    turn_left_btn->setCustomTooltip("Previous");
    turn_right_btn->setCustomTooltip("Next");

    button_list.append(mask_to_graphicsitem);
    button_list.append(camera_btn);
    button_list.append(zoom_btn);
    button_list.append(color_reverse_btn);
    button_list.append(fix_screen_btn);
    button_list.append(center_on_btn);
    button_list.append(rotateR_btn);
    button_list.append(rotateL_btn);
    button_list.append(turn_left_btn);
    button_list.append(turn_right_btn);
    button_list.append(clear_scene_btn);
    button_list.append(clear_calculate_btn);

    foreach(QPushButton * btn, button_list) {
        btn->setObjectName("view_attach_btn");
        btn->setFixedSize(24, 24);
    }

    connect(mask_to_graphicsitem, &QPushButton::clicked, this, &SceneToolWidget::onMaskToGraphicsItemBtn);
    connect(camera_btn, &QPushButton::clicked, this, &SceneToolWidget::onCameraBtn);
    connect(zoom_btn, &QPushButton::clicked, this, &SceneToolWidget::onZoomSceneBtn);
    connect(color_reverse_btn, &QPushButton::clicked, this, &SceneToolWidget::onColorReverseBtn);
    connect(fix_screen_btn, &QPushButton::clicked, this, &SceneToolWidget::onFixScreenBtn);
    connect(center_on_btn, &QPushButton::clicked, this, &SceneToolWidget::onCenterOnBtn);
    connect(rotateR_btn, &QPushButton::clicked, this, &SceneToolWidget::onRotateRBtn);
    connect(rotateL_btn, &QPushButton::clicked, this, &SceneToolWidget::onRotateLBtn);
    connect(turn_left_btn, &QPushButton::clicked, this, &SceneToolWidget::onTurnLeftBtn);
    connect(turn_right_btn, &QPushButton::clicked, this, &SceneToolWidget::onTurnRightBtn);

    connect(clear_scene_btn, &QPushButton::clicked, this, [this]() {
        m_view->getGraphicsScene()->clearSceneGraphicsItem(); });

    connect(clear_calculate_btn, &QPushButton::clicked, this, [this]() {
        m_view->deCalLine(); });
    main_layout->addWidget(clear_scene_btn);
    main_layout->addWidget(clear_calculate_btn);

    main_layout->addSpacing(10);

    main_layout->addWidget(turn_left_btn);
    main_layout->addWidget(turn_right_btn);

    main_layout->addSpacing(10);

    main_layout->addWidget(fix_screen_btn);
    main_layout->addWidget(center_on_btn);
    main_layout->addWidget(color_reverse_btn);
    main_layout->addWidget(rotateL_btn);
    main_layout->addWidget(rotateR_btn);

    main_layout->addSpacing(10);

    main_layout->addWidget(camera_btn);
    main_layout->addWidget(zoom_btn);
    main_layout->addWidget(mask_to_graphicsitem);
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->setSpacing(6);

    setLayout(main_layout);
}

SceneToolWidget::~SceneToolWidget()
{
    delete main_layout;
}

void SceneToolWidget::onMaskToGraphicsItemBtn()
{
    m_view->getGraphicsScene()->getScenePromptItemModel()->Mask2Item();
}

void SceneToolWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QStyleOption styleOpt;
    styleOpt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &styleOpt, &painter, this);
}

void SceneToolWidget::setGraphicsView(GraphicsView* vlc)
{
    m_view = vlc;
}

void SceneToolWidget::setFileView(FileView* f)
{
    file_view = f;
}

void SceneToolWidget::onCameraBtn()
{
    m_view->saveViewImage();
}

void SceneToolWidget::onZoomSceneBtn(int checked)
{
    emit m_view->getGraphicsScene()->zoom3DLayout(checked);
    m_view->getViewTransFormModel()->setZoomIn(checked);
    m_view->getViewTransFormModel()->fitScreen();
}

void SceneToolWidget::onColorReverseBtn()
{
    QImage org_image = m_view->getGraphicsScene()->
        getPixmapItem()->getShowImage();
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

    m_view->getGraphicsScene()->
        getPixmapItem()->updateShowImage(org_image);
}

void SceneToolWidget::onFixScreenBtn()
{
    m_view->getViewTransFormModel()->fitScreen();
}

void SceneToolWidget::onCenterOnBtn()
{
    m_view->getViewTransFormModel()->originPositonReturn();
}

void SceneToolWidget::onRotateRBtn()
{
    m_view->getViewTransFormModel()->rotateR();
    m_view->getGraphicsScene()->updateTextPos();
}

void SceneToolWidget::onRotateLBtn()
{
    m_view->getViewTransFormModel()->rotateL();
    m_view->getGraphicsScene()->updateTextPos();
}

void SceneToolWidget::onTurnLeftBtn()
{
    file_view->PreviousIndex();
}

void SceneToolWidget::onTurnRightBtn()
{
    file_view->nextIndex();
}

