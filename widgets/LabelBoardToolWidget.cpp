#include "LabelBoardToolWidget.h"
#include "LabelBoard.h"
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

    board_add_row_btn->setCustomTooltip("Add Row");
	board_remove_row_btn->setCustomTooltip("Remove Row");

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

void LabelBoardToolWidget::setLabelBoardWidget(LabelBoard*lbw)
{
    label_board = lbw;
    connect(board_remove_row_btn, &QPushButton::clicked, label_board, &LabelBoard::onRemoveSelectedRowClicked);
    connect(board_add_row_btn, &QPushButton::clicked, label_board, &LabelBoard::onAppendRowClicked);
}
