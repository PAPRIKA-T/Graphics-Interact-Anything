#include "graphicsItemEditTextDialog.h"
#include <QDebug>

graphicsItemEditTextDialog::graphicsItemEditTextDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle("edit text");
    setFixedSize(260, 78);
    
    if (objectName().isEmpty())
        setObjectName("graphicsItemEditTextDialog");
    buttonBox = new QDialogButtonBox(this);
    buttonBox->setObjectName("buttonBox");
    buttonBox->setGeometry(QRect(-90, 40, 341, 32));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
    lineEdit = new QLineEdit(this);
    lineEdit->setObjectName("lineEdit");
    lineEdit->setGeometry(QRect(10, 10, 241, 21));
    lineEdit->setMinimumSize(QSize(0, 21));
    lineEdit->setCursor(QCursor(Qt::IBeamCursor));

    QObject::connect(buttonBox, &QDialogButtonBox::accepted, this, qOverload<>(&QDialog::accept));
    QObject::connect(buttonBox, &QDialogButtonBox::rejected, this, qOverload<>(&QDialog::reject));

    QMetaObject::connectSlotsByName(this);
}

graphicsItemEditTextDialog::~graphicsItemEditTextDialog()
{

}

void graphicsItemEditTextDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    Q_UNUSED(button);
}

void graphicsItemEditTextDialog::on_lineEdit_editingFinished()
{
    emit updateText(lineEdit->text());
}
