#include "XmlIOstreamModel.h"
#include "graphs/Graphicsitem.h"
#include "graphs/Graphicsscene.h"
#include "utils/FileOperation.h"
#include <QXmlStreamReader>
#include <QtXml/QDomDocument>
#include <QPointF>
#include <QMessageBox>
#include "graphs/AllGraphics.h"
#include "graphs/GraphicsItemIoParameter.h"

#define EPS 1e-5

XmlIOstreamModel::XmlIOstreamModel()
{

}

XmlIOstreamModel::~XmlIOstreamModel()
{

}

bool XmlIOstreamModel::saveItemToXmlFile(const QString& savepath,  GraphicsScene* scene)
{
    if (savepath.isEmpty()) return false;
    QFile m_file(savepath);
    if (!m_file.open(QIODevice::WriteOnly | QFile::Text))
    {
        qDebug() << " Write Open false";
        return false;
    }

    QDomDocument domDoc; //创建xml文档
    QDomElement annotation_element = domDoc.createElement("annotation");   //创建顶节点annotation
    domDoc.appendChild(annotation_element);

    createSaveXmlHeader(domDoc, annotation_element, scene); //创建文件头部信息

    QList<GraphicsItem*> sort_item_list; //需要保存的item列表
    createGraphicsItemName(sort_item_list, scene);
    createSaveXmlGraphicsItemInfo(domDoc, annotation_element, sort_item_list, scene);
    m_file.write(domDoc.toString().toLocal8Bit().data());
    m_file.close();
    return true;
}

