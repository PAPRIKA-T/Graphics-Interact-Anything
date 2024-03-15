#pragma once
#include <QWidget>
class LabelBoardToolWidget;
class LabelBoard;
class QVBoxLayout;

class LabelBoardWithTool : public QWidget
{
	Q_OBJECT
public:
	LabelBoardWithTool(QWidget* parent = nullptr);
	~LabelBoardWithTool();
	LabelBoard* getLabelBoardWidget() const;
	LabelBoardToolWidget* getLabelBoardToolWidget() const;
private:
	LabelBoard* label_board_widget = nullptr;
	LabelBoardToolWidget* label_board_tool_widget = nullptr;
	QVBoxLayout* main_layout = nullptr;
};

