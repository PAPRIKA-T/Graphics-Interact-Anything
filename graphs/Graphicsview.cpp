#include "GraphicsView.h"
#include "GraphicsScene.h"
#include "ThumbnailPixmapItem.h"
#include "GraphicsTextItem.h"
#include "widgets/FileView.h"
#include "widgets/ForePlayWidget.h"
#include "widgets/StatusWidget.h"
#include "widgets/MagnifyingGlassWidget.h"
#include "widgets/GOpenGLWidgets.h"
#include "Model/ScenePromptItemModel.h"
#include "Widgets/ImageSceneWidget2D.h"
#include "GraphicsPolygon.h"
#include <QTimer>
#include <qdebug.h>
#include <qfile.h>
#include <memory>

#define EPS (1e-5) //除数最小量

GraphicsView::GraphicsView(QWidget* parent) :
    QGraphicsView(parent)
{
    setMinimumSize(250, 250);
    setViewport(new OpenGLWidget());
    // 隐藏水平/竖直滚动条
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 反锯齿
    setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate); //FullViewportUpdate,需要再多研究一下设置模式
    setAlignment(Qt::AlignCenter);
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
    setCacheMode(QGraphicsView::CacheBackground);
    setOptimizationFlag(QGraphicsView::DontSavePainterState, true);
    setOptimizationFlag(DontAdjustForAntialiasing, true);
    /************************************************************************/
    initGraphicsScene();
}

GraphicsView::~GraphicsView()
{
    delete m_scene;
}

GraphicsScene* GraphicsView::getGraphicsScene()
{
    return m_scene;
}

void GraphicsView::initGraphicsScene()
{
    m_scene = new GraphicsScene(this);
    m_scene->setGraphicsView(this);
    setScene(m_scene);
    pixmap_item = m_scene->getPixmapItem();
    generic_interaction_model.setGraphicsView(this);
    m_transform_model.setGraphicsView(this);
    m_graphics_calculate_model.setGraphicsScene(m_scene);
    updateLtText();
}

GenericInteractionModel* GraphicsView::getGenericInteractionModel()
{
    return &generic_interaction_model;
}

ViewTransFormModel* GraphicsView::getViewTransFormModel()
{
    return &m_transform_model;
}

GraphicsCalculateModel* GraphicsView::getGraphicsCalculateModel()
{
    return &m_graphics_calculate_model;
}

QPointF GraphicsView::getPresentPosOnOriginImage()
{
    return m_present_pos_on_origin_image;
}

void GraphicsView::updateLtText()
{
    if (pixmap_item->getPixmap().isNull()) {
        m_present_pos_on_origin_image = mapToScene(m_present_pos).toPoint();
    }
    else {
        qreal scale{};
        scale = pixmap_item->getOriginWidth() / pixmap_item->getFscaleW();
        m_present_pos_on_origin_image = scale * pixmap_item->mapFromScene(mapToScene(m_present_pos)).toPoint();
    }
    QString str1 = "X:" + QString::number(m_present_pos_on_origin_image.rx(), 'f', 0) + "px ";
    QString str2 = "Y:" + QString::number(m_present_pos_on_origin_image.ry(), 'f', 0) + "px";
    QString mouse_pos_to_img_pos = str1 + str2;
    m_scene->getLeftUpTextItem()->setPlainText(mouse_pos_to_img_pos);
}

void GraphicsView::setMagImage(const QPointF& p)
{
    //QSize mag_size = mag_glass_widget->size();
    //QRect r{};
    //r.setRect(p.x() - mag_size.width() / 2,
    //    p.y() - mag_size.height() / 2, mag_size.width(), mag_size.height());
    //QRect visibleRect(0, 0, pixmap_item->getOriginWidth(), pixmap_item->getOriginHeight());
    //QPoint top_left = r.topLeft();
    //r = r.intersected(visibleRect);
    //top_left = r.topLeft() - top_left;
    //mag_glass_widget->setPaintPos(top_left);
    //grab_map = pixmap_item->getPixmap().copy(r);
    //mag_glass_widget->update();
}

