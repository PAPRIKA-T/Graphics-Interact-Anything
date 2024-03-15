#include "TabWidget.h"
#include <QTabBar>
/****************************************************标签控件*********************************************************/
TabWidget::TabWidget(QWidget* parent)
    :QTabWidget(parent)
{
    tabBar()->setMovable(true);
}

TabWidget::~TabWidget()
{

}
