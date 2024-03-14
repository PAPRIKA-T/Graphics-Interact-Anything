#include "LabelBoardWithTool.h"
#include "LabelBoard.h"
#include "LabelBoardToolWidget.h"
#include <QVBoxLayout>

LabelBoardWithTool::LabelBoardWithTool(QWidget* parent)
	: QWidget(parent)
{
	label_board_widget = new LabelBoard();
	label_board_tool_widget = new LabelBoardToolWidget();
	label_board_tool_widget->setObjectName("label_board_tool_wodget");
	label_board_tool_widget->setLabelBoardWidget(label_board_widget);
	main_layout = new QVBoxLayout(this);
	main_layout->addWidget(label_board_tool_widget);
	main_layout->addWidget(label_board_widget);
	setLayout(main_layout);
}

LabelBoard* LabelBoardWithTool::getLabelBoardWidget() const
{
	return label_board_widget;
}

LabelBoardToolWidget* LabelBoardWithTool::getLabelBoardToolWidget() const
{
	return label_board_tool_widget;
}
