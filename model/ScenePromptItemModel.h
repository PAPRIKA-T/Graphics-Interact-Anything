#pragma once
#include <opencv2/opencv.hpp>
#include <QObject>
#include "GenericAbstractModel.h"

class GraphicsItem;
class AiModelInteractWidget;
class Sam;

enum class MaskToItemType
{
	MaskNoConvert,
	MaskToPolygon,
	MaskToRect,
};

class ScenePromptItemModel : public QObject, public GenericAbstractModel
{
	Q_OBJECT
public:
	ScenePromptItemModel();
	~ScenePromptItemModel();
	void setSamInteractWidget(AiModelInteractWidget* s);
	void setSam(Sam*);
	QList<GraphicsItem*> getPromptItemList();
	void addPromptItem(GraphicsItem* item); //添加模型提示图元
	void generateAnnotation();
	void Mask2Item();
	void setSamModelInteraction(bool ok);
	void clearMask(); //清除掩码

public slots:
	void onDeleteAllPromptItemBtn();
	void acceptMaskItem();

private:
	struct SamPromptItems {
		std::list<cv::Point> positive_points{};
		std::list<cv::Point> negative_points{};
		cv::Rect box_prompt = {};
		bool isEmpty();
	};
	void initMaskItem(bool); //初始化掩码图元
	bool loadImage(const QString&); //加载图片

	void getSamPromptItems(QList<GraphicsItem*>&, SamPromptItems&); //获取模型提示图元
	void generateGiantMaskItem(const cv::Mat&); //生成掩码图元
	void segmentAnything(); //模型分割

	void removeAllPromptsItems(); //移除所有提示图元
	void removeItemFromPromptList(); //从模型提示图元列表移除
	void clearPromptList(); //清除提示图元列表

	void mask2Rect(const cv::Mat&); //掩码转矩形
	void mask2Polygon(const cv::Mat&); //掩码转多边形

	Sam* sam = nullptr;
	AiModelInteractWidget* sam_interact_widget = nullptr;
	QList<GraphicsItem*> prompt_list{};//model used prompt item list
	SamPromptItems sam_prompt_items{};
	MaskToItemType MASK2ITEM_TYPE = MaskToItemType::MaskNoConvert; //掩码生成item的类型
	cv::Size input_size{};
	bool is_load_image = false;
	QString load_image_path = "";
	
	cv::Mat mask{};
};
