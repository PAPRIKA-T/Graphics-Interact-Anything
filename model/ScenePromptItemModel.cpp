#include "ScenePromptItemModel.h"
#include "graphs/Graphicsscene.h"
#include "graphs/GiantMaskItem.h"
#include "graphs/GraphicsPolygon.h"
#include "graphs/GraphicsLineSegment.h"
#include "widgets/LabelBoard.h"
#include "utils/CVOperation.h"
#include "widgets/ComboBoxWidget.h"
#include "widgets/AiModelInteractWidget.h"
#include <QComboBox>
#include <QPushButton>
#include "widgets/SAM/sam.h"
#include "graphs/GraphicsView.h"

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
    connect(sam_interact_widget->getAcceptBtn(), &QPushButton::clicked,
        		this, &ScenePromptItemModel::acceptMaskItem);
}

void ScenePromptItemModel::setSam(Sam* s)
{
    sam = s;
    input_size = sam->getInputSize();
}

void ScenePromptItemModel::setGraphicsScene(GraphicsScene* s)
{
	m_scene = s;
    pixmap_item = m_scene->getPixmapItem();
}

QList<GraphicsItem*> ScenePromptItemModel::getPromptItemList()
{
    return prompt_list;
}

void ScenePromptItemModel::onDeleteAllPromptItemBtn()
{
    removeAllPromptsItems();
}

void ScenePromptItemModel::acceptMaskItem()
{
    if (prompt_list.size() <= 1)return;
    current_mask_item->acceptMask();
    m_scene->addMaskItem(current_mask_item);
    initMaskItem(true);
    removeAllPromptsItems();
}

void ScenePromptItemModel::initMaskItem(bool ok)
{
    if (ok) {
        current_mask_item = new GiantMaskItem();
        current_mask_item->setImageSize(pixmap_item->getFscaleSize(), pixmap_item->getOriginSize());
        current_mask_item->setParentItem(pixmap_item);
        m_scene->addMaskItem(current_mask_item);
    }
    else {
        removeAllPromptsItems();
        clearMask();
        delete current_mask_item;
        current_mask_item = nullptr;
    }
}

