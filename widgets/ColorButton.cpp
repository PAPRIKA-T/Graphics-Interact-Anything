#include "ColorButton.h"
#include <QColorDialog>

ColorButton::ColorButton(QColor c, QWidget* parent)
    :QPushButton(parent)
{
    setContentsMargins(0, 0, 0, 0);
    setFixedSize(20, 20);
    setFlat(true);
    setBackgrondColor(c);
    connect(this, &QPushButton::clicked, this, &ColorButton::openColorDialog);
}

ColorButton::~ColorButton()
{
}

void ColorButton::setBackgrondColor(QColor c)
{
    setPalette(QPalette(c));
    setAutoFillBackground(true);
    background_color = c;
}

const QString ColorButton::colorToQString(const QColor& color, const QString type)
{
    if (type == "RGBA") {
        return QString("rgba(%1, %2, %3, %4)")
            .arg(color.red())
            .arg(color.green())
            .arg(color.blue())
            .arg(color.alpha());
    }
    else if (type == "RGB") {
        return QString("rgba(%1, %2, %3)")
            .arg(color.red())
            .arg(color.green())
            .arg(color.blue());
    }
    else if (type == "HEX") {
        return QString().asprintf("#%1%02X%02X%02X",
            color.red(),
            color.green(),
            color.blue()).arg(color.alpha() != 255 ? QString().asprintf("%02X", color.alpha()) : QString());
    }
    return color.name();
}

void ColorButton::openColorDialog()
{
    QColor c = QColorDialog::getColor(Qt::white, this);
    if (c.isValid()) {
        setBackgrondColor(c);
        emit sentSelf(this);
    }
}

