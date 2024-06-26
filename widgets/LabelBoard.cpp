﻿#include "LabelBoard.h"
#include "Model/ViewListContainer.h"
#include "LabelBoardToolWidget.h"
#include "ColorButton.h"
#include <QHBoxLayout>

/*************************LabelBoard************************/
LabelBoard::LabelBoard(QWidget* parent)
    :QTableWidget(parent)
{
    initWidget();
}

LabelBoard::LabelBoard(int rows, int columns, QWidget* parent)
    :QTableWidget(rows, columns, parent)
{
    initWidget();
}

void LabelBoard::setViewListContainer(ViewListContainer* vlc)
{
    view_list_container = vlc;
}

void LabelBoard::initWidget()
{
    setObjectName("LabelBoardWidget");
    setContextMenuPolicy(Qt::CustomContextMenu);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setColumnCount(3);
    setHorizontalHeaderLabels(QStringList() << "ID" << "CLR" << "Label_desc");
    setShowGrid(true);
    QHeaderView* header = horizontalHeader();
    verticalHeader()->setVisible(false);
    verticalHeader()->setDefaultSectionSize(20);
    header->setStretchLastSection(true);
    header->setSectionResizeMode(QHeaderView::Interactive);
    header->setMinimumSectionSize(30);//设置最小列宽
    header->setFixedHeight(27);

    setColumnWidth(0, 50);//设置第0列宽40 需要在setModel之后设置才能生效
    setColumnWidth(1, 50);
    setColumnWidth(2, 65);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    appendClearColorBoardRow();
    appendBoardRow(DEFAULT_LABEL_ID, DEFAULT_COLOR_ITEM, DEFAULT_LABEL);
    appendBoardRow(DEFAULT_LABEL_ID, DEFAULT_COLOR_POINT_NOSELECTED, DEFAULT_LABEL);
    appendBoardRow(DEFAULT_LABEL_ID, DEFAULT_COLOR_POINT_SELECTED, DEFAULT_LABEL);
    appendBoardRow(DEFAULT_LABEL_ID, Qt::blue, DEFAULT_LABEL);
    appendBoardRow(DEFAULT_LABEL_ID, Qt::green, DEFAULT_LABEL);
    appendBoardRow(DEFAULT_LABEL_ID, Qt::cyan, DEFAULT_LABEL);
    appendBoardRow(DEFAULT_LABEL_ID, QColor(180, 120, 150), DEFAULT_LABEL);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setEditTriggers(QAbstractItemView::DoubleClicked);

    connect(this, &QTableWidget::cellChanged, this, &LabelBoard::onCellChanged);
    setAlternatingRowColors(true);
    connect(selectionModel(), &QItemSelectionModel::selectionChanged, 
        this, &LabelBoard::onSelectionChanged);
    setCurrentCell(1, 1);
}

void LabelBoard::appendBoardRow(const QString& ID, const QColor& c, const QString& label)
{
    blockSignals(true);
    int last_row = this->rowCount();
    QTableWidgetItem* id_item{};
    QTableWidgetItem* label_item{};
    for (int row = 0; row < last_row; ++row) {
        id_item = this->item(row, 2);
        label_item = this->item(row, 2);
        if (ID == id_item->text() && label == label_item->text()) return;
    }
    insertRow(last_row);
    setRowHeight(last_row, 25);

    QWidget* button_widget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(button_widget);
    ColorButton* color_btn = new ColorButton{ c , button_widget };
    color_btn->setFixedSize(15, 15);
    color_btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(color_btn);
    layout->setAlignment(Qt::AlignCenter);
    button_widget->setLayout(layout);

    QTableWidgetItem* item_0 = new QTableWidgetItem(ID);
    QTableWidgetItem* item_2 = new QTableWidgetItem(label);

    setItem(last_row, 0, item_0);
    setCellWidget(last_row, 1, button_widget);
    setItem(last_row, 2, item_2);

    item_0->setForeground(QColor(Qt::white));
    item_2->setForeground(QColor(Qt::white));
    item_0->setTextAlignment(Qt::AlignCenter);
    item_2->setTextAlignment(Qt::AlignCenter);
    clr_btn_list.push_back(color_btn);
    scrollToItem(item_0);
    connect(color_btn, &ColorButton::sentSelf, this, &LabelBoard::onColorChanged);
    blockSignals(false);
    emit sentInsertRow(last_row, color_btn->getBackgroundColor());
    setCurrentItem(item_0);
}

bool LabelBoard::isRowHasAdded(const QString& id, const QString& label)
{
    for (int row = 0; row < rowCount(); ++row) {
        QTableWidgetItem* label_item = item(row, 2);
        QTableWidgetItem* id_item = item(row, 0);
        if (id_item->text() == id && label_item->text() == label) {
            setCurrentItem(item(row, 0));
            return true;
        }
    }
    return false;
}

