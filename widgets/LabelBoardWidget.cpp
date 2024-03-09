#include "LabelBoardWidget.h"
#include "Model/ViewListContainer.h"
#include "ColorButton.h"
#include <QHBoxLayout>
/*************************LabelBoard************************/
LabelBoardWidget::LabelBoardWidget(QWidget* parent)
    :QTableWidget(parent)
{
    initWidget();
}

LabelBoardWidget::LabelBoardWidget(int rows, int columns, QWidget* parent)
    :QTableWidget(rows, columns, parent)
{
    initWidget();
}

void LabelBoardWidget::setViewListContainer(ViewListContainer* vlc)
{
    view_list_container = vlc;
}

void LabelBoardWidget::initWidget()
{
    setObjectName("LabelBoardWidget");
    setContextMenuPolicy(Qt::CustomContextMenu);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setColumnCount(3);
    setHorizontalHeaderLabels(QStringList() << "ID" << "CLR" << "Label_desc");

    QHeaderView* header = horizontalHeader();
    verticalHeader()->setVisible(false);
    header->setStretchLastSection(true);
    header->setSectionResizeMode(QHeaderView::Interactive);
    header->setMinimumSectionSize(30);//设置最小列宽
    header->setFixedHeight(27);

    setColumnWidth(0, 40);//设置第0列宽40 需要在setModel之后设置才能生效
    setColumnWidth(1, 35);
    setColumnWidth(2, 80);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    appendBoardRow(DEFAULT_LABEL_ID, DEFAULT_COLOR_ITEM, DEFAULT_LABEL);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setEditTriggers(QAbstractItemView::DoubleClicked);
    connect(this, &QTableWidget::cellChanged, this, &LabelBoardWidget::onCellChanged);
    setAlternatingRowColors(true);
}

void LabelBoardWidget::appendBoardRow(const QString& ID, const QColor& c, const QString& label)
{
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
    setCurrentItem(item_0);
    scrollToItem(item_0);
    connect(color_btn, &ColorButton::sentSelf, this, &LabelBoardWidget::onColorChanged);
}

bool LabelBoardWidget::isRowHasAdded(const QString& id, const QString& label)
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

void LabelBoardWidget::setItemParameters(GraphicsItem* item)
{
    QList<QTableWidgetItem*> items = selectedItems();
    int selected_row = items.at(0)->row();
    item->getGraphicsTextModel().setLabelID(items.at(0)->text());
    item->getGraphicsTextModel().setLabelText(items.at(1)->text());
    item->setGraphicsColor(clr_btn_list.at(selected_row)->getBackgroundColor());
}

QColor LabelBoardWidget::getSelectedColor()
{
    QList<QTableWidgetItem*> items = selectedItems();
    int selected_row = items.at(0)->row();
    return clr_btn_list.at(selected_row)->getBackgroundColor();
}

void LabelBoardWidget::removeLabelRow(int row)
{
    if (row >= rowCount() || row < 0)return;
    removeRow(row);
    clr_btn_list.removeAt(row);
}

void LabelBoardWidget::onRemoveSelectedRowClicked()
{
    if (rowCount() == 1)return; //保证至少有一行用于item默认设置
    QList<QTableWidgetItem*> items = selectedItems();
    int selected_row = items.at(0)->row();
    removeLabelRow(selected_row);
}

void LabelBoardWidget::onAppendRowClicked()
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

void LabelBoardWidget::saveLabelFileToTxt()
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

void LabelBoardWidget::readLabelFileFromTxt()
{
    blockSignals(true);
    QString filepath = QFileDialog::getOpenFileName(this, "Load Label_desc", "./", "Txt files(*.txt)");
    if (filepath.isEmpty())return;
    clearContents();
    setRowCount(0);
    clr_btn_list.clear();
    QFile txtFile(filepath);
    if (txtFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&txtFile);
        QVector<QStringList> txt_data;
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList list = line.split(" ");
            txt_data.append(list);
        }
        for (int i = 0; i < txt_data.size(); i++) {
            QString id = txt_data[i].at(0);
            QString color_str = txt_data[i].at(1);
            QString label_desc = txt_data[i].at(2);
            QColor color = QColor::fromString(color_str);
            appendBoardRow(id, color, label_desc);
        }
    }
    blockSignals(false);
}

void LabelBoardWidget::onCellChanged(int row, int column)
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

void LabelBoardWidget::onColorChanged(ColorButton* clr)
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
}