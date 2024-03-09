#pragma once

#include <QWidget>
#include "ColorButton.h"

class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class ColorWidget : public QWidget
{
    Q_OBJECT
public:
    ColorWidget(QWidget* parent = nullptr);
    ~ColorWidget();
    void setLabelText(QString s);//设置标签内容
    void setLabelTextFont(QFont f); //设置标签字体
    QList<ColorButton*> getColorBtnList() { return color_button_list; }//获取颜色按钮链表
    void setColorBtnBackground(int index, QColor c); //设置index处按钮背景颜色
    QColor getColorBtnBackground(int index) { return color_button_list.at(index)->getBackgroundColor(); } //获取index处按钮背景颜色
public slots:
    void openColorDialog(); //打开默认调色板
    void changeFirstBtnColor(QColor c); //更改第一个按钮的背景颜色
private:
    QColor chosen_color; //获取默认调色板选择的颜色
    QVBoxLayout* main_layout; //主布局器
    QHBoxLayout* color_btn_layout; //颜色按钮布局器
    QLabel* name_label; //标签
    QWidget* color_btn_wid; //颜色按钮布局器底层控件
    QList<ColorButton*> color_button_list; //颜色按钮链表
};