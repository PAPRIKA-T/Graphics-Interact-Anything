#include "GraphicsScene.h"
#include "GraphicsView.h"
#include "ThumbnailPixmapItem.h"
#include "widgets/LabelBoard.h"
#include "widgets/ItemIndexView.h"
#include "widgets/StatusWidget.h"
#include "GraphicsTextItem.h"
#include "utils/ColorOperation.h"
#include "AllGraphics.h"
#include <QTimer>
#define EPS (1e-5)

GraphicsScene::GraphicsScene(QWidget *parent)
    :QGraphicsScene (parent)
{
    QImage img;
    pixmap_item = new GraphicsPixmapItem(img);
    thumbnail_item = new ThumbnailPixmapItem(img);
    thumbnail_item->setGraphicsScene(this);
    setItemIndexMethod(QGraphicsScene::NoIndex);
    addItem(pixmap_item);
    addItem(thumbnail_item);
    pixmap_item->setFlag(QGraphicsItem::ItemIsMovable,false);
    initTextItem();
    scene_prompt_model.setGraphicsScene(this);
}

GraphicsScene::~GraphicsScene()
{
    delete text_right_up;
    delete text_right_bottom;
    delete text_left_bottom;
    delete text_left_up;
    delete pixmap_item;
    delete thumbnail_item;
}

void GraphicsScene::setGraphicsView(GraphicsView* v)
{
    m_view = v; 
    updateRtText();
}

GraphicsView* GraphicsScene::getGraphicsView()
{
    return m_view;
}

void GraphicsScene::setItemIndexView(ItemIndexView* i)
{
    item_index_view = i;
}

ItemIndexView* GraphicsScene::getItemIndexView()
{
    return item_index_view;
}

void GraphicsScene::setLabelBoardWidget(LabelBoard* w)
{
    label_board_widget = w;
}

LabelBoard* GraphicsScene::getLabelBoardWidget()
{
    return label_board_widget;
}

ScenePromptItemModel* GraphicsScene::getScenePromptItemModel()
{
    return &scene_prompt_model;
}

bool GraphicsScene::getIsPaintPromptItem()
{
    return is_paint_prompt_item;
}

void GraphicsScene::changeShowImage(const QImage& img)
{
    pixmap_item->setShowImage(img);
    thumbnail_item->setShowImage(pixmap_item->getShowImage());
    m_view->getViewTransFormModel()->setOriginPosition(QPoint((width() - pixmap_item->getFscaleW()) / 2,
        (height() - pixmap_item->getFscaleH()) / 2));
    m_view->getViewTransFormModel()->resetTransform();
}

void GraphicsScene::changeShowImage(const QString& image_path)
{
    pixmap_item->setShowImage(image_path);
    thumbnail_item->setShowImage(pixmap_item->getShowImage());
    m_view->getViewTransFormModel()->setOriginPosition(QPoint((width() - pixmap_item->getFscaleW()) / 2,
        (height() - pixmap_item->getFscaleH()) / 2));
    m_view->getViewTransFormModel()->resetTransform();
}

GraphicsPixmapItem* GraphicsScene::getPixmapItem()
{
    return pixmap_item;
}

ThumbnailPixmapItem* GraphicsScene::getThumbnailItem()
{
    return thumbnail_item;
}

GraphicsTextItem* GraphicsScene::getLeftBottomTextItem()
{
    return text_left_bottom;
}

GraphicsTextItem* GraphicsScene::getLeftUpTextItem()
{
    return text_left_up;
}

GraphicsTextItem* GraphicsScene::getRightBottomTextItem()
{
    return text_right_bottom;
}

GraphicsTextItem* GraphicsScene::getRightUpTextItem()
{
    return text_right_up;
}

