#ifndef UNDOCOMMANDMODEL_H
#define UNDOCOMMANDMODEL_H

#include <QUndoCommand>
#include <QUndoStack>
#include <QGraphicsScene>
#include <QPointF>
#include <QList>
#include "Graphs/GraphicsPoint.h"
#include "Graphs/GraphicsScene.h"
#include <QPointer>

class UndoStack : public QUndoStack {
public:
    UndoStack(QObject* parent = nullptr)
        : QUndoStack(parent)
    {
        setUndoLimit(10);
    }

    //void push(QUndoCommand* cmd)  
    //{
    //    // 在添加新命令之前检查是否需要移除最旧的命令
    //    while (count() >= 100)
    //    {
    //        const QUndoCommand* oldestCmd = command(0);
    //        //removeCommand(oldestCmd);
    //        delete oldestCmd; // 确保删除命令来释放内存
    //    }

    //    QUndoStack::push(cmd); // 调用基类的 push 方法添加命令
    //}

};

class addCommand : public QUndoCommand
{
public:
    addCommand(GraphicsScene* graphicsScene, GraphicsItem* item, QUndoCommand* parent = nullptr);
    void undo() override;

    void redo() override;

private:
    GraphicsScene* m_scene;
    GraphicsItem* m_item;
    QPointF m_initPos;
};

class rubberCommand : public QUndoCommand
{
public:
    rubberCommand(QGraphicsScene* graphicsScene, QList<QGraphicsItem*> item, QUndoCommand* parent = nullptr);
    void undo() override;

    void redo() override;

private:
    QList<QGraphicsItem*> m_item;
    QGraphicsScene* m_scene;
    bool firstTime = true; // 确保初次执行时不会重复删除
};

#endif // UNDOCOMMANDMODEL_H