void GraphicsView::grabItemForCalculate(const QPoint& p)
{
    GraphicsItem* calculate_item = nullptr;
    QGraphicsItem* grab_item = m_scene->itemAt(mapToScene(p), QTransform());
    if (grab_item->data(0) == "GraphicsTextItem") calculate_item = dynamic_cast<GraphicsItem*>(grab_item->parentItem());
    else if (grab_item == pixmap_item) qDebug() << "抓到底层图片咯";
    else calculate_item = dynamic_cast<GraphicsItem*>(grab_item);
    if (calculate_item) {
        calculate_item->setSelected(true);
        m_graphics_calculate_model.receptItem(calculate_item);
    }
}

void GraphicsView::keyDelete()
{
    QList<QGraphicsItem*> scene_list = scene()->selectedItems();
    if (scene_list.size() == 1) {
        GraphicsItem* new_item = dynamic_cast<GraphicsItem*>(scene_list.at(0));

        if (m2_undoStack != nullptr)
        {
            rubberCommand* command = new rubberCommand(scene(), scene_list);
            m2_undoStack->push(command);

            m_scene->removeItem(new_item);

            qDebug() << "ok";
        }
        else
        {
            qDebug() << "m2_undoStack is nullptr, cannot push command";
        }

        return;
    }//单点删除

    foreach(QGraphicsItem * item, scene_list)
    {
        if (!items().contains(item)) continue;
        if (m_scene->isPaintItem(item))
        {
            GraphicsItem* new_item = dynamic_cast<GraphicsItem*>(item);
            if (new_item->data(1) == "CalculateLine")
            {
                if (m2_undoStack != nullptr)
                {
                    rubberCommand* command = new rubberCommand(scene(), scene_list);
                    m2_undoStack->push(command);

                    //m2_undoStack->setMaximumUndoSteps(100);

                    //m_scene->removeItem(new_item);
                    //delete command;

                    qDebug() << "ok";
                }
                else
                {
                    qDebug() << "m2_undoStack is nullptr, cannot push command";
                }

                new_item->onActionRemoveSelf();

            }
        }
    }
    foreach(QGraphicsItem * item, scene()->selectedItems())
    {
        if (!m_scene->items().contains(item)) continue;
        if (m_scene->isPaintItemWithChild(item))
        {
            GraphicsItem* new_item = dynamic_cast<GraphicsItem*>(item);

            if (m2_undoStack != nullptr)
            {
                rubberCommand* command = new rubberCommand(scene(), scene_list);
                m2_undoStack->push(command);

                //delete command;
                //m_scene->removeItem(new_item);


                qDebug() << "ok";
            }
            else
            {
                qDebug() << "m2_undoStack is nullptr, cannot push command";
            }

            new_item->onActionRemoveSelf();

        }
    }//框选批量删除
}





void GraphicsView::startPaintMode(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_scene->createPaintItemAtPoint(mapToScene(event->pos()));
    }
    else if (event->button() == Qt::RightButton) {
        if (m_scene->getIsCreatePolygon()) {
            m_scene->finishCreatePolygon();
            emit m_scene->paintContinue();
        }
    }
}