void GraphicsScene::initTextItem()
{
    if (text_left_up || text_left_bottom || text_right_bottom || text_right_up) {
        qDebug() << "GraphicsScene::initTextItem has inited";
        return;
    }
    //初始化左上文本item
    text_left_up = new GraphicsTextItem(QPointF(0, 0));
    QFont font = QFont("微软黑体");
    //font.setBold(true);
    font.setPointSize(10);
    text_left_up->setFont(font);
    text_left_up->setDefaultTextColor(Qt::green);
    text_left_up->setTextWidth(-1);
    addItem(text_left_up);
    QString str1 = "X:" + QString::number(0, 'f', 0) + "px ";
    QString str2 = "Y:" + QString::number(0, 'f', 0) + "px";
    text_left_up->setPlainText(str1 + str2);
    text_left_up->setVisible(false);

    //初始化左下文本item
    text_left_bottom = new GraphicsTextItem(QPointF(0,0));
    text_left_bottom->setFont(font);
    text_left_bottom->setDefaultTextColor(Qt::green);
    text_left_bottom->setTextWidth(-1);
    addItem(text_left_bottom);
    text_left_bottom->setPlainText(str1 + str2);
    text_left_bottom->setVisible(false);

    //初始化右下文本item
    text_right_bottom = new GraphicsTextItem(QPointF(0, 0));
    text_right_bottom->setFont(font);
    text_right_bottom->setDefaultTextColor(Qt::green);
    text_right_bottom->setTextWidth(-1);
    addItem(text_right_bottom);
    QString str3 = QString::number(0, 'i', 0) + " of " + QString::number(0, 'i', 0);
    text_right_bottom->setPlainText(str3);

    //初始化右上文本item
    text_right_up = new GraphicsTextItem(QPointF(0, 0));
    text_right_up->setFont(font);
    text_right_up->setDefaultTextColor(DEFAULT_COLOR_POINT_NOSELECTED);
    text_right_up->setTextWidth(-1);
    addItem(text_right_up);
}

void GraphicsScene::updateRbText(int index, int total_index)
{
    QString str1 = QString::number(index, 'i', 0);
    QString str2 = QString::number(total_index, 'i', 0);
    text_right_bottom->setPlainText(str1 + " of " + str2);
}

void GraphicsScene::updateTextPos()
{
    text_left_up->setPos(m_view->mapToScene(0, 0));//左上item
    text_left_bottom->setPos(m_view->mapToScene(0, m_view->height() - text_left_bottom->boundingRect().height()));
    text_right_bottom->setPos(m_view->mapToScene(m_view->width() - text_right_bottom->boundingRect().width(),
        m_view->height() - text_right_bottom->boundingRect().height()));
    text_right_up->setPos(m_view->mapToScene(m_view->width() - text_right_up->boundingRect().width(), 30));

    thumbnail_item->setPos(m_view->mapToScene(3, m_view->height() - thumbnail_item->boundingRect().height() - 5));
}

void GraphicsScene::pixmapItemMoveStart()
{
    image_pos_before_move = pixmap_item->pos();
    foreach(QGraphicsItem * move, items())
    {
        if (isPaintItem(move)) {
            GraphicsItem* m_item = dynamic_cast<GraphicsItem*>(move);
            m_item->getGraphicsTransformModel().setScenePressPos(m_item->pos());
        }
    }
}

void GraphicsScene::pixmapItemMoveBy(const QPointF& delta)
{
    pixmap_item->setPos(image_pos_before_move + delta);
    foreach(QGraphicsItem * move, items()) {
        if (isPaintItem(move)) {
            GraphicsItem* m_item = dynamic_cast<GraphicsItem*>(move);
            m_item->setPos(m_item->getGraphicsTransformModel().getScenePressPos() + delta);
        }
    }
    updateThumbnailBox();
}

void GraphicsScene::createPaintItemAtPoint(const QPointF& p)
{
    if (!painting_item) return;
    if (is_creating_polygon) {
        pushBackPolygonPoint(p);
        if (polygon_list.size() == 1) {
            addItemInitAfterPaint(painting_item);
            is_paint_new_item = false;
            return;
        }
    }
    if (is_paint_new_item) {
        addItemInitAfterPaint(painting_item);
    }
    setPaintItemPoint(p);
}

