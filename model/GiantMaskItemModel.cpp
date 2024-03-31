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
	background_mask_item->setImageShowSize(pixmap_item->getFscaleSize(), pixmap_item->getOriginSize());

	for (GiantMaskItem* mask : mask_item_list) {
		mask->setImageShowSize(pixmap_item->getFscaleSize(), pixmap_item->getOriginSize());
	}
	all_color_mask = cv::Mat::zeros(pixmap_item->getFscaleSize().height(), 
		pixmap_item->getFscaleSize().width(), CV_8UC1);

	all_color_image = cv::Mat::zeros(pixmap_item->getFscaleSize().height(),
		pixmap_item->getFscaleSize().width(), CV_8UC3);
}

void GiantMaskItemModel::resetMaskItemList()
{
	for(GiantMaskItem * mask : mask_item_list) {
		mask->resetMask();
	}
	all_color_mask = cv::Mat::zeros(pixmap_item->getFscaleSize().height(),
		pixmap_item->getFscaleSize().width(), CV_8UC1);

	all_color_image = cv::Mat::zeros(pixmap_item->getFscaleSize().height(),
		pixmap_item->getFscaleSize().width(), CV_8UC3);
}

void GiantMaskItemModel::setGraphicsScene(GraphicsScene* s)
{
	GenericAbstractModel::setGraphicsScene(s);
	initForegroundMaskItem();
	initMaskItemList();
}

void GiantMaskItemModel::applyMaskRangeToLabel(const cv::Mat& m)
{
	//if(is_cover_label)
	if (current_label_index == 0) {
		int s = mask_item_list.size();
		for (int i = 1; i < s; ++i) {
			mask_item_list[i]->applyMaskRangeToLabel(m.clone(), false);
		}
		cv::bitwise_not(m, m);
		cv::bitwise_and(m, all_color_mask, all_color_mask); 
		return;
	};
	mask_item_list[current_label_index]->applyMaskRangeToLabel(m);
	cv::bitwise_or(m, all_color_mask, all_color_mask);
}

void GiantMaskItemModel::getAvaliableRange(const cv::Mat& src ,cv::Mat& ar) const
{
	if (is_cover_label || current_label_index == 0) {
		ar = src; return;
	}
	cv::bitwise_not(all_color_mask, ar);
	cv::bitwise_and(ar, src, ar);
}

void GiantMaskItemModel::removeMaskItemLabel(GiantMaskItem* m)
{
	cv::Mat convert_m{};
	cv::bitwise_not(m->getScaledMask(), convert_m);
	cv::bitwise_and(all_color_mask, convert_m, all_color_mask);
}

void GiantMaskItemModel::onClearMaskItemList()
{
	for (GiantMaskItem* mask : mask_item_list) {
		mask->onDeleteSelf();
	}
	mask_item_list.clear();
}

void GiantMaskItemModel::onCoverCheckBoxChecked(int i)
{
	is_cover_label = i;
}

void GiantMaskItemModel::onInsertMaskItem(int i, const QColor& c)
{
	if (i > mask_item_list.size())return;
	GiantMaskItem* m = new GiantMaskItem();
	mask_item_list.insert(i, m);
	m->setParentItem(pixmap_item);
	m->setColor(c);
	m->setImageShowSize(pixmap_item->getFscaleSize(), pixmap_item->getOriginSize());
	connect(m, &GiantMaskItem::preparedToDelete, [=]() {
		removeMaskItemLabel(m);
		});
}

void GiantMaskItemModel::onRemoveMaskItem(int i, int r)
{
	if (i > mask_item_list.size()-1)return;
	mask_item_list.at(i)->onDeleteSelf();
	mask_item_list.removeAt(i);
	current_label_index = r;
}

void GiantMaskItemModel::receiveSelectedLabelBoardRowColor(int r, const QColor& c)
{
	current_label_index = r;
	mask_item_list.at(r)->setColor(c);
	foreground_mask_item->setColor(c);
	background_mask_item->setColor(c);
	mask_item_list.at(r)->update();
}

void GiantMaskItemModel::initForegroundMaskItem()
{
	foreground_mask_item = new GiantMaskItem();
    foreground_mask_item->setImageShowSize(pixmap_item->getFscaleSize(), pixmap_item->getOriginSize());
    foreground_mask_item->setParentItem(pixmap_item);
	foreground_mask_item->setZValue(2);
	foreground_mask_item->setColor(DEFAULT_COLOR_ITEM);
    foreground_mask_item->setMaskOpacity(0.4);

	background_mask_item = new GiantMaskItem();
	background_mask_item->setImageShowSize(pixmap_item->getFscaleSize(), pixmap_item->getOriginSize());
	background_mask_item->setParentItem(pixmap_item);
	background_mask_item->setZValue(0);
	background_mask_item->setColor(DEFAULT_COLOR_ITEM);
	background_mask_item->setMaskOpacity(0.6);
}

void GiantMaskItemModel::initMaskItemList()
{
	int label_count = m_scene->getLabelBoardWidget()->rowCount();
	for (int i = 0; i < label_count; ++i) {
		GiantMaskItem* mask_item = new GiantMaskItem{};
		mask_item->setParentItem(pixmap_item);
		mask_item_list.push_back(mask_item);
		mask_item->setColor(m_scene->getLabelBoardWidget()->getRowColor(i));
		connect(mask_item, &GiantMaskItem::preparedToDelete, [=]() {
			removeMaskItemLabel(mask_item);
			});
	}
}

void GiantMaskItemModel::applyForegroundMask2Label()
{
	if (foreground_mask_item->getOriginalPixmap().isNull())return;
	if (!mask_item_list.at(current_label_index)) {
		qDebug() << "GiantMaskItemModel::applyForegroundMask2Label() invalid index";
	}
	else {
		cv::Mat avaliable_mask{};
		getAvaliableRange(foreground_mask_item->getScaledMask(), avaliable_mask);
		applyMaskRangeToLabel(avaliable_mask);
	}
}

void GiantMaskItemModel::applySparyRect2Label(const QRect&r)
{
	cv::Mat rect_mask{}; cv::Mat avaliable_mask{};
	mask_item_list.at(current_label_index)->getRectMask(r, rect_mask);
	getAvaliableRange(rect_mask, avaliable_mask);
	applyMaskRangeToLabel(avaliable_mask);
}
