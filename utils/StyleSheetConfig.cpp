#include "StyleSheetConfig.h"
#include <QMenu>

void StyleSheetConfig::setMenuStyle(QMenu* menu)
{
	QFile file(":/qss/MenuStyle.qss");
	file.open(QFile::ReadOnly);
	QString styleSheet = QLatin1String(file.readAll());
	menu->setStyleSheet(styleSheet);
	file.close();
}
