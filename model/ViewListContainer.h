#pragma once
#include "graphs/Graphicsview.h"
#include "graphs/Graphicsscene.h"
class GraphicsView;
class ViewListContainer
{
public:
	ViewListContainer();
	~ViewListContainer();
	GraphicsView* getActivedView() const;
	QList< GraphicsView*> getViewList() const;
	void setActivatdView(GraphicsView*);
	void pushBackView(GraphicsView*);
	void clearViewList();
private:
	GraphicsView* actived_view = nullptr;
	QList<GraphicsView*> view_list;
};
