#pragma once

#include <QWidget>

class GenericToolButton;
class LabelBoard;
class QHBoxLayout;
class QLabel;
class QCheckBox;

class LabelBoardToolWidget  : public QWidget
{
	Q_OBJECT
public:
	LabelBoardToolWidget(QWidget*parent = nullptr);
	~LabelBoardToolWidget();
	void setLabelBoardWidget(LabelBoard*);

private:
	QHBoxLayout* main_layout = nullptr;
	QLabel* auto_nextline_label = nullptr;
	QCheckBox* auto_nextline_checkbox = nullptr;

	GenericToolButton* board_add_row_btn = nullptr;
	GenericToolButton* board_remove_row_btn = nullptr;

	LabelBoard* label_board = nullptr;
};
