#pragma once

#include <QWidget>
class QLabel;
class QLineEdit;
class QGridLayout;
class QPushButton;
class QComboBox;

class ChosePathWidget : public QWidget
{
    Q_OBJECT
public:
    ChosePathWidget(QWidget* parent = nullptr);
    ~ChosePathWidget();
    void setLabelText(QString s);
    void setSubtitleText(QString s);
    void setLabelTextFont(QFont f);
    void setPathChoseEditContent(QString s); //设置文件路径显示控件内容
    const QString getPathChoseEditContent(); //获取文件路径显示控件内容
    const QString getFormComboxContent();//获取格式combox内容
    QLineEdit* getPathChoseEdit(); //获取路径编辑控件
public slots:
    void path_chose_btn_clicked();
private:
    QGridLayout* main_layout;
    QLabel* name_label;
    QLabel* subtitle_label;
    QLineEdit* path_chose_edit;
    QPushButton* path_chose_btn;
    QComboBox* form_selection;
};
