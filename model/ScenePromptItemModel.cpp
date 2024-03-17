#include "ScenePromptItemModel.h"
#include "graphs/Graphicsscene.h"
#include "graphs/Graphicspixmapitem.h"
#include "graphs/GraphicsPolygon.h"
#include "graphs/GraphicsLineSegment.h"
#include "widgets/LabelBoard.h"
#include "utils/CVOperation.h"
#include "widgets/ComboBoxWidget.h"
#include "widgets/AiModelInteractWidget.h"
#include <QComboBox>
#include "widgets/SAM/sam.h"

#define EPS 1e-5

ScenePromptItemModel::ScenePromptItemModel()
{
	
}

ScenePromptItemModel::~ScenePromptItemModel()
{

}

void ScenePromptItemModel::setSamInteractWidget(AiModelInteractWidget* s)
{
    sam_interact_widget = s;
}

void ScenePromptItemModel::setSam(Sam* s)
{
    sam = s;
    input_size = sam->getInputSize();
}

void ScenePromptItemModel::setGraphicsScene(GraphicsScene* s)
{
	m_scene = s;
}

QList<GraphicsItem*> ScenePromptItemModel::getPromptItemList()
{
    return prompt_list;
}

void ScenePromptItemModel::onDeleteAllPromptItemBtn()
{
    removeAllPromptsItems();
}

bool ScenePromptItemModel::loadImage(const QString& image_path)
{
    if (!sam)return false;
    cv::Mat load_image{};
    cv::resize(m_scene->getPixmapItem()->getOrignImageMat(false), load_image, input_size);
    if (sam->loadImage(load_image)) {
        is_load_image = true;
        load_image_path = image_path;
        return true;
    }
    else {
        qDebug() << "SamWidget::loadImage load image fail";
        return false;
    }
}

void ScenePromptItemModel::segmentAnything()
{
    if (!m_scene) {
        qDebug() << "SamWidget::no scene";
        return;
    }
    if (!sam) {
        qDebug() << "SamWidget::no load model";
        return;
    }
    if (m_scene->getPixmapItem()->getPixmapPath() == "") {
        qDebug() << "SamWidget::no input image";
        return;
    }
    if (m_scene->getPixmapItem()->getPixmapPath() != load_image_path)
    {
        QString path = m_scene->getPixmapItem()->getPixmapPath();
        loadImage(path);
    }
    cv::Mat maskAuto = sam->autoSegment({ 10, 10 });

    cv::normalize(maskAuto, maskAuto, 0, 255, cv::NORM_MINMAX, CV_8UC1);
    maskAuto.convertTo(maskAuto, CV_8UC1);

    QSize origin_size = m_scene->getPixmapItem()->getShowImage().size();
    cv::Size cv_origin_size = { origin_size.width(),origin_size.height() };
    cv::resize(maskAuto, maskAuto, cv_origin_size);

    cv::Mat org_image = CVOperation::getAnnotation(m_scene->getPixmapItem()->getOrignImageMat(true), maskAuto);
    QImage mask_image = CVOperation::matToQImage(org_image);
    mask = maskAuto;
    m_scene->getPixmapItem()->updateShowImage(mask_image);
}

void ScenePromptItemModel::mask2Rect(const cv::Mat& mask)
{

}

void ScenePromptItemModel::mask2Polygon(const cv::Mat& mask)
{
    // 2. 使用轮廓检测来查找多边形的轮廓
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    // 找到面积最大的轮廓
    double max_area = -1;
    int max_area_index = -1;
    for (int i = 0; i < contours.size(); i++) {
        double area = cv::contourArea(contours[i]);
        if (area > max_area) {
            max_area = area;
            max_area_index = i;
        }
    }
    std::vector<cv::Point> approx; //save the polygon points
    if (max_area_index >= 0) {
        cv::approxPolyDP(contours[max_area_index], approx, 0.01 * cv::arcLength(contours[max_area_index], true), true);
    }

    InteractionPolygon* painting_pol_item = new InteractionPolygon(approx, m_scene->getPixmapItem(),
        m_scene->getPixmapItem()->getSceneCompareOriginScale());
    m_scene->addItemAcceptLabelBoardSetting(painting_pol_item);
    m_scene->addItemInit(painting_pol_item);
    removeAllPromptsItems();
}


void ScenePromptItemModel::mask2img(const cv::Mat& mask)
{
    QElapsedTimer timer;
    timer.start();

    GraphicsPixmapItem* pixmap_item = m_scene->getPixmapItem();
    QColor c = m_scene->getLabelBoardWidget()->getSelectedColor();
    QImage mask_image = CVOperation::getAnnotation(pixmap_item->getOriginalImage(), mask, c, false);
    pixmap_item->updateShowImage(mask_image);
}

void ScenePromptItemModel::removeAllPromptsItems()
{
    clearMask();
    if (prompt_list.size() == 0) return;
    foreach(GraphicsItem * prompt_item, prompt_list){
        prompt_item->onActionRemoveSelf();
    }
    emit m_scene->promptContinue();
}