bool XmlIOstreamModel::readItemFromXmlFile(const QString& readpath, GraphicsScene* scene)
{
    if (readpath.isEmpty()) return false;
    QFileInfo info(readpath);
    if (!info.exists())return false;

    QFile file(readpath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))   //打开文件失败
    {
        QMessageBox::information(nullptr, QString("title"), QString("xml open error!"));
        qDebug() << "annotation file open error!";
        return false;
    }

    QList<GraphicsItem*> items; //读取所有item（不包含自身子图元）
    GiantImageItem* pixmap_item = scene->getPixmapItem();
    double m_fscale = pixmap_item->getFscaleW() / (pixmap_item->getOriginWidth() + EPS);
    QPointF point_scale(m_fscale, m_fscale);

    QXmlStreamReader xmlreader(&file);   //读取xml文件的迭代器
    QString nodename{};   //节点的名字
    while (!xmlreader.atEnd() || !xmlreader.hasError())   //当文件没有结束且没有出错执行下面的代码
    {
        xmlreader.readNextStartElement(); //找到非根节点下的第一个子节点
        nodename = xmlreader.name().toString(); //第一个子节点Data
        if (nodename == "object" && xmlreader.isStartElement())
        {
            GraphicsItem* object_item = nullptr;
            GraphicsItemIoParameter item_para{};
            QList<int> child_name;
            while (!(nodename == "object" && xmlreader.isEndElement())) //如果子节点中的内容没有结束
            {
                xmlreader.readNextStartElement();
                nodename = xmlreader.name().toString();
                //readGenericItemParameter(xmlreader, nodename, item_para);
                if (nodename == "name" && xmlreader.isStartElement())
                {
                    while (!(nodename == "name" && xmlreader.isEndElement()))
                        item_para.text = xmlreader.readElementText();
                }
                else if (nodename == "ID" && xmlreader.isStartElement())
                {
                    while (!(nodename == "ID" && xmlreader.isEndElement()))
                        item_para.ID = xmlreader.readElementText();
                }
                else if (nodename == "color" && xmlreader.isStartElement())
                {
                    while (!(nodename == "color" && xmlreader.isEndElement()))
                        item_para.color = xmlreader.readElementText();
                }
                else if (nodename == "calNumber" && xmlreader.isStartElement())
                {
                    while (!(nodename == "calNumber" && xmlreader.isEndElement()))
                        item_para.name = xmlreader.readElementText().toInt();
                }
                else if (nodename == "rotate" && xmlreader.isStartElement())
                {
                    while (!(nodename == "rotate" && xmlreader.isEndElement()))
                    {
                        xmlreader.readNextStartElement();
                        nodename = xmlreader.name().toString();
                        if (nodename == "angle" && xmlreader.isStartElement())
                        {
                            while (!(nodename == "angle" && xmlreader.isEndElement()))
                                item_para.rotate_angle = xmlreader.readElementText().toDouble();
                        }
                        else if (nodename == "x" && xmlreader.isStartElement())
                        {
                            while (!(nodename == "x" && xmlreader.isEndElement()))
                                item_para.rotate_point.setX(xmlreader.readElementText().toDouble());
                        }
                        else if (nodename == "y" && xmlreader.isStartElement())
                        {
                            while (!(nodename == "y" && xmlreader.isEndElement()))
                                item_para.rotate_point.setY(xmlreader.readElementText().toDouble());
                        }
                    }
                }
                else if (nodename == "bndbox" && xmlreader.isStartElement())
                {
                    object_item = readInteractionRectXml(xmlreader, nodename, items, 
                        item_para, pixmap_item, point_scale);
                }
                else if (nodename == "BPoint" && xmlreader.isStartElement())
                {
                    readBPointXml(xmlreader, nodename, items,
                        item_para, pixmap_item, point_scale);
                }
                else if (nodename == "DoubleEndPointLine" && xmlreader.isStartElement())
                {
                    object_item = readDoubleEndPointLineXml(xmlreader, nodename, items,
                        item_para, pixmap_item, point_scale);
                }
                else if (nodename == "InteractionEllipse" && xmlreader.isStartElement())
                {
                    object_item = readInteractionEllipseXml(xmlreader, nodename, items,
                        item_para, pixmap_item, point_scale);
                }
                else if (nodename == "InteractionRound" && xmlreader.isStartElement())
                {
                    object_item = readInteractionRoundXml(xmlreader, nodename, items,
                        item_para, pixmap_item, point_scale);
                }
                else if (nodename == "polygon" && xmlreader.isStartElement())
                {
                    object_item = readPolygonXml(xmlreader, nodename, items,
                        item_para, pixmap_item, point_scale);
                }
                else if (nodename == "LineSegment" && xmlreader.isStartElement())
                {
                    object_item = readLineSegmentXml(xmlreader, nodename, items,
                        item_para, pixmap_item, point_scale);
                }
                else if (nodename == "InteractionPie" && xmlreader.isStartElement())
                {
                    object_item = readInteractionPieXml(xmlreader, nodename, items,
                        item_para, pixmap_item, point_scale);
                }
                else if (nodename == "Angle" && xmlreader.isStartElement())
                {
                    object_item = readAngleXml(xmlreader, nodename, items,
                        item_para, pixmap_item, point_scale);
                }
                else if (nodename == "ParallelLine" && xmlreader.isStartElement())
                {
                    object_item = readParallelLineXml(xmlreader, nodename, items,
                        item_para, pixmap_item, point_scale);
                }
                else if (nodename == "CalCentreLine" && xmlreader.isStartElement())
                {
                    object_item = readCalculateLineXml(xmlreader, nodename, items,
                        item_para);
                }
                else if (nodename == "CalVerticalLine" && xmlreader.isStartElement())
                {
                    object_item = readCalVerticalLineXml(xmlreader, nodename, items,
                        item_para);
                }
                else if (nodename == "CalLineAngleLine" && xmlreader.isStartElement())
                {
                    object_item = readCalLineAngleLineXml(xmlreader, nodename, items,
                        item_para);
                }
                else if (nodename == "childCalNumber" && xmlreader.isStartElement())
                {
                    if (object_item)
                    {
                        if (object_item->data(0) == "polygon")
                        {
                            if (object_item->data(1) == "LineSegment")
                            {
                                int j = 0;
                                int i = 0;
                                while (!(nodename == "childCalNumber" && xmlreader.isEndElement()))
                                {
                                    xmlreader.readNextStartElement();
                                    nodename = xmlreader.name().toString();
                                    if (nodename == "LineSegmentPoint" + QString::number(i) && xmlreader.isStartElement())
                                    {
                                        while (!(nodename == "LineSegmentPoint" + QString::number(i) && xmlreader.isEndElement()))
                                            child_name.append(xmlreader.readElementText().toInt());
                                        i++;
                                    }
                                    else if (nodename == "LineSegmentLine" + QString::number(j) && xmlreader.isStartElement())
                                    {
                                        while (!(nodename == "LineSegmentLine" + QString::number(j) && xmlreader.isEndElement()))
                                            child_name.append(xmlreader.readElementText().toInt());
                                        j++;
                                    }
                                }
                            }
                            else
                            {
                                int j = 0;
                                int i = 0;
                                while (!(nodename == "childCalNumber" && xmlreader.isEndElement()))
                                {
                                    xmlreader.readNextStartElement();
                                    nodename = xmlreader.name().toString();
                                    if (nodename == "PolygonPoint" + QString::number(i) && xmlreader.isStartElement())
                                    {
                                        while (!(nodename == "PolygonPoint" + QString::number(i) && xmlreader.isEndElement()))
                                            child_name.append(xmlreader.readElementText().toInt());
                                        i++;
                                    }
                                    if (nodename == "PolygonLine" + QString::number(j) && xmlreader.isStartElement())
                                    {
                                        while (!(nodename == "PolygonLine" + QString::number(j) && xmlreader.isEndElement()))
                                            child_name.append(xmlreader.readElementText().toInt());
                                        j++;
                                    }
                                }
                            }
                        }
                        else if (object_item->data(2) == "ParallelLine")
                        {
                            ParallelLine* item = dynamic_cast<ParallelLine*>(object_item);
                            while (!(nodename == "childCalNumber" && xmlreader.isEndElement()))
                            {
                                xmlreader.readNextStartElement();
                                nodename = xmlreader.name().toString();
                                if (nodename == "ParallelLineAnother" + QString::number(1) && xmlreader.isStartElement())
                                {
                                    while (!(nodename == "ParallelLineAnother" + QString::number(1) && xmlreader.isEndElement()))
                                        item->getAuxiliaryLine()->setName(xmlreader.readElementText().toInt());
                                }
                                if (nodename == "ParallelLineSPoint" && xmlreader.isStartElement())
                                {
                                    while (!(nodename == "ParallelLineSPoint" && xmlreader.isEndElement()))
                                        item->getSPointItem()->setName(xmlreader.readElementText().toInt());
                                }
                                if (nodename == "ParallelLineEPoint" && xmlreader.isStartElement())
                                {
                                    while (!(nodename == "ParallelLineEPoint" && xmlreader.isEndElement()))
                                        item->getEPointItem()->setName(xmlreader.readElementText().toInt());
                                }
                                if (nodename == "ParallelLineAnotherSPoint" && xmlreader.isStartElement())
                                {
                                    while (!(nodename == "ParallelLineAnotherSPoint" && xmlreader.isEndElement()))
                                        item->getAuxiliaryLine()->getSPointItem()->setName(xmlreader.readElementText().toInt());
                                }
                                if (nodename == "ParallelLineAnotherEPoint" && xmlreader.isStartElement())
                                {
                                    while (!(nodename == "ParallelLineAnotherEPoint" && xmlreader.isEndElement()))
                                        item->getAuxiliaryLine()->getEPointItem()->setName(xmlreader.readElementText().toInt());
                                }
                            }
                        }
                        else if (object_item->data(1) == "InteractionRect") {
                            InteractionRect* item = dynamic_cast<InteractionRect*>(object_item);
                            while (!(nodename == "childCalNumber" && xmlreader.isEndElement()))
                            {
                                xmlreader.readNextStartElement();
                                nodename = xmlreader.name().toString();
                                if (nodename == "SPoint" && xmlreader.isStartElement())
                                {
                                    while (!(nodename == "SPoint" && xmlreader.isEndElement()))
                                        item->getSPointItem()->setName(xmlreader.readElementText().toInt());
                                }
                                if (nodename == "EPoint" && xmlreader.isStartElement())
                                {
                                    while (!(nodename == "EPoint" && xmlreader.isEndElement()))
                                        item->getEPointItem()->setName(xmlreader.readElementText().toInt());
                                }
                                if (nodename == "RtPoint" && xmlreader.isStartElement())
                                {
                                    while (!(nodename == "RtPoint" && xmlreader.isEndElement()))
                                        item->getRtPointItem()->setName(xmlreader.readElementText().toInt());
                                }
                                if (nodename == "LbPoint" && xmlreader.isStartElement())
                                {
                                    while (!(nodename == "LbPoint" && xmlreader.isEndElement()))
                                        item->getLbPointItem()->setName(xmlreader.readElementText().toInt());
                                }
                            }
                        }
                        else if (object_item->data(1) == "InteractionEllipse") {
                            InteractionEllipse* item = dynamic_cast<InteractionEllipse*>(object_item);
                            while (!(nodename == "childCalNumber" && xmlreader.isEndElement()))
                            {
                                xmlreader.readNextStartElement();
                                nodename = xmlreader.name().toString();
                                if (nodename == "SPoint" && xmlreader.isStartElement())
                                {
                                    while (!(nodename == "SPoint" && xmlreader.isEndElement()))
                                        item->getSPointItem()->setName(xmlreader.readElementText().toInt());
                                }
                                if (nodename == "EPoint" && xmlreader.isStartElement())
                                {
                                    while (!(nodename == "EPoint" && xmlreader.isEndElement()))
                                        item->getEPointItem()->setName(xmlreader.readElementText().toInt());
                                }
                                if (nodename == "RtPoint" && xmlreader.isStartElement())
                                {
                                    while (!(nodename == "RtPoint" && xmlreader.isEndElement()))
                                        item->getRtPointItem()->setName(xmlreader.readElementText().toInt());
                                }
                                if (nodename == "LbPoint" && xmlreader.isStartElement())
                                {
                                    while (!(nodename == "LbPoint" && xmlreader.isEndElement()))
                                        item->getLbPointItem()->setName(xmlreader.readElementText().toInt());
                                }
                            }
                        }
                        else if (object_item->data(1) == "InteractionRound") {
                            InteractionRound* item = dynamic_cast<InteractionRound*>(object_item);
                            while (!(nodename == "childCalNumber" && xmlreader.isEndElement()))
                            {
                                xmlreader.readNextStartElement();
                                nodename = xmlreader.name().toString();
                                if (nodename == "CPoint" && xmlreader.isStartElement())
                                {
                                    while (!(nodename == "CPoint" && xmlreader.isEndElement()))
                                        item->getCPointItem()->setName(xmlreader.readElementText().toInt());
                                }
                                if (nodename == "EPoint" && xmlreader.isStartElement())
                                {
                                    while (!(nodename == "EPoint" && xmlreader.isEndElement()))
                                        item->getEPointItem()->setName(xmlreader.readElementText().toInt());
                                }
                            }
                        }
                        else if (object_item->data(1) == "InteractionPie") {
                            InteractionPie* item = dynamic_cast<InteractionPie*>(object_item);
                            while (!(nodename == "childCalNumber" && xmlreader.isEndElement()))
                            {
                                xmlreader.readNextStartElement();
                                nodename = xmlreader.name().toString();
                                if (nodename == "CPoint" && xmlreader.isStartElement())
                                {
                                    while (!(nodename == "CPoint" && xmlreader.isEndElement()))
                                        item->getCPointItem()->setName(xmlreader.readElementText().toInt());
                                }
                                if (nodename == "SPoint" && xmlreader.isStartElement())
                                {
                                    while (!(nodename == "SPoint" && xmlreader.isEndElement()))
                                        item->getSPointItem()->setName(xmlreader.readElementText().toInt());
                                }
                                if (nodename == "EPoint" && xmlreader.isStartElement())
                                {
                                    while (!(nodename == "EPoint" && xmlreader.isEndElement()))
                                        item->getEPointItem()->setName(xmlreader.readElementText().toInt());
                                }
                            }
                        }
                        else if (object_item->data(1) == "DoubleEndPointLine") {
                            DoubleEndPointLine* item = dynamic_cast<DoubleEndPointLine*>(object_item);
                            while (!(nodename == "childCalNumber" && xmlreader.isEndElement()))
                            {
                                xmlreader.readNextStartElement();
                                nodename = xmlreader.name().toString();
                                if (nodename == "SPoint" && xmlreader.isStartElement())
                                {
                                    while (!(nodename == "SPoint" && xmlreader.isEndElement()))
                                        item->getSPointItem()->setName(xmlreader.readElementText().toInt());
                                }
                                if (nodename == "EPoint" && xmlreader.isStartElement())
                                {
                                    while (!(nodename == "EPoint" && xmlreader.isEndElement()))
                                        item->getEPointItem()->setName(xmlreader.readElementText().toInt());
                                }
                            }
                        }
                        else if (object_item->data(2) == "CalVerticalLine") {
                            CalVerticalLine* item = dynamic_cast<CalVerticalLine*>(object_item);
                            while (!(nodename == "childCalNumber" && xmlreader.isEndElement()))
                            {
                                xmlreader.readNextStartElement();
                                nodename = xmlreader.name().toString();
                                if (nodename == "SPoint" && xmlreader.isStartElement())
                                {
                                    while (!(nodename == "SPoint" && xmlreader.isEndElement()))
                                        item->getSPointItem()->setName(xmlreader.readElementText().toInt());
                                }
                                if (nodename == "EPoint" && xmlreader.isStartElement())
                                {
                                    while (!(nodename == "EPoint" && xmlreader.isEndElement()))
                                        item->getEPointItem()->setName(xmlreader.readElementText().toInt());
                                }
                            }
                        }
                        else if (object_item->data(2) == "AngleLine") {
                            CalLineAngleLine* item = dynamic_cast<CalLineAngleLine*>(object_item);
                            while (!(nodename == "childCalNumber" && xmlreader.isEndElement()))
                            {
                                xmlreader.readNextStartElement();
                                nodename = xmlreader.name().toString();
                                if (nodename == "SPoint" && xmlreader.isStartElement())
                                {
                                    while (!(nodename == "SPoint" && xmlreader.isEndElement()))
                                        item->getSPointItem()->setName(xmlreader.readElementText().toInt());
                                }
                                if (nodename == "EPoint" && xmlreader.isStartElement())
                                {
                                    while (!(nodename == "EPoint" && xmlreader.isEndElement()))
                                        item->getEPointItem()->setName(xmlreader.readElementText().toInt());
                                }
                            }
                        }
                        else if (object_item->data(1) == "CalculateLine") {
                            CalculateLine* item = dynamic_cast<CalculateLine*>(object_item);
                            while (!(nodename == "childCalNumber" && xmlreader.isEndElement()))
                            {
                                xmlreader.readNextStartElement();
                                nodename = xmlreader.name().toString();
                                if (nodename == "SPoint" && xmlreader.isStartElement())
                                {
                                    while (!(nodename == "SPoint" && xmlreader.isEndElement()))
                                        item->getSPointItem()->setName(xmlreader.readElementText().toInt());
                                }
                                if (nodename == "EPoint" && xmlreader.isStartElement())
                                {
                                    while (!(nodename == "EPoint" && xmlreader.isEndElement()))
                                        item->getEPointItem()->setName(xmlreader.readElementText().toInt());
                                }
                            }
                        }
                        else if (object_item->data(0) == "Angle")
                        {
                            Angle* item = dynamic_cast<Angle*>(object_item);
                            while (!(nodename == "childCalNumber" && xmlreader.isEndElement()))
                            {
                                xmlreader.readNextStartElement();
                                nodename = xmlreader.name().toString();
                                if (nodename == "AngleLine" + QString::number(1) && xmlreader.isStartElement())
                                {
                                    while (!(nodename == "AngleLine" + QString::number(1) && xmlreader.isEndElement()))
                                        item->getLine1()->setName(xmlreader.readElementText().toInt());
                                }
                                if (nodename == "AngleLine" + QString::number(2) && xmlreader.isStartElement())
                                {
                                    while (!(nodename == "AngleLine" + QString::number(2) && xmlreader.isEndElement()))
                                        item->getLine2()->setName(xmlreader.readElementText().toInt());
                                }
                                if (nodename == "CPoint" && xmlreader.isStartElement())
                                {
                                    while (!(nodename == "CPoint" && xmlreader.isEndElement()))
                                        item->getCPointItem()->setName(xmlreader.readElementText().toInt());
                                }
                                if (nodename == "AngleLine1P" && xmlreader.isStartElement())
                                {
                                    while (!(nodename == "AngleLine1P" && xmlreader.isEndElement()))
                                        item->getLine1()->getEPointItem()->setName(xmlreader.readElementText().toInt());
                                }
                                if (nodename == "AngleLine2P" && xmlreader.isStartElement())
                                {
                                    while (!(nodename == "AngleLine2P" && xmlreader.isEndElement()))
                                        item->getLine2()->getEPointItem()->setName(xmlreader.readElementText().toInt());
                                }
                            }
                        }
                    }
                }
            }
            if (object_item)
            {
                if (object_item->data(1) == "LineSegment")
                {
                    LineSegment* lineseg_item = dynamic_cast<LineSegment*>(object_item);
                    QList<PolygonLine*>& line_list = lineseg_item->getLineList();
                    int i = 0;
                    for (i = 0; i < lineseg_item->getPointItemList().size(); i++)
                        lineseg_item->getPointItemList()[i]->setName(child_name.at(i));

                    for (int j = 0; j < lineseg_item->getLineList().size(); j++)
                        line_list[j]->setName(child_name.at(i + j));
                }
                else if (object_item->data(0) == "polygon")
                {
                    InteractionPolygon* polygon_item = dynamic_cast<InteractionPolygon*>(object_item);
                    int i = 0;
                    for (i = 0; i < polygon_item->getPointItemList().size(); i++)
                        polygon_item->getPointItemList()[i]->setName(child_name.at(i));

                    for (int j = 0; j < polygon_item->getLineList().size(); j++)
                        polygon_item->getLineList()[j]->setName(child_name.at(i + j));
                }
            }
        }
    }
    file.close();
    addItemList(items, scene);
    return true;
}

