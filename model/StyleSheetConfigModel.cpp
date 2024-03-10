#include "StyleSheetConfigModel.h"
#include <QMenu>
#include <QFile>

void StyleSheetConfigModel::setGlobalStyleSheet(QWidget* w)
{
    QFile file(":/res/qss/Default.qss");
    if (file.open(QFile::ReadOnly)) {
        QString stylesheet = QLatin1String(file.readAll());
        w->setStyleSheet(stylesheet);
        file.close();
    }
    else {
        qDebug() << "StyleSheetConfig::setGlobalStyleSheet: open file failed";
    }
}

void StyleSheetConfigModel::setMenuStyle(QMenu* menu)
{
    QFile file(":/res/qss/MenuStyle.qss");
    if (file.open(QFile::ReadOnly)) {
        QString stylesheet = QLatin1String(file.readAll());
        menu->setStyleSheet(stylesheet);
        file.close();
    }
    else {
        qDebug() << "StyleSheetConfig::setMenuStyle: open file failed";
    }
}
