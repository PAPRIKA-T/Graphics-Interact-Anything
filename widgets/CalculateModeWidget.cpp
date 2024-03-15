#include "CalculateModeWidget.h"
#include "GenericToolButton.h"
#include "graphs/GraphicsView.h"
#include <QHBoxLayout>
#include <QButtonGroup>

CalculateModeWidget::CalculateModeWidget(QWidget* parent)
	: QWidget(parent)
{
	int btn_width = 28; int btn_height = 28;
	main_layout = new QHBoxLayout(this);

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

    main_layout->addSpacing(0);
    main_layout->addWidget(cal_center_btn);
    main_layout->addWidget(cal_vertical_btn);
    main_layout->addWidget(cal_angle_btn);
    main_layout->addStretch();
    cal_center_btn->setChecked(true);
    connect(cal_center_btn, &QPushButton::toggled, this, &CalculateModeWidget::onCalCenterBtnClicked);
    connect(cal_vertical_btn, &QPushButton::toggled, this, &CalculateModeWidget::onCalVerticalBtnClicked);
    connect(cal_angle_btn, &QPushButton::toggled, this, &CalculateModeWidget::onCalAngleBtnClicked);
}

CalculateModeWidget::~CalculateModeWidget()
{
    delete main_layout;
}

void CalculateModeWidget::setGraphicsView(GraphicsView* v)
{
    m_view = v;
}

void CalculateModeWidget::onCalCenterBtnClicked(int checked)
{
    if (checked) {
        m_view->getGraphicsCalculateModel()->setCalMode(GraphicsCalculateModel::CalModeType::CalCenterDis);
    }
    else {
    }
}

void CalculateModeWidget::onCalVerticalBtnClicked(int checked)
{
    if (checked) {
        m_view->getGraphicsCalculateModel()->setCalMode(GraphicsCalculateModel::CalModeType::CalVertiDis);
    }
    else {

    }
}

void CalculateModeWidget::onCalAngleBtnClicked(int checked)
{
    if (checked) {
        m_view->getGraphicsCalculateModel()->setCalMode(GraphicsCalculateModel::CalModeType::CalLineAngle);
    }
    else {

    }
}
