#include "SprayModeWidget.h"
#include "graphs/GraphicsView.h"
#include <QHBoxLayout>
#include <QSlider>
#include <QLabel>
SprayModeWidget::SprayModeWidget(QWidget* parent)
	:QWidget(parent)
{
	main_layout = new QHBoxLayout(this);
	spray_radius_label = new QLabel("Block Size:");
	spray_radius_slider = new QSlider(Qt::Horizontal);
	spray_radius_slider->setRange(1, 50);
	spray_radius_slider->setFixedWidth(100);
	main_layout->addWidget(spray_radius_label);
	main_layout->addWidget(spray_radius_slider);
	main_layout->addStretch();
	main_layout->setSpacing(6);
	main_layout->setContentsMargins(6, 2, 6, 2);
	setFixedWidth(spray_radius_label->width()+ spray_radius_slider->width()+20);
}

SprayModeWidget::~SprayModeWidget()
{
	delete main_layout;
}

void SprayModeWidget::setGraphicsView(GraphicsView* view)
{
	connect(spray_radius_slider, &QSlider::valueChanged, view, &GraphicsView::setSprayRectSize);
}
