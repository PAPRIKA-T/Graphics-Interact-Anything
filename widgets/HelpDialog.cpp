#include "HelpDialog.h"
#include <QVBoxLayout>
#include <QLabel>

HelpDialog::HelpDialog(QWidget* parent)
    :QDialog(parent)
{
    setWindowTitle("Help");
    //setFixedSize(400, 200);
    main_layout = new QVBoxLayout();
    message = new QLabel();
    QFont font = QFont("宋体"); //设置初始字体
    font.setBold(false);
    font.setPointSize(18);
    message->setFont(font);
    message->setWordWrap(true);
    message->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    main_layout->addWidget(message);
    setLayout(main_layout);
}


HelpDialog::~HelpDialog()
{
    delete main_layout;
}

