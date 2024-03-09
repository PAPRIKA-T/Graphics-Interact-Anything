#include "ColorOperation.h"
#include <QCryptographicHash>
#include <QString>
#include <QColor>

QColor ColorOperation::generate_color_by_text(const QString& text)
{
    //translate text to hash
    QByteArray data_2_hash = text.toUtf8();
    QByteArray hash = QCryptographicHash::hash(data_2_hash, QCryptographicHash::Sha256);
    QString hexHash = QString(hash.toHex().left(6));
    QColor c("#" + hexHash);
    return QColor(c);
}
