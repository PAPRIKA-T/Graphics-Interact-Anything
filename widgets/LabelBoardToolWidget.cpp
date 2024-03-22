#include "LabelBoardToolWidget.h"
#include "LabelBoard.h"
#include "GenericToolButton.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QLabel>
#include <QCheckBox>

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

    auto_nextline_label = new QLabel("Auto");
    auto_nextline_checkbox = new QCheckBox();

    main_layout->addWidget(auto_nextline_label);
    main_layout->addWidget(auto_nextline_checkbox);
    main_layout->addStretch();
    main_layout->addWidget(board_add_row_btn);
    main_layout->addWidget(board_remove_row_btn);
    main_layout->setContentsMargins(6, 2, 2, 6);
    main_layout->setSpacing(6);
    setLayout(main_layout);
}

LabelBoardToolWidget::~LabelBoardToolWidget()
{
    delete auto_nextline_label;
    delete auto_nextline_checkbox;
    delete main_layout;
}

void LabelBoardToolWidget::setLabelBoardWidget(LabelBoard*lbw)
{
    label_board = lbw;
    connect(board_remove_row_btn, &QPushButton::clicked, label_board, &LabelBoard::onRemoveSelectedRowClicked);
    connect(board_add_row_btn, &QPushButton::clicked, label_board, &LabelBoard::onAppendRowClicked);
    connect(auto_nextline_checkbox, &QCheckBox::stateChanged, label_board, &LabelBoard::onAutoNextLineChecked);
}
