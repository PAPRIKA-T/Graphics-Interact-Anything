#include "LabelBoardToolWidget.h"
#include "LabelBoardWidget.h"
#include "GenericToolButton.h"
#include <QHBoxLayout>
#include <QPainter>

LabelBoardToolWidget::LabelBoardToolWidget(QWidget *parent)
	: QWidget(parent)
{
    setFixedHeight(32);
    main_layout = new QHBoxLayout();
    board_add_row_btn = new GenericToolButton();
    board_add_row_btn->setIcon(QIcon(":/res/qss/GenericStyle/background-image/add_row.png"));

    board_remove_row_btn = new GenericToolButton();
    board_remove_row_btn->setIcon(QIcon(":/res/qss/GenericStyle/background-image/remove_row.png"));

    read_label_btn = new GenericToolButton();
    read_label_btn->setIcon(QIcon(":/res/qss/Dark/background-image/read_label.png"));

    save_label_btn = new GenericToolButton();
    save_label_btn->setIcon(QIcon(":/res/qss/Dark/background-image/save_label.png"));

    board_add_row_btn->setCustomTooltip("Add Row");
	board_remove_row_btn->setCustomTooltip("Remove Row");
    read_label_btn->setCustomTooltip("Read Label");
    save_label_btn->setCustomTooltip("Save Label");

    main_layout->addWidget(read_label_btn);
    main_layout->addWidget(save_label_btn);
    main_layout->addStretch();
    main_layout->addWidget(board_add_row_btn);
    main_layout->addWidget(board_remove_row_btn);
    main_layout->setContentsMargins(2, 2, 2, 2);
    main_layout->setSpacing(6);
    setLayout(main_layout);
}

LabelBoardToolWidget::~LabelBoardToolWidget()
{
    delete main_layout;
}

void LabelBoardToolWidget::setLabelBoardWidget(LabelBoardWidget*lbw)
{
    label_board = lbw;
    connect(read_label_btn, &QPushButton::clicked, label_board, &LabelBoardWidget::readLabelFileFromTxt);
    connect(save_label_btn, &QPushButton::clicked, label_board, &LabelBoardWidget::saveLabelFileToTxt);
    connect(board_remove_row_btn, &QPushButton::clicked, label_board, &LabelBoardWidget::onRemoveSelectedRowClicked);
    connect(board_add_row_btn, &QPushButton::clicked, label_board, &LabelBoardWidget::onAppendRowClicked);
}

void LabelBoardToolWidget::paintEvent(QPaintEvent* event)
{
    QStyleOption styleOpt;
    styleOpt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &styleOpt, &painter, this);
}
