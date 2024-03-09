#pragma once

#include <QProxyStyle>
#include <QStyleOptionTab>
#include <QPainter>

class CustomTabStyle  : public QProxyStyle
{
	Q_OBJECT
public:
    //将原来的tab转置，重新绘制，实现到左右旁边，还可以在tabPosition中再次设置
    QSize sizeFromContents(ContentsType type, const QStyleOption* option,
        const QSize& size, const QWidget* widget) const
    {
        QSize t = QProxyStyle::sizeFromContents(type, option, size, widget);
        if (type == QStyle::CT_TabBarTab) {
            t.transpose();
            t.rwidth() = 25; // 设置每个tabBar中item的大小
            t.rheight() = 25;
        }
        return t;
    }

    void drawControl(ControlElement element, const QStyleOption* option, 
        QPainter* painter, const QWidget* widget) const
    {
        if (element == CE_TabBarTabLabel) {
            if (const QStyleOptionTab* tab = qstyleoption_cast<const QStyleOptionTab*>(option)) {
                QRect allRect = tab->rect;

                if (tab->state & QStyle::State_Selected) {
                    painter->save();
                    painter->setPen(0x89cfff);
                    painter->setBrush(QBrush(0x89cfff));
                    painter->drawRect(allRect.adjusted(6, 6, -6, -6));
                    painter->restore();
                }
                QTextOption option;
                option.setAlignment(Qt::AlignCenter);
                if (tab->state & QStyle::State_Selected) {
                    painter->setPen(0xf8fcff);
                }
                else {
                    painter->setPen(0x5d5d5d);
                }

                painter->drawText(allRect, tab->text, option);
                return;
            }
        }

        if (element == CE_TabBarTab) {
            QProxyStyle::drawControl(element, option, painter, widget);
        }
    }

};
