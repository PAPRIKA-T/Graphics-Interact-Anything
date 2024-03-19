#pragma once
#include <opencv2/opencv.hpp>
#include <QObject>
class GraphicsScene;
class GraphicsItem;
class AiModelInteractWidget;
class Sam;
class GiantImageItem;
class GiantMaskItem;

enum class MaskToItemType
{
	MaskToImg,
	MaskToPolygon,
	MaskToRect,
};

class ScenePromptItemModel : public QObject
{
	Q_OBJECT
public:
	ScenePromptItemModel();
	~ScenePromptItemModel();
	void setSamInteractWidget(AiModelInteractWidget* s);
	void setSam(Sam*);
	void setGraphicsScene(GraphicsScene*);
	QList<GraphicsItem*> getPromptItemList();
	void addPromptItem(GraphicsItem* item); //添加模型提示图元
	void generateAnnotation();
	void Mask2Item();

	void clearMask();
	void removeAllPromptsItems(); //移除所有提示图元

public slots:
	void onDeleteAllPromptItemBtn();
	void acceptMaskItem();

private:
	struct SamPromptItems {
		std::list<cv::Point> positive_points{};
		std::list<cv::Point> negative_points{};
		cv::Rect box_prompt = {};
	};
	void initMaskItem();
	bool loadImage(const QString& image_path);
	void segmentAnything();
	void mask2Rect(const cv::Mat& mask);
	void mask2Polygon(const cv::Mat& mask);
	void generateGiantMaskItem(const cv::Mat& mask);
	void removeItemFromPromptList(); //从模型提示图元列表移除
	void getSamPromptItems(QList<GraphicsItem*>&, SamPromptItems&);
	void  clearPromptList();
	GraphicsScene* m_scene = nullptr;
	GiantImageItem* pixmap_item = nullptr;
	Sam* sam = nullptr;
	AiModelInteractWidget* sam_interact_widget = nullptr;
	QList<GraphicsItem*> prompt_list{};//model used prompt item list
	SamPromptItems sam_prompt_items{};
	MaskToItemType MASK2ITEM_TYPE = MaskToItemType::MaskToPolygon; //掩码生成item的类型
	bool is_load_image = false;
	QString load_image_path = "";
	
	cv::Mat mask{};
	cv::Size input_size{};
	GiantMaskItem* current_mask_item = nullptr;
	QList<GiantMaskItem*> mask_item_list{};
};
