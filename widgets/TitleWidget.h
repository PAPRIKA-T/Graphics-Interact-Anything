#pragma once
#include <QHBoxLayout>
#include <QToolButton>
#include <QWidget>

class GenericToolButton;
class QLabel;
class Widget;

/*****************************************ToolButton***************************************/
class ToolButton : public QToolButton
{
    Q_OBJECT
public:
    ToolButton(QWidget* parent = nullptr);
    ~ToolButton() { ; }
    virtual void paintEvent(QPaintEvent* event) override;
};
/*****************************************TitleWidget***************************************/
class TitleWidget : public QWidget
{
    Q_OBJECT
public:
    TitleWidget(QWidget* parent = nullptr);
    ~TitleWidget();
    void setTitleName(const QString& s);
    void setTitleIcon(const QPixmap& icon);
    void setParentWidget(Widget*);
    void setMenuStyle();

public slots:
    void on_help_doc_clicked();
private:
    Widget* par_widget = nullptr;
    QHBoxLayout* main_layout = nullptr;
    QLabel* title_icon = nullptr;
    QLabel* title_name = nullptr;
    bool is_left_mouse_pressed = false;
    QPoint m_mouse_press;

    GenericToolButton* file_menu_btn = nullptr;
    GenericToolButton* segment_menu_btn = nullptr;
    GenericToolButton* help_menu_btn = nullptr;
    GenericToolButton* view_menu_btn = nullptr;
    GenericToolButton* edit_menu_btn = nullptr;

    QMenu* file_menu = nullptr;
    QMenu* segment_menu = nullptr;
    QMenu* help_menu = nullptr;
    QMenu* view_menu = nullptr;
    QMenu* edit_menu = nullptr;
};