#include "GraphicsCalculateModel.h"
#include "graphs/GraphicsCalculateLine.h"
#include "graphs/Graphicsscene.h"
#include <QDebug>
GraphicsCalculateModel::GraphicsCalculateModel()
{

}

GraphicsCalculateModel::~GraphicsCalculateModel()
{

}

void GraphicsCalculateModel::receptItem(GraphicsItem* g)
{
    if (!cal_dis1) {
        cal_dis1 = g;
        return;
    }
    if (CAL_MODE == CalModeType::CalCenterDis){
        cal_dis2 = g;
        createCalCentreDisLine(cal_dis1, cal_dis2);
    }
    else if (CAL_MODE == CalModeType::CalVertiDis){
        cal_dis2 = g;
        createCalVerticalDisToLine(cal_dis1, cal_dis2);
    }
    else if (CAL_MODE == CalModeType::CalLineAngle) {
        cal_dis2 = g;
        createCalLineToLineAngle(cal_dis1, cal_dis2);
    }
}

void GraphicsCalculateModel::setCalMode(CalModeType t)
{
    CAL_MODE = t;
}

void GraphicsCalculateModel::createCalCentreDisLine(GraphicsItem* cal_dis1, GraphicsItem* cal_dis2)
{
    if (cal_dis1 == nullptr || cal_dis2 == nullptr)return;
    if (cal_dis2 != cal_dis1)
    {
        CalculateLine* cal_line = new CalculateLine(cal_dis1->mapToScene(cal_dis1->getCenter()),
            cal_dis2->mapToScene(cal_dis2->getCenter()));
        cal_line->setCalItemStart(cal_dis1);
        cal_line->setCalItemEdge(cal_dis2);

        cal_line->setStart(cal_line->mapFromItem(cal_dis1, cal_dis1->getCenter()));
        cal_line->setEdge(cal_line->mapFromItem(cal_dis2, cal_dis2->getCenter()));

        m_scene->addItemInit(cal_line);
        connect(cal_dis1, &GraphicsItem::prepareToRemove, cal_line, &GraphicsItem::onActionRemoveSelf);
        connect(cal_dis2, &GraphicsItem::prepareToRemove, cal_line, &GraphicsItem::onActionRemoveSelf);
    }
    else qDebug() << "GraphicsCalculateModel::createCalCentreDisLine::grab item wrong";
    finishCreate();
}

void GraphicsCalculateModel::createCalVerticalDisToLine(GraphicsItem* cal_dis1, GraphicsItem* cal_dis2)
{
    if (cal_dis1 != nullptr && cal_dis1->data(0) == "BPoint")
    {
        if (cal_dis2 != nullptr && cal_dis2->data(0) == "BLine")
        {
            QPointF Verticalpoint;
            Verticalpoint = CoordinateCalculation::getPointToLineVerticalpoint
            (cal_dis1->mapToScene(cal_dis1->getCenter()),
                cal_dis2->mapToScene(cal_dis2->getStart()), cal_dis2->mapToScene(cal_dis2->getEdge()));

            CalVerticalLine* cal_line = new CalVerticalLine(cal_dis1->mapToScene(cal_dis1->getCenter()), Verticalpoint);

            cal_line->setCalItemStart(cal_dis1);
            cal_line->setCalItemEdge(cal_dis2);

            cal_line->setStart(cal_line->mapFromItem(cal_dis1, cal_dis1->getCenter()));
            cal_line->setEdge(cal_line->mapFromItem(cal_dis2, cal_dis2->getCenter()));
            m_scene->addItemInit(cal_line);
            connect(cal_dis1, &GraphicsItem::prepareToRemove, cal_line, &GraphicsItem::onActionRemoveSelf);
            connect(cal_dis2, &GraphicsItem::prepareToRemove, cal_line, &GraphicsItem::onActionRemoveSelf);
        }
        else if (cal_dis2 != nullptr)qDebug() << "应该抓取线段" << cal_dis2->data(0);
    }
    else if (cal_dis1 != nullptr && cal_dis1->data(0) == "BLine")
    {
        if (cal_dis2 != nullptr && cal_dis2->data(0) == "BPoint")
        {
            QPointF Verticalpoint;
            Verticalpoint = CoordinateCalculation::getPointToLineVerticalpoint
            (cal_dis2->mapToScene(cal_dis2->getCenter()),
                cal_dis1->mapToScene(cal_dis1->getStart()), cal_dis1->mapToScene(cal_dis1->getEdge()));

            CalVerticalLine* cal_line = new CalVerticalLine(cal_dis2->mapToScene(cal_dis2->getCenter()),
                Verticalpoint);
            cal_line->setCalItemStart(cal_dis2);
            cal_line->setCalItemEdge(cal_dis1);

            cal_line->setStart(cal_line->mapFromItem(cal_dis1, cal_dis1->getCenter()));
            cal_line->setEdge(cal_line->mapFromItem(cal_dis2, cal_dis2->getCenter()));

            m_scene->addItemInit(cal_line);
            connect(cal_dis1, &GraphicsItem::prepareToRemove, cal_line, &GraphicsItem::onActionRemoveSelf);
            connect(cal_dis2, &GraphicsItem::prepareToRemove, cal_line, &GraphicsItem::onActionRemoveSelf);
        }
        else if (cal_dis2 != nullptr)qDebug() << "应该抓取点" << cal_dis2->data(0);
    }
    finishCreate();
}

void GraphicsCalculateModel::createCalLineToLineAngle(GraphicsItem* cal_dis1, GraphicsItem* cal_dis2)
{
    if (cal_dis1 == nullptr || cal_dis2 == nullptr)return;
    if (cal_dis2->data(0) == "BLine" && cal_dis2 != cal_dis1)
    {
        CalLineAngleLine* cal_line = new CalLineAngleLine(cal_dis1->mapToScene(cal_dis1->getCenter()),
            cal_dis2->mapToScene(cal_dis2->getCenter()));
        cal_line->setCalItemStart(cal_dis1);
        cal_line->setCalItemEdge(cal_dis2);

        cal_line->setStart(cal_line->mapFromItem(cal_dis1, cal_dis1->getCenter()));
        cal_line->setEdge(cal_line->mapFromItem(cal_dis2, cal_dis2->getCenter()));

        m_scene->addItemInit(cal_line);
        connect(cal_dis1, &GraphicsItem::prepareToRemove, cal_line, &GraphicsItem::onActionRemoveSelf);
        connect(cal_dis2, &GraphicsItem::prepareToRemove, cal_line, &GraphicsItem::onActionRemoveSelf);
    }
    else qDebug() << "应该抓取直线" << cal_dis2->data(0);
    finishCreate();
}

void GraphicsCalculateModel::finishCreate()
{
    if (cal_dis2) cal_dis2->setSelected(false);
    cal_dis1 = nullptr;
    cal_dis2 = nullptr;
}

void GraphicsCalculateModel::setGraphicsScene(GraphicsScene* s)
{
    	m_scene = s;
}