void GraphicsScene::setPaintItemPoint(const QPointF& p)
{
    if (!painting_item)return;
    for (const auto& function_pair : painting_item->getSetPointFunctionList()) {
        auto class_pointer = function_pair.first;
        auto member_function_pointer = function_pair.second;
        if (member_function_pointer == &GraphicsItem::generateOtherItems) continue;
        (class_pointer->*member_function_pointer)(class_pointer->mapFromScene(p));
    }
    painting_item->onPointSelected();
}

void GraphicsScene::afterSetPaintItemPoint(const QPointF& p)
{
    if (!painting_item)return;
    if (is_creating_polygon) {
        if (is_paint_prompt_item) {
            finishCreatePolygon();
            emit paintContinue();
        }
        return;
    }
    QList<std::pair<GraphicsItem*, void (GraphicsItem::*)(const QPointF&)>>& 
        set_point_fp_list = painting_item->getSetPointFunctionList();
    if (!set_point_fp_list.isEmpty()) {
        set_point_fp_list.removeFirst();
        if (!set_point_fp_list.isEmpty() && set_point_fp_list.first().second == &GraphicsItem::generateOtherItems) {
            auto class_pointer = set_point_fp_list.first().first;
            auto member_function_pointer = set_point_fp_list.first().second;
            (class_pointer->*member_function_pointer)(class_pointer->mapFromScene(p));
            set_point_fp_list.removeFirst();
        }
    }

    is_paint_new_item = false;
    if (set_point_fp_list.isEmpty()) {
        painting_item = nullptr;
        emit paintContinue();
    }
}

void GraphicsScene::afterSetPromptItemPoint(const QPointF& p)
{
    if (!painting_item)return;
    if (is_creating_polygon) {
        if (is_paint_prompt_item) {
            finishCreatePolygon();
            emit promptContinue();
        }
        return;
    }
    QList<std::pair<GraphicsItem*, void (GraphicsItem::*)(const QPointF&)>>&
        set_point_fp_list = painting_item->getSetPointFunctionList();
    if (!set_point_fp_list.isEmpty()) {
        set_point_fp_list.removeFirst();
        if (!set_point_fp_list.isEmpty() && set_point_fp_list.first().second == &GraphicsItem::generateOtherItems) {
            auto class_pointer = set_point_fp_list.first().first;
            auto member_function_pointer = set_point_fp_list.first().second;
            (class_pointer->*member_function_pointer)(class_pointer->mapFromScene(p));
            set_point_fp_list.removeFirst();
        }
    }

    is_paint_new_item = false;
    if (set_point_fp_list.isEmpty()) {
        painting_item = nullptr;
        emit promptContinue();
        startAiModelSegment();
    }
}

void GraphicsScene::initPaintGraphicsItem()
{
    is_paint_new_item = true;
    if (painting_item) {
        emit painting_item->prepareToRemove();
        delete painting_item;
        painting_item = nullptr;
    }
}

void GraphicsScene::initPaintFinishGraphicsItem()
{
    if (painting_item != nullptr) {
        if (is_creating_polygon) {
            finishCreatePolygon();
        }
        else {
            emit painting_item->prepareToRemove();
            delete painting_item;
        }
        painting_item = nullptr;
    }
    is_paint_new_item = false;
}

void GraphicsScene::initPaintPromptItem()
{
    is_paint_prompt_item = true;
    painting_item->getGraphicsTextModel().setIsHideText(true);
    blockSignals(true);
    initItemSettingAfterPaint(painting_item);
    blockSignals(false);
    createPromptItem();
}

void GraphicsScene::initPaintFinishPromptItem()
{
    is_paint_prompt_item = false;
}

bool GraphicsScene::isPaintItem(QGraphicsItem *item)
{
    if(
        item!=nullptr&&
        item!=pixmap_item&&
        item!=thumbnail_item&&
        item!=text_left_up&&
        item!=text_left_bottom&&
        item!=text_right_up&&
        item!=text_right_bottom&&
        item->parentItem() == nullptr)
    {
        return true;
    }
    else return false;
}