void ScenePromptItemModel::generateAnnotation()
{
    QElapsedTimer timer;
    timer.start();

    if (!sam)return;
    clearPromptList();
    QSize origin_size = m_scene->getPixmapItem()->getPixmap().size(); //返回的是原始图像的尺寸
    cv::Size cv_origin_size = { origin_size.width(),origin_size.height() };
    cv::Rect box_prompt = {};
    GraphicsPixmapItem* pixmap_item = m_scene->getPixmapItem();
    foreach(GraphicsItem * prompt_item, prompt_list) {
        if (prompt_item->data(1) == "PositivePoint") {
            QPointF p = prompt_item->getCenterMeasurePos();
            qreal p_x_convert = (p.x() * input_size.width) / cv_origin_size.width;
            qreal p_y_convert = (p.y() * input_size.height) / cv_origin_size.height;
            cv::Point cvPoint(p_x_convert, p_y_convert);
            positive_points.push_back(cvPoint);
        }
        else if (prompt_item->data(1) == "NegativePoint") {
            QPointF p = prompt_item->getCenterMeasurePos();
            qreal p_x_convert = (p.x() * input_size.width) / cv_origin_size.width;
            qreal p_y_convert = (p.y() * input_size.height) / cv_origin_size.height;
            cv::Point cvPoint(p_x_convert, p_y_convert);
            negative_points.push_back(cvPoint);
        }
        else if (prompt_item->data(1) == "PPLine") {
            LineSegment* lineseg_item = dynamic_cast<LineSegment*>(prompt_item);
            QPointF p{};
            qreal scale = m_scene->getPixmapItem()->getOriginWidth() / (pixmap_item->getFscaleW() + EPS);
            QPointF img_scale_p(scale, scale);
            for (PolygonPoint* p_item : lineseg_item->getPointItemList()) {
                p = QpointFMultiplication(p_item->mapToItem(pixmap_item, p_item->getCenter()), img_scale_p);
                qreal p_x_convert = (p.x() * input_size.width) / cv_origin_size.width;
                qreal p_y_convert = (p.y() * input_size.height) / cv_origin_size.height;
                cv::Point cvPoint(p_x_convert, p_y_convert);
                positive_points.push_back(cvPoint);
            }
        }
        else if (prompt_item->data(1) == "NPLine") {
            LineSegment* lineseg_item = dynamic_cast<LineSegment*>(prompt_item);
            QPointF p{};
            qreal scale = pixmap_item->getOriginWidth() / (pixmap_item->getFscaleW() + EPS);
            QPointF img_scale_p(scale, scale);
            for (BPoint* p_item : lineseg_item->getPointItemList()) {
                p = QpointFMultiplication(p_item->mapToItem(pixmap_item, p_item->getCenter()), img_scale_p);
                qreal p_x_convert = (p.x() * input_size.width) / cv_origin_size.width;
                qreal p_y_convert = (p.y() * input_size.height) / cv_origin_size.height;
                cv::Point cvPoint(p_x_convert, p_y_convert);
                negative_points.push_back(cvPoint);
            }
        }
        else if (prompt_item->data(1) == "PromptRect") {
            QPointF p_s = prompt_item->getStartMeasurePos();
            QPointF p_e = prompt_item->getEdgeMeasurePos();
            double ps_x_convert = (p_s.x() * input_size.width) / cv_origin_size.width;
            double ps_y_convert = (p_s.y() * input_size.height) / cv_origin_size.height;
            double pe_x_convert = (p_e.x() * input_size.width) / cv_origin_size.width;
            double pe_y_convert = (p_e.y() * input_size.height) / cv_origin_size.height;
            cv::Point cvPSoint(ps_x_convert, ps_y_convert);
            cv::Point cvPEoint(pe_x_convert, pe_y_convert);
            box_prompt = cv::Rect{ cvPSoint, cvPEoint };

        }
    }
    QString image_path = pixmap_item->getPixmapPath();
    if (pixmap_item->getPixmapPath() != load_image_path) {
        loadImage(image_path);
    }
    mask = sam->getMask(positive_points, negative_points, box_prompt);
    cv::resize(mask, mask, cv_origin_size);
    mask2img(mask);
    qDebug() << "Elapsed time:" << timer.elapsed() << "milliseconds";
}

void ScenePromptItemModel::Mask2Item()
{
    if (mask.empty())return;
    int output_shape = sam_interact_widget->getOutputShapeWidget()->getComboBox()->currentIndex();
    if (output_shape == 0) {
        MASK2ITEM_TYPE = MaskToItemType::MaskToPolygon;
        mask2Polygon(mask);
    }
    else if (output_shape == 1) {
        MASK2ITEM_TYPE = MaskToItemType::MaskToRect;
        mask2Rect(mask);
    }
}

cv::Mat ScenePromptItemModel::getMask(bool clone)
{
    if(clone) return mask.clone();
    else return mask;
}

void ScenePromptItemModel::clearMask()
{
    mask = {};
}

void ScenePromptItemModel::removeItemFromPromptList()
{
    GraphicsItem* self = dynamic_cast<GraphicsItem*>(sender());
    prompt_list.removeOne(self);
    if (prompt_list.size() == 0) {
        m_scene->getPixmapItem()->showOriginalImage();
        clearMask();
    }
}

void ScenePromptItemModel::clearPromptList()
{
    positive_points.clear();
    negative_points.clear();
}

void ScenePromptItemModel::addPromptItem(GraphicsItem* item)
{
    prompt_list.append(item);
    connect(item, &GraphicsItem::prepareToRemove,
        this, &ScenePromptItemModel::removeItemFromPromptList);
}