void GraphicsView::mousePressEvent(QMouseEvent* event)
{
    m_press_pos = event->pos();
    mouse_interaction = MOUSE_INTERACTION::PRESSED_NO_MOVE;
    if (!is_actived) emit mousePressed(dynamic_cast<ImageSceneWidget2D*>(parentWidget()));
    if (event->button() == Qt::LeftButton) {
        mouse_press_status = MOUSE_PRESS_STATUS::LEFT_BUTTON_PRESSED;
    }
    else if (event->button() == Qt::RightButton) {
        mouse_press_status = MOUSE_PRESS_STATUS::RIGHT_BUTTON_PRESSED;
        centerOn(sceneRect().center());
        setDragMode(QGraphicsView::NoDrag);
    }
    else if (event->button() == Qt::MiddleButton) {
        mouse_press_status = MOUSE_PRESS_STATUS::MIDDLE_BUTTON_PRESSED;
        setDragMode(QGraphicsView::NoDrag);
        m_scene->pixmapItemMoveStart();
    }
    if (generic_interaction_model.getInteractionStatus() ==
        GenericInteractionModel::InteractionStatus::INTERACTION_SELECT) {
        if (event->button() == Qt::LeftButton) {
            setDragMode(QGraphicsView::RubberBandDrag);
        }
        else if (event->button() == Qt::RightButton) {
        }
        QGraphicsView::mousePressEvent(event);
    }
    else if (generic_interaction_model.getInteractionStatus() ==
        GenericInteractionModel::InteractionStatus::INTERACTION_EDIT_POLYGON) {
        QGraphicsView::mousePressEvent(event);
    }
    else if (generic_interaction_model.getInteractionStatus() ==
        GenericInteractionModel::InteractionStatus::INTERACTION_OBSERVE) {
        if (event->button() == Qt::LeftButton) {
            m_scene->pixmapItemMoveStart();
        }
    }
    else if (generic_interaction_model.getInteractionStatus() ==
        GenericInteractionModel::InteractionStatus::INTERACTION_RUBBER) {
        if (event->button() == Qt::LeftButton) {
            setDragMode(QGraphicsView::RubberBandDrag);
            QGraphicsView::mousePressEvent(event);
        }
    }
    else if (generic_interaction_model.getInteractionStatus() ==
        GenericInteractionModel::InteractionStatus::INTERACTION_PAINT) {
        startPaintMode(event);
    }
    else if (generic_interaction_model.getInteractionStatus() ==
        GenericInteractionModel::InteractionStatus::INTERACTION_CALCULATE) {
        if (event->button() == Qt::LeftButton) {
            m_scene->clearSelection();
            grabItemForCalculate(event->pos());
        }
    }
}

void GraphicsView::mouseMoveEvent(QMouseEvent* event)
{
    QGraphicsView::mouseMoveEvent(event);
    if (mouse_press_status == MOUSE_PRESS_STATUS::LEFT_BUTTON_PRESSED) {
        mouse_interaction = MOUSE_INTERACTION::PRESSED_AND_MOVE;
    }
    else if (mouse_press_status == MOUSE_PRESS_STATUS::RIGHT_BUTTON_PRESSED) {
        if (is_zoom_right_mouse) {
            if (event->y() < m_present_pos.ry()) {
                m_transform_model.zoom(1 + 0.05);
            }
            else {
                m_transform_model.zoom(1 - 0.05);
            }
            m_scene->updateThumbnailBox();
        }
        mouse_interaction = MOUSE_INTERACTION::PRESSED_AND_MOVE;
    }
    else if (mouse_press_status == MOUSE_PRESS_STATUS::MIDDLE_BUTTON_PRESSED) {
        QPointF delta = mapToScene(m_present_pos) - mapToScene(m_press_pos);
        m_scene->pixmapItemMoveBy(delta);
        mouse_interaction = MOUSE_INTERACTION::PRESSED_AND_MOVE;
    }
    else if (mouse_press_status == MOUSE_PRESS_STATUS::NO_BUTTON_PRESSED) {
        mouse_interaction = MOUSE_INTERACTION::HOVERING;
    }
    m_present_pos = event->pos();

    if (generic_interaction_model.getInteractionStatus() ==
        GenericInteractionModel::InteractionStatus::INTERACTION_OBSERVE) {
        if (mouse_press_status == MOUSE_PRESS_STATUS::LEFT_BUTTON_PRESSED) {
            QPointF delta = mapToScene(m_present_pos) - mapToScene(m_press_pos);
            m_scene->pixmapItemMoveBy(delta);
        }
    }
    else if (generic_interaction_model.getInteractionStatus() ==
        GenericInteractionModel::InteractionStatus::INTERACTION_PAINT) {
        if (m_scene->getIsCreatePolygon()) {
            if (mouse_press_status == MOUSE_PRESS_STATUS::LEFT_BUTTON_PRESSED) {
                m_scene->pushBackPolygonPointConsitantly(mapToScene(m_present_pos));
            }
        }
        else m_scene->setPaintItemPoint(mapToScene(m_present_pos));
    }
    updateLtText();
}

void GraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
    mouse_press_status = MOUSE_PRESS_STATUS::NO_BUTTON_PRESSED;
    if (generic_interaction_model.getInteractionStatus() ==
        GenericInteractionModel::InteractionStatus::INTERACTION_SELECT) {
        if (event->button() == Qt::LeftButton) {
            m_scene->updateItemIndexView();
        }
        else if (event->button() == Qt::RightButton) {
            if (mouse_interaction == MOUSE_INTERACTION::PRESSED_NO_MOVE) {
                if (itemAt(event->pos()) == nullptr || itemAt(event->pos()) == pixmap_item)
                    is_context_menu = true;
            }
        }
    }
    else if (generic_interaction_model.getInteractionStatus() ==
        GenericInteractionModel::InteractionStatus::INTERACTION_PAINT) {
        if (event->button() == Qt::LeftButton) {
            m_scene->afterSetPaintItemPoint(mapToScene(event->pos()));
        }
    }


    else if (generic_interaction_model.getInteractionStatus() ==
        GenericInteractionModel::InteractionStatus::INTERACTION_RUBBER)
    {

        keyDelete();


        /*addCommand* command = new addCommand(this, painting_item);
        m1_undoStack->push(command);*/


        if (m_scene->getIsCreatePolygon())
        {
            if (m_scene->painting_pol_item->getPointItemList().size() == 0)
                emit m_scene->paintContinue();
        }
    }


    else if (generic_interaction_model.getInteractionStatus() ==
        GenericInteractionModel::InteractionStatus::INTERACTION_OBSERVE) {

    }
    QGraphicsView::mouseReleaseEvent(event);
}

void GraphicsView::wheelEvent(QWheelEvent* event)
{
    int WheelDeltaValue = event->angleDelta().y();
    if (event->modifiers() == Qt::ControlModifier) {
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        if (WheelDeltaValue > 0) m_transform_model.zoomIn();
        else m_transform_model.zoomOut();
        m_scene->updateThumbnailBox();
        setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    }
    else {
        if (WheelDeltaValue > 0) emit sliceChangeOneByOne(true);
        else emit sliceChangeOneByOne(false);
    }
}

void GraphicsView::contextMenuEvent(QContextMenuEvent* event)
{
    if (is_context_menu)
    {
        QMenu menu;

        QFile file(":/res/qss/Default.qss");
        if (file.open(QFile::ReadOnly)) {
            QString stylesheet = QLatin1String(file.readAll());
            menu.setStyleSheet(stylesheet);
            file.close();
        }

        QAction* showAllTextAction = menu.addAction(QStringLiteral("Show All Item Text"), this, &GraphicsView::showAllText);
        QAction* hideAllTextAction = menu.addAction(QStringLiteral("Hide All Item Text"), this, &GraphicsView::hideAllText);
        QAction* showAllItemMessage = menu.addAction(QStringLiteral("Show All Item Message"), this, &GraphicsView::showAllItemMessage);
        QAction* hideAllItemMessage = menu.addAction(QStringLiteral("Hide All Item Message"), this, &GraphicsView::hideAllItemMessage);
        QAction* deCalLineAction = menu.addAction(QStringLiteral("Del All Measure Line"), this, &GraphicsView::deCalLine);
        Q_UNUSED(showAllTextAction);
        Q_UNUSED(hideAllTextAction);
        Q_UNUSED(deCalLineAction);
        Q_UNUSED(showAllItemMessage);
        Q_UNUSED(hideAllItemMessage);
        Q_UNUSED(event);
        menu.exec(QCursor::pos());
        menu.clear();
        is_context_menu = false;
        event->accept();



    }
    if (generic_interaction_model.getInteractionStatus() ==
        GenericInteractionModel::InteractionStatus::INTERACTION_SELECT) {
        if (mouse_interaction == MOUSE_INTERACTION::PRESSED_NO_MOVE)
            QGraphicsView::contextMenuEvent(event);
    }
}

