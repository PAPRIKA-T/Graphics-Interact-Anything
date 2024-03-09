#pragma once

#include <QWidget>

class GenericToolButton;
class LabelBoardWidget;
class QHBoxLayout;

class LabelBoardToolWidget  : public QWidget
{
	Q_OBJECT
public:
	LabelBoardToolWidget(QWidget*parent = nullptr);
	~LabelBoardToolWidget();
	void setLabelBoardWidget(LabelBoardWidget*);
	virtual void paintEvent(QPaintEvent* event) override;
private:
	QHBoxLayout* main_layout = nullptr;
	GenericToolButton* board_add_row_btn = nullptr;
	GenericToolButton* board_remove_row_btn = nullptr;
	GenericToolButton* read_label_btn = nullptr;
	GenericToolButton* save_label_btn = nullptr;
	LabelBoardWidget* label_board = nullptr;
};