/***************读取相关私有函数*******************/
void XmlIOstreamModel::readGenericItemParameter(QXmlStreamReader& xmlreader, 
    QString& nodename, GraphicsItemIoParameter& item_para)
{
    if (nodename == "name" && xmlreader.isStartElement())
    {
        while (!(nodename == "name" && xmlreader.isEndElement()))
            item_para.text = xmlreader.readElementText();
    }
    else if (nodename == "ID" && xmlreader.isStartElement())
    {
        while (!(nodename == "ID" && xmlreader.isEndElement()))
            item_para.ID = xmlreader.readElementText();
    }
    else if (nodename == "color" && xmlreader.isStartElement())
    {
        while (!(nodename == "color" && xmlreader.isEndElement()))
            item_para.color = xmlreader.readElementText();
    }
    else if (nodename == "calNumber" && xmlreader.isStartElement())
    {
        while (!(nodename == "calNumber" && xmlreader.isEndElement()))
            item_para.name = xmlreader.readElementText().toInt();
    }
    else if (nodename == "rotate" && xmlreader.isStartElement())
    {
        while (!(nodename == "rotate" && xmlreader.isEndElement()))
        {
            xmlreader.readNextStartElement();
            nodename = xmlreader.name().toString();
            if (nodename == "angle" && xmlreader.isStartElement())
            {
                while (!(nodename == "angle" && xmlreader.isEndElement()))
                    item_para.rotate_angle = xmlreader.readElementText().toDouble();
            }
            else if (nodename == "x" && xmlreader.isStartElement())
            {
                while (!(nodename == "x" && xmlreader.isEndElement()))
                    item_para.rotate_point.setX(xmlreader.readElementText().toDouble());
            }
            else if (nodename == "y" && xmlreader.isStartElement())
            {
                while (!(nodename == "y" && xmlreader.isEndElement()))
                    item_para.rotate_point.setY(xmlreader.readElementText().toDouble());
            }
        }
    }
}

void XmlIOstreamModel::addItemList(QList<GraphicsItem*>& items, GraphicsScene* scene)
{
    QList<GraphicsItem*> calculate_items; //测量线段item
    QList<GraphicsItem*> items_with_child; //带有item自身子图元的列表

    for (int i = 0; i < items.size(); i++)
    {
        GraphicsItem* item = items[i];
        item->getSetPointFunctionList().clear();
        if (item->data(1) != "CalculateLine") {
            //qDebug() << "first readItem..." << item->data(0);
            scene->addItemInit(item);
        }
        else calculate_items.push_back(item);
        items_with_child.push_back(item);

        QList<GraphicsItem*> child_item_list;
        item->findAllGraphicsChildItems(child_item_list);
        for (GraphicsItem* child_item : child_item_list)
            items_with_child.push_back(child_item);
    }
    for (GraphicsItem* cal_item : calculate_items) {
        switch (cal_item->type())
        {
        case CalculateLineType:
        {
            CalculateLine* cal_line = dynamic_cast<CalculateLine*>(cal_item);
            GraphicsItem* cal_dis1 = nullptr;
            GraphicsItem* cal_dis2 = nullptr;
            foreach(GraphicsItem * m_item, items_with_child) {
                if (m_item->getName() == cal_line->getStartName()) cal_dis1 = m_item;
                else if (m_item->getName() == cal_line->getEdgeName()) cal_dis2 = m_item;
                if (cal_dis1 && cal_dis2)break;
            }
            if (cal_dis1 != nullptr && cal_dis2 != nullptr) {
                cal_line->setCalItemStart(cal_dis1);
                cal_line->setCalItemEdge(cal_dis2);

                cal_line->setStart(scene->sceneRect().center());
                cal_line->setEdge(scene->sceneRect().center());

                scene->connect(cal_line->getCalItemStart(), &GraphicsItem::prepareToRemove, cal_line, &GraphicsItem::onActionRemoveSelf);
                scene->connect(cal_line->getCalItemEdge(), &GraphicsItem::prepareToRemove, cal_line, &GraphicsItem::onActionRemoveSelf);
                scene->addItemInit(cal_line);
            }
            else {
                qDebug() << "reading cal_center_line::not find item_name";
            }
            break;
        }
        case CalVerticalLineType:
        {
            CalVerticalLine* cal_line = dynamic_cast<CalVerticalLine*>(cal_item);
            GraphicsItem* cal_dis1 = nullptr;
            GraphicsItem* cal_dis2 = nullptr;
            QPointF Verticalpoint = QPointF(0, 0);
            foreach(GraphicsItem * m_item, items_with_child) {
                if (m_item->getName() == cal_line->getStartName()) cal_dis1 = m_item;
                if (m_item->getName() == cal_line->getEdgeName()) cal_dis2 = m_item;
            }
            if (cal_dis1 != nullptr && cal_dis2 != nullptr) {
                if (cal_dis1->data(0) == "BPoint") {
                    if (cal_dis2->data(0) == "BLine")
                    {
                        Verticalpoint = CoordinateCalculation::getPointToLineVerticalpoint
                        (cal_dis1->mapToScene(cal_dis1->getCenter()),
                            cal_dis2->mapToScene(cal_dis2->getStart()), cal_dis2->mapToScene(cal_dis2->getEdge()));
                        cal_line->setCalItemStart(cal_dis1);
                        cal_line->setCalItemEdge(cal_dis2);

                        cal_line->setStart(scene->sceneRect().center());
                        cal_line->setEdge(scene->sceneRect().center());

                    }
                }
                else if (cal_dis1->data(0) == "BLine") {
                    if (cal_dis2->data(0) == "BPoint")
                    {
                        Verticalpoint = CoordinateCalculation::getPointToLineVerticalpoint
                        (cal_dis2->mapToScene(cal_dis2->getCenter()),
                            cal_dis1->mapToScene(cal_dis1->getStart()), cal_dis1->mapToScene(cal_dis1->getEdge()));
                        cal_line->setCalItemStart(cal_dis2);
                        cal_line->setCalItemEdge(cal_dis1);

                        cal_line->setStart(scene->sceneRect().center());
                        cal_line->setEdge(scene->sceneRect().center());

                    }
                }
                scene->connect(cal_line->getCalItemStart(), &GraphicsItem::prepareToRemove, cal_line, &GraphicsItem::onActionRemoveSelf);
                scene->connect(cal_line->getCalItemEdge(), &GraphicsItem::prepareToRemove, cal_line, &GraphicsItem::onActionRemoveSelf);
                scene->addItemInit(cal_line);
            }
            else {
                qDebug() << "reading cal_ver_line::not find item_name";
            }
            break;
        }
        case CalLineAngleLineType:
        {
            CalLineAngleLine* cal_line = dynamic_cast<CalLineAngleLine*>(cal_item);
            GraphicsItem* cal_dis1 = nullptr;
            GraphicsItem* cal_dis2 = nullptr;

            foreach(GraphicsItem * m_item, items_with_child) {
                if (m_item->getName() == cal_line->getStartName()) cal_dis1 = m_item;
                if (m_item->getName() == cal_line->getEdgeName()) cal_dis2 = m_item;
            }
            if (cal_dis1 != nullptr && cal_dis2 != nullptr) {

                cal_line->setCalItemStart(cal_dis1);
                cal_line->setCalItemEdge(cal_dis2);
                cal_line->setStart(scene->sceneRect().center());
                cal_line->setEdge(scene->sceneRect().center());

                scene->connect(cal_line->getCalItemStart(), &GraphicsItem::prepareToRemove, cal_line, &GraphicsItem::onActionRemoveSelf);
                scene->connect(cal_line->getCalItemEdge(), &GraphicsItem::prepareToRemove, cal_line, &GraphicsItem::onActionRemoveSelf);
                scene->addItemInit(cal_line);
            }
            else {
                qDebug() << "reading cal_angle_line::not find item_name";
            }
            break;
        }
        default:
            break;
        }
    }
    if (items.isEmpty())return;
    items.last()->setSelected(false);
}

GraphicsItem* XmlIOstreamModel::readBPointXml(QXmlStreamReader& xmlreader, QString& nodename,
    QList<GraphicsItem*>& items, GraphicsItemIoParameter& item_para,
    GiantImageItem* pixmap_item, QPointF& point_scale)
{
    BPoint* bpoint = new BPoint();
    QPointF itemPosToPixC;
    item_para.setGraphicsItemIoParameter(bpoint);
    while (!(nodename == "BPoint" && xmlreader.isEndElement()))
    {
        xmlreader.readNextStartElement();
        nodename = xmlreader.name().toString();
        if (nodename == "x" && xmlreader.isStartElement())
        {
            while (!(nodename == "x" && xmlreader.isEndElement()))
                itemPosToPixC.setX(xmlreader.readElementText().toDouble());
        }
        else if (nodename == "y" && xmlreader.isStartElement())
        {
            while (!(nodename == "y" && xmlreader.isEndElement()))
                itemPosToPixC.setY(xmlreader.readElementText().toDouble());
        }
        else if (nodename == "grab" && xmlreader.isStartElement())
        {
            while (!(nodename == "grab" && xmlreader.isEndElement()))
                bpoint->setIsGrab(xmlreader.readElementText().toInt());
        }
    }
    itemPosToPixC = QpointFMultiplication(itemPosToPixC, point_scale);
    bpoint->setCenter(bpoint->mapFromItem(pixmap_item, itemPosToPixC));
    items.append(bpoint);
    return bpoint;
}

