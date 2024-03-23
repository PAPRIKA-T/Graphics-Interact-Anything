#include "SprayModeWidget.h"
#include "graphs/GraphicsView.h"
#include <QHBoxLayout>
#include <QSlider>
#include <QLabel>
#include <QCheckBox>
SprayModeWidget::SprayModeWidget(QWidget* parent)
	:QWidget(parent)
{
	main_layout = new QHBoxLayout(this);
	spray_radius_label = new QLabel("Block Size:");
	spray_radius_label->setObjectName("view_attach_widget");
	spray_radius_slider = new QSlider(Qt::Horizontal);
	spray_radius_slider->setRange(1, 50);
	spray_radius_slider->setFixedWidth(100);

	cover_label = new QLabel("Cover");
	cover_label->setObjectName("view_attach_widget");
	cover_checkbox = new QCheckBox();

	main_layout->addWidget(spray_radius_label);
	main_layout->addWidget(spray_radius_slider);
	main_layout->addWidget(cover_label);
	main_layout->addWidget(cover_checkbox);
	main_layout->addStretch();
	main_layout->setSpacing(6);
	main_layout->setContentsMargins(6, 2, 6, 2);
	setFixedWidth(270);
}

SprayModeWidget::~SprayModeWidget()
{
	delete main_layout;
}

void SprayModeWidget::setGraphicsView(GraphicsView* view)
{
	connect(spray_radius_slider, &QSlider::valueChanged, view, &GraphicsView::setSprayRectSize);
}

QCheckBox* SprayModeWidget::getCoverCheckBox() const
{
	return cover_checkbox;
}
