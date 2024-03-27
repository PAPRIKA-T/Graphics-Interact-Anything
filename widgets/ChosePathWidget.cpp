#include "ChosePathWidget.h"
#include "GenericToolButton.h"
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QFileDialog>

ChosePathWidget::ChosePathWidget(QWidget* parent)
    :QWidget(parent)
{
    setObjectName("chosePathWidget"); //初始化控件属性
    QFont font = QFont("MicrosoftYaHei"); //设置初始字体
    font.setBold(false);
    font.setPointSize(10);
    name_label = new QLabel(); //初始化label
    name_label->setFixedHeight(25);
    subtitle_label = new QLabel();
    name_label->setFont(font);
    subtitle_label->setFont(font);
    subtitle_label->setText(QStringLiteral("Form"));
    form_selection = new QComboBox(); //初始化Combox
    form_selection->addItem("pascalVoc");
    form_selection->addItem("YOLO");
    form_selection->addItem("COCO");

    path_chose_btn = new GenericToolButton(); //初始化路径选择按钮
    path_chose_btn->setFixedSize(21, 21);
    path_chose_btn->setIcon(QIcon(":/res/qss/Dark/background-image/ellipsis.png"));
    connect(path_chose_btn, &QPushButton::clicked, this, &ChosePathWidget::path_chose_btn_clicked);

    path_chose_edit = new QLineEdit(); //初始化路径编辑框
    path_chose_edit->setObjectName("path_chose_edit");
    path_chose_edit->setFixedHeight(25);
    form_selection->setFixedHeight(25);
    form_selection->setMinimumWidth(95);
    path_chose_edit->setPlaceholderText(QStringLiteral("Filepath..."));

    main_layout = new QGridLayout(this);
    main_layout->addWidget(name_label, 0, 0, 1, 4);
    main_layout->addWidget(path_chose_edit, 1, 0, 1, 3);
    main_layout->addWidget(path_chose_btn, 1, 3, 1, 1);
    main_layout->addWidget(subtitle_label, 2, 0, 1, 2);
    main_layout->addWidget(form_selection, 2, 2, 1, 2);
    main_layout->setSpacing(2);
    main_layout->setContentsMargins(2, 2, 2, 2);
}

const QString ChosePathWidget::getFormComboxContent()
{
    return form_selection->currentText();
}

QLineEdit* ChosePathWidget::getPathChoseEdit()
{
    return path_chose_edit;
}

ChosePathWidget::~ChosePathWidget()
{
    delete main_layout;
    delete path_chose_edit;
    delete path_chose_btn;
    delete form_selection;
    delete subtitle_label;
    delete name_label;
}

void ChosePathWidget::setLabelText(QString s)
{
    name_label->setText(s);
}

void ChosePathWidget::setSubtitleText(QString s)
{
    subtitle_label->setText(s);
}

void ChosePathWidget::setLabelTextFont(QFont f)
{
    name_label->setFont(f); 
    subtitle_label->setFont(f);
}

void ChosePathWidget::setPathChoseEditContent(QString s)
{
    path_chose_edit->setText(s);
}

const QString ChosePathWidget::getPathChoseEditContent()
{
    return path_chose_edit->text();
}

void ChosePathWidget::path_chose_btn_clicked()
{
    QString dirpath = QFileDialog::getExistingDirectory(this, tr("读取文件夹路径"), "C:/Users/Administrator/Desktop",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);  //文件夹路径
    if (dirpath == "") return;
    dirpath = dirpath + "/";
    path_chose_edit->setText(dirpath);
}

