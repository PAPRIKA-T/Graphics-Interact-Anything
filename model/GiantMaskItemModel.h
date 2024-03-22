#pragma once
#include <QList>
#include "GenericAbstractModel.h"

class GiantMaskItem;

class GiantMaskItemModel : public GenericAbstractModel
{
public:
	GiantMaskItemModel();
	~GiantMaskItemModel();
	GiantMaskItem* getForegroundMaskItem() const;
	QList<GiantMaskItem*> getMaskItemList() const;
	void initForegroundMaskItemImageSetting();
	void applyForegroundMask2Label();
	void clearMaskItemList(); //清除mask图元列表
	void setGraphicsScene(GraphicsScene*);

public slots:
	void pushBackMaskItem(const QColor&);
private:
	void initForegroundMaskItem(); //初始化前景图层

	QList<GiantMaskItem*> mask_item_list{}; //mask图元列表
	GiantMaskItem* foreground_mask_item = nullptr; //前景图层
};

