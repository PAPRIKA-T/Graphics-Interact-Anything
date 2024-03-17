#include <QMouseEvent>
#include "ItemIndexView.h"
#include "LabelBoard.h"
#include "graphs/GraphicsItem.h"
#include "utils/ColorOperation.h"
#include "model/StyleSheetConfigModel.h"

/****************************************************图元索引控件*********************************************************/

StandardGrphicsItem::StandardGrphicsItem(const QString s)
    :QStandardItem(s)
{

}

StandardGrphicsItem::~StandardGrphicsItem()
{

}

ItemSelectionModel::ItemSelectionModel(QAbstractItemModel* model, QObject* parent)
    :QItemSelectionModel(model, parent)
{

}

ItemIndexView::ItemIndexView(QWidget* parent)
    : QTableView(parent)
{
    setObjectName("ItemIndexWidget");
    setContextMenuPolicy(Qt::CustomContextMenu);
    item_data_model = new QStandardItemModel(this); //data model
    item_select_model = new ItemSelectionModel(item_data_model, this); //selection model
    item_data_model->setColumnCount(3);
    item_data_model->setHorizontalHeaderLabels(QStringList() << "ID" << "ItemType" << "Label_desc");
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    QHeaderView* header = horizontalHeader();
    verticalHeader()->setVisible(false);
    verticalHeader()->setDefaultSectionSize(18);
    header->setStretchLastSection(true);
    header->setSectionResizeMode(QHeaderView::Interactive);
    header->setMinimumSectionSize(30);//设置最小列宽
    //header->setMaximumSectionSize(500);//设置最大列宽
    //header->setDefaultAlignment(Qt::AlignLeft);
    setModel(item_data_model);
    setSelectionModel(item_select_model);
    setColumnWidth(0, 40);//设置第0列宽40 需要在setModel之后设置才能生效
    setColumnWidth(1, 75);
    setColumnWidth(2, 75);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setWindowFlags(Qt::FramelessWindowHint);
    setEditTriggers(QAbstractItemView::DoubleClicked);//单击编辑
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setAlternatingRowColors(true);

    connect(this, &ItemIndexView::clicked, this, &ItemIndexView::setItemSelected);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomContextMenuRequested(QPoint)));
    connect(selectionModel(), &QItemSelectionModel::selectionChanged, this, &ItemIndexView::selectionRegionChanged);
    connect(item_data_model, &QStandardItemModel::itemChanged, this, &ItemIndexView::onItemChanged);
    connect(remove_img, &QAction::triggered, this, &ItemIndexView::onActionRemoveSelf);
}

ItemIndexView::~ItemIndexView()
{
    delete item_data_model;
    delete item_select_model;
    delete remove_img;
}

void ItemIndexView::mousePressEvent(QMouseEvent* event)
{
    mouse_press_pos_global = event->globalPos();
    QTableView::mousePressEvent(event);
}

void ItemIndexView::enterEvent(QEvent* event)
{
    Q_UNUSED(event);
    setCursor(Qt::ArrowCursor);
}

//槽添加item
void ItemIndexView::addItemInitAfterPaint(GraphicsItem* item)
{
    const QString message = item->getGraphicsTextModel().getLabelText();
    QList<QStandardItem*> standard_item_list;
    StandardGrphicsItem* ID_item = new StandardGrphicsItem(item->getGraphicsTextModel().getLabelID());
    ID_item->setItem(item);
    ID_item->setForeground(QBrush(Qt::white));
    ID_item->setTextAlignment(Qt::AlignCenter);
    standard_item_list.push_back(ID_item);

    StandardGrphicsItem* type_item = new StandardGrphicsItem(item->getGraphicsTextModel().getItemType());
    type_item->setItem(item);
    type_item->setForeground(QBrush(Qt::white));
    type_item->setTextAlignment(Qt::AlignCenter);
    standard_item_list.push_back(type_item);

    StandardGrphicsItem* graphics_item = new StandardGrphicsItem(message);
    graphics_item->setTextAlignment(Qt::AlignCenter);
    graphics_item->setItem(item);
    graphics_item->setCheckable(true);
    graphics_item->setCheckState(Qt::Checked);
    connect(item, &GraphicsItem::prepareToRemove, this, &ItemIndexView::removeItemRow);
    connect(&item->getGraphicsTextModel(), &GraphicsTextModel::textChangeSignal, this, &ItemIndexView::changeIndexItemText);
    connect(item, &GraphicsItem::sentUnselectedColor, this, &ItemIndexView::changeIndexItemColor);
    connect(&item->getGraphicsTextModel(), &GraphicsTextModel::IdChangeSignal, this, &ItemIndexView::changeIndexItemID);

    QColor c = item->getGraphicsPaintModel().getPenNoSelected().color();
    graphics_item->setData(c, Qt::FontRole);
    graphics_item->setForeground(QBrush(c));
    standard_item_list.push_back(graphics_item);

    item_data_model->insertRow(item_data_model->rowCount(), standard_item_list);
    QModelIndex last = model()->index(item_data_model->rowCount() - 1, 0);
    setCurrentIndex(last);
}