bool GraphicsScene::isPaintItemWithChild(QGraphicsItem *item)
{
    if(
        item!=nullptr&&
        item->data(0)!="GraphicsTextItem"&&
        item!=pixmap_item&&
        item!=thumbnail_item &&
        item!=text_left_up&&
        item!=text_left_bottom&&
        item!=text_right_up&&
        item!=text_right_bottom)
    {
        return true;
    }
    else return false;
}

bool GraphicsScene::isPaintItemOnScene()
{
    foreach(QGraphicsItem * item, items()) {
        if (isPaintItem(item)) return true;
    }
    return false;
}

void GraphicsScene::setContinuousPointDrawInterval(int intercal)
{
    Continuous_point_draw_interval = intercal;
}

void GraphicsScene::updateThumbnailBox()
{
    if (pixmap_item->getFscaleW() > pixmap_item->getFscaleH()) {
        if (pixmap_item->getFscaleW() * m_view->getViewTransFormModel()->getViewScale() > width()) {
            thumbnail_item->setVisible(true);
            thumbnail_item->updateDecorator();
        }
        else thumbnail_item->setVisible(false);
    }
    else {
        if (pixmap_item->getFscaleH() * m_view->getViewTransFormModel()->getViewScale() > height()) {
            thumbnail_item->setVisible(true);
            thumbnail_item->updateDecorator();
        }
        else thumbnail_item->setVisible(false);
    }
}

void GraphicsScene::updateItemIndexView()
{
    //interact with item_index_view
    QList<int> selected_rows = {};
    ItemSelectionModel* selection_model = item_index_view->getSelectionModel();
    QStandardItemModel* data_model = item_index_view->getItemDataModel();
    for (int i = 0; i < data_model->rowCount(); i++) {
        StandardGrphicsItem* label_item = dynamic_cast<StandardGrphicsItem*>(data_model->item(i, 2));
        GraphicsItem* item = label_item->getItem();
        if (item->isSelected()) selected_rows.push_back(i);
    }
    selection_model->clearSelection();
    for (int i : selected_rows) {
        QModelIndex index1 = data_model->index(i, 0);
        QModelIndex index2 = data_model->index(i, 2);
        QItemSelection selection(index1, index2);
        selection_model->select(selection, QItemSelectionModel::Select);
    }
}

void GraphicsScene::updateRtText()
{
    QString str1 = "W:" + QString::number(pixmap_item->getOriginWidth(), 'f', 0);
    QString str2 = "H:" + QString::number(pixmap_item->getOriginHeight(), 'f', 0);

    qreal image_scale_total = m_view->getViewTransFormModel()->getImageScaleTotal();
    if (pixmap_item->getPixmap().isNull()) image_scale_total = 0;
    else image_scale_total = pixmap_item->getFscaleW() / pixmap_item->getOriginWidth() * 
        m_view->getViewTransFormModel()->getViewScale();
    QString str3 = "Zoom: " + QString::number(image_scale_total, 'f', 2) + " ";

    QPoint m_present_pos_on_origin_image = m_view->getMouseCoordinate();

    QString str4 = "X:" + QString::number(m_present_pos_on_origin_image.rx(), 'f', 0);
    QString str5 = "Y:" + QString::number(m_present_pos_on_origin_image.ry(), 'f', 0);

    text_right_up->setPlainText(str1 + " " + str2 + "\n" + str3 + "\n" + str4 + " " + str5);
    m_view->getViewTransFormModel()->setImageScaleTotal(image_scale_total);
}