void GraphicsView::keyPressEvent(QKeyEvent* event)
{
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_A)
    {
        foreach(QGraphicsItem * item, m_scene->items())
            item->setSelected(true);
        return;
    }
    switch (event->key())
    {
    case Qt::Key_Delete:
        keyDelete();
        break;
    case Qt::Key_F:
        foreach(QGraphicsItem * item, scene()->selectedItems())
        {
            if (m_scene->isPaintItem(item)) {
                GraphicsItem* new_item = dynamic_cast<GraphicsItem*>(item);
                if (new_item->getGraphicsPaintModel().getIsCloseItem())
                    new_item->getGraphicsPaintModel().setIsFillItem(!new_item->getGraphicsPaintModel().getIsFillItem());
            }
        }
        break;
    case Qt::Key_T:
        foreach(QGraphicsItem * item, scene()->selectedItems())
        {
            if (m_scene->isPaintItem(item)) {
                GraphicsItem* new_item = dynamic_cast<GraphicsItem*>(item);
                new_item->getGraphicsTextModel().setIsHideText(!new_item->getGraphicsTextModel().getIsHideText());
            }
        }
        break;
    case Qt::Key_W:
    {
        //模拟按下上键操作
        QKeyEvent upKeyEvent(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier);
        QGraphicsView::keyPressEvent(&upKeyEvent);
        m_scene->updateTextPos();
        m_scene->updateThumbnailBox();
        break;
    }
    case Qt::Key_A:
    {
        //模拟按下左键操作
        QKeyEvent leftKeyEvent(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier);
        QGraphicsView::keyPressEvent(&leftKeyEvent);
        m_scene->updateTextPos();
        m_scene->updateThumbnailBox();
        break;
    }
    case Qt::Key_S:
    {
        //模拟按下下键操作
        QKeyEvent downKeyEvent(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier);
        QGraphicsView::keyPressEvent(&downKeyEvent);
        m_scene->updateTextPos();
        m_scene->updateThumbnailBox();
        break;
    }
    case Qt::Key_D:
    {
        //模拟按下右键操作
        QKeyEvent rightKeyEvent(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier);
        QGraphicsView::keyPressEvent(&rightKeyEvent);
        m_scene->updateTextPos();
        m_scene->updateThumbnailBox();
        break;
    }
    default:
        QGraphicsView::keyPressEvent(event);
        m_scene->updateTextPos();
        m_scene->updateThumbnailBox();
        break;
    }
}

void GraphicsView::hideAllText()
{
    foreach(QGraphicsItem * item, items())
    {
        if (m_scene->isPaintItemWithChild(item))
        {
            GraphicsItem* new_item = dynamic_cast<GraphicsItem*>(item);
            if (new_item->parentItem() == nullptr)new_item->getGraphicsTextModel().setIsHideText(true);
        }
    }
}

void GraphicsView::showAllText()
{
    foreach(QGraphicsItem * item, items())
    {
        if (m_scene->isPaintItem(item)) {
            GraphicsItem* new_item = dynamic_cast<GraphicsItem*>(item);
            if (new_item->parentItem() == nullptr)new_item->getGraphicsTextModel().setIsHideText(false);
        }
    }
}

void GraphicsView::showAllItemMessage()
{
    foreach(QGraphicsItem * item, items())
    {
        if (m_scene->isPaintItem(item)) {
            GraphicsItem* new_item = dynamic_cast<GraphicsItem*>(item);
            if (new_item->parentItem() == nullptr)new_item->getGraphicsTextModel().setIsHideExtraText(false);
        }
    }
}

void GraphicsView::hideAllItemMessage()
{
    foreach(QGraphicsItem * item, items())
    {
        if (m_scene->isPaintItem(item)) {
            GraphicsItem* new_item = dynamic_cast<GraphicsItem*>(item);
            if (new_item->parentItem() == nullptr)new_item->getGraphicsTextModel().setIsHideExtraText(true);
        }
    }
}

void GraphicsView::deCalLine()
{
    m_scene = dynamic_cast<GraphicsScene*>(scene());
    foreach(QGraphicsItem * item, items())
    {
        if (m_scene->isPaintItem(item)) {
            GraphicsItem* new_item = dynamic_cast<GraphicsItem*>(item);
            if (new_item->data(1) == "CalculateLine")
                new_item->onActionRemoveSelf();
        }
    }
}

