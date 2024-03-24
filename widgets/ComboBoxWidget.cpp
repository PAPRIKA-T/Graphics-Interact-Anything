#include "ComboBoxWidget.h"
#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
/**************************************************ComboBoxVWidget********************************************************/
ComboBoxVWidget::ComboBoxVWidget(QWidget* parent)
    :QWidget(parent)
{
    setObjectName("ComboBoxVWidget");
    name_label = new QLabel();
    name_label->setObjectName("sam_label");
    QFont font = QFont("MicrosoftYaHei");
    font.setBold(false);
    name_label->setFont(font);
    combo_box = new QComboBox();
    combo_box->setFixedHeight(28);
    main_layout = new QVBoxLayout(this);
    main_layout->addWidget(name_label);
    main_layout->addWidget(combo_box);
    main_layout->setContentsMargins(2, 2, 2, 2);
    //main_layout->addStretch();
}

ComboBoxVWidget::~ComboBoxVWidget()
{
    delete main_layout;
    delete combo_box;
    delete name_label;
}

void ComboBoxVWidget::setLabelText(const QString& s)
{
    name_label->setText(s);
}

void ComboBoxVWidget::setLabelTextFont(const QFont& f)
{
    name_label->setFont(f);
}

int ComboBoxVWidget::getComBoxContentInt()
{
    return combo_box->currentIndex();
}

void ComboBoxVWidget::setComBoxContentInt(int index)
{
    combo_box->setCurrentIndex(index);
}

QString ComboBoxVWidget::getComBoxContentString()
{
    return combo_box->currentText();
}

void ComboBoxVWidget::setComBoxContentString(const QString& s)
{
    combo_box->setCurrentText(s);
}

QComboBox* ComboBoxVWidget::getComboBox()
{
    return combo_box;
}

QLabel* ComboBoxVWidget::getLabel()
{
    return name_label;
}

/**************************************************ComboBoxHWidget********************************************************/
ComboBoxHWidget::ComboBoxHWidget(QWidget* parent)
    :QWidget(parent)
{
    setObjectName("ComboBoxHWidget");
    name_label = new QLabel();
    name_label->setObjectName("");
    QFont font = QFont("MicrosoftYaHei");
    font.setBold(false);
    name_label->setFont(font);
    combo_box = new QComboBox();
    main_layout = new QHBoxLayout(this);
    main_layout->addWidget(name_label);
    main_layout->addWidget(combo_box);
    main_layout->setContentsMargins(2, 0, 2, 0);
    main_layout->addStretch();
    setContentsMargins(0, 0, 0, 0);
}

ComboBoxHWidget::~ComboBoxHWidget()
{
    delete main_layout;
    delete combo_box;
    delete name_label;
}

void ComboBoxHWidget::setLabelText(const QString& s)
{
    name_label->setText(s);
}

void ComboBoxHWidget::setLabelTextFont(const QFont& f)
{
    name_label->setFont(f);
}

int ComboBoxHWidget::getComBoxContentInt()
{
    return combo_box->currentIndex();
}

void ComboBoxHWidget::setComBoxContentInt(int index)
{
    combo_box->setCurrentIndex(index);
}

QString ComboBoxHWidget::getComBoxContentString()
{
    return combo_box->currentText();
}

void ComboBoxHWidget::setComBoxContentString(const QString& s)
{
    combo_box->setCurrentText(s);
}

QComboBox* ComboBoxHWidget::getComboBox()
{
    return combo_box;
}

QLabel* ComboBoxHWidget::getLabel()
{
    return name_label;
}