#pragma once
#include <QMenu>
class GraphicsView;
class GraphicsScene;
class QAction;
class QActionGroup;

class GraphicsItemMenu : public QMenu
{
	Q_OBJECT
public: 
	GraphicsItemMenu(QWidget* parent = nullptr);
	~GraphicsItemMenu();
	QList<QAction*> getDrawActionList();
	void setGraphicsView(GraphicsView*);

private slots:
    void onDestoryBtn(); //!!!测试使用，添加上万个item
private:
    void connectSceneSignal(GraphicsScene*);
	GraphicsView* m_view = nullptr;
    QList<QAction*> draw_action_list; //绘图按钮链表
    QActionGroup* exclusive_action_group = nullptr;
    QAction* destory_btn = nullptr; //!!!用于测试
    QAction* point_btn = nullptr;
    QAction* rect_btn = nullptr;
    QAction* line_btn = nullptr;
    QAction* round_btn = nullptr;
    QAction* pie_btn = nullptr;
    QAction* ellipse_btn = nullptr;
    QAction* line_segment_btn = nullptr;
    QAction* polygon_btn = nullptr;
    QAction* angle_btn = nullptr;
    QAction* parallel_line_btn = nullptr;
};

