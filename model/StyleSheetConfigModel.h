#pragma once
class QMenu;
class QWidget;
class QString;

class StyleSheetConfigModel
{
private:
	enum class StyleSheetType
    {
		Dark,
		White,
	};
	static StyleSheetType sheet_type;
	QString getStyleSheetParentPath();

public:
	void setStyleType(StyleSheetType);
	void setGlobalStyleSheet(QWidget*);
	void setMenuStyle(QMenu*);
};

