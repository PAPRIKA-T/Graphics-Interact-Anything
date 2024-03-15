#pragma once
#include <QWidget>

class QVBoxLayout;
class GraphicsView;
class QPushButton;
class QButtonGroup;
class GraphicsScene;
class GenericToolButton;
class GiantInteractionModeWidget;

class GraphicsItemWidget  : public QWidget
{
	Q_OBJECT
public:
	GraphicsItemWidget(QWidget *parent = nullptr);
	~GraphicsItemWidget();
	void setGraphicsView(GraphicsView*);
    QList<QPushButton*> getDrawButtonList();
    virtual void paintEvent(QPaintEvent* event) override;

private slots:
    void onDestoryBtn(); //!!!测试使用，添加上万个item

private:
    void connectSceneSignal(GraphicsScene*);
    void disconnectSceneSignal(GraphicsScene*);
    GraphicsView* m_view = nullptr; //视图容器指针
	QVBoxLayout* shape_btn_layout = nullptr; //绘图按钮垂直布局器
	QList<QPushButton*> draw_button_list; //绘图按钮链表
    GenericToolButton* destory_btn = nullptr; //!!!用于测试
    GenericToolButton* point_btn = nullptr;
    GenericToolButton* rect_btn = nullptr;
    GenericToolButton* line_btn = nullptr;
    GenericToolButton* round_btn = nullptr;
    GenericToolButton* pie_btn = nullptr;
    GenericToolButton* ellipse_btn = nullptr;
    GenericToolButton* line_segment_btn = nullptr;
    GenericToolButton* polygon_btn = nullptr;
    GenericToolButton* angle_btn = nullptr;
    GenericToolButton* parallel_line_btn = nullptr;

};
