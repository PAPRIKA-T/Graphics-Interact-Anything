#include "StatusWidget.h"
#include <QStyleOption>
#include <QPainter>

/****************************************************StatusWidget*********************************************************/

StatusWidget::StatusWidget(QWidget* parent)
    :QWidget(parent)
{
    setObjectName("status_widget"); //初始化控件属性
    setFixedHeight(20);
    right_label = new QLabel(); //初始化label
    left_label = new QLabel();
    main_layout = new QHBoxLayout(this);
    main_layout->addWidget(left_label);
    main_layout->addStretch();
    main_layout->addWidget(right_label);
    main_layout->setContentsMargins(10, 2, 10, 2);

    /************************************************************************/
    QString str0 = "Image_Size:" + QString::number(0, 'f', 0) + "px," + QString::number(0, 'f', 0) + "px ";
    QString str1 = "MouseX:" + QString::number(0, 'f', 0) + "px ";
    QString str2 = "MouseY:" + QString::number(0, 'f', 0) + "px ";
    QString str3 = "Zoom:" + QString::number(1, 'f', 2) + " ";
    //setLeftLabelText(str0 + str1 + str2 + str3);
}

StatusWidget::~StatusWidget()
{
    delete main_layout;
    delete left_label;
    delete right_label;
}

//修改标题显示文字颜色
void StatusWidget::setStatusLabelTextColor(int R, int G, int B)
{
    left_label->setStyleSheet(QString("QLabel{background-color:transparent;color:rgb(%1,%2,%3);outline: none;border:none;qproperty-alignment: 'AlignVCenter | AlignLeft';}").arg(R).arg(G).arg(B));
    right_label->setStyleSheet(QString("QLabel{background-color:transparent;color:rgb(%1,%2,%3);outline: none;border:none;qproperty-alignment: 'AlignVCenter | AlignLeft';}").arg(R).arg(G).arg(B));
}
