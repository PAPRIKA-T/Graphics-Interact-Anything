#pragma once
#include <QString>
#include "graphs/GraphicsTextItem.h"

#define DEFAULT_EXTRA_MESSAGE "????"
#define DEFAULT_LABEL "N/A"
#define DEFAULT_LABEL_ID "#00"
#define DEFAULT_ITEM_TYPE "GraphicsItem"

class GraphicsItem;
class GraphicsTextModel : public QObject
{
    Q_OBJECT
public:
	GraphicsTextModel(GraphicsItem*);
	~GraphicsTextModel();
    void initTextColorSetting();

    GraphicsTextItem* getTextItem();

    bool getIsHideText();
    void setIsHideText(bool ok);//设置是否隐藏文本
    bool getIsHideExtraText();
    void setIsHideExtraText(bool ok);
    void setIsHideIdText(bool ok);
    bool getIsHideIdText();
    void updateText();

    void setTextColorSelected(const QColor& c);//设置文本选中颜色
    QColor getTextColorSelected();//获取文本选中颜色
    void setTextColorNoSelected(const QColor& c);//设置文本未选中颜色
    QColor getTextColorNoSelected();//获取文本未选中颜色

    void setLabelID(const QString& s);
    const QString& getLabelID();
    QString getExtraMessage(); //获取extra_message信息
    void setExtraMessage(const QString& s); //设置extra_message信息
    QString getItemType(); //设置item_type
    void setItemType(const QString& s); //获取item_type
    const QString getLabelText(); //获取文本信息

signals:
    void IdChangeSignal(GraphicsItem* item, const QString& s); //ID改变信号
    void textChangeSignal(GraphicsItem* item, const QString& s); //文本改变信号

public slots:
    void onActionEditText(); //显示编辑文本框
    void setLabelText(const QString& string); //设置label文本

private:
    GraphicsTextItem* text_item; //文本item
    QString label_text = DEFAULT_LABEL; //文本
    QString ID_label = DEFAULT_LABEL_ID;
    QString item_type = DEFAULT_ITEM_TYPE;
    QString extra_text = DEFAULT_EXTRA_MESSAGE;

    QColor m_color_font_is_selected; //文本颜色选中
    QColor m_color_font_no_selected; //文本颜色未选中

    bool is_hide_extra_text = false; //是否隐藏额外信息文本
    bool is_hide_labelId_text = false; //是否隐藏labelID文本
    bool is_hide_text = false; //是否隐藏文本
};
