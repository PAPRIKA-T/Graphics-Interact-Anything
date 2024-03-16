#include "CalculateModeMenu.h"
#include "graphs/Graphicsview.h"
#include "model/StyleSheetConfigModel.h"
#include <QActionGroup>
#include <QAction>

CalculateModeMenu::CalculateModeMenu(QWidget* parent)
{
    cal_center_btn = new QAction();
    cal_center_btn->setObjectName("graphicsitem_btn");
    cal_center_btn->setCheckable(true);

    cal_vertical_btn = new QAction();
    cal_vertical_btn->setObjectName("graphicsitem_btn");
    cal_vertical_btn->setCheckable(true);

    cal_angle_btn = new QAction();
    cal_angle_btn->setObjectName("graphicsitem_btn");
    cal_angle_btn->setCheckable(true);

    cal_center_btn->setIcon(QIcon(":/res/background-image/cal_center.png"));
    cal_vertical_btn->setIcon(QIcon(":/res/background-image/cal_verti.png"));
    cal_angle_btn->setIcon(QIcon(":/res/background-image/cal_angle.png"));

    cal_center_btn->setText("CalCenter");
    cal_vertical_btn->setText("CalVertical");
    cal_angle_btn->setText("CalAngle");

    exclusive_button_group = new QActionGroup(this);
    exclusive_button_group->addAction(cal_center_btn);
    exclusive_button_group->addAction(cal_vertical_btn);
    exclusive_button_group->addAction(cal_angle_btn);
    exclusive_button_group->setExclusive(true);

    cal_center_btn->setChecked(true);
    connect(cal_center_btn, &QAction::toggled, this, &CalculateModeMenu::onCalCenterBtnClicked);
    connect(cal_vertical_btn, &QAction::toggled, this, &CalculateModeMenu::onCalVerticalBtnClicked);
    connect(cal_angle_btn, &QAction::toggled, this, &CalculateModeMenu::onCalAngleBtnClicked);

    addAction(cal_center_btn);
    addAction(cal_vertical_btn);
    addAction(cal_angle_btn);

    StyleSheetConfigModel* style_model = new StyleSheetConfigModel();
    style_model->setMenuStyle(this);
}

CalculateModeMenu::~CalculateModeMenu()
{
    delete cal_center_btn;
    delete cal_vertical_btn;
    delete cal_angle_btn;
}

void CalculateModeMenu::setGraphicsView(GraphicsView* v)
{
    m_view = v;
}

void CalculateModeMenu::onCalCenterBtnClicked(int checked)
{
    if (checked) {
        m_view->getGraphicsCalculateModel()->setCalMode(GraphicsCalculateModel::CalModeType::CalCenterDis);
    }
    else {
    }
}

void CalculateModeMenu::onCalVerticalBtnClicked(int checked)
{
    if (checked) {
        m_view->getGraphicsCalculateModel()->setCalMode(GraphicsCalculateModel::CalModeType::CalVertiDis);
    }
    else {

    }
}

void CalculateModeMenu::onCalAngleBtnClicked(int checked)
{
    if (checked) {
        m_view->getGraphicsCalculateModel()->setCalMode(GraphicsCalculateModel::CalModeType::CalLineAngle);
    }
    else {

    }
}
