#pragma once
#include <QWidget>
class QHBoxLayout;
class QSlider;
class QLabel;
class GraphicsView;

class SprayModeWidget : public QWidget
{
public:
	SprayModeWidget(QWidget* parent = nullptr);
	~SprayModeWidget();
	void setGraphicsView(GraphicsView*);
private:
	QHBoxLayout* main_layout{ nullptr };
	QSlider* spray_radius_slider{};
	QLabel* spray_radius_label{};
};