void GraphicsView::saveViewImage()
{
    // 抓取图像前,关闭信息显示
    m_scene->getLeftUpTextItem()->setVisible(false);
    m_scene->getLeftBottomTextItem()->setVisible(false);
    m_scene->getRightUpTextItem()->setVisible(false);
    m_scene->getRightBottomTextItem()->setVisible(false);

    hideAllText();//隐藏文本
    m_transform_model.resetTransform(); //重置图像
    QPixmap pix;
    if (pixmap_item != nullptr) {
        pix = grab(QRect(static_cast<int>(pixmap_item->x() + 1),
            static_cast<int>(pixmap_item->y() + 1),
            static_cast<int>(pixmap_item->getFscaleW() - 2),
            static_cast<int>(pixmap_item->getFscaleH()) - 2));
    }
    else pix = grab(viewport()->rect());
    // 抓取图像后,开启信息显示
    m_scene->getLeftUpTextItem()->setVisible(true);
    m_scene->getLeftBottomTextItem()->setVisible(true);
    m_scene->getRightUpTextItem()->setVisible(true);
    m_scene->getRightBottomTextItem()->setVisible(true);
    showAllText();
    QString savepath = QFileDialog::getSaveFileName(
        this,
        tr("SingleShot"),
        "D:/",
        tr("*.bmp;; *.jpg;; *.png;; *.tif;; *.GIF")
    );

    if (savepath.isEmpty()) {
        return;
    }
    else {
        if (!(pix.save(savepath)))
        {
            QMessageBox* m_box = new QMessageBox(QMessageBox::Information, QString("Save"), QString("Save Failed！"));
            QTimer::singleShot(500, m_box, SLOT(accept()));    // 在500ms后退出
            m_box->exec();
        }
        else
        {
            QMessageBox* m_box = new QMessageBox(QMessageBox::Information, QString("Save"), QString("Save Success！"));
            QTimer::singleShot(500, m_box, SLOT(accept()));    // 在500ms后退出
            m_box->exec();
        }
    }
}

void GraphicsView::setPaintCross(bool ok)
{
    paint_cross = ok;
    viewport()->update();
}

void GraphicsView::setActived(bool ok)
{
    is_actived = ok;
}

void GraphicsView::paintEvent(QPaintEvent* event)
{
    QGraphicsView::paintEvent(event);
    if (is_enter_view && paint_cross)
    {
        //画一个交叉线，显示鼠标此时的位置
        QPainter painter(viewport());
        QPen pen;
        pen.setColor(QColor(91, 220, 47));
        pen.setWidth(1);
        painter.setPen(pen);
        //绘制横向线
        painter.drawLine(QPoint(0, m_present_pos.ry()), QPoint(width(), m_present_pos.ry()));
        pen.setColor(QColor(220, 60, 74));
        painter.setPen(pen);
        //绘制纵向线
        painter.drawLine(QPoint(m_present_pos.rx(), 0), QPoint(m_present_pos.rx(), height()));
    }

}

void GraphicsView::enterEvent(QEnterEvent* event)
{
    emit mouseEnter(dynamic_cast<ImageSceneWidget2D*>(parentWidget()));
    is_enter_view = true;
    event->accept();
    viewport()->update();
}

void GraphicsView::leaveEvent(QEvent* event)
{
    emit mouseLeave(dynamic_cast<ImageSceneWidget2D*>(parentWidget()));
    is_enter_view = false;
    event->accept();
    viewport()->update();
}

void GraphicsView::resizeEvent(QResizeEvent* event)
{
    QGraphicsView::resizeEvent(event);
    m_scene->setSceneRect(0, 0, width(), height());
    //设置图像居中位置
    m_transform_model.setOriginPosition(QPoint((m_scene->width() - m_scene->getPixmapItem()->getFscaleW()) / 2,
        (m_scene->height() - m_scene->getPixmapItem()->getFscaleH()) / 2));
    m_transform_model.originPositonReturn();
}

UndoStack* GraphicsView::m2_undoStack = nullptr;
