#include "GenericInteractionModel.h"
#include "graphs/Graphicsview.h"
#include "graphs/Graphicsscene.h"
#include "graphs/GraphicsPolygon.h"
#include <QCursor>

GenericInteractionModel::GenericInteractionModel()
{
}

void GenericInteractionModel::setGraphicsView(GraphicsView* view)
{
	m_view = view;
}

GenericInteractionModel::InteractionStatus 
GenericInteractionModel::getInteractionStatus()
{
	return interaciton_status;
}

void GenericInteractionModel::setObserveInteraction(bool ok)
{
	if (ok) {
		interaciton_status = InteractionStatus::INTERACTION_OBSERVE;
		m_view->viewport()->setCursor(QCursor(Qt::ArrowCursor));
		m_view->setDragMode(QGraphicsView::NoDrag);
	}
	else {
		m_view->viewport()->unsetCursor();
	}
}

void GenericInteractionModel::setRubberInteraction(bool ok)
{
	if (ok) {
		interaciton_status = InteractionStatus::INTERACTION_RUBBER;
		QPixmap rubber(":/res/qss/GenericStyle/background-image/rubber_mode.png");
		m_view->viewport()->setCursor(QCursor(rubber.scaled(QSize(28, 28)), 0, 18));
		m_view->setDragMode(QGraphicsView::RubberBandDrag);
		foreach(QGraphicsItem * item, m_view->items()) {
			GraphicsItem* new_item = dynamic_cast<GraphicsItem*>(item);
			if (!new_item)continue;
			new_item->setFlag(QGraphicsItem::ItemIsMovable, false);
			if (new_item->data(0) == "polygon") {
				new_item->setFlag(QGraphicsItem::ItemIsSelectable, false);
				new_item->getGraphicsRelationModel().setHideChildListWhenHoverLeave(false);
				new_item->getGraphicsRelationModel().setAllChildVisible(true);
			}
		}
	}
	else {
		m_view->viewport()->unsetCursor();
		foreach(auto * item, m_view->items()) {
			GraphicsItem* new_item = dynamic_cast<GraphicsItem*>(item);
			if (!new_item)continue;
			new_item->setFlag(QGraphicsItem::ItemIsMovable, true);
			if (new_item->data(0) == "polygon") {
				auto* painting_pol_item = dynamic_cast<InteractionPolygon*>(new_item);
				if (painting_pol_item->getIsCreateFinished())painting_pol_item->setFlag(QGraphicsItem::ItemIsSelectable, true);
				if (new_item->data(1) != "LineSegment")painting_pol_item->getGraphicsRelationModel().setHideChildListWhenHoverLeave(true);
			}
		}
	}
}

void GenericInteractionModel::setPaintInteraction(bool ok)
{
	m_view->setPaintCross(ok);
	if (ok) {
		interaciton_status = InteractionStatus::INTERACTION_PAINT;
		m_view->viewport()->setCursor(QCursor(Qt::BlankCursor));
		m_view->setDragMode(QGraphicsView::NoDrag);
	}
	else {
		m_view->viewport()->unsetCursor();
	}
}

void GenericInteractionModel::setSelectInteraction(bool ok)
{
	if (ok) {
		interaciton_status = InteractionStatus::INTERACTION_SELECT;
		m_view->viewport()->setCursor(QCursor(Qt::ArrowCursor));
		m_view->setDragMode(QGraphicsView::RubberBandDrag);
	}
	else {
		m_view->viewport()->unsetCursor();
	}
}

void GenericInteractionModel::setCalculateInteraction(bool ok)
{
	if (ok) {
		interaciton_status = InteractionStatus::INTERACTION_CALCULATE;
		m_view->setDragMode(QGraphicsView::NoDrag);
		m_view->viewport()->setCursor(Qt::PointingHandCursor);
		m_view->getGraphicsScene()->clearSelection();
	}
	else {
		m_view->viewport()->unsetCursor();
	}
}

void GenericInteractionModel::setEditPolygonInteraction(bool ok)
{
	m_view->setPaintCross(ok);
	setPolygonItemEdit(ok);
	if (ok) {
		interaciton_status = InteractionStatus::INTERACTION_EDIT_POLYGON;
		m_view->viewport()->setCursor(QCursor(Qt::BlankCursor));
		m_view->setDragMode(QGraphicsView::NoDrag);
	}
	else {
		m_view->viewport()->unsetCursor();
	}
}

void GenericInteractionModel::setSamModelInteraction(bool ok)
{
	if (ok) {
		interaciton_status = InteractionStatus::INTERACTION_SAM;
		m_view->setDragMode(QGraphicsView::NoDrag);
		m_view->viewport()->setCursor(QCursor(Qt::ArrowCursor));
	}
	else {
		m_view->viewport()->unsetCursor();
		m_view->getGraphicsScene()->initPaintFinishPromptItem();
	}
}

void GenericInteractionModel::setPolygonItemEdit(bool ok)
{
	GraphicsItem* new_item = nullptr;
	InteractionPolygon* pol = nullptr;
	foreach(auto * item, m_view->items()) {
		new_item = dynamic_cast<GraphicsItem*>(item);
		if (!new_item)continue;
		if (new_item->data(0) == "polygon") {
			pol = dynamic_cast<InteractionPolygon*>(new_item);
			pol->setEdit(ok);
		}
	}
}

