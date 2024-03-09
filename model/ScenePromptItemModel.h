#pragma once
#include <opencv2/opencv.hpp>
#include <QObject>
class GraphicsScene;
class GraphicsItem;
class SamWidget;

enum class MaskToItemType
{
	MaskToPolygon,
	MaskToRect,
};

class ScenePromptItemModel : public QObject
{
	Q_OBJECT
public:
	ScenePromptItemModel();
	~ScenePromptItemModel();
	void setSamWidget(SamWidget* s);

	void setGraphicsScene(GraphicsScene*);
	QList<GraphicsItem*> getPromptItemList();
	bool getIsAiSegment();
	void addPromptItem(GraphicsItem* item); //添加模型提示图元
	void generateAnnotation();
	void Mask2Item();

	cv::Mat getMask(bool clone);
	void clearMask();
	void removeAllPromptsItems(); //移除所有提示图元

public slots:
	void onLoadModelBtn();
	void onDeleteAllPromptItemBtn();

private:
	void loadSamModel();
	void unloadSamModel();
	bool loadImage(const QString& image_path);
	void segmentAnything();
	void mask2Rect(const cv::Mat& mask);
	void mask2Polygon(const cv::Mat& mask);
	void removeItemFromPromptList(); //从模型提示图元列表移除
	GraphicsScene* m_scene = nullptr;
	SamWidget* sam_widget = nullptr;

	QList<GraphicsItem*> prompt_list;//model used prompt item list
	std::list<cv::Point> positive_points = {};
	std::list<cv::Point> negative_points = {};
	MaskToItemType MASK2ITEM_TYPE = MaskToItemType::MaskToPolygon; //掩码生成item的类型
	bool start_ai_segment = false; //使用模型自动分割
	bool is_load_model = false;
	bool is_load_image = false;
	QString load_image_path = "";
	cv::Mat mask = {};
};
