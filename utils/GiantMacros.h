#pragma once
#include <QStyleOption>
#include <QPainter>
#include <QMouseEvent>

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