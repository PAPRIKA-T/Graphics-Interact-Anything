#pragma once
#include <QHBoxLayout>
#include <QToolButton>
#include <QWidget>
#include <qtoolbar.h>
#include <qpushbutton.h>
#include <qdockwidget.h>

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

    void toggleTools();
    

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
public slots:
    void min_btn_clicked();
    void max_btn_clicked();
    void close_btn_clicked();
    void on_help_doc_clicked();
private:

    QDockWidget* dockWidget;
    GenericToolButton* toolButton1;
    GenericToolButton* toolButton2;
    GenericToolButton* toolButton3;


    Widget* par_widget = nullptr;
    QHBoxLayout* main_layout = nullptr;
    QLabel* title_icon = nullptr;
    QLabel* title_name = nullptr;
    GenericToolButton* max_btn = nullptr;
    GenericToolButton* min_btn = nullptr;
    GenericToolButton* close_btn = nullptr;
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

