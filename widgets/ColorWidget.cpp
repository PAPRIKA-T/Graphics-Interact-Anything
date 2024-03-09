#include "ColorWidget.h"
#include <QColorDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>

ColorWidget::ColorWidget(QWidget* parent)
    :QWidget(parent)
{
    setObjectName("ColorWidget");
    main_layout = new QVBoxLayout(this);

    name_label = new QLabel();
    QFont font = QFont("宋体");
    font.setBold(false);
    font.setPointSize(10);
    name_label->setFont(font);

    QFrame* hvrame1 = new QFrame(this);
    hvrame1->setFrameShape(QFrame::VLine);

    color_btn_wid = new QWidget();
    color_btn_layout = new QHBoxLayout(color_btn_wid);
    color_btn_layout->setContentsMargins(2, 2, 2, 2);
    for (int i = 0; i < 9; i++)
    {
        ColorButton* color = new ColorButton();
        color_button_list.append(color);
        color_btn_layout->addWidget(color);
        switch (i) {
        case 0:
            color->setBackgrondColor(Qt::red);
            color_btn_layout->addWidget(hvrame1);
            break;
        case 1:
            color->setBackgrondColor(Qt::red);
            connect(color, &QPushButton::clicked, color, &ColorButton::sentColor);
            connect(color, &ColorButton::sentSelfColor, this, &ColorWidget::changeFirstBtnColor);
            break;
        case 2:
            color->setBackgrondColor(QColor(255, 128, 0));
            connect(color, &QPushButton::clicked, color, &ColorButton::sentColor);
            connect(color, &ColorButton::sentSelfColor, this, &ColorWidget::changeFirstBtnColor);
            break;
        case 3:
            color->setBackgrondColor(QColor(255, 20, 147)); //DeepPink
            connect(color, &QPushButton::clicked, color, &ColorButton::sentColor);
            connect(color, &ColorButton::sentSelfColor, this, &ColorWidget::changeFirstBtnColor);
            break;
        case 4:
            color->setBackgrondColor(QColor(144, 238, 144));
            connect(color, &QPushButton::clicked, color, &ColorButton::sentColor);
            connect(color, &ColorButton::sentSelfColor, this, &ColorWidget::changeFirstBtnColor);
            break;//LightGreen
        case 5:
            color->setBackgrondColor(QColor(0, 139, 139));
            connect(color, &QPushButton::clicked, color, &ColorButton::sentColor);
            connect(color, &ColorButton::sentSelfColor, this, &ColorWidget::changeFirstBtnColor);
            break; //DarkCyan
        case 6:
            color->setBackgrondColor(QColor(122, 103, 238));
            connect(color, &QPushButton::clicked, color, &ColorButton::sentColor);
            connect(color, &ColorButton::sentSelfColor, this, &ColorWidget::changeFirstBtnColor);
            break; //StateBlue
        case 7:
            color->setBackgrondColor(QColor(128, 0, 128));
            connect(color, &QPushButton::clicked, color, &ColorButton::sentColor);
            connect(color, &ColorButton::sentSelfColor, this, &ColorWidget::changeFirstBtnColor);
            break;//purple
        case 8:
            color->setIcon(QIcon(":/res/background-image/ellipsis.png"));
            connect(color, &QPushButton::clicked, this, &ColorWidget::openColorDialog);
            break;
        }
    }

    main_layout->addWidget(name_label);
    main_layout->addWidget(color_btn_wid);
    main_layout->setContentsMargins(2, 2, 2, 2);
    main_layout->setSpacing(5);
}

ColorWidget::~ColorWidget()
{
    delete color_btn_layout;
    delete color_btn_wid;
    delete name_label;
    delete main_layout;
}
void ColorWidget::setLabelText(QString s)
{
    name_label->setText(s);
}

void ColorWidget::setLabelTextFont(QFont f)
{
    name_label->setFont(f);
}

//设置index处按钮背景颜色
void ColorWidget::setColorBtnBackground(int index, QColor c)
{
    color_button_list.at(index)->setBackgrondColor(c);
}
//打开默认调色板
void ColorWidget::openColorDialog()
{
    chosen_color = QColorDialog::getColor(Qt::white, this);
    if (chosen_color.isValid())color_button_list[0]->setBackgrondColor(chosen_color);
}
//更改第一个按钮的背景颜色
void ColorWidget::changeFirstBtnColor(QColor c)
{
    color_button_list[0]->setBackgrondColor(c);
}
