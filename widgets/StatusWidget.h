#pragma once
#include <QLabel>
#include <QHBoxLayout>
#include <QWidget>

/****************************************************底部状态栏控件*********************************************************/
class StatusWidget : public QWidget
{
    Q_OBJECT
public:
    StatusWidget(QWidget* parent = nullptr);
    ~StatusWidget() override;
    void setLeftLabelText(QString s) { left_label->setText(s); }
    void setRightLabelText(QString s) { right_label->setText(s); }
    void setLabelTextFont(QFont f) { left_label->setFont(f); right_label->setFont(f); }
    void setStatusLabelTextColor(int R, int G, int B); //设置字体颜色
private:
    QHBoxLayout* main_layout;
    QLabel* left_label;
    QLabel* right_label;
};