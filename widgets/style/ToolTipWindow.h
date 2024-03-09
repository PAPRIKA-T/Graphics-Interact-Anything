#pragma once

#include <QWidget>

class ToolTipWindow  : public QWidget
{
	Q_OBJECT

public:
	ToolTipWindow(const QString& text, QWidget* parent = nullptr);
	~ToolTipWindow();
};
