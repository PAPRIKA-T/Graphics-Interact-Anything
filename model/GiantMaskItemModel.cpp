#include "GiantMaskItemModel.h"
#include "graphs/GraphicsScene.h"
#include "graphs/GiantMaskItem.h"

GiantMaskItemModel::GiantMaskItemModel()
{
}

GiantMaskItemModel::~GiantMaskItemModel()
{
    delete foreground_mask_item;
}

GiantMaskItem* GiantMaskItemModel::getForegroundMaskItem() const
{
    return foreground_mask_item;
}

QList<GiantMaskItem*> GiantMaskItemModel::getMaskItemList() const
{
    return QList<GiantMaskItem*>(mask_item_list);
}

void GiantMaskItemModel::initForegroundMaskItemImageSetting()
{
    foreground_mask_item->setImageShowSize(pixmap_item->getFscaleSize());
}

void GiantMaskItemModel::clearMaskItemList()
{
	foreach(GiantMaskItem * mask, mask_item_list) {
		delete mask;
	}
	mask_item_list.clear();
}

void GiantMaskItemModel::setGraphicsScene(GraphicsScene* s)
{
	GenericAbstractModel::setGraphicsScene(s);
	initForegroundMaskItem();
}

void GiantMaskItemModel::initForegroundMaskItem()
{
    foreground_mask_item = new GiantMaskItem();
    foreground_mask_item->setImageShowSize(pixmap_item->getFscaleSize());
    foreground_mask_item->setParentItem(pixmap_item);
    foreground_mask_item->setMaskOpacity(0.5);
}

void GiantMaskItemModel::applyForegroundMask2Label()
{
	if (foreground_mask_item->getOriginalPixmap().isNull())return;
	if (!mask_item_list.at(2)) {
		qDebug() << "invalid";
	}
}