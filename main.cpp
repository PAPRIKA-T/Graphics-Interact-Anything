#include <QApplication>
#include <iostream>
#include "widgets/widget.h"
#include "graphs/Graphicsitem.h"
#include "model/StyleSheetConfigModel.h"
#include "graphs/GiantMaskItem.h"

int GraphicsItem::count = 0; 
int GiantMaskItem::count = 0;
StyleSheetConfigModel::StyleSheetType StyleSheetConfigModel::sheet_type = 
StyleSheetConfigModel::StyleSheetType::Dark;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    StyleSheetConfigModel* style_model = new StyleSheetConfigModel();
    style_model->setGlobalStyleSheet(&w);
    delete style_model;
    w.show();
    return a.exec();
}
