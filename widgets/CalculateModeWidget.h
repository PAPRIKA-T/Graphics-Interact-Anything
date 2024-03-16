#pragma once
#include <QWidget>
class QButtonGroup;
class GenericToolButton;
class QHBoxLayout;
class GraphicsView;

class CalculateModeWidget : public QWidget
{
    Q_OBJECT
public:
    CalculateModeWidget(QWidget *parent = nullptr);
    ~CalculateModeWidget();
    void setGraphicsView(GraphicsView*);

private slots:
    void onCalCenterBtnClicked(int checked);
    void onCalVerticalBtnClicked(int checked);
    void onCalAngleBtnClicked(int checked);

private:
    QHBoxLayout* main_layout = nullptr; //绘图按钮垂直布局器
    GraphicsView* m_view = nullptr; //视图指针
    //测量功能模块
    QButtonGroup* exclusive_button_group = nullptr;
    GenericToolButton* cal_center_btn = nullptr;
    GenericToolButton* cal_vertical_btn = nullptr;
    GenericToolButton* cal_angle_btn = nullptr;
};

