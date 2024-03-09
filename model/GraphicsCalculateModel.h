#pragma once
#include <QObject>
class GraphicsItem;
class GraphicsScene;
class GraphicsCalculateModel  : public QObject
{
	Q_OBJECT

public:
    enum class CalModeType
    {
        NoCal,
        CalCenterDis,
        CalVertiDis,
        CalLineAngle,
    };
	GraphicsCalculateModel();
	~GraphicsCalculateModel();
    void receptItem(GraphicsItem*);
    void setCalMode(CalModeType);
    void finishCreate();
    void setGraphicsScene(GraphicsScene* s);
private:
    void createCalCentreDisLine(GraphicsItem* cal_dis1, GraphicsItem* cal_dis2); //生成测量item中心点线段
    void createCalVerticalDisToLine(GraphicsItem* cal_dis1, GraphicsItem* cal_dis2); //生成测量点线垂线线段
    void createCalLineToLineAngle(GraphicsItem* cal_dis1, GraphicsItem* cal_dis2); //生成测量线线角度线段
    CalModeType CAL_MODE = CalModeType::CalCenterDis; //测量模式
    GraphicsScene* m_scene = nullptr;
    GraphicsItem* cal_dis1 = nullptr; //指向测量图形项1
    GraphicsItem* cal_dis2 = nullptr; //指向测量图形项2
};
