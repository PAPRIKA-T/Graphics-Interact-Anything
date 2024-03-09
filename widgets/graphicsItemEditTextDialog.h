#ifndef DIALOG_H
#define DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLineEdit>

class graphicsItemEditTextDialog : public QDialog
{
    Q_OBJECT

public:
    explicit graphicsItemEditTextDialog(QWidget *parent = nullptr);
    ~graphicsItemEditTextDialog();

signals:
    void updateText(QString text);

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
    void on_lineEdit_editingFinished();

private:
    QDialogButtonBox* buttonBox;
    QLineEdit* lineEdit;
};

#endif // DIALOG_H
