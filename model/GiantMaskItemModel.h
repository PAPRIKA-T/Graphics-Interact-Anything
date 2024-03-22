#pragma once
#include <QObject>
#include "GenericAbstractModel.h"
#include "graphs/GiantMaskItem.h"

class GiantMaskItemModel : public QObject, public GenericAbstractModel
{
	Q_OBJECT
public:
	GiantMaskItemModel();
	~GiantMaskItemModel();
	GiantMaskItem* getForegroundMaskItem();
	QList<GiantMaskItem*> getMaskItemList() const;
	void initForegroundMaskItemImageSetting();
	void applyForegroundMask2Label();
	void resetMaskItemList(); //重置mask图元列表
	void setGraphicsScene(GraphicsScene*);

public slots:
	void insertMaskItem(int, const QColor&);
	void removeMaskItem(int);
	void receiveSelectedLabelBoardRowColor(int, const QColor&); //接收labelBoard选中行颜色
	void clearMaskItemList(); //清除mask图元列表
private:
	void initForegroundMaskItem(); //初始化前景图层
	void initMaskItemList(); //初始化mask图元列表
	QList<GiantMaskItem*> mask_item_list{}; //mask图元列表
	GiantMaskItem* foreground_mask_item = nullptr; //前景图层

	int current_label_index{}; //当前选中label索引
};

