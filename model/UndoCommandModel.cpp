#include "UndoCommandModel.h"
#include "Graphs/GraphicsPoint.h"
#include "Graphs/GraphicsScene.h"
#include <QPointer>


addCommand::addCommand(QGraphicsScene* graphicsScene, GraphicsItem* item, QUndoCommand* parent)
	: QUndoCommand(parent)
{
    m_scene = graphicsScene;
    m_item = item;

    if (m_scene && m_item && !m_scene->items().contains(m_item))
    {
        m_initPos = item->pos();
    }

    setText("add item");
}

void addCommand::redo()//stack push 时会自动调用
{
    // 实现重做，将Item添加到场景中
    if (m_scene && m_item && !m_scene->items().contains(m_item))
    {
        m_scene->addItem(m_item);
        m_item->setPos(m_initPos);
        qDebug() << "addRedo";


    }
}

void addCommand::undo()
{
    // 实现撤销，从场景中移除Item
    if (m_scene && m_item && m_scene->items().contains(m_item))
    {
        m_scene->removeItem(m_item);
        /*delete m_item;
        m_item = nullptr;*/
        qDebug() << "addUndo";
    }
}

rubberCommand::rubberCommand(QGraphicsScene* graphicsScene, QList<QGraphicsItem*> item, QUndoCommand* parent)
    : QUndoCommand(parent)
{
    m_scene = graphicsScene;
    m_item = item;

    /*if (m_scene && m_item && !m_scene->items().contains(m_item))
    {
        m_initPos = item->pos();
    }*/

    setText("add item");
}


void rubberCommand::undo()
{
    // 实现重做，将Item添加到场景中
    foreach(QGraphicsItem * item, m_item)
    {
        m_scene->addItem(item); // 恢复图形项到场景
    }
}

void rubberCommand::redo()//stack push 时会自动调用
{
    if (!firstTime) 
    {
        foreach(QGraphicsItem * item, m_item)
        {
            m_scene->removeItem(item); // 从场景中删除图形项
        }
    }
    firstTime = false;
}


