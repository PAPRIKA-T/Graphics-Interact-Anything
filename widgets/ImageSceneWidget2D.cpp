#include "ImageSceneWidget2D.h"
#include "Graphs/Graphicsview.h"
#include "Graphs/Graphicsscene.h"
#include <QVBoxLayout>

ImageSceneWidget2D::ImageSceneWidget2D(QWidget *parent)
	: QWidget(parent)
{
	m_layout = new QVBoxLayout(this);
	m_view = new GraphicsView(this);
	setObjectName("ImageSceneWidget2D");
	//m_layout->addWidget(m_view);
	//m_layout->setContentsMargins(0, 0, 0, 0);
}

ImageSceneWidget2D::~ImageSceneWidget2D()
{
	delete m_view;
	delete m_layout;
}

ViewToolBar* ImageSceneWidget2D::getViewToolBar() const
{
	return m_view->getViewToolBar();
}

GiantInteractionModeWidget* ImageSceneWidget2D::getGiantInteractionModeWidget() const
{
	return m_view->getGiantInteractionModeWidget();
}

GraphicsView* ImageSceneWidget2D::getGraphicsView() const
{
	return m_view;
}

GraphicsScene* ImageSceneWidget2D::getGraphicsScene() const
{
	return m_view->getGraphicsScene();
}

void ImageSceneWidget2D::updateViewGeometry()
{
	m_view->setGeometry(0, 0, width(), height());
}

void ImageSceneWidget2D::resizeEvent(QResizeEvent * event)
{
	QWidget::resizeEvent(event);
	updateViewGeometry();
}

void ImageSceneWidget2D::paintEvent(QPaintEvent* event)
{
	QWidget::paintEvent(event);
	m_view->getGraphicsScene()->updateTextPos();
}
