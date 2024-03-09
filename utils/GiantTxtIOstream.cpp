#include "GiantTxtIOstream.h"
#include <QFileDialog>

void GiantTxtIOstream::writeTxtFile(QString& data)
{
    QString filepath = QFileDialog::getSaveFileName(nullptr, "Save Txt", "./", "Txt files(*.txt)");
    if (filepath.isEmpty())return;
    QFile txtFile(filepath);
    if (txtFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream out(&txtFile);
        out << data;
        txtFile.close();
    }
    return;
}