GraphicsItem* XmlIOstreamModel::readDoubleEndPointLineXml(QXmlStreamReader& xmlreader, QString& nodename,
    QList<GraphicsItem*>& items, GraphicsItemIoParameter& item_para,
    GiantImageItem* pixmap_item, QPointF& point_scale)
{
    DoubleEndPointLine* bline = new DoubleEndPointLine();
    QPointF itemPosToPixS;
    QPointF itemPosToPixE;
    item_para.setGraphicsItemIoParameter(bline);
    while (!(nodename == "DoubleEndPointLine" && xmlreader.isEndElement()))
    {
        xmlreader.readNextStartElement();
        nodename = xmlreader.name().toString();
        if (nodename == "xmin" && xmlreader.isStartElement())
        {
            while (!(nodename == "xmin" && xmlreader.isEndElement()))
                itemPosToPixS.setX(xmlreader.readElementText().toDouble());
        }
        else if (nodename == "ymin" && xmlreader.isStartElement())
        {
            while (!(nodename == "ymin" && xmlreader.isEndElement()))
                itemPosToPixS.setY(xmlreader.readElementText().toDouble());
        }
        else if (nodename == "xmax" && xmlreader.isStartElement())
        {
            while (!(nodename == "xmax" && xmlreader.isEndElement()))
                itemPosToPixE.setX(xmlreader.readElementText().toDouble());
        }
        else if (nodename == "ymax" && xmlreader.isStartElement())
        {
            while (!(nodename == "ymax" && xmlreader.isEndElement()))
                itemPosToPixE.setY(xmlreader.readElementText().toDouble());
        }
    }
    bline->setStart(bline->mapFromItem(pixmap_item, QpointFMultiplication(itemPosToPixS, point_scale)));
    bline->setEdge(bline->mapFromItem(pixmap_item, QpointFMultiplication(itemPosToPixE, point_scale)));
    items.append(bline);
    return bline;
}

GraphicsItem* XmlIOstreamModel::readInteractionRectXml(QXmlStreamReader& xmlreader, QString& nodename,
    QList<GraphicsItem*>& items, GraphicsItemIoParameter& item_para, 
    GiantImageItem* pixmap_item, QPointF& point_scale)
{
    InteractionRect* rect = new InteractionRect();
    QPointF itemPosToPixS;
    QPointF itemPosToPixE;
    item_para.setGraphicsItemIoParameter(rect);
    while (!(nodename == "bndbox" && xmlreader.isEndElement()))
    {
        xmlreader.readNextStartElement();
        nodename = xmlreader.name().toString();
        if (nodename == "xmin" && xmlreader.isStartElement())
        {
            while (!(nodename == "xmin" && xmlreader.isEndElement()))
                itemPosToPixS.setX(xmlreader.readElementText().toDouble());
        }
        else if (nodename == "ymin" && xmlreader.isStartElement())
        {
            while (!(nodename == "ymin" && xmlreader.isEndElement()))
                itemPosToPixS.setY(xmlreader.readElementText().toDouble());
        }
        else if (nodename == "xmax" && xmlreader.isStartElement())
        {
            while (!(nodename == "xmax" && xmlreader.isEndElement()))
                itemPosToPixE.setX(xmlreader.readElementText().toDouble());
        }
        else if (nodename == "ymax" && xmlreader.isStartElement())
        {
            while (!(nodename == "ymax" && xmlreader.isEndElement()))
                itemPosToPixE.setY(xmlreader.readElementText().toDouble());
        }
    }
    rect->setStart(rect->mapFromItem(pixmap_item, QpointFMultiplication(itemPosToPixS, point_scale)));
    rect->setEdge(rect->mapFromItem(pixmap_item, QpointFMultiplication(itemPosToPixE, point_scale)));
    items.push_back(rect);
    return rect;
}

GraphicsItem* XmlIOstreamModel::readInteractionEllipseXml(QXmlStreamReader& xmlreader, QString& nodename,
    QList<GraphicsItem*>& items, GraphicsItemIoParameter& item_para,
    GiantImageItem* pixmap_item, QPointF& point_scale)
{
    InteractionEllipse* bellipse = new InteractionEllipse();
    QPointF itemPosToPixS;
    QPointF itemPosToPixE;
    item_para.setGraphicsItemIoParameter(bellipse);
    while (!(nodename == "InteractionEllipse" && xmlreader.isEndElement()))
    {
        xmlreader.readNextStartElement();
        nodename = xmlreader.name().toString();
        if (nodename == "xmin" && xmlreader.isStartElement())
        {
            while (!(nodename == "xmin" && xmlreader.isEndElement()))
                itemPosToPixS.setX(xmlreader.readElementText().toDouble());
        }
        else if (nodename == "ymin" && xmlreader.isStartElement())
        {
            while (!(nodename == "ymin" && xmlreader.isEndElement()))
                itemPosToPixS.setY(xmlreader.readElementText().toDouble());
        }
        else if (nodename == "xmax" && xmlreader.isStartElement())
        {
            while (!(nodename == "xmax" && xmlreader.isEndElement()))
                itemPosToPixE.setX(xmlreader.readElementText().toDouble());
        }
        else if (nodename == "ymax" && xmlreader.isStartElement())
        {
            while (!(nodename == "ymax" && xmlreader.isEndElement()))
                itemPosToPixE.setY(xmlreader.readElementText().toDouble());
        }
    }

    bellipse->setStart(bellipse->mapFromItem(pixmap_item, QpointFMultiplication(itemPosToPixS, point_scale)));
    bellipse->setEdge(bellipse->mapFromItem(pixmap_item, QpointFMultiplication(itemPosToPixE, point_scale)));
    items.append(bellipse);
    return bellipse;
}

GraphicsItem* XmlIOstreamModel::readInteractionRoundXml(QXmlStreamReader& xmlreader, QString& nodename,
    QList<GraphicsItem*>& items, GraphicsItemIoParameter& item_para,
    GiantImageItem* pixmap_item, QPointF& point_scale)
{
    InteractionRound* round = new InteractionRound();
    QPointF itemPosToPixE;
    QPointF itemPosToPixC;
    item_para.setGraphicsItemIoParameter(round);
    while (!(nodename == "InteractionRound" && xmlreader.isEndElement()))
    {
        xmlreader.readNextStartElement();
        nodename = xmlreader.name().toString();
        if (nodename == "centreX" && xmlreader.isStartElement())
        {
            while (!(nodename == "centreX" && xmlreader.isEndElement()))
                itemPosToPixC.setX(xmlreader.readElementText().toDouble());
        }
        else if (nodename == "centreY" && xmlreader.isStartElement())
        {
            while (!(nodename == "centreY" && xmlreader.isEndElement()))
                itemPosToPixC.setY(xmlreader.readElementText().toDouble());
        }
        else if (nodename == "edgeX" && xmlreader.isStartElement())
        {
            while (!(nodename == "edgeX" && xmlreader.isEndElement()))
                itemPosToPixE.setX(xmlreader.readElementText().toDouble());
        }
        else if (nodename == "edgeY" && xmlreader.isStartElement())
        {
            while (!(nodename == "edgeY" && xmlreader.isEndElement()))
                itemPosToPixE.setY(xmlreader.readElementText().toDouble());
        }
    }
    itemPosToPixC = QpointFMultiplication(itemPosToPixC, point_scale);
    itemPosToPixE = QpointFMultiplication(itemPosToPixE, point_scale);
    round->setCenter(round->mapFromItem(pixmap_item, itemPosToPixC));
    round->setEdge(round->mapFromItem(pixmap_item, itemPosToPixE));
    items.append(round);
    return round;
}

GraphicsItem* XmlIOstreamModel::readPolygonXml(QXmlStreamReader& xmlreader, QString& nodename,
    QList<GraphicsItem*>& items, GraphicsItemIoParameter& item_para,
    GiantImageItem* pixmap_item, QPointF& point_scale)
{
    InteractionPolygon* polygon_item = new InteractionPolygon();
    QList<ItemPointF> map_to_pixmap_list;
    item_para.setGraphicsItemIoParameter(polygon_item);
    int index = 0;
    while (!(nodename == "polygon" && xmlreader.isEndElement()))
    {
        xmlreader.readNextStartElement();
        nodename = xmlreader.name().toString();
        QPointF p;
        if (nodename == "x" + QString::number(index) && xmlreader.isStartElement())
        {
            while (!(nodename == "x" + QString::number(index) && xmlreader.isEndElement()))
                p.setX(xmlreader.readElementText().toDouble());
        }
        if (nodename != "polygon")
        {
            xmlreader.readNextStartElement();
            nodename = xmlreader.name().toString();
            if (nodename == "y" + QString::number(index) && xmlreader.isStartElement())
            {
                while (!(nodename == "y" + QString::number(index) && xmlreader.isEndElement()))
                    p.setY(xmlreader.readElementText().toDouble());
            }
            p = polygon_item->mapFromItem(pixmap_item, QpointFMultiplication(p, point_scale));
            map_to_pixmap_list.append(p);
            index++;
        }
    }
    polygon_item->setPointList(map_to_pixmap_list);
    polygon_item->setIsCreateFinished(true);
    polygon_item->getGraphicsRelationModel().setHideChildListWhenHoverLeave(true);
    polygon_item->replaceWithItem();
    polygon_item->setBodRect(CoordinateCalculation::getListRect(map_to_pixmap_list));
    items.append(polygon_item);
    return polygon_item;
}

GraphicsItem* XmlIOstreamModel::readLineSegmentXml(QXmlStreamReader& xmlreader, QString& nodename,
    QList<GraphicsItem*>& items, GraphicsItemIoParameter& item_para,
    GiantImageItem* pixmap_item, QPointF& point_scale)
{
    LineSegment* lineseg_item = new LineSegment();
    QList<ItemPointF> map_to_pixmap_list;
    item_para.setGraphicsItemIoParameter(lineseg_item);
    int index = 0;
    while (!(nodename == "LineSegment" && xmlreader.isEndElement()))
    {
        xmlreader.readNextStartElement();
        nodename = xmlreader.name().toString();
        QPointF p;
        if (nodename == "x" + QString::number(index) && xmlreader.isStartElement())
        {
            while (!(nodename == "x" + QString::number(index) && xmlreader.isEndElement()))
                p.setX(xmlreader.readElementText().toDouble());
        }
        if (nodename != "LineSegment")
        {
            xmlreader.readNextStartElement();
            nodename = xmlreader.name().toString();
            if (nodename == "y" + QString::number(index) && xmlreader.isStartElement())
            {
                while (!(nodename == "y" + QString::number(index) && xmlreader.isEndElement()))
                    p.setY(xmlreader.readElementText().toDouble());
            }
            p = lineseg_item->mapFromItem(pixmap_item, QpointFMultiplication(p, point_scale));
            map_to_pixmap_list.append(p);
            index++;
        }
    }
    lineseg_item->setPointList(map_to_pixmap_list);
    lineseg_item->setIsCreateFinished(true);
    lineseg_item->replaceWithItem();
    lineseg_item->setBodRect(CoordinateCalculation::getListRect(map_to_pixmap_list));
    items.append(lineseg_item);
    return lineseg_item;
}