void LabelBoard::setItemParameters(GraphicsItem* item)
{
    QList<QTableWidgetItem*> items = selectedItems();
    int selected_row = items.at(0)->row();
    item->getGraphicsTextModel().setLabelID(items.at(0)->text());
    item->getGraphicsTextModel().setLabelText(items.at(1)->text());
    item->setGraphicsColor(clr_btn_list.at(selected_row)->getBackgroundColor());
}

QColor LabelBoard::getSelectedColor()
{
    QList<QTableWidgetItem*> items = selectedItems();
    int selected_row = items.at(0)->row();
    return clr_btn_list.at(selected_row)->getBackgroundColor();
}

QColor LabelBoard::getRowColor(int r)
{
    QWidget* clr_w = cellWidget(r, 1);
    if (clr_w == nullptr)return QColor{};
    ColorButton* clr_btn = dynamic_cast<ColorButton*>(clr_w->layout()->itemAt(0)->widget());
    return clr_btn->getBackgroundColor();
}

void LabelBoard::removeLabelRow(int row)
{
    if (row >= rowCount() || row < 0)return;
    removeRow(row);
    delete clr_btn_list.at(row);
    clr_btn_list.removeAt(row);
}

void LabelBoard::clearClrBtnList()
{
    for(auto clr_btn : clr_btn_list) {
		delete clr_btn;
	};
	clr_btn_list.clear();
}

void LabelBoard::appendClearColorBoardRow()
{
    blockSignals(true);

    int last_row = this->rowCount();
    insertRow(last_row);
    setRowHeight(last_row, 25);

    QWidget* button_widget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(button_widget);
    ColorButton* color_btn = new ColorButton{ Qt::black , button_widget };
    color_btn->setFixedSize(15, 15);
    color_btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(color_btn);
    layout->setAlignment(Qt::AlignCenter);
    button_widget->setLayout(layout);

    QTableWidgetItem* item_0 = new QTableWidgetItem(DEFAULT_LABEL_ID);
    QTableWidgetItem* item_2 = new QTableWidgetItem("Clear Color");

    setItem(last_row, 0, item_0);
    setCellWidget(last_row, 1, button_widget);
    setItem(last_row, 2, item_2);

    item_0->setForeground(QColor(Qt::white));
    item_2->setForeground(QColor(Qt::white));
    item_0->setTextAlignment(Qt::AlignCenter);
    item_2->setTextAlignment(Qt::AlignCenter);
    clr_btn_list.push_back(color_btn);
    scrollToItem(item_0);
    blockSignals(false);
    emit sentInsertRow(last_row, color_btn->getBackgroundColor());
    setCurrentItem(item_0);
}

bool LabelBoard::getIsAutoNextline()
{
    return is_auto_nextline;
}

void LabelBoard::selectNextRow()
{
    QList<QTableWidgetItem*> items = selectedItems();
	    int selected_row = items.at(0)->row();
	    if (selected_row < rowCount() - 1) {
		    setCurrentItem(item(selected_row + 1, 0));
	    }
	    else {
            return;
	    }
}


void LabelBoard::onRemoveSelectedRowClicked()
{
    if (rowCount() == 1)return; //保证至少有一行用于item默认设置
    QList<QTableWidgetItem*> items = selectedItems();
    int selected_row = items.at(0)->row();
    removeLabelRow(selected_row);
    items = selectedItems();
    int next_selected_row = items.at(0)->row();
    emit sentRemoveRow(selected_row, next_selected_row);
}

void LabelBoard::onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
    Q_UNUSED(deselected);
    QModelIndexList indexes = selected.indexes();
    if (indexes.size() <= 0) return;
    int row = indexes.at(0).row();
    QWidget* clr_w = cellWidget(row, 1);
    if (clr_w == nullptr)return;
    ColorButton* clr_btn = dynamic_cast<ColorButton*>(clr_w->layout()->itemAt(0)->widget());
    const QColor bg_c = clr_btn->getBackgroundColor();

    emit sentSelectedRowColor(row, bg_c);
}

void LabelBoard::onAppendRowClicked()
{
    // 生成随机的RGB值
    int red = QRandomGenerator::global()->bounded(256); // 0-255
    int green = QRandomGenerator::global()->bounded(256);
    int blue = QRandomGenerator::global()->bounded(256);
    // 创建随机颜色
    QColor randomColor(red, green, blue);
    appendBoardRow(DEFAULT_LABEL_ID, randomColor, DEFAULT_LABEL);
    QTableWidgetItem* lastItem = item(rowCount() - 1, 2);
    if (lastItem) {
        // 设置为编辑状态
        lastItem->setSelected(true);
        editItem(lastItem);
    }
}

