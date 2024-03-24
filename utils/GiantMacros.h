#pragma once
#include <QStyleOption>
#include <QPainter>
#include <QMouseEvent>

#define PEN_WIDTHF 1 //画笔宽度
#define DEFAULT_COLOR_ITEM Qt::red //item color
#define DEFAULT_COLOR_POINT_NOSELECTED QColor(255, 249, 57)
#define DEFAULT_COLOR_POINT_SELECTED QColor(255, 20, 147)
#define DEFAULT_COLOR_UNIQUE_POINT QColor(237, 153, 74)
#define ITEM_LINE_LENGTH 6 //item交互十字（点）的大小（px）
#define INTERACTION_RANGE 6 //item交互十字接收事件的范围

class GraphicsView;
#define G_Widget_PaintEventOverride \
protected: \
    virtual void paintEvent(QPaintEvent *event) override{\
        Q_UNUSED(event);\
        QStyleOption styleOpt;\
        styleOpt.initFrom(this);\
        QPainter painter(this);\
        style()->drawPrimitive(QStyle::PE_Widget, &styleOpt, &painter, this);\
    };\

#define G_Menu_StayOpenAfterPress \
protected: \
    virtual void mouseReleaseEvent(QMouseEvent *event) override{\
        QAction* action = this->actionAt(event->pos());\
        if(action){\
			    action->activate(QAction::Trigger);\
		    }\
        else{\
			    QMenu::mouseReleaseEvent(event);\
		    }\
    };
#include <graphs/Graphicsview.h>
#define G_Widget_EnterLeaveView \
protected: \
    virtual void enterEvent(QEnterEvent* event) override { \
        Q_UNUSED(event); \
        m_view->setEnterView(false); \
    } \
    virtual void leaveEvent(QEvent* event) override { \
        Q_UNUSED(event); \
        m_view->setEnterView(true); \
    }

#define G_Widget_SetGraphicsView \
public: \
    void setGraphicsView(GraphicsView* v){ \
	    m_view = v; \
    }