GraphicsItem* XmlIOstreamModel::readInteractionPieXml(QXmlStreamReader& xmlreader, QString& nodename,
    QList<GraphicsItem*>& items, GraphicsItemIoParameter& item_para,
    GiantImageItem* pixmap_item, QPointF& point_scale)
{
    InteractionPie* pie = new InteractionPie();
    QPointF itemPosToPixS;
    QPointF itemPosToPixE;
    QPointF itemPosToPixC;
    item_para.setGraphicsItemIoParameter(pie);
    while (!(nodename == "InteractionPie" && xmlreader.isEndElement()))
    {
        xmlreader.readNextStartElement();
        nodename = xmlreader.name().toString();
        if (nodename == "centreX" && xmlreader.isStartElement())
        {
            while (!(nodename == "centreX" && xmlreader.isEndElement()))
                itemPosToPixC.setX(xmlreader.readElementText().toDouble());
        }
        else if (nodename == "centreY" && xmlreader.isStartElement())
        {
            while (!(nodename == "centreY" && xmlreader.isEndElement()))
                itemPosToPixC.setY(xmlreader.readElementText().toDouble());
        }
        else if (nodename == "edgeX" && xmlreader.isStartElement())
        {
            while (!(nodename == "edgeX" && xmlreader.isEndElement()))
                itemPosToPixE.setX(xmlreader.readElementText().toDouble());
        }
        else if (nodename == "edgeY" && xmlreader.isStartElement())
        {
            while (!(nodename == "edgeY" && xmlreader.isEndElement()))
                itemPosToPixE.setY(xmlreader.readElementText().toDouble());
        }
        else if (nodename == "angleX" && xmlreader.isStartElement())
        {
            while (!(nodename == "angleX" && xmlreader.isEndElement()))
                itemPosToPixS.setX(xmlreader.readElementText().toDouble());
        }
        else if (nodename == "angleY" && xmlreader.isStartElement())
        {
            while (!(nodename == "angleY" && xmlreader.isEndElement()))
                itemPosToPixS.setY(xmlreader.readElementText().toDouble());
        }
    }
    itemPosToPixC = QpointFMultiplication(itemPosToPixC, point_scale);
    itemPosToPixE = QpointFMultiplication(itemPosToPixE, point_scale);
    itemPosToPixS = QpointFMultiplication(itemPosToPixS, point_scale);
    pie->setCenter(pie->mapFromItem(pixmap_item, itemPosToPixC));
    pie->setEdge(pie->mapFromItem(pixmap_item, itemPosToPixE));
    pie->setStart(pie->mapFromItem(pixmap_item, itemPosToPixS));
    items.append(pie);
    return pie;
}

GraphicsItem* XmlIOstreamModel::readAngleXml(QXmlStreamReader& xmlreader, QString& nodename,
    QList<GraphicsItem*>& items, GraphicsItemIoParameter& item_para,
    GiantImageItem* pixmap_item, QPointF& point_scale)
{
    Angle* angle_item = new Angle();
    QPointF itemPosToPixC;
    item_para.setGraphicsItemIoParameter(angle_item);
    QPointF line1s, line2s;
    while (!(nodename == "Angle" && xmlreader.isEndElement()))
    {
        xmlreader.readNextStartElement();
        nodename = xmlreader.name().toString();
        if (nodename == "centreX" && xmlreader.isStartElement())
        {
            while (!(nodename == "centreX" && xmlreader.isEndElement()))
                itemPosToPixC.setX(xmlreader.readElementText().toDouble());
        }
        else if (nodename == "centreY" && xmlreader.isStartElement())
        {
            while (!(nodename == "centreY" && xmlreader.isEndElement()))
                itemPosToPixC.setY(xmlreader.readElementText().toDouble());
        }
        else if (nodename == "line1X" && xmlreader.isStartElement())
        {
            while (!(nodename == "line1X" && xmlreader.isEndElement()))
                line1s.setX(xmlreader.readElementText().toDouble());
        }
        else if (nodename == "line1Y" && xmlreader.isStartElement())
        {
            while (!(nodename == "line1Y" && xmlreader.isEndElement()))
                line1s.setY(xmlreader.readElementText().toDouble());
        }
        else if (nodename == "line2X" && xmlreader.isStartElement())
        {
            while (!(nodename == "line2X" && xmlreader.isEndElement()))
                line2s.setX(xmlreader.readElementText().toDouble());
        }
        else if (nodename == "line2Y" && xmlreader.isStartElement())
        {
            while (!(nodename == "line2Y" && xmlreader.isEndElement()))
                line2s.setY(xmlreader.readElementText().toDouble());
        }
    }
    itemPosToPixC = QpointFMultiplication(itemPosToPixC, point_scale);
    line1s = QpointFMultiplication(line1s, point_scale);
    line2s = QpointFMultiplication(line2s, point_scale);
    angle_item->setCenter(angle_item->mapFromItem(pixmap_item, itemPosToPixC));
    angle_item->getLine1()->setEdge(angle_item->mapFromItem(pixmap_item, line1s));
    angle_item->getLine2()->setEdge(angle_item->mapFromItem(pixmap_item, line2s));
    items.append(angle_item);
    return angle_item;
}

GraphicsItem* XmlIOstreamModel::readParallelLineXml(QXmlStreamReader& xmlreader, QString& nodename,
    QList<GraphicsItem*>& items, GraphicsItemIoParameter& item_para,
    GiantImageItem* pixmap_item, QPointF& point_scale)
{
    ParallelLine* parallel_line = new ParallelLine();
    QPointF itemPosToPixS;
    QPointF itemPosToPixE;
    item_para.setGraphicsItemIoParameter(parallel_line);
    QPointF line1s, line1e;
    while (!(nodename == "ParallelLine" && xmlreader.isEndElement()))
    {
        xmlreader.readNextStartElement();
        nodename = xmlreader.name().toString();
        if (nodename == "x1min" && xmlreader.isStartElement())
        {
            while (!(nodename == "x1min" && xmlreader.isEndElement()))
                itemPosToPixS.setX(xmlreader.readElementText().toDouble());
        }
        else if (nodename == "y1min" && xmlreader.isStartElement())
        {
            while (!(nodename == "y1min" && xmlreader.isEndElement()))
                itemPosToPixS.setY(xmlreader.readElementText().toDouble());
        }
        else if (nodename == "x1max" && xmlreader.isStartElement())
        {
            while (!(nodename == "x1max" && xmlreader.isEndElement()))
                itemPosToPixE.setX(xmlreader.readElementText().toDouble());
        }
        else if (nodename == "y1max" && xmlreader.isStartElement())
        {
            while (!(nodename == "y1max" && xmlreader.isEndElement()))
                itemPosToPixE.setY(xmlreader.readElementText().toDouble());
        }
        else if (nodename == "x2min" && xmlreader.isStartElement())
        {
            while (!(nodename == "x2min" && xmlreader.isEndElement()))
                line1s.setX(xmlreader.readElementText().toDouble());
        }
        else if (nodename == "y2min" && xmlreader.isStartElement())
        {
            while (!(nodename == "y2min" && xmlreader.isEndElement()))
                line1s.setY(xmlreader.readElementText().toDouble());
        }
        else if (nodename == "x2max" && xmlreader.isStartElement())
        {
            while (!(nodename == "x2max" && xmlreader.isEndElement()))
                line1e.setX(xmlreader.readElementText().toDouble());
        }
        else if (nodename == "y2max" && xmlreader.isStartElement())
        {
            while (!(nodename == "y2max" && xmlreader.isEndElement()))
                line1e.setY(xmlreader.readElementText().toDouble());
        }
    }
    line1s = QpointFMultiplication(line1s, point_scale);
    line1e = QpointFMultiplication(line1e, point_scale);
    itemPosToPixS = QpointFMultiplication(itemPosToPixS, point_scale);
    itemPosToPixE = QpointFMultiplication(itemPosToPixE, point_scale);
    parallel_line->setStart(parallel_line->mapFromItem(pixmap_item, itemPosToPixS));
    parallel_line->setEdge(parallel_line->mapFromItem(pixmap_item, itemPosToPixE));
    parallel_line->getAuxiliaryLine()->setStart(parallel_line->mapFromItem(pixmap_item, line1s));
    parallel_line->getAuxiliaryLine()->setEdge(parallel_line->mapFromItem(pixmap_item, line1e));
    parallel_line->getAuxiliaryLine()->setVisible(true);
    items.append(parallel_line);
    return parallel_line;
}

GraphicsItem* XmlIOstreamModel::readCalculateLineXml(QXmlStreamReader& xmlreader, QString& nodename,
    QList<GraphicsItem*>& items, GraphicsItemIoParameter& item_para)
{
    CalculateLine* cal_line = new CalculateLine();
    item_para.setGraphicsItemIoParameter(cal_line);
    while (!(nodename == "CalCentreLine" && xmlreader.isEndElement()))
    {
        xmlreader.readNextStartElement();
        nodename = xmlreader.name().toString();
        if (nodename == "calItemNumber" && xmlreader.isStartElement())
        {
            while (!(nodename == "calItemNumber" && xmlreader.isEndElement()))
            {
                xmlreader.readNextStartElement();
                nodename = xmlreader.name().toString();
                if (nodename == "calItem" + QString::number(1) && xmlreader.isStartElement())
                {
                    while (!(nodename == "calItem" + QString::number(1) && xmlreader.isEndElement()))
                        cal_line->setStartName(xmlreader.readElementText().toInt());
                }
                else if (nodename == "calItem" + QString::number(2) && xmlreader.isStartElement())
                {
                    while (!(nodename == "calItem" + QString::number(2) && xmlreader.isEndElement()))
                        cal_line->setEdgeName(xmlreader.readElementText().toInt());
                }
            }
        }
    }
    items.append(cal_line);
    return cal_line;
}