void LabelBoard::onAutoNextLineChecked(int ok)
{
    is_auto_nextline = ok;  
}

void LabelBoard::saveLabelFileToTxt()
{
    QString textData;
    int rows = rowCount();
    int columns = columnCount();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (j == 1) {
                QWidget* clr_w = cellWidget(i, 1);
                ColorButton* clr_btn = dynamic_cast<ColorButton*>(clr_w->layout()->itemAt(0)->widget());
                const QColor bg_c = clr_btn->getBackgroundColor();
                textData += clr_btn->colorToQString(bg_c, "");
                textData += " "; continue;
            }
            textData += item(i, j)->text();
            textData += " ";
        }
        textData += "\n";
    }
    QString filepath = QFileDialog::getSaveFileName(this, "Save Label_desc", "./", "Txt files(*.txt)");
    if (filepath.isEmpty())return;
    QFile txtFile(filepath);
    if (txtFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream out(&txtFile);
        out << textData;
        txtFile.close();
    }
}

void LabelBoard::readLabelFileFromTxt()
{

    QString filepath = QFileDialog::getOpenFileName(this, "Load Label_desc", "./", "Txt files(*.txt)");
    if (filepath.isEmpty())return;
    emit sentClearAllRows();
    blockSignals(true);
    clearContents();
    setRowCount(0);

    clearClrBtnList();
    QFile txtFile(filepath);
    if (txtFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&txtFile);
        QVector<QStringList> txt_data;
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList list = line.split(" ");
            txt_data.append(list);
        }
        for (int i = 0; i < txt_data.size(); ++i) {
            QString id = txt_data[i].at(0);
            QString color_str = txt_data[i].at(1);
            QString label_desc = txt_data[i].at(2);
            QColor color = QColor::fromString(color_str);
            appendBoardRow(id, color, label_desc);
        }
    }
    blockSignals(false);
}

void LabelBoard::onCellChanged(int row, int column)
{
    QTableWidgetItem* item = this->item(row, column);
    QString text = item->text();
    GraphicsItem* m_item{};
    QWidget* clr_w = cellWidget(row, 1);
    if (clr_w == nullptr)return;

    ColorButton* clr_btn = dynamic_cast<ColorButton*>(clr_w->layout()->itemAt(0)->widget());
    const QColor bg_c = clr_btn->getBackgroundColor();

    if (column == 0) {
        const QString label_text = this->item(row, 2)->text();
        foreach(GraphicsView* v, view_list_container->getViewList()) {
            GraphicsScene* m_scene = v->getGraphicsScene();
            foreach(QGraphicsItem * g_item, m_scene->items()) {
                if (m_scene->isPaintItem(g_item)) {
                    m_item = dynamic_cast<GraphicsItem*>(g_item);
                    if (m_item->getGraphicsPaintModel().getPenNoSelected().color() == bg_c
                        && m_item->getGraphicsTextModel().getLabelText() == label_text) {
                        m_item->getGraphicsTextModel().setLabelID(text);
                    }
                }
            }
        }
    }
    else if (column == 2) {
        const QString id_text = this->item(row, 0)->text();
        foreach(GraphicsView * v, view_list_container->getViewList()) {
            GraphicsScene* m_scene = v->getGraphicsScene();
            foreach(QGraphicsItem * g_item, m_scene->items()) {
                if (m_scene->isPaintItem(g_item)) {
                    m_item = dynamic_cast<GraphicsItem*>(g_item);
                    if (m_item->getGraphicsPaintModel().getPenNoSelected().color() == bg_c
                        && m_item->getGraphicsTextModel().getLabelID() == id_text) {
                        m_item->getGraphicsTextModel().setLabelText(text);
                    }
                }
            }
        }
    }
}

void LabelBoard::onColorChanged(ColorButton* clr)
{
    QColor c = clr->getBackgroundColor();
    int row_index = indexAt(clr->parentWidget()->pos()).row();
    const QString id_text = this->item(row_index, 0)->text();
    const QString label_text = this->item(row_index, 2)->text();
    foreach(GraphicsView * v, view_list_container->getViewList()) {
        GraphicsScene* m_scene = v->getGraphicsScene();
        foreach(QGraphicsItem * g_item, m_scene->items()) {
            if (m_scene->isPaintItem(g_item)) {
                GraphicsItem* m_item = dynamic_cast<GraphicsItem*>(g_item);
                if (m_item->getGraphicsTextModel().getLabelID() == id_text
                    && m_item->getGraphicsTextModel().getLabelText() == label_text) {
                    m_item->setGraphicsColor(c);
                }
            }
        }
    }
    emit sentSelectedRowColor(row_index, c);
}