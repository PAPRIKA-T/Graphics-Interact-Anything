#pragma once
#include <QTableWidget>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QRandomGenerator>

#define DEFAULT_LABEL "N/A"

class ColorButton;
class GraphicsItem;
class ViewListContainer;
class LabelBoardToolWidget;

/*************************LabelBoard************************/
class LabelBoard :public QTableWidget
{
    Q_OBJECT
public:
    explicit LabelBoard(QWidget* parent = nullptr);
    LabelBoard(int rows, int columns, QWidget* parent = nullptr);
    void setViewListContainer(ViewListContainer*);

    void appendBoardRow(const QString& ID, const QColor& c, const QString& label = DEFAULT_LABEL);
    bool isRowHasAdded(const QString& id, const QString& label);
    void setItemParameters(GraphicsItem* item);
    QColor getSelectedColor();
    QColor getRowColor(int);
    bool getIsAutoNextline();

    void selectNextRow();
signals:
    void sentSelectedRowColor(int, const QColor&);
    void sentRemoveRow(int, int);
    void sentInsertRow(int, const QColor&);
    void sentClearAllRows();

public slots:
    void onSelectionChanged(const QItemSelection&, const QItemSelection&);
    void onColorChanged(ColorButton* clr);
    void onCellChanged(int row, int column);
    void saveLabelFileToTxt();
    void readLabelFileFromTxt();
    void onRemoveSelectedRowClicked();
    void onAppendRowClicked();
    void onAutoNextLineChecked(int);
    
private:
    void initWidget();
    void removeLabelRow(int row);
    void clearClrBtnList();
    QList<ColorButton*> clr_btn_list;
    ViewListContainer* view_list_container = nullptr; //视图容器指针
    bool is_auto_nextline = false; //是否自动换行
};