GraphicsItem* XmlIOstreamModel::readCalVerticalLineXml(QXmlStreamReader& xmlreader, QString& nodename,
    QList<GraphicsItem*>& items, GraphicsItemIoParameter& item_para)
{
    CalVerticalLine* cal_ver_line = new CalVerticalLine();
    item_para.setGraphicsItemIoParameter(cal_ver_line);
    while (!(nodename == "CalVerticalLine" && xmlreader.isEndElement()))
    {
        xmlreader.readNextStartElement();
        nodename = xmlreader.name().toString();
        if (nodename == "calItemNumber" && xmlreader.isStartElement())
        {
            while (!(nodename == "calItemNumber" && xmlreader.isEndElement()))
            {
                xmlreader.readNextStartElement();
                nodename = xmlreader.name().toString();
                if (nodename == "calItem" + QString::number(1) && xmlreader.isStartElement())
                {
                    while (!(nodename == "calItem" + QString::number(1) && xmlreader.isEndElement()))
                        cal_ver_line->setStartName(xmlreader.readElementText().toInt());
                }
                else if (nodename == "calItem" + QString::number(2) && xmlreader.isStartElement())
                {
                    while (!(nodename == "calItem" + QString::number(2) && xmlreader.isEndElement()))
                        cal_ver_line->setEdgeName(xmlreader.readElementText().toInt());
                }
            }
        }
    }
    items.append(cal_ver_line);
    return cal_ver_line;
}

GraphicsItem* XmlIOstreamModel::readCalLineAngleLineXml(QXmlStreamReader& xmlreader, QString& nodename,
    QList<GraphicsItem*>& items, GraphicsItemIoParameter& item_para)
{
    CalLineAngleLine* cal_angle_line = new CalLineAngleLine();
    item_para.setGraphicsItemIoParameter(cal_angle_line);
    while (!(nodename == "CalLineAngleLine" && xmlreader.isEndElement()))
    {
        xmlreader.readNextStartElement();
        nodename = xmlreader.name().toString();
        if (nodename == "calItemNumber" && xmlreader.isStartElement())
        {
            while (!(nodename == "calItemNumber" && xmlreader.isEndElement()))
            {
                xmlreader.readNextStartElement();
                nodename = xmlreader.name().toString();
                if (nodename == "calItem" + QString::number(1) && xmlreader.isStartElement())
                {
                    while (!(nodename == "calItem" + QString::number(1) && xmlreader.isEndElement()))
                        cal_angle_line->setStartName(xmlreader.readElementText().toInt());
                }
                else if (nodename == "calItem" + QString::number(2) && xmlreader.isStartElement())
                {
                    while (!(nodename == "calItem" + QString::number(2) && xmlreader.isEndElement()))
                        cal_angle_line->setEdgeName(xmlreader.readElementText().toInt());
                }
            }
        }
    }
    items.append(cal_angle_line);
    return cal_angle_line;
}

/***************保存相关私有函数*******************/

bool XmlIOstreamModel::createXmlElement(QDomDocument& domDoc, QDomElement& rootElement, 
    const QString& childName, const QString& data)
{
    QDomElement folderElement = domDoc.createElement(childName);  //创建节点folder
    QDomText folderText = domDoc.createTextNode(data);
    folderElement.appendChild(folderText);     //将子节点数据绑定
    rootElement.appendChild(folderElement);  //将子节点绑定到父节点
    return true;
}

void XmlIOstreamModel::createGraphicsItemName(QList<GraphicsItem*>& sort_item_list, GraphicsScene* scene)
{
    //保存前戏 先编号
    int index = 0;
    foreach(QGraphicsItem * item, scene->items())
    {
        GraphicsItem* m_item = dynamic_cast<GraphicsItem*>(item);
        if(!m_item)continue;
        m_item->setName(index);
        index++;
        if (m_item->parentItem() == nullptr)sort_item_list.append(m_item);
    }
}

void XmlIOstreamModel::createSaveXmlHeader(QDomDocument& domDoc, QDomElement& annotation_element, GraphicsScene* scene)
{
    QDomProcessingInstruction version;
    version = domDoc.createProcessingInstruction("xml", "version = \"1.0\" encoding = \"UTF-8\"");
    domDoc.appendChild(version);

    GiantImageItem* pixmap_item = scene->getPixmapItem();
    QFileInfo info = QFileInfo(scene->getPixmapItem()->getImagePath());

    createXmlElement(domDoc, annotation_element, "folder", 
        FilePathOperation::getFileFolder(pixmap_item->getImagePath())); //创建节点folder
    createXmlElement(domDoc, annotation_element, "filename", 
        info.fileName()); //创建节点filename
    createXmlElement(domDoc, annotation_element, "path", 
        pixmap_item->getImagePath()); //创建节点path

    QDomElement source_element = domDoc.createElement("source");  //创建父节点source
    {
        createXmlElement(domDoc, source_element, "database", "Unknown"); //创建节点source
    }
    QDomElement size_Element = domDoc.createElement("size");  //创建父节点size
    {
        createXmlElement(domDoc, size_Element, "width", 
            QString::number(pixmap_item->getOriginWidth())); //创建节点width
        createXmlElement(domDoc, size_Element, "height", 
            QString::number(pixmap_item->getOriginHeight())); //创建节点height
        createXmlElement(domDoc, size_Element, "depth", 
            QString::number(3)); //创建节点depth(需要处理)
    }
    annotation_element.appendChild(source_element); //绑定到顶结点
    annotation_element.appendChild(size_Element); //绑定到顶结点

    createXmlElement(domDoc, annotation_element, "segmented", 
        QString::number(0)); //创建节点segmented(需要处理)

    return;
}

void XmlIOstreamModel::createSaveXmlGraphicsItemInfo(QDomDocument& domDoc, QDomElement& domRootElement,
    QList<GraphicsItem*>& sort_item_list, GraphicsScene* scene)
{
    GiantImageItem* pixmap_item = scene->getPixmapItem();
    //计算item坐标系映射到图像坐标系的参数
    double m_fscale = pixmap_item->getOriginWidth() / (pixmap_item->getFscaleW() + EPS);
    QPointF point_scale(m_fscale, m_fscale);

    foreach(GraphicsItem* item, sort_item_list)
    {
        QPointF start = item->getStartMeasurePos();
        QPointF edge = item->getEdgeMeasurePos();
        QPointF center = item->getCenterMeasurePos();

        QDomElement objectElement = domDoc.createElement("object");  //创建父节点
        {
            {
                createXmlElement(domDoc, objectElement, "ID",
                    item->getGraphicsTextModel().getLabelID()); //创建节点ID

                createXmlElement(domDoc, objectElement, "name",
                    item->getGraphicsTextModel().getLabelText()); //创建节点name

                createXmlElement(domDoc, objectElement, "pose", "Unspecified"); //创建节点pose(需要处理)
                createXmlElement(domDoc, objectElement, "truncated",
                    QString::number(0)); //创建节点truncated(需要处理)

                createXmlElement(domDoc, objectElement, "difficult",
                    QString::number(0)); //创建节点difficult(需要处理)

                createXmlElement(domDoc, objectElement, "calNumber",
                    QString::number(item->getName())); //创建节点calNumber(需要处理)

                createXmlElement(domDoc, objectElement, "color",
                    item->getGraphicsPaintModel().getPenNoSelected().color().name()); //创建节点color

                QDomElement rotateElement = domDoc.createElement("rotate");           //创建子结点
                {
                    createXmlElement(domDoc, rotateElement, "angle",
                        QString::number(item->getGraphicsTransformModel().getRotateAngle())); //创建节点angle
                    createXmlElement(domDoc, rotateElement, "x",
                        QString::number(center.rx(), 'f', 0)); //创建节点x
                    createXmlElement(domDoc, rotateElement, "y",
                        QString::number(center.ry(), 'f', 0)); //创建节点y
                }
                objectElement.appendChild(rotateElement);
            }

            switch (item->type())
            {
            case BPointType:
            {
                saveBPointXml(domDoc, domRootElement, objectElement, item, center);
                break;
            }
            case InteractionRectType:
            {
                saveInteractionRectXml(domDoc, domRootElement, objectElement, item, start, edge);
                break;
            }
            case DoubleEndPointLineType:
            {
                saveDoubleEndPointLineXml(domDoc, domRootElement, objectElement, item, start, edge);
                break;
            }
            case InteractionEllipseType:
            {
                saveInteractionEllipseXml(domDoc, domRootElement, objectElement, item, start, edge);
                break;
            }
            case InteractionRoundType:
            {
                saveInteractionRoundXml(domDoc, domRootElement, objectElement, item, center, edge);
                break;
            }
            case PolygonType:
            {
                savePolygonXml(domDoc, domRootElement, objectElement, item, pixmap_item, point_scale);
                break;
            }
            case LineSegmentType:
            {
                saveLineSegmentXml(domDoc, domRootElement, objectElement, item, pixmap_item, point_scale);
                break;
            }
            case InteractionPieType:
            {
                saveInteractionPieXml(domDoc, domRootElement, objectElement, item, start, center, edge);
                break;
            }
            case AngleType:
            {   
                saveInteractionAngleXml(domDoc, domRootElement, objectElement, item, center, pixmap_item, point_scale);
                break;
            }
            case ParallelLineType:
            {
                saveParallelLineXml(domDoc, domRootElement, objectElement, item, 
                start, edge, pixmap_item, point_scale);
                break;
            }
            case CalculateLineType:
            {
                saveCalculateLineXml(domDoc, domRootElement, objectElement, item);
                break;
            }
            case CalVerticalLineType:
            {
                saveCalVerticalLineXml(domDoc, domRootElement, objectElement, item);
                break;
            }
            case CalLineAngleLineType:
            {
                saveCalLineAngleLineXml(domDoc, domRootElement, objectElement, item);
                break;
            }
            }
        }
    }
}

/***************所有需要保存的item信息*******************/
void XmlIOstreamModel::saveBPointXml(QDomDocument& domDoc, QDomElement& domRootElement, 
    QDomElement& objectElement, GraphicsItem* item, QPointF& center)
{
    QDomElement BpointElement = domDoc.createElement("BPoint");  //创建父节点
    {
        createXmlElement(domDoc, BpointElement, "x",
            QString::number(center.rx(), 'f', 0)); //创建节点x
        createXmlElement(domDoc, BpointElement, "y",
            QString::number(center.ry(), 'f', 0)); //创建节点y
        createXmlElement(domDoc, BpointElement, "grab",
            QString::number(dynamic_cast<BPoint*>(item)->getIsGrab())); //创建节点grab
    }
    objectElement.appendChild(BpointElement); //绑定到顶结点
    domRootElement.appendChild(objectElement); //绑定到顶结点
}

