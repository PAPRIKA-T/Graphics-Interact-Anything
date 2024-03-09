#pragma once

#include <QWidget>
class QComboBox;
class QLabel;
class QVBoxLayout;

/*****************************************ComboBoxVWidget***************************************/
class ComboBoxVWidget : public QWidget
{
    Q_OBJECT
public:
    ComboBoxVWidget(QWidget* parent = nullptr);
    ~ComboBoxVWidget();
    void setLabelText(const QString& s);
    void setLabelTextFont(const QFont& f);
    int getComBoxContentInt();
    void setComBoxContentInt(int index);
    QString getComBoxContentString();
    void  setComBoxContentString(const QString& s);
    QComboBox* getComboBox();
    QLabel* getLabel();
private:
    QVBoxLayout* main_layout = nullptr;
    QLabel* name_label = nullptr;
    QComboBox* combo_box = nullptr;
};