#include "GraphicsRelationModel.h"
#include "graphs/Graphicsitem.h"

GraphicsRelationModel::GraphicsRelationModel()
{
}

GraphicsRelationModel::~GraphicsRelationModel()
{
}

void GraphicsRelationModel::setIsDependWithparent(bool ok)
{
	is_depend_with_parent = ok;
}

bool GraphicsRelationModel::getIsDependWithparent()
{
	return is_depend_with_parent;
}

void GraphicsRelationModel::setUnselectedWhileParentSelected(bool ok)
{
	is_selected_while_parent_select = ok;
}

bool GraphicsRelationModel::getUnselectedWhileParentSelected()
{
	return is_selected_while_parent_select;
}

void GraphicsRelationModel::setHideChildListWhenHoverLeave(bool ok)
{
	is_hide_child_list = ok;
}

void GraphicsRelationModel::setAllChildVisible(bool ok)
{
	for (GraphicsItem* item : child_item_list) {
		item->setVisible(ok);
	}
}

void GraphicsRelationModel::setIsHideChildList(bool ok)
{
	is_hide_child_list = ok;
}

void GraphicsRelationModel::pushBackChildList(GraphicsItem* i)
{
	child_item_list.push_back(i);
}

void GraphicsRelationModel::removeOneOfChildList(GraphicsItem* i)
{
	child_item_list.removeOne(i);
}

bool GraphicsRelationModel::getIsChildConsistentColor()
{
	return is_child_consistent_color;
}

void GraphicsRelationModel::setIsChildConsistentColor(bool ok)
{
	is_child_consistent_color = ok;
}

bool GraphicsRelationModel::getIsHideChildItemList()
{
	return is_hide_child_list;
}

QGraphicsItem* GraphicsRelationModel::findOriginParentItem(QGraphicsItem* child)
{
	if(!child) return nullptr;
	QGraphicsItem* origin_parent = child->parentItem();
	while (origin_parent->parentItem() != nullptr)
		origin_parent = origin_parent->parentItem();
	return origin_parent;
}

QList<GraphicsItem*> GraphicsRelationModel::getChildItemList()
{
	return child_item_list;
}

