#pragma once
#include <QList>
class GraphicsScene;
class GraphicsItem;
class QDomDocument;
class QDomElement;
class QPointF;
class GraphicsPixmapItem;
class GraphicsItemIoParameter;
class QXmlStreamReader;

class XmlIOstreamModel
{
public:
	XmlIOstreamModel();
	~XmlIOstreamModel();
	//保存场景scene里的Graphicsitem到savepath
	bool saveItemToXmlFile(const QString&, GraphicsScene*); //pascalVoc Output
	bool readItemFromXmlFile(const QString&, GraphicsScene*); //pascalVoc Input

private:
	/***************保存相关私有函数*******************/
	/*******在domDoc节点下创建rootElement节点，并添加***********/
	bool createXmlElement(QDomDocument& domDoc, QDomElement& rootElement, 
		const QString& childName, const QString& data); //创建xml结点

	void createGraphicsItemName(QList<GraphicsItem*>&, GraphicsScene*); //保存前戏，给item编号
	void createSaveXmlHeader(QDomDocument& domDoc, QDomElement&, GraphicsScene*); //创建xml文件头信息,返回根节点引用

	void createSaveXmlGraphicsItemInfo(QDomDocument&, QDomElement&, 
		QList<GraphicsItem*>&, GraphicsScene*); //创建文件图元信息

	/***************读取相关私有函数*******************/
	void readGenericItemParameter(QXmlStreamReader&, QString&, GraphicsItemIoParameter&);
	void addItemList(QList<GraphicsItem*>&, GraphicsScene*);
	/***************读取item信息*******************/
	GraphicsItem* readBPointXml(QXmlStreamReader&, QString&, QList<GraphicsItem*>&,
		GraphicsItemIoParameter&, GraphicsPixmapItem*, QPointF&);
	GraphicsItem* readInteractionRectXml(QXmlStreamReader&, QString&, QList<GraphicsItem*>&,
		GraphicsItemIoParameter&, GraphicsPixmapItem*, QPointF&);
	GraphicsItem* readDoubleEndPointLineXml(QXmlStreamReader&, QString&, QList<GraphicsItem*>&,
		GraphicsItemIoParameter&, GraphicsPixmapItem*, QPointF&);
	GraphicsItem* readInteractionEllipseXml(QXmlStreamReader&, QString&, QList<GraphicsItem*>&,
		GraphicsItemIoParameter&, GraphicsPixmapItem*, QPointF&);
	GraphicsItem* readInteractionRoundXml(QXmlStreamReader&, QString&, QList<GraphicsItem*>&,
		GraphicsItemIoParameter&, GraphicsPixmapItem*, QPointF&);

	GraphicsItem* readPolygonXml(QXmlStreamReader&, QString&, QList<GraphicsItem*>&,
		GraphicsItemIoParameter&, GraphicsPixmapItem*, QPointF&);
	GraphicsItem* readLineSegmentXml(QXmlStreamReader&, QString&, QList<GraphicsItem*>&,
		GraphicsItemIoParameter&, GraphicsPixmapItem*, QPointF&);

	GraphicsItem* readInteractionPieXml(QXmlStreamReader&, QString&, QList<GraphicsItem*>&,
		GraphicsItemIoParameter&, GraphicsPixmapItem*, QPointF&);
	GraphicsItem* readAngleXml(QXmlStreamReader&, QString&, QList<GraphicsItem*>&,
		GraphicsItemIoParameter&, GraphicsPixmapItem*, QPointF&);
	GraphicsItem* readParallelLineXml(QXmlStreamReader&, QString&, QList<GraphicsItem*>&,
		GraphicsItemIoParameter&, GraphicsPixmapItem*, QPointF&);

	GraphicsItem* readCalculateLineXml(QXmlStreamReader&, QString&, QList<GraphicsItem*>&,
		GraphicsItemIoParameter&);
	GraphicsItem* readCalVerticalLineXml(QXmlStreamReader&, QString&, QList<GraphicsItem*>&,
		GraphicsItemIoParameter&);
	GraphicsItem* readCalLineAngleLineXml(QXmlStreamReader&, QString&, QList<GraphicsItem*>&,
		GraphicsItemIoParameter&);

	/***************保存item信息*******************/
	void saveBPointXml(QDomDocument&, QDomElement&, 
		QDomElement&, GraphicsItem*, QPointF&); 
	void saveInteractionRectXml(QDomDocument&, QDomElement&, 
		QDomElement&, GraphicsItem*, QPointF&, QPointF&);
	void saveDoubleEndPointLineXml(QDomDocument&, QDomElement&,
		QDomElement&, GraphicsItem*, QPointF&, QPointF&);
	void saveInteractionEllipseXml(QDomDocument&, QDomElement&,
		QDomElement&, GraphicsItem*, QPointF&, QPointF&);
	void saveInteractionRoundXml(QDomDocument&, QDomElement&,
		QDomElement&, GraphicsItem*, QPointF&, QPointF&);
	void savePolygonXml(QDomDocument&, QDomElement&,
		QDomElement&, GraphicsItem*, GraphicsPixmapItem*, QPointF&);
	void saveLineSegmentXml(QDomDocument&, QDomElement&,
		QDomElement&, GraphicsItem*, GraphicsPixmapItem*, QPointF&);
	void saveInteractionPieXml(QDomDocument&, QDomElement&,
		QDomElement&, GraphicsItem*, QPointF&, QPointF&, QPointF&);
	void saveInteractionAngleXml(QDomDocument&, QDomElement&,
		QDomElement&, GraphicsItem*, QPointF&, GraphicsPixmapItem*, QPointF&);
	void saveParallelLineXml(QDomDocument&, QDomElement&,
		QDomElement&, GraphicsItem*, QPointF&, QPointF&, GraphicsPixmapItem*, QPointF&);
	void saveCalculateLineXml(QDomDocument&, QDomElement&, QDomElement&, GraphicsItem*);
	void saveCalVerticalLineXml(QDomDocument&, QDomElement&, QDomElement&, GraphicsItem*);
	void saveCalLineAngleLineXml(QDomDocument&, QDomElement&, QDomElement&, GraphicsItem*);
};
