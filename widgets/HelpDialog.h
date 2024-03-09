#pragma once

#include <QDialog>
class QLabel;
class QVBoxLayout;

class HelpDialog : public QDialog
{
public:
    HelpDialog(QWidget* parent = nullptr);
    ~HelpDialog();
    QLabel* getLabel() { return message; }
private:
    QVBoxLayout* main_layout = nullptr;
    QLabel* message = nullptr;
};

