#include "LabelBoardWithTool.h"
#include "LabelBoard.h"
#include "LabelBoardToolWidget.h"
#include <QVBoxLayout>

LabelBoardWithTool::LabelBoardWithTool(QWidget* parent)
	: QWidget(parent)
{
	setObjectName("label_board_with_tool");
	label_board_widget = new LabelBoard();
	label_board_tool_widget = new LabelBoardToolWidget();
	label_board_tool_widget->setObjectName("label_board_tool_wodget");
	label_board_tool_widget->setLabelBoardWidget(label_board_widget);
	main_layout = new QVBoxLayout(this);
	main_layout->addWidget(label_board_tool_widget);
	main_layout->addWidget(label_board_widget);
	main_layout->setSpacing(0);
	main_layout->setContentsMargins(0, 0, 0, 0);
	setLayout(main_layout);
}

LabelBoardWithTool::~LabelBoardWithTool()
{
	delete main_layout;
	delete label_board_widget;
	delete label_board_tool_widget;
}

LabelBoard* LabelBoardWithTool::getLabelBoardWidget() const
{
	return label_board_widget;
}

LabelBoardToolWidget* LabelBoardWithTool::getLabelBoardToolWidget() const
{
	return label_board_tool_widget;
}

void LabelBoardWithTool::readLabelFileFromTxt()
{
	label_board_widget->readLabelFileFromTxt();
}

void LabelBoardWithTool::saveLabelFileToTxt()
{
	label_board_widget->saveLabelFileToTxt();
}