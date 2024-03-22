#include "GenericAbstractModel.h"
#include "graphs/Graphicsscene.h"

GenericAbstractModel::GenericAbstractModel()
{
}

GenericAbstractModel::~GenericAbstractModel()
{
}

void GenericAbstractModel::setGraphicsScene(GraphicsScene* s)
{
	m_scene = s;
	pixmap_item = m_scene->getPixmapItem();
}