void XmlIOstreamModel::saveInteractionRectXml(QDomDocument& domDoc, QDomElement& domRootElement, 
    QDomElement& objectElement, GraphicsItem* item, QPointF& start , QPointF& edge)
{
    QDomElement bndboxElement = domDoc.createElement("bndbox");  //创建父节点
    {
        createXmlElement(domDoc, bndboxElement, "xmin",
            QString::number(start.x(), 'f', 0)); //创建节点xmin
        createXmlElement(domDoc, bndboxElement, "ymin",
            QString::number(start.y(), 'f', 0)); //创建节点ymin
        createXmlElement(domDoc, bndboxElement, "xmax",
            QString::number(edge.rx(), 'f', 0)); //创建节点xmax
        createXmlElement(domDoc, bndboxElement, "ymax",
            QString::number(edge.ry(), 'f', 0)); //创建节点ymax
    }
    InteractionRect* rect_item = dynamic_cast<InteractionRect*>(item);
    QDomElement childElement = domDoc.createElement("childCalNumber");  //创建父节点
    {
        createXmlElement(domDoc, childElement, "SPoint",
            QString::number(rect_item->getSPointItem()->getName()));
        createXmlElement(domDoc, childElement, "EPoint",
            QString::number(rect_item->getEPointItem()->getName()));
        createXmlElement(domDoc, childElement, "RtPoint",
            QString::number(rect_item->getRtPointItem()->getName()));
        createXmlElement(domDoc, childElement, "LbPoint",
            QString::number(rect_item->getLbPointItem()->getName()));
    }
    objectElement.appendChild(bndboxElement); //绑定到顶结点
    objectElement.appendChild(childElement);
    domRootElement.appendChild(objectElement); //绑定到顶结点
}

void XmlIOstreamModel::saveDoubleEndPointLineXml(QDomDocument& domDoc, QDomElement& domRootElement,
    QDomElement& objectElement, GraphicsItem* item, QPointF& start, QPointF& edge)
{
    QDomElement BLineElement = domDoc.createElement("DoubleEndPointLine");  //创建父节点
    {
        createXmlElement(domDoc, BLineElement, "xmin",
            QString::number(start.x(), 'f', 0)); //创建节点xmin
        createXmlElement(domDoc, BLineElement, "ymin",
            QString::number(start.y(), 'f', 0)); //创建节点ymin
        createXmlElement(domDoc, BLineElement, "xmax",
            QString::number(edge.x(), 'f', 0)); //创建节点xmax
        createXmlElement(domDoc, BLineElement, "ymax",
            QString::number(edge.y(), 'f', 0)); //创建节点ymax
    }
    DoubleEndPointLine* line_item = dynamic_cast<DoubleEndPointLine*>(item);
    QDomElement childElement = domDoc.createElement("childCalNumber");  //创建父节点
    {
        createXmlElement(domDoc, childElement, "SPoint",
            QString::number(line_item->getSPointItem()->getName()));
        createXmlElement(domDoc, childElement, "EPoint",
            QString::number(line_item->getEPointItem()->getName()));
    }
    objectElement.appendChild(BLineElement); //绑定到顶结点
    objectElement.appendChild(childElement); //绑定到顶结点
    domRootElement.appendChild(objectElement); //绑定到顶结点
}

void XmlIOstreamModel::saveInteractionEllipseXml(QDomDocument& domDoc, QDomElement& domRootElement,
    QDomElement& objectElement, GraphicsItem* item, QPointF& start, QPointF& edge)
{
    QDomElement BEllipseElement = domDoc.createElement("InteractionEllipse");  //创建父节点
    {
        createXmlElement(domDoc, BEllipseElement, "xmin",
            QString::number(start.rx(), 'f', 0)); //创建节点xmin
        createXmlElement(domDoc, BEllipseElement, "ymin",
            QString::number(start.ry(), 'f', 0)); //创建节点ymin
        createXmlElement(domDoc, BEllipseElement, "xmax",
            QString::number(edge.rx(), 'f', 0)); //创建节点xmax
        createXmlElement(domDoc, BEllipseElement, "ymax",
            QString::number(edge.ry(), 'f', 0)); //创建节点ymax
    }
    InteractionEllipse* elli_item = dynamic_cast<InteractionEllipse*>(item);
    QDomElement childElement = domDoc.createElement("childCalNumber");  //创建父节点
    {
        createXmlElement(domDoc, childElement, "SPoint",
            QString::number(elli_item->getSPointItem()->getName()));
        createXmlElement(domDoc, childElement, "EPoint",
            QString::number(elli_item->getEPointItem()->getName()));
        createXmlElement(domDoc, childElement, "RtPoint",
            QString::number(elli_item->getRtPointItem()->getName()));
        createXmlElement(domDoc, childElement, "LbPoint",
            QString::number(elli_item->getLbPointItem()->getName()));
    }
    objectElement.appendChild(BEllipseElement); //绑定到顶结点
    objectElement.appendChild(childElement);
    domRootElement.appendChild(objectElement); //绑定到顶结点
}

void XmlIOstreamModel::saveInteractionRoundXml(QDomDocument& domDoc, QDomElement& domRootElement,
    QDomElement& objectElement, GraphicsItem* item, QPointF& center, QPointF& edge)
{
    QDomElement BRoundElement = domDoc.createElement("InteractionRound");  //创建父节点
    {
        createXmlElement(domDoc, BRoundElement, "centreX",
            QString::number(center.rx(), 'f', 0)); //创建节点centreX
        createXmlElement(domDoc, BRoundElement, "centreY",
            QString::number(center.ry(), 'f', 0)); //创建节点centreY
        createXmlElement(domDoc, BRoundElement, "edgeX",
            QString::number(edge.rx(), 'f', 0)); //创建节点edgeX
        createXmlElement(domDoc, BRoundElement, "edgeY",
            QString::number(edge.ry(), 'f', 0)); //创建节点edgeY
    }
    InteractionRound* round_item = dynamic_cast<InteractionRound*>(item);
    QDomElement childElement = domDoc.createElement("childCalNumber");  //创建父节点
    {
        createXmlElement(domDoc, childElement, "CPoint",
            QString::number(round_item->getCPointItem()->getName()));
        createXmlElement(domDoc, childElement, "EPoint",
            QString::number(round_item->getEPointItem()->getName()));
    }
    objectElement.appendChild(BRoundElement); //绑定到顶结点
    objectElement.appendChild(childElement); //绑定到顶结点
    domRootElement.appendChild(objectElement); //绑定到顶结点
}

void XmlIOstreamModel::savePolygonXml(QDomDocument& domDoc, QDomElement& domRootElement,
    QDomElement& objectElement, GraphicsItem* item, GiantImageItem* pixmap_item, QPointF& point_scale)
{
    InteractionPolygon* painting_pol_item = dynamic_cast<InteractionPolygon*>(item);
    QDomElement polygonElement = domDoc.createElement("polygon");  //创建父节点
    {
        int index = 0;
        foreach(QPointF p, painting_pol_item->getPointList())
        {
            p = QpointFMultiplication(painting_pol_item->mapToItem(pixmap_item, p), point_scale);

            createXmlElement(domDoc, polygonElement, "x" + QString::number(index),
                QString::number(p.rx(), 'f', 0)); //创建节点x(index)

            createXmlElement(domDoc, polygonElement, "y" + QString::number(index),
                QString::number(p.ry(), 'f', 0)); //创建节点y(index)
            index++;
        }
    }

    QDomElement childElement = domDoc.createElement("childCalNumber");  //创建父节点
    int j = 0;
    foreach(PolygonPoint * l, painting_pol_item->getPointItemList())
    {
        createXmlElement(domDoc, childElement,
            "PolygonPoint" + QString::number(j), QString::number(l->getName())); //创建节点x(index)
        j++;
    }
    j = 0;
    foreach(PolygonLine * l, painting_pol_item->getLineList())
    {
        createXmlElement(domDoc, childElement,
            "PolygonLine" + QString::number(j), QString::number(l->getName())); //创建节点x(index)
        j++;
    }
    objectElement.appendChild(polygonElement); //绑定到顶结点
    objectElement.appendChild(childElement); //绑定到顶结点
    domRootElement.appendChild(objectElement); //绑定到顶结点
}

void XmlIOstreamModel::saveLineSegmentXml(QDomDocument& domDoc, QDomElement& domRootElement,
    QDomElement& objectElement, GraphicsItem* item, GiantImageItem* pixmap_item, QPointF& point_scale)
{
    LineSegment* lineseg_item = dynamic_cast<LineSegment*>(item);
    QDomElement linesegElement = domDoc.createElement("LineSegment");  //创建父节点
    {
        int index = 0;
        foreach(QPointF p, lineseg_item->getPointList())
        {
            p = QpointFMultiplication(lineseg_item->mapToItem(pixmap_item, p), point_scale);

            createXmlElement(domDoc, linesegElement,
                "x" + QString::number(index), QString::number(p.rx(), 'f', 0)); //创建节点x(index)

            createXmlElement(domDoc, linesegElement,
                "y" + QString::number(index), QString::number(p.ry(), 'f', 0)); //创建节点y(index)
            index++;
        }
    }

    QDomElement childElement = domDoc.createElement("childCalNumber");  //创建父节点
    int j = 0;
    foreach(PolygonPoint * l, lineseg_item->getPointItemList())
    {
        createXmlElement(domDoc, childElement,
            "LineSegmentPoint" + QString::number(j), QString::number(l->getName())); //创建节点x(index)
        j++;
    }
    j = 0;
    foreach(PolygonLine * l, lineseg_item->getLineList())
    {
        createXmlElement(domDoc, childElement,
            "LineSegmentLine" + QString::number(j), QString::number(l->getName())); //创建节点x(index)

        j++;
    }
    objectElement.appendChild(linesegElement); //绑定到顶结点
    objectElement.appendChild(childElement); //绑定到顶结点
    domRootElement.appendChild(objectElement); //绑定到顶结点
}

void XmlIOstreamModel::saveInteractionPieXml(QDomDocument& domDoc, QDomElement& domRootElement,
    QDomElement& objectElement, GraphicsItem* item, QPointF& start, QPointF& center, QPointF& edge)
{
    QDomElement PieElement = domDoc.createElement("InteractionPie");  //创建父节点
    {
        createXmlElement(domDoc, PieElement, "centreX",
            QString::number(center.rx(), 'f', 0)); //创建节点centreX
        createXmlElement(domDoc, PieElement, "centreY",
            QString::number(center.ry(), 'f', 0)); //创建节点centreY
        createXmlElement(domDoc, PieElement, "edgeX",
            QString::number(edge.rx(), 'f', 0)); //创建节点edgeX
        createXmlElement(domDoc, PieElement, "edgeY",
            QString::number(edge.ry(), 'f', 0)); //创建节点edgeY
        createXmlElement(domDoc, PieElement, "angleX",
            QString::number(start.rx(), 'f', 0)); //创建节点angleX
        createXmlElement(domDoc, PieElement, "angleY",
            QString::number(start.ry(), 'f', 0)); //创建节点angleY
    }
    InteractionPie* pie_item = dynamic_cast<InteractionPie*>(item);
    QDomElement childElement = domDoc.createElement("childCalNumber");  //创建父节点
    {
        createXmlElement(domDoc, childElement, "CPoint",
            QString::number(pie_item->getCPointItem()->getName()));
        createXmlElement(domDoc, childElement, "SPoint",
            QString::number(pie_item->getSPointItem()->getName()));
        createXmlElement(domDoc, childElement, "EPoint",
            QString::number(pie_item->getEPointItem()->getName()));
    }
    objectElement.appendChild(PieElement); //绑定到顶结点
    objectElement.appendChild(childElement); //绑定到顶结点
    domRootElement.appendChild(objectElement); //绑定到顶结点
}

