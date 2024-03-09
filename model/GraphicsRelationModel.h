#pragma once
#include <QList>
class GraphicsItem;
class QGraphicsItem;
class GraphicsRelationModel
{
public:
	GraphicsRelationModel();
	~GraphicsRelationModel();

    void setIsDependWithparent(bool ok); //设置作为子item是否能独立移动
    bool getIsDependWithparent(); //获取作为子item是否能独立移动
    void setUnselectedWhileParentSelected(bool ok); //设置作为子项时，父项被选中时，是否被选中
    bool getUnselectedWhileParentSelected();
    void setHideChildListWhenHoverLeave(bool ok);
    void setAllChildVisible(bool ok);

    QGraphicsItem* findOriginParentItem(QGraphicsItem*);
    QList<GraphicsItem*> getChildItemList();

    void setIsHideChildList(bool ok);
    bool getIsHideChildItemList();

    void pushBackChildList(GraphicsItem *);
    void removeOneOfChildList(GraphicsItem*);

    bool getIsChildConsistentColor();
    void setIsChildConsistentColor(bool ok);
private:
    QList<GraphicsItem*> child_item_list; //子图元列表(会与父类有同样的颜色属性设置)
	bool is_selected_while_parent_select = true; //作为子项时，当父项被选中时，是否设置为选中状态（视觉上呈现选中状态，实际还是setSelected(false)）
	bool is_depend_with_parent = false; //作为子项时是否独立移动
	bool is_hide_child_list = false;
	bool is_child_consistent_color = false;
};
