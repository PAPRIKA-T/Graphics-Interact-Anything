#pragma once

#include <QWidget>

class GenericToolButton;
class LabelBoard;
class QHBoxLayout;

class LabelBoardToolWidget  : public QWidget
{
	Q_OBJECT
public:
	LabelBoardToolWidget(QWidget*parent = nullptr);
	~LabelBoardToolWidget();
	void setLabelBoardWidget(LabelBoard*);
	virtual void paintEvent(QPaintEvent* event) override;
private:
	QHBoxLayout* main_layout = nullptr;
	GenericToolButton* board_add_row_btn = nullptr;
	GenericToolButton* board_remove_row_btn = nullptr;
	GenericToolButton* read_label_btn = nullptr;
	GenericToolButton* save_label_btn = nullptr;
	LabelBoard* label_board = nullptr;
};
