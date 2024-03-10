#include "StyleSheetConfigModel.h"
#include <QMenu>
#include <QFile>
#include <QString>

void StyleSheetConfigModel::setGlobalStyleSheet(QWidget* w)
{
    QFile file(getStyleSheetParentPath() + "Default.qss");
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
    QFile file( getStyleSheetParentPath() + "MenuStyle.qss");
    if (file.open(QFile::ReadOnly)) {
        QString stylesheet = QLatin1String(file.readAll());
        menu->setStyleSheet(stylesheet);
        file.close();
    }
    else {
        qDebug() << "StyleSheetConfig::setMenuStyle: open file failed";
    }
}

QString StyleSheetConfigModel::getStyleSheetParentPath()
{
    QString p{};
    if(sheet_type == StyleSheetType::Dark)
		p = ":/res/qss/Dark/";
	else if(sheet_type == StyleSheetType::White)
		p = ":/res/qss/White/";
	else
		qDebug() << "StyleSheetConfig::getStyleSheetParentPath: invalid type";
    return p;
}

void StyleSheetConfigModel::setStyleType(StyleSheetType t)
{
    sheet_type = t;
}
