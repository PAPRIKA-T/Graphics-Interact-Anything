#include "ViewListContainer.h"

ViewListContainer::ViewListContainer()
{
}

ViewListContainer::~ViewListContainer()
{
}

GraphicsView* ViewListContainer::getActivedView() const
{
	return actived_view;
}

QList<GraphicsView*> ViewListContainer::getViewList() const
{
	return view_list;
}

void ViewListContainer::setActivatdView(GraphicsView* v)
{
	actived_view = v;
}

void ViewListContainer::pushBackView(GraphicsView* v)
{
	view_list.push_back(v);
}

void ViewListContainer::clearViewList()
{
	view_list.clear();
}
