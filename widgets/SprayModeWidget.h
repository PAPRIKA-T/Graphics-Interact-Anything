#pragma once
#include <QWidget>
class QHBoxLayout;
class QSlider;
class QLabel;
class GraphicsView;
class QCheckBox;

class SprayModeWidget : public QWidget
{
public:
	SprayModeWidget(QWidget* parent = nullptr);
	~SprayModeWidget();
	void setGraphicsView(GraphicsView*);
	QCheckBox* getCoverCheckBox() const;
private:
	QHBoxLayout* main_layout{ nullptr };
	QSlider* spray_radius_slider{ nullptr };
	QLabel* spray_radius_label{ nullptr };

	QCheckBox* cover_checkbox{ nullptr };
	QLabel* cover_label{ nullptr };
};

