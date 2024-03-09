#include <QApplication>
#include <iostream>

#include "widgets/widget.h"
#include "graphs/Graphicsitem.h"

int GraphicsItem::count = 0;

//Paprika
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setStyle("Default");
    w.show();
    return a.exec();
}