bool ScenePromptItemModel::loadImage(const QString& image_path)
{
    if (!sam)return false;
    cv::Mat load_image = pixmap_item->getOrignImageMat(false);
    if (load_image.channels() != 3) {
        if(load_image.channels() == 4)cvtColor(load_image, load_image, cv::COLOR_BGRA2BGR);
        else if(load_image.channels() == 1) cvtColor(load_image, load_image, cv::COLOR_GRAY2BGR);
        else qDebug() << "ScenePromptItemModel::loadImage can ont convert image channel to 3" << load_image.channels();
    }
    cv::resize(load_image, load_image, input_size);
    if (sam->loadImage(load_image)) {
        is_load_image = true;
        load_image_path = image_path;
        current_mask_item->setImageSize(pixmap_item->getFscaleSize(), pixmap_item->getOriginSize());
        return true;
    }
    else {
        is_load_image = false;
        qDebug() << "ScenePromptItemModel::loadImage load image fail";
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
    if (m_scene->getPixmapItem()->getImagePath() == "") {
        qDebug() << "SamWidget::no input image";
        return;
    }
    if (pixmap_item->getImagePath() != load_image_path)
    {
        QString path = pixmap_item->getImagePath();
        loadImage(path);
    }
    cv::Mat maskAuto = sam->autoSegment({ 10, 10 });

    cv::normalize(maskAuto, maskAuto, 0, 255, cv::NORM_MINMAX, CV_8UC1);
    maskAuto.convertTo(maskAuto, CV_8UC1);

    QSize origin_size = pixmap_item->getShowImage().size();
    cv::Size cv_origin_size = { origin_size.width(),origin_size.height() };
    cv::resize(maskAuto, maskAuto, cv_origin_size);

    cv::Mat org_image = CVOperation::setMaskOnImage(m_scene->getPixmapItem()->getOrignImageMat(true), maskAuto);
    QImage mask_image = CVOperation::cvMat2QImage(org_image);
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

void ScenePromptItemModel::generateGiantMaskItem(const cv::Mat& mask)
{
    QColor c = m_scene->getLabelBoardWidget()->getSelectedColor();
    current_mask_item->setColor(c);
    cv::bitwise_not(mask, mask);
    current_mask_item->setMask(QBitmap::fromImage(CVOperation::cvMat2QImage(mask)));
}

void ScenePromptItemModel::removeAllPromptsItems()
{
    clearMask();
    if (prompt_list.size() == 0) return;
    foreach(GraphicsItem * prompt_item, prompt_list){
        prompt_item->onActionRemoveSelf();
        if(prompt_item->scene() == m_scene)m_scene->removeItem(prompt_item);
    }
    emit m_scene->promptContinue();
}

void ScenePromptItemModel::generateAnnotation()
{
    if (!sam)return;
    if (!pixmap_item->getIsLoadImageAllData()) return;

    QSize fscale_size = pixmap_item->getFscaleSize(); //返回的是原始图像的尺寸
    if (fscale_size.isEmpty())return;

    cv::Size cv_fscale_size = { fscale_size.width(),fscale_size.height() };
    clearPromptList();
    getSamPromptItems(prompt_list, sam_prompt_items);
    if (sam_prompt_items.isEmpty()) return;

    QString image_path = pixmap_item->getImagePath();
    if (pixmap_item->getImagePath() != load_image_path) {
        if(!loadImage(image_path))return;
    }
    mask = sam->getMask(sam_prompt_items.positive_points, sam_prompt_items.negative_points, sam_prompt_items.box_prompt);
    cv::resize(mask, mask, cv_fscale_size);
    generateGiantMaskItem(mask);
}

void ScenePromptItemModel::Mask2Item()
{
    if (mask.empty())return;
    int output_shape = sam_interact_widget->getOutputShapeWidget()->getComboBox()->currentIndex();
    if (output_shape == 1) {
        MASK2ITEM_TYPE = MaskToItemType::MaskToPolygon;
        mask2Polygon(mask);
    }
    else if (output_shape == 2) {
        MASK2ITEM_TYPE = MaskToItemType::MaskToRect;
        mask2Rect(mask);
    }
}

void ScenePromptItemModel::clearMask()
{
    mask = {};
    if (!current_mask_item)return;
    current_mask_item->resetMask();
}

void ScenePromptItemModel::setSamModelInteraction(bool ok)
{
    initMaskItem(ok);
}

void ScenePromptItemModel::removeItemFromPromptList()
{
    GraphicsItem* self = dynamic_cast<GraphicsItem*>(sender());
    if(!self)return;
    prompt_list.removeOne(self);
    if (prompt_list.size() == 0) {
        clearMask();
    }
}

void ScenePromptItemModel::getSamPromptItems(QList<GraphicsItem*>& l, SamPromptItems& s)
{
    QSize origin_size = pixmap_item->getShowImage().size(); //返回的是原始图像的尺寸
    if (origin_size.isEmpty())return;
    cv::Size cv_origin_size = { origin_size.width(),origin_size.height() };

    foreach(GraphicsItem * prompt_item, l) {
        if (prompt_item->data(1) == "PositivePoint") {
            QPointF p = prompt_item->getCenterMeasurePos();
            qreal p_x_convert = (p.x() * input_size.width) / cv_origin_size.width;
            qreal p_y_convert = (p.y() * input_size.height) / cv_origin_size.height;
            cv::Point cvPoint(p_x_convert, p_y_convert);
            s.positive_points.push_back(cvPoint);
        }
        else if (prompt_item->data(1) == "NegativePoint") {
            QPointF p = prompt_item->getCenterMeasurePos();
            qreal p_x_convert = (p.x() * input_size.width) / cv_origin_size.width;
            qreal p_y_convert = (p.y() * input_size.height) / cv_origin_size.height;
            cv::Point cvPoint(p_x_convert, p_y_convert);
            s.negative_points.push_back(cvPoint);
        }
        else if (prompt_item->data(1) == "PPLine") {
            LineSegment* lineseg_item = dynamic_cast<LineSegment*>(prompt_item);
            QPointF p{};
            qreal scale = pixmap_item->getOriginWidth() / (pixmap_item->getFscaleW() + EPS);
            QPointF img_scale_p(scale, scale);
            for (PolygonPoint* p_item : lineseg_item->getPointItemList()) {
                p = QpointFMultiplication(p_item->mapToItem(pixmap_item, p_item->getCenter()), img_scale_p);
                qreal p_x_convert = (p.x() * input_size.width) / cv_origin_size.width;
                qreal p_y_convert = (p.y() * input_size.height) / cv_origin_size.height;
                cv::Point cvPoint(p_x_convert, p_y_convert);
                s.positive_points.push_back(cvPoint);
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
                s.negative_points.push_back(cvPoint);
            }
        }
        else if (prompt_item->data(1) == "PromptRect") {
            QPointF p_s = prompt_item->getStartMeasurePos();
            QPointF p_e = prompt_item->getEdgeMeasurePos();
            if (p_s.rx() == p_e.rx() || p_s.ry() == p_e.ry()) {
                sam_prompt_items.box_prompt = cv::Rect{};
                continue;
            }
            double ps_x_convert = (p_s.x() * input_size.width) / cv_origin_size.width;
            double ps_y_convert = (p_s.y() * input_size.height) / cv_origin_size.height;
            double pe_x_convert = (p_e.x() * input_size.width) / cv_origin_size.width;
            double pe_y_convert = (p_e.y() * input_size.height) / cv_origin_size.height;
            cv::Point cvPSoint(ps_x_convert, ps_y_convert);
            cv::Point cvPEoint(pe_x_convert, pe_y_convert);
            s.box_prompt = cv::Rect{ cvPSoint, cvPEoint };
        }
    }
}

void ScenePromptItemModel::clearPromptList()
{
    sam_prompt_items.positive_points.clear();
    sam_prompt_items.negative_points.clear();
    sam_prompt_items.box_prompt = cv::Rect{};
}

void ScenePromptItemModel::addPromptItem(GraphicsItem* item)
{
    prompt_list.push_back(item);
    connect(item, &GraphicsItem::prepareToRemove,
        this, &ScenePromptItemModel::removeItemFromPromptList);
}

bool ScenePromptItemModel::SamPromptItems::isEmpty()
{
    if (positive_points.empty() && box_prompt.empty() && negative_points.empty()) return true;
    else return false;
}