void XmlIOstreamModel::saveInteractionAngleXml(QDomDocument& domDoc, QDomElement& domRootElement,
    QDomElement& objectElement, GraphicsItem* item, QPointF& center, 
    GiantImageItem* pixmap_item, QPointF& point_scale)
{
    Angle* angle = dynamic_cast<Angle*>(item);
    QDomElement AngleElement = domDoc.createElement("Angle");  //创建父节点
    {
        createXmlElement(domDoc, AngleElement, "centreX",
            QString::number(center.rx(), 'f', 0)); //创建节点centreX
        createXmlElement(domDoc, AngleElement, "centreY",
            QString::number(center.ry(), 'f', 0)); //创建节点centreY

        QPointF p1 = angle->mapToItem(pixmap_item, angle->getLine1()->getEdge());
        QPointF p2 = angle->mapToItem(pixmap_item, angle->getLine2()->getEdge());

        createXmlElement(domDoc, AngleElement, "line1X",
            QString::number(QpointFMultiplication(p1, point_scale).rx(), 'f', 0)); //创建节点line1X
        createXmlElement(domDoc, AngleElement, "line1Y",
            QString::number(QpointFMultiplication(p1, point_scale).ry(), 'f', 0)); //创建节点line1Y
        createXmlElement(domDoc, AngleElement, "line2X",
            QString::number(QpointFMultiplication(p2, point_scale).rx(), 'f', 0)); //创建节点line2X
        createXmlElement(domDoc, AngleElement, "line2Y",
            QString::number(QpointFMultiplication(p2, point_scale).ry(), 'f', 0)); //创建节点line2Y
    }

    QDomElement childElement = domDoc.createElement("childCalNumber");  //创建父节点
    {
        createXmlElement(domDoc, childElement, "AngleLine" + QString::number(1),
            QString::number(angle->getLine1()->getName())); //创建节点AngleLine1
        createXmlElement(domDoc, childElement, "AngleLine" + QString::number(2),
            QString::number(angle->getLine2()->getName())); //创建节点AngleLine2
        createXmlElement(domDoc, childElement, "CPoint",
            QString::number(angle->getCPointItem()->getName()));
        createXmlElement(domDoc, childElement, "AngleLine1P",
            QString::number(angle->getLine1()->getEPointItem()->getName()));
        createXmlElement(domDoc, childElement, "AngleLine2P",
            QString::number(angle->getLine2()->getEPointItem()->getName()));
    }
    objectElement.appendChild(AngleElement); //绑定到顶结点
    objectElement.appendChild(childElement); //绑定到顶结点
    domRootElement.appendChild(objectElement); //绑定到顶结点
}

void XmlIOstreamModel::saveParallelLineXml(QDomDocument& domDoc, QDomElement& domRootElement,
    QDomElement& objectElement, GraphicsItem* item, QPointF& start, QPointF& edge,
    GiantImageItem* pixmap_item, QPointF& point_scale)
{
    ParallelLine* para_item = dynamic_cast<ParallelLine*>(item);
    QDomElement ParallelLineElement = domDoc.createElement("ParallelLine");  //创建父节点
    {
        createXmlElement(domDoc, ParallelLineElement, "x1min",
            QString::number(start.rx(), 'f', 0)); //创建节点x1min
        createXmlElement(domDoc, ParallelLineElement, "y1min",
            QString::number(start.ry(), 'f', 0)); //创建节点y1min
        createXmlElement(domDoc, ParallelLineElement, "x1max",
            QString::number(edge.rx(), 'f', 0)); //创建节点x1max
        createXmlElement(domDoc, ParallelLineElement, "y1max",
            QString::number(edge.ry(), 'f', 0)); //创建节点y1max

        QPointF p1 = para_item->getAuxiliaryLine()->mapToItem(pixmap_item,
            para_item->getAuxiliaryLine()->getStart());
        QPointF p2 = para_item->getAuxiliaryLine()->mapToItem(pixmap_item,
            para_item->getAuxiliaryLine()->getEdge());

        createXmlElement(domDoc, ParallelLineElement, "x2min",
            QString::number(QpointFMultiplication(p1, point_scale).rx(), 'f', 0)); //创建节点x2min
        createXmlElement(domDoc, ParallelLineElement, "y2min",
            QString::number(QpointFMultiplication(p1, point_scale).ry(), 'f', 0)); //创建节点y2min
        createXmlElement(domDoc, ParallelLineElement, "x2max",
            QString::number(QpointFMultiplication(p2, point_scale).rx(), 'f', 0)); //创建节点x2max
        createXmlElement(domDoc, ParallelLineElement, "y2max",
            QString::number(QpointFMultiplication(p2, point_scale).ry(), 'f', 0)); //创建节点y2max
    }

    QDomElement childElement = domDoc.createElement("childCalNumber");  //创建父节点
    {
        createXmlElement(domDoc, childElement, "ParallelLineAnother",
            QString::number(para_item->getAuxiliaryLine()->getName())); //ParallelLineAnother
        createXmlElement(domDoc, childElement, "ParallelLineSPoint",
            QString::number(para_item->getSPointItem()->getName()));
        createXmlElement(domDoc, childElement, "ParallelLineEPoint",
            QString::number(para_item->getEPointItem()->getName()));
        createXmlElement(domDoc, childElement, "ParallelLineAnotherSPoint",
            QString::number(para_item->getAuxiliaryLine()->getSPointItem()->getName()));
        createXmlElement(domDoc, childElement, "ParallelLineAnotherEPoint",
            QString::number(para_item->getAuxiliaryLine()->getEPointItem()->getName()));
    }

    objectElement.appendChild(ParallelLineElement); //绑定到顶结点
    objectElement.appendChild(childElement); //绑定到顶结点
    domRootElement.appendChild(objectElement); //绑定到顶结点
}

void XmlIOstreamModel::saveCalculateLineXml(QDomDocument& domDoc, QDomElement& domRootElement,
    QDomElement& objectElement, GraphicsItem* item)
{
    CalculateLine* CalCentreLine = dynamic_cast<CalculateLine*>(item);
    QDomElement CalCentreLineElement = domDoc.createElement("CalCentreLine");  //创建父节点
    {
        QDomElement childElement = domDoc.createElement("calItemNumber");  //创建父节点
        {
            createXmlElement(domDoc, childElement, "calItem" + QString::number(1),
                QString::number(CalCentreLine->getCalItemStart()->getName())); //创建节点calItem1

            createXmlElement(domDoc, childElement, "calItem" + QString::number(2),
                QString::number(CalCentreLine->getCalItemEdge()->getName())); //创建节点calItem2
        }
        CalCentreLineElement.appendChild(childElement);
    }
    QDomElement childElement = domDoc.createElement("childCalNumber");  //创建父节点
    {
        createXmlElement(domDoc, childElement, "SPoint",
            QString::number(CalCentreLine->getSPointItem()->getName()));
        createXmlElement(domDoc, childElement, "EPoint",
            QString::number(CalCentreLine->getEPointItem()->getName()));
    }
    objectElement.appendChild(CalCentreLineElement); //绑定到顶结点
    objectElement.appendChild(childElement); //绑定到顶结点
    domRootElement.appendChild(objectElement); //绑定到顶结点
}

void XmlIOstreamModel::saveCalVerticalLineXml(QDomDocument& domDoc, QDomElement& domRootElement,
    QDomElement& objectElement, GraphicsItem* item)
{
    CalVerticalLine* cal = dynamic_cast<CalVerticalLine*>(item);
    QDomElement CalVerticalLineElement = domDoc.createElement("CalVerticalLine");  //创建父节点
    {
        QDomElement childElement = domDoc.createElement("calItemNumber");  //创建父节点
        {
            createXmlElement(domDoc, childElement, "calItem" + QString::number(1),
                QString::number(cal->getCalItemStart()->getName())); //创建节点calItem1

            createXmlElement(domDoc, childElement, "calItem" + QString::number(2),
                QString::number(cal->getCalItemEdge()->getName())); //创建节点calItem2
        }
        CalVerticalLineElement.appendChild(childElement);
    }
    QDomElement childElement = domDoc.createElement("childCalNumber");  //创建父节点
    {
        createXmlElement(domDoc, childElement, "SPoint",
            QString::number(cal->getSPointItem()->getName()));
        createXmlElement(domDoc, childElement, "EPoint",
            QString::number(cal->getEPointItem()->getName()));
    }
    objectElement.appendChild(CalVerticalLineElement); //绑定到顶结点
    objectElement.appendChild(childElement); //绑定到顶结点
    domRootElement.appendChild(objectElement); //绑定到顶结点
}

void XmlIOstreamModel::saveCalLineAngleLineXml(QDomDocument& domDoc, QDomElement& domRootElement,
    QDomElement& objectElement, GraphicsItem* item)
{
    CalLineAngleLine* cal = dynamic_cast<CalLineAngleLine*>(item);
    QDomElement CalLineAngleLineElement = domDoc.createElement("CalLineAngleLine");  //创建父节点
    {
        QDomElement childElement = domDoc.createElement("calItemNumber");  //创建父节点
        {
            createXmlElement(domDoc, childElement, "calItem" + QString::number(1),
                QString::number(cal->getCalItemStart()->getName())); //创建节点calItem1

            createXmlElement(domDoc, childElement, "calItem" + QString::number(2),
                QString::number(cal->getCalItemEdge()->getName())); //创建节点calItem2
        }
        CalLineAngleLineElement.appendChild(childElement);
    }
    QDomElement childElement = domDoc.createElement("childCalNumber");  //创建父节点
    {
        createXmlElement(domDoc, childElement, "SPoint",
            QString::number(cal->getSPointItem()->getName()));
        createXmlElement(domDoc, childElement, "EPoint",
            QString::number(cal->getEPointItem()->getName()));
    }
    objectElement.appendChild(CalLineAngleLineElement); //绑定到顶结点
    objectElement.appendChild(childElement); //绑定到顶结点
    domRootElement.appendChild(objectElement); //绑定到顶结点
}
