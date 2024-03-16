#pragma once
#include <QStandardItemModel>
#include <QHeaderView>
#include <QTableView>
#include <QStyledItemDelegate>

class GraphicsItem;
class LabelBoard;
/*************************ItemIndexView************************/
class StandardGrphicsItem : public QStandardItem
{
public:
    StandardGrphicsItem(const QString s);
    ~StandardGrphicsItem();
    GraphicsItem* getItem() { return item; }
    void setItem(GraphicsItem* i) { item = i; }
private:
    GraphicsItem* item = nullptr;
};

class CustomItemDelegate : public QStyledItemDelegate
{
public:
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override
    {
        if (index.column() == nonEditableColumn)
            return nullptr; // 返回 nullptr 表示该列不可编辑
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
    void setNonEditableColumn(int column)
    {
        nonEditableColumn = column;
    }

private:
    int nonEditableColumn = -1;
};

class ItemSelectionModel : public QItemSelectionModel
{
    Q_OBJECT
public:
    explicit ItemSelectionModel(QAbstractItemModel* model = nullptr, QObject* parent = nullptr);

};

class ItemIndexView :public QTableView
{
    Q_OBJECT
public:
    ItemIndexView(QWidget* parent = nullptr);
    ~ItemIndexView();

    void setLabelBoardWidget(LabelBoard* w) { label_board_widget = w; }
    LabelBoard* getLabelBoardWidget() { return label_board_widget; }
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void enterEvent(QEvent* event);
    QStandardItemModel* getItemDataModel() { return item_data_model; }
    ItemSelectionModel* getSelectionModel() { return item_select_model; }

public slots:
    void addItemInitAfterPaint(GraphicsItem* item); //m_scene加入item时控件添加一个item_index
    void removeItemRow(); //移除Item所在行
    void changeIndexItemText(GraphicsItem* item, const QString& s);//修改表格文本信息
    void changeIndexItemColor(GraphicsItem* item, QColor c);//修改表格文本颜色
    void changeIndexItemID(GraphicsItem* item, QString id); //修改表格ID信息
    void setItemSelected(const QModelIndex index); //设置对应item状态selected
    void selectionRegionChanged(const QItemSelection& selected, const QItemSelection& deselected);//选择区域变化
    void slotCustomContextMenuRequested(QPoint); //右键菜单栏槽
    void onActionRemoveSelf(); //删除图元
    void onItemChanged(QStandardItem* item); //单元编辑
    //void onCurrentIndexChanged(const QModelIndex& current, const QModelIndex& previous);

private:
    LabelBoard* label_board_widget = nullptr;
    QModelIndex index; // 当前选中的位置
    QStandardItemModel* item_data_model = nullptr; //数据模型
    ItemSelectionModel* item_select_model = nullptr; //单元选择模型
    QAction* remove_img = new QAction("remove");
    QPointF mouse_press_pos_global; //鼠标点击处位置
};
