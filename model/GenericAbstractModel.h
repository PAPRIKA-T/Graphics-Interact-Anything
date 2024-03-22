#pragma once
class GiantImageItem;
class GraphicsScene;
class GraphicsView;
class GenericAbstractModel
{
public:
	GenericAbstractModel();
	~GenericAbstractModel();
	void setGraphicsScene(GraphicsScene*);
	void setGraphicsView(GraphicsView*);
protected:
	GraphicsScene* m_scene = nullptr;
	GraphicsView* m_view = nullptr;
	GiantImageItem* pixmap_item = nullptr;
};