//设置item选中
void ItemIndexView::setItemSelected(QModelIndex index)
{
    StandardGrphicsItem* item_index = dynamic_cast<StandardGrphicsItem*>(item_data_model->item(index.row(), 2));
    item_index->getItem()->setSelected(true);
    if (item_index->checkState() == Qt::Checked) item_index->getItem()->setVisible(true);
    else item_index->getItem()->setVisible(false);
}

//选择区域变化
void ItemIndexView::selectionRegionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
    foreach(const QItemSelectionRange & range, selected) {
        for (int row = range.top(); row <= range.bottom(); ++row) {
            StandardGrphicsItem* item_index = dynamic_cast<StandardGrphicsItem*>(item_data_model->item(row, 2));
            item_index->getItem()->setSelected(true);
        }
    }
    foreach(const QItemSelectionRange & range, deselected) {
        for (int row = range.top(); row <= range.bottom(); ++row) {
            StandardGrphicsItem* item_index = dynamic_cast<StandardGrphicsItem*>(item_data_model->item(row, 2));
            item_index->getItem()->setSelected(false);
        }
    }
}

//移除item对应row
void ItemIndexView::removeItemRow()
{
    GraphicsItem* item = dynamic_cast<GraphicsItem*>(sender());
    if (!item)return;
;    for (int i = 0; i < item_data_model->rowCount(); i++) {
        StandardGrphicsItem* index = dynamic_cast<StandardGrphicsItem*>(item_data_model->item(i, 2));
        if (item == index->getItem()) item_data_model->removeRow(i);
    }
}

//修改indexItem文本
void ItemIndexView::changeIndexItemText(GraphicsItem* item, const QString& s)
{
    for (int i = 0; i < item_data_model->rowCount(); i++) {
        StandardGrphicsItem* label_item = dynamic_cast<StandardGrphicsItem*>(item_data_model->item(i, 2));
        if (label_item && item == label_item->getItem()) {
            label_item->setText(s);
        }
    }
}

void ItemIndexView::changeIndexItemColor(GraphicsItem* item, QColor c)
{
    for (int i = 0; i < item_data_model->rowCount(); i++) {
        StandardGrphicsItem* index = dynamic_cast<StandardGrphicsItem*>(item_data_model->item(i, 2));
        if (index && item == index->getItem()) {
            index->setForeground(c);
        }
    }
}

void ItemIndexView::changeIndexItemID(GraphicsItem* item, QString id)
{
    for (int i = 0; i < item_data_model->rowCount(); i++) {
        StandardGrphicsItem* id_item = dynamic_cast<StandardGrphicsItem*>(item_data_model->item(i, 0));
        if (id_item && item == id_item->getItem()) {
            id_item->setText(id);
        }
    }
}

//右键菜单栏
void ItemIndexView::slotCustomContextMenuRequested(QPoint pos)
{
    index = indexAt(pos);
    if (index.isValid() && index.column() == 2) {
        QMenu menu;
        menu.addAction(remove_img);
        StyleSheetConfigModel style_sheet;
        style_sheet.setMenuStyle(&menu);
        menu.exec(QCursor::pos());  //QCursor::pos()让menu的位置在鼠标点击的的位置
    }
}

//移除item
void ItemIndexView::onActionRemoveSelf()
{
    StandardGrphicsItem* index_item = dynamic_cast<StandardGrphicsItem*>(item_data_model->itemFromIndex(index));
    index_item->getItem()->onActionRemoveSelf();
}

//item数据改变
void ItemIndexView::onItemChanged(QStandardItem* item)
{
    if (item) {
        QModelIndex index = item_data_model->indexFromItem(item);
        StandardGrphicsItem* gra_item = dynamic_cast<StandardGrphicsItem*>(item);
        GraphicsItem* m_item = gra_item->getItem();
        if (index.column() == 2) {
            QString text = item->text();
            if (m_item->getGraphicsTextModel().getLabelText() == text || text == "")return;
            m_item->getGraphicsTextModel().setLabelText(text);
            const QString id = m_item->getGraphicsTextModel().getLabelID();
            if (!label_board_widget->isRowHasAdded(id, text))
                label_board_widget->appendBoardRow(id, ColorOperation::generate_color_by_text(text), text);
            label_board_widget->setItemParameters(m_item);
        }
        else if (index.column() == 0) {
            QString text = item->text();
            if (m_item->getGraphicsTextModel().getLabelID() == text || text == "")return;
            m_item->getGraphicsTextModel().setLabelID(text);
            const QString label_text = m_item->getGraphicsTextModel().getLabelText();
            if (!label_board_widget->isRowHasAdded(text, label_text))
                label_board_widget->appendBoardRow(text, ColorOperation::generate_color_by_text(text), label_text);
            label_board_widget->setItemParameters(m_item);
        }
        else if (index.column() == 1) {
            m_item->getGraphicsTextModel().setItemType(item->text());
        }
    }
}