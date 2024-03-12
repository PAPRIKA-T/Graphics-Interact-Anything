#pragma once
#include <QWidget>
class QButtonGroup;
class GraphicsView;
class QVBoxLayout;
class GenericToolButton;
class GraphicsItemMenu;
class CalculateModeMenu;

class GiantInteractionModeWidget  : public QWidget
{
	Q_OBJECT

public:
	GiantInteractionModeWidget(QWidget *parent = nullptr);
	~GiantInteractionModeWidget();
    void returnToDefaultMode();
    void setGraphicsView(GraphicsView* view);
    GraphicsItemMenu* getGraphicsItemMenu() const;
    bool eventFilter(QObject* object, QEvent* event) override;

private slots:
    void onRubberBtn(int checked); //橡皮擦模式
    void onObserveBtn(int checked); //观察模式
    void onSelectBtn(int checked); //选择模式
    void onDrawBtn(int checked); //绘画模式
    void onEditPolygonBtn(int checked); //编辑多边形模式
    void onCalculateBtn(int checked); //测量模式
private:
    void initModeMenu();
    QButtonGroup* exclusive_button_group = nullptr;
    QVBoxLayout* main_layout = nullptr;
    GraphicsView* m_view = nullptr;
    GenericToolButton* rubber_btn = nullptr;
    GenericToolButton* observe_btn = nullptr;
    GenericToolButton* select_btn = nullptr;
    GenericToolButton* draw_btn = nullptr;
    GenericToolButton* edit_polygon_btn = nullptr;
    GenericToolButton* calculate_btn = nullptr;

    GraphicsItemMenu* item_menu = nullptr;
    CalculateModeMenu* calculate_menu = nullptr;
};