void GraphicsScene::initItemSettingAfterPaint(GraphicsItem* item)
{
    qreal scale = pixmap_item->getOriginWidth() / (pixmap_item->getFscaleW() + EPS);
    qreal view_scale = m_view->getViewTransFormModel()->getViewScale();
    if (!pixmap_item->getPixmap().isNull()){
        item->getGraphicsTransformModel().setMeasureObject(pixmap_item);
        item->getGraphicsTransformModel().setImageScale(scale);
    }
    addItemAcceptLabelBoardSetting(item);
    item->setViewScale(view_scale);
    QList<GraphicsItem*> child_item_list;
    item->findAllGraphicsChildItems(child_item_list);
    foreach(GraphicsItem * qitem, child_item_list) {
        qitem->getGraphicsTransformModel().setMeasureObject(pixmap_item);
        qitem->getGraphicsTransformModel().setImageScale(scale);
        qitem->setViewScale(view_scale);
    }
    emit createItemIndex(item);
}

void GraphicsScene::addItemInitAfterPaint(GraphicsItem *item)
{
    initItemSettingAfterPaint(item);
    addItem(item);
}

void GraphicsScene::addItemAcceptLabelBoardSetting(GraphicsItem* item)
{
    if (item->getIsAcceptOthersSetting()) {
        label_board_widget->setItemParameters(item);
    }
}

void GraphicsScene::addItemInit(GraphicsItem* item)
{
    qreal scale = pixmap_item->getOriginWidth() / (pixmap_item->getFscaleW() + EPS);
    qreal view_scale = m_view->getViewTransFormModel()->getViewScale();
    if (!pixmap_item->getPixmap().isNull())
    {
        item->getGraphicsTransformModel().setMeasureObject(pixmap_item);
        item->getGraphicsTransformModel().setImageScale(scale);
    }
    item->setViewScale(view_scale);
    addItem(item);
    QList<GraphicsItem*> child_item_list;
    item->findAllGraphicsChildItems(child_item_list);
    foreach(GraphicsItem * qitem, child_item_list) {
        qitem->getGraphicsTransformModel().setMeasureObject(pixmap_item);
        qitem->getGraphicsTransformModel().setImageScale(scale);
        qitem->setViewScale(view_scale);
    }
    item->onPointSelected();
    item->onUpdatePointMessage();
    emit createItemIndex(item);
}

bool GraphicsScene::getIsCreatePolygon()
{
    return is_creating_polygon;
}

void GraphicsScene::pushBackPolygonPointConsitantly(const QPointF& p)
{
    if (CoordinateCalculation::calculatePointToPoint(polygon_list.last(), p) >=
        (Continuous_point_draw_interval / m_view->getViewTransFormModel()->getViewScale()) &&
        painting_pol_item->getIsDrawContinuously()) {
        polygon_list.push_back(p);
        emit updatePoint(p, false);
    }
}

void GraphicsScene::pushBackPolygonPoint(const QPointF& p)
{
    polygon_list.push_back(p);
    emit updatePoint(p, false);
}

void GraphicsScene::startCreatePolygon()
{
    is_creating_polygon = true;
    initPaintGraphicsItem();
    polygon_list.clear();
}

void GraphicsScene::cancelCreatePolygon()
{
    if (polygon_list.size() >= 1)painting_pol_item->onActionRemoveSelf();
    else {
        if(painting_pol_item)
            delete painting_pol_item;
    }
    painting_pol_item = nullptr;
    painting_item = nullptr;
    polygon_list.clear();
    is_creating_polygon = false;
    is_paint_new_item = false;
}

void GraphicsScene::createPromptItem()
{
    if (!is_paint_prompt_item)return;
    if (is_paint_new_item) {
        if (painting_item->data(1) == "PromptRect") {
            foreach(GraphicsItem * prompt_item, scene_prompt_model.getPromptItemList()) {
                if (prompt_item->data(1) == "PromptRect") prompt_item->onActionRemoveSelf();
            }
        }
        scene_prompt_model.addPromptItem(painting_item);
    }
}

void GraphicsScene::startAiModelSegment()
{
    if (!is_paint_prompt_item)return;
    if (pixmap_item->getPixmapPath() == "") {
        qDebug() << "no load iamge";
        return;
    }
    scene_prompt_model.generateAnnotation();
}
void GraphicsScene::samSegmentRealTime()
{
    pixmap_item->showOriginalImage();
    startAiModelSegment();
}

void GraphicsScene::finishCreatePolygon()
{
    emit updatePoint(QPointF(0, 0), true);
    if (polygon_list.size() >= 3)
    {
        polygon_list.clear();
        is_creating_polygon = false;
        is_paint_new_item = false;
        disconnect(this, SIGNAL(updatePoint(QPointF, bool)), painting_pol_item, SLOT(pullPoint(QPointF, bool)));
        foreach(QGraphicsItem * item, painting_pol_item->childItems()) {
            GraphicsItem* m_item = dynamic_cast<GraphicsItem*>(item);
            if (m_item) {
                m_item->getGraphicsTransformModel().setMeasureObject(pixmap_item);
                m_item->getGraphicsTransformModel().setImageScale(pixmap_item->getOriginWidth() / (pixmap_item->getFscaleW() + EPS));
            }
        }
        if (is_paint_prompt_item) {
            if (pixmap_item->getPixmapPath() == "") {
                qDebug() << "no load iamge";
                return;
            }
            if(is_paint_prompt_item){
                scene_prompt_model.generateAnnotation();
            }
        }
        painting_pol_item = nullptr;
        painting_item = nullptr;
    }
    else {
        cancelCreatePolygon();
    }
}

void GraphicsScene::pointClicked(int checked)
{
    if(checked){
        initPaintGraphicsItem();
        painting_item = new BPoint{};
    }
    else {
        initPaintFinishGraphicsItem();
    }
}

void GraphicsScene::ellipseClicked(int checked)
{
    if (checked) {
        initPaintGraphicsItem();
        painting_item = new InteractionEllipse{};
    }
    else {
        initPaintFinishGraphicsItem();
    }
}

void GraphicsScene::roundClicked(int checked)
{
    if(checked){
        initPaintGraphicsItem();
        painting_item = new InteractionRound{};
    }
    else {
        initPaintFinishGraphicsItem();
    }
}

void GraphicsScene::lineClicked(int checked)
{
    if (checked) {
        initPaintGraphicsItem();
        painting_item = new DoubleEndPointLine{};
    }
    else {
        initPaintFinishGraphicsItem();
    }
}

void GraphicsScene::rectClicked(int checked)
{
    if (checked) {
        initPaintGraphicsItem();
        painting_item = new InteractionRect{};
    }
    else {
        initPaintFinishGraphicsItem();
    }
}

void GraphicsScene::polygonClicked(int checked)
{
    if (checked) {
        startCreatePolygon();
        painting_pol_item = new InteractionPolygon{};
        painting_item = painting_pol_item;
        connect(this, SIGNAL(updatePoint(QPointF, bool)), painting_pol_item, SLOT(pullPoint(QPointF, bool)));
    }
    else {
        initPaintFinishGraphicsItem();
    }
}

void GraphicsScene::angleClicked(int checked)
{
    if (checked) {
        initPaintGraphicsItem();
        painting_item = new Angle{};
    }
    else {
        initPaintFinishGraphicsItem();
    }
}

void GraphicsScene::parallelLineClicked(int checked)
{
    if (checked) {
        initPaintGraphicsItem();
        painting_item = new ParallelLine{};
    }
    else {
        initPaintFinishGraphicsItem();
    }
}

void GraphicsScene::pieClicked(int checked)
{
    if (checked) {
        initPaintGraphicsItem();
        painting_item = new InteractionPie{};
    }
    else {
        initPaintFinishGraphicsItem();
    }
}

void GraphicsScene::lineSegmentClicked(int checked)
{
    if (checked) {
        startCreatePolygon();
        painting_pol_item = new LineSegment{};
        painting_item = painting_pol_item;
        
        connect(this, SIGNAL(updatePoint(QPointF, bool)), 
            painting_pol_item, SLOT(pullPoint(QPointF, bool)));
    }
    else {
        initPaintFinishGraphicsItem();
    }
}

