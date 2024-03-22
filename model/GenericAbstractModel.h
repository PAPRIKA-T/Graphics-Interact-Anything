#pragma once
class GiantImageItem;
class GraphicsScene;

class GenericAbstractModel
{
public:
	GenericAbstractModel();
	~GenericAbstractModel();
	void setGraphicsScene(GraphicsScene*);

protected:
	GraphicsScene* m_scene = nullptr;
	GiantImageItem* pixmap_item = nullptr;
};

