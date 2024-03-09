#include "GraphicsItemIoParameter.h"
#include "Graphicsitem.h"

void GraphicsItemIoParameter::setGraphicsItemIoParameter(GraphicsItem* item)
{
	item->getGraphicsTextModel().setLabelText(text);
	item->getGraphicsTextModel().setLabelID(ID);
	item->setName(name);
	item->setGraphicsColor(QColor::fromString(color));
	item->setRotateAngle(rotate_angle);

}
