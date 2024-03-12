#pragma once

#include <QWidget>
class GraphicsView;
class GraphicsScene;
class ViewToolBar;
class GiantInteractionModeWidget;

class ImageSceneWidget2D  : public QWidget
{
	Q_OBJECT

public:
	ImageSceneWidget2D(QWidget *parent);
	~ImageSceneWidget2D();
	ViewToolBar* getViewToolBar() const;
	GiantInteractionModeWidget* getGiantInteractionModeWidget() const;
	GraphicsView* getGraphicsView() const;
	GraphicsScene* getGraphicsScene() const;
	void updateViewGeometry();
protected:
	virtual void resizeEvent(QResizeEvent* event) override;
	virtual void paintEvent(QPaintEvent* event) override;
private:
	GraphicsView* m_view = nullptr;
};
