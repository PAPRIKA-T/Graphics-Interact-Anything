#pragma once
#include <QTableWidget>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QRandomGenerator>

#define DEFAULT_LABEL "N/A"

class ColorButton;
class GraphicsItem;
class ViewListContainer;
/*************************LabelBoard************************/
class LabelBoardWidget :public QTableWidget
{
    Q_OBJECT
public:
    explicit LabelBoardWidget(QWidget* parent = nullptr);
    LabelBoardWidget(int rows, int columns, QWidget* parent = nullptr);
    void setViewListContainer(ViewListContainer*);

    void initWidget();
    void appendBoardRow(const QString& ID, const QColor& c, const QString& label = DEFAULT_LABEL);
    bool isRowHasAdded(const QString& id, const QString& label);
    void setItemParameters(GraphicsItem* item);
    QColor getSelectedColor();
    void removeLabelRow(int row);
public slots:
    void onColorChanged(ColorButton* clr);
    void onCellChanged(int row, int column);
    void saveLabelFileToTxt();
    void readLabelFileFromTxt();
    void onRemoveSelectedRowClicked();
    void onAppendRowClicked();

private:
    QList<ColorButton*> clr_btn_list;
    ViewListContainer* view_list_container = nullptr; //视图容器指针
};
