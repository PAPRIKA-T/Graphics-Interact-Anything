#include "TabWidget.h"
#include <QTabBar>
/****************************************************±êÇ©¿Ø¼þ*********************************************************/
TabWidget::TabWidget(QWidget* parent)
    :QTabWidget(parent)
{
    tabBar()->setMovable(true);
}

TabWidget::~TabWidget()
{

}
