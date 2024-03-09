#include "GraphicsTextModel.h"
#include "graphs/Graphicsitem.h"
#include "widgets/graphicsItemEditTextDialog.h"

GraphicsTextModel::GraphicsTextModel(GraphicsItem* parent)
{
    // 文本item初始化
    text_item = new GraphicsTextItem(item_type);
    text_item->setParentItem(parent);
    QFont font("微软雅黑");
    font.setBold(false);
    font.setPointSize(8);
    text_item->setFont(font);
    text_item->setTextWidth(-1);
    text_item->setData(0, "GraphicsTextItem");
}

GraphicsTextModel::~GraphicsTextModel()
{
    delete text_item;
}

void GraphicsTextModel::initTextColorSetting()
{
    m_color_font_is_selected = QColor(0, 255, 0);
    m_color_font_no_selected = QColor(255, 255, 255);
    text_item->setDefaultTextColor(m_color_font_no_selected);
    text_item->setBackGroundColor(DEFAULT_COLOR_ITEM);
}

GraphicsTextItem* GraphicsTextModel::getTextItem()
{
    return text_item;
}

bool GraphicsTextModel::getIsHideText()
{
    return is_hide_text;
}

void GraphicsTextModel::setIsHideText(bool ok)
{
    is_hide_text = ok;
    text_item->setVisible(!ok);
}

bool GraphicsTextModel::getIsHideExtraText()
{
    return is_hide_extra_text;
}

void GraphicsTextModel::setIsHideExtraText(bool ok)
{
    is_hide_extra_text = ok; 
    updateText();
}

void GraphicsTextModel::setIsHideIdText(bool ok)
{
    is_hide_labelId_text = ok; 
    updateText();
}

bool GraphicsTextModel::getIsHideIdText()
{
    return is_hide_labelId_text;
}

void GraphicsTextModel::updateText()
{
    if (!is_hide_extra_text && !is_hide_labelId_text)
        text_item->setPlainText(ID_label + " " + label_text + "\n" + extra_text);
    else if (!is_hide_extra_text)
        text_item->setPlainText(label_text + "\n" + extra_text);
    else if (!is_hide_labelId_text)
        text_item->setPlainText(ID_label + " " + label_text);
    else
        text_item->setPlainText(label_text);
}

void GraphicsTextModel::setTextColorSelected(const QColor& c)
{
    m_color_font_is_selected = c;
}

QColor GraphicsTextModel::getTextColorSelected()
{
    return m_color_font_is_selected;
}

void GraphicsTextModel::setTextColorNoSelected(const QColor& c)
{
    m_color_font_no_selected = c;
}

QColor GraphicsTextModel::getTextColorNoSelected()
{
    return m_color_font_no_selected;
}

void GraphicsTextModel::setLabelID(const QString& s)
{
    GraphicsItem* par_item = dynamic_cast<GraphicsItem*>(text_item->parentItem());
    emit IdChangeSignal(par_item, s);
    ID_label = s; 
    updateText();
}

const QString& GraphicsTextModel::getLabelID()
{
    return ID_label;
}

QString GraphicsTextModel::getExtraMessage()
{
    return extra_text;
}

void GraphicsTextModel::setExtraMessage(const QString& s)
{
    extra_text = s; updateText();
}

QString GraphicsTextModel::getItemType()
{
    return item_type;
}

void GraphicsTextModel::setItemType(const QString& s)
{
    item_type = s;
}

const QString GraphicsTextModel::getLabelText()
{
    return label_text;
}

void GraphicsTextModel::setLabelText(const QString& string)
{
    GraphicsItem* par_item = dynamic_cast<GraphicsItem*>(text_item->parentItem());
    emit textChangeSignal(par_item, string);
    label_text = string;
    updateText();
}

void GraphicsTextModel::onActionEditText()
{
    graphicsItemEditTextDialog* dialog = new graphicsItemEditTextDialog();
    connect(dialog, SIGNAL(updateText(QString)), this, SLOT(setLabelText(QString)));
    dialog->show();
}