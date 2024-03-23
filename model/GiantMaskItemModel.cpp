#include "GiantMaskItemModel.h"
#include "graphs/GraphicsScene.h"
#include "widgets/LabelBoard.h"

GiantMaskItemModel::GiantMaskItemModel()
{
}

GiantMaskItemModel::~GiantMaskItemModel()
{
}

GiantMaskItem* GiantMaskItemModel::getForegroundMaskItem()
{
    return foreground_mask_item;
}

QList<GiantMaskItem*> GiantMaskItemModel::getMaskItemList() const
{
    return QList<GiantMaskItem*>(mask_item_list);
}

void GiantMaskItemModel::initForegroundMaskItemImageSetting()
{
    foreground_mask_item->setImageShowSize(pixmap_item->getFscaleSize(), pixmap_item->getOriginSize());
	for (GiantMaskItem* mask : mask_item_list) {
		mask->setImageShowSize(pixmap_item->getFscaleSize(), pixmap_item->getOriginSize());
	}
}

void GiantMaskItemModel::resetMaskItemList()
{
	for(GiantMaskItem * mask : mask_item_list) {
		mask->resetMask();
	}
}

void GiantMaskItemModel::setGraphicsScene(GraphicsScene* s)
{
	GenericAbstractModel::setGraphicsScene(s);
	initForegroundMaskItem();
	initMaskItemList();
}

void GiantMaskItemModel::clearMaskItemList()
{
	for (GiantMaskItem* mask : mask_item_list) {
		delete mask;
	}
	mask_item_list.clear();
}

void GiantMaskItemModel::insertMaskItem(int i, const QColor& c)
{
	if (i > mask_item_list.size())return;
	mask_item_list.insert(i, new GiantMaskItem());
	mask_item_list.at(i)->setParentItem(pixmap_item);
	mask_item_list.at(i)->setColor(c);
	mask_item_list.at(i)->setImageShowSize(pixmap_item->getFscaleSize(), pixmap_item->getOriginSize());
}

void GiantMaskItemModel::removeMaskItem(int i)
{
	qDebug() << i;
	if (i > mask_item_list.size()-1)return;
	delete mask_item_list.at(i);
	mask_item_list.removeAt(i);
}

void GiantMaskItemModel::receiveSelectedLabelBoardRowColor(int r, const QColor& c)
{
	current_label_index = r;
	foreground_mask_item->setColor(c);
}

void GiantMaskItemModel::initForegroundMaskItem()
{
	foreground_mask_item = new GiantMaskItem();
    foreground_mask_item->setImageShowSize(pixmap_item->getFscaleSize(), pixmap_item->getOriginSize());
    foreground_mask_item->setParentItem(pixmap_item);
	foreground_mask_item->setColor(DEFAULT_COLOR_ITEM);
    foreground_mask_item->setMaskOpacity(0.5);
}

void GiantMaskItemModel::initMaskItemList()
{
	int label_count = m_scene->getLabelBoardWidget()->rowCount();
	for (int i = 0; i < label_count; ++i) {
		GiantMaskItem* mask_item = new GiantMaskItem{};
		mask_item->setParentItem(pixmap_item);
		mask_item_list.push_back(mask_item);
		mask_item->setColor(m_scene->getLabelBoardWidget()->getRowColor(i));
	}
}

void GiantMaskItemModel::applyForegroundMask2Label()
{
	if (foreground_mask_item->getOriginalPixmap().isNull())return;
	if (!mask_item_list.at(current_label_index)) {
		qDebug() << "GiantMaskItemModel::applyForegroundMask2Label() invalid index";
	}
	else {
		qDebug()<<"GiantMaskItemModel::applyForegroundMask2Label() "<<current_label_index;
		mask_item_list[current_label_index]->addMaskRange(foreground_mask_item->getScaledMask());
	}
}

void GiantMaskItemModel::applySparyRect2Label(const QRect&r)
{
	mask_item_list.at(current_label_index)->addRectRange(r);
}
