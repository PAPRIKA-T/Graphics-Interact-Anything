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
	void applySparyRect2Label(const QRect&); //将喷枪mask应用到label

	void resetMaskItemList(); //重置mask图元列表
	void setGraphicsScene(GraphicsScene*);

public slots:
	void onInsertMaskItem(int, const QColor&);
	void onRemoveMaskItem(int, int);
	void receiveSelectedLabelBoardRowColor(int, const QColor&); //接收labelBoard选中行颜色
	void onClearMaskItemList(); //清除mask图元列表
	void onCoverCheckBoxChecked(int);
private:
	void applyMaskRangeToLabel(const cv::Mat&);
	void initForegroundMaskItem(); //初始化前景图层
	void initMaskItemList(); //初始化mask图元列表
	void getAvaliableRange(const cv::Mat& in, cv::Mat& out) const; //输入mask，输出可覆盖范围

	void removeMaskItemLabel(GiantMaskItem*); //移除mask图元对应的label
	QList<GiantMaskItem*> mask_item_list{}; //mask图元列表
	GiantMaskItem* foreground_mask_item{ nullptr }; //前景图层
	GiantMaskItem* background_mask_item{ nullptr }; //背景图层

	int current_label_index{}; //当前选中label索引
	cv::Mat all_color_mask{}; //所有mask图元合成的mask
	cv::Mat all_color_image{}; //所有mask图元合成的mask对应的图像(3通道)
	bool is_cover_label{ false }; //是否覆盖label
};

