#include "TabWidget.h"
#include <QTabBar>
/****************************************************��ǩ�ؼ�*********************************************************/
TabWidget::TabWidget(QWidget* parent)
    :QTabWidget(parent)
{
    tabBar()->setMovable(true);
}

TabWidget::~TabWidget()
{

}
