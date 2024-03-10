#pragma once
class QMenu;
class QWidget;
class QString;

class StyleSheetConfigModel
{
public:
	enum class StyleSheetType
    {
		Dark,
		Light,
	};
	void setStyleType(StyleSheetType);
	void setGlobalStyleSheet(QWidget*);
	void setMenuStyle(QMenu*);
	StyleSheetType getStyleType();
private:
	static StyleSheetType sheet_type;
	QString getStyleSheetParentPath();
};