void GraphicsScene::positivePointClicked(int checked)
{
    if (checked) {
        initPaintGraphicsItem();
        painting_item = new PositivePoint{};
        initPaintPromptItem();
    }
    else {
        initPaintFinishGraphicsItem();
        initPaintFinishPromptItem();
    }
}

void GraphicsScene::negativePointClicked(int checked)
{
    if (checked) {
        initPaintGraphicsItem();
        painting_item = new NegativePoint{};
        initPaintPromptItem();
    }
    else {
        initPaintFinishGraphicsItem();
        initPaintFinishPromptItem();
    }
}

void GraphicsScene::promptRectClicked(int checked)
{
    if (checked) {
        initPaintGraphicsItem();
        painting_item = new PromptRect{};
        initPaintPromptItem();
    }
    else {
        initPaintFinishGraphicsItem();
        initPaintFinishPromptItem();
    }
}

void GraphicsScene::PPlineSegmentClicked(int checked)
{
    if (checked) {
        startCreatePolygon();
        painting_pol_item = new LineSegment{};
        painting_item = painting_pol_item;
        initPaintPromptItem();
        painting_pol_item->setFlag(QGraphicsItem::ItemIsMovable, false);
        painting_pol_item->setFlag(QGraphicsItem::ItemIsSelectable, false);
        connect(this, SIGNAL(updatePoint(QPointF, bool)), painting_pol_item, SLOT(pullPoint(QPointF, bool)));
        painting_pol_item->setIsAcceptOthersSetting(false);
        painting_pol_item->setGraphicsColor(ColorOperation::generate_color_by_text("positiveP"));
        painting_pol_item->getGraphicsTextModel().setLabelText("PPLine");
        painting_pol_item->setData(1, "PPLine");
    }
    else {
        initPaintFinishGraphicsItem();
        initPaintFinishPromptItem();
    }
}

void GraphicsScene::NPlineSegmentClicked(int checked)
{
    if (checked) {
        startCreatePolygon();
        painting_pol_item = new LineSegment{};
        painting_item = painting_pol_item;
        initPaintPromptItem();
        painting_pol_item->setFlag(QGraphicsItem::ItemIsMovable, false);
        painting_pol_item->setFlag(QGraphicsItem::ItemIsSelectable, false);
        painting_pol_item->setIsAcceptOthersSetting(false);
        painting_pol_item->setGraphicsColor(ColorOperation::generate_color_by_text("negtiveP"));
        painting_pol_item->getGraphicsTextModel().setLabelText("NPLineSegment");
        painting_pol_item->setData(1, "NPLine");
        connect(this, SIGNAL(updatePoint(QPointF, bool)), painting_pol_item, SLOT(pullPoint(QPointF, bool)));
    }
    else {
        initPaintFinishGraphicsItem();
        initPaintFinishPromptItem();
    }
}

void GraphicsScene::clearSceneGraphicsItem()
{
    if (is_creating_polygon)finishCreatePolygon();
    QList<QGraphicsItem *>item_list = items();
    foreach(QGraphicsItem *item, item_list)
    {
        if (!items().contains(item)) continue;
        if(isPaintItem(item)){
            GraphicsItem* new_item = dynamic_cast<GraphicsItem*>(item);
            if (new_item->parentItem() == nullptr) {
                new_item->onActionRemoveSelf();
            }
        }
    }
    initPaintFinishGraphicsItem();

    if(is_paint_prompt_item) emit promptContinue();
    else emit paintContinue();
}

void GraphicsScene::resetScene()
{
    QImage img{};
    clearSceneGraphicsItem();
    changeShowImage(img);
    text_right_bottom->setPlainText("0 of 0");
    updateRtText();
    update();
}

void GraphicsScene::clearPaintCache()
{
    initPaintFinishGraphicsItem();
    initPaintFinishPromptItem();
}

