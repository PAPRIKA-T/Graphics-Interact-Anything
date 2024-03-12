#include "TitleWidget.h"
#include "widget.h"
#include "HelpDialog.h"
#include "GenericToolButton.h"
#include "FileView.h"
#include <QLabel>
#include <QDesktopServices>
#include <QMenu>
#include <qfile.h>

/********************************************TitleWidget Class******************************************/
ToolButton::ToolButton(QWidget* parent)
    :QToolButton(parent)
{
    setFixedSize(43, 30);
}

void ToolButton::paintEvent(QPaintEvent* event)
{
    QStyleOptionButton  styleOpt;
    styleOpt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &styleOpt, &painter, this);
    QToolButton::paintEvent(event);
}



/********************************************TitleWidget Class******************************************/
TitleWidget::TitleWidget(QWidget* parent)
    :QWidget(parent)
{
    setFixedHeight(30);
    main_layout = new QHBoxLayout();
    title_icon = new QLabel();
    title_name = new QLabel();
    title_name->setObjectName("title_name");
    max_btn = new GenericToolButton();
    max_btn->setObjectName("title_right_btn");
    max_btn->setFixedSize(43, 26);
    min_btn = new GenericToolButton();
    min_btn->setObjectName("title_right_btn");
    min_btn->setFixedSize(43, 26);
    close_btn = new GenericToolButton();
    close_btn->setObjectName("title_right_btn");
    close_btn->setFixedSize(43, 26);
    close_btn->setStyleSheet("border-top-right-radius: 6px;");
    max_btn->setIcon(QIcon(":/res/background-image/max.png"));
    min_btn->setIcon(QIcon(":/res/background-image/min.png"));
    close_btn->setIcon(QIcon(":/res/background-image/close.png"));
    min_btn->setIconSize(QSize(12, 12));
    max_btn->setIconSize(QSize(12, 12));
    close_btn->setIconSize(QSize(12, 12));
    connect(min_btn, &QToolButton::clicked, this, &TitleWidget::min_btn_clicked);
    connect(max_btn, &QToolButton::clicked, this, &TitleWidget::max_btn_clicked);
    connect(close_btn, &QToolButton::clicked, this, &TitleWidget::close_btn_clicked);

    //file菜单栏创建
    file_menu_btn = new GenericToolButton("文件(F)");
    file_menu_btn->setFixedSize(60, 22);

    file_menu = new QMenu;
    QAction* open_image = new QAction("Open 2D_Image");
    QAction* open_dir = new QAction("Open 2D_Dir");
    QAction* open_nii = new QAction("Open NiiImage");
    QAction* open_nii_dir = new QAction("Open Nii_Dir");

    file_menu->addAction(open_image);
    file_menu->addAction(open_dir);
    file_menu->addAction(open_nii);
    file_menu->addAction(open_nii_dir);
    file_menu_btn->setMenu(file_menu);

    //segment菜单栏创建
    segment_menu_btn = new GenericToolButton("分割(S)");
    segment_menu_btn->setFixedSize(60, 22);

    segment_menu = new QMenu;
    QAction* open_annotation = new QAction("Open Annotation");
    QAction* open_segmentation = new QAction("Open Segmentation");

    segment_menu->addAction(open_annotation);
    segment_menu->addAction(open_segmentation);
    segment_menu_btn->setMenu(segment_menu);

    //edit菜单栏创建
    edit_menu_btn = new GenericToolButton("编辑(E)");
    edit_menu_btn->setFixedSize(60, 22);

    edit_menu = new QMenu;
    edit_menu_btn->setMenu(edit_menu);

    //view菜单栏创建
    view_menu_btn = new GenericToolButton("视图(V)");
    view_menu_btn->setFixedSize(60, 22);

    view_menu = new QMenu;
    QAction* dimension_trans = new QAction("Dimension Trans");
    view_menu->addAction(dimension_trans);
    view_menu_btn->setMenu(view_menu);

    //help菜单栏创建
    help_menu_btn = new GenericToolButton("帮助(H)");
    help_menu_btn->setFixedSize(60, 22);
    help_menu = new QMenu;

    QAction* help_doc = new QAction("ReadMe");
    QAction* help_contact = new QAction("Contact");
    QAction* help_version = new QAction("Version");
    QAction* help_about = new QAction("About");
    connect(help_doc, &QAction::triggered, this, &TitleWidget::on_help_doc_clicked);
    connect(help_contact, &QAction::triggered, [=]() {
        HelpDialog* dialog = new HelpDialog();
        dialog->setWindowTitle("Contact");
        dialog->getLabel()->setText("作者邮箱：3338382639@qq.com\n请多多交流指教！");
        dialog->show();
        });
    connect(help_version, &QAction::triggered, [=]() {
        HelpDialog* dialog = new HelpDialog();
        dialog->setWindowTitle("Version");
        dialog->getLabel()->setText("当前软件版本：V1.0");
        dialog->show();
        });
    connect(help_about, &QAction::triggered, [=]() {
        HelpDialog* dialog = new HelpDialog();
        dialog->setWindowTitle("About");
        dialog->getLabel()->setText("医学影像半自动式图形交互系统由深圳大学医学部生物医学工程学院开发。本软件可用于交互式图形生成，主要提供给相关工作人员或研究人员使用。");
        dialog->show();
        });
    help_menu->addAction(help_doc);
    help_menu->addAction(help_contact);
    help_menu->addAction(help_version);
    help_menu->addAction(help_about);
    help_menu_btn->setMenu(help_menu);

    main_layout->addSpacing(5);
    main_layout->addWidget(title_icon);
    main_layout->addSpacing(5);
    main_layout->addWidget(title_name);
    main_layout->addSpacing(5);
    main_layout->addWidget(file_menu_btn);
    main_layout->addWidget(segment_menu_btn);
    main_layout->addWidget(edit_menu_btn);
    main_layout->addWidget(view_menu_btn);
    main_layout->addWidget(help_menu_btn);

    main_layout->addStretch();

    QFile file(":/res/qss/Default.qss");
    if (file.open(QFile::ReadOnly)) {
        QString stylesheet = QLatin1String(file.readAll());
        file_menu->setStyleSheet(stylesheet);
        edit_menu->setStyleSheet(stylesheet);
        segment_menu->setStyleSheet(stylesheet);
        view_menu->setStyleSheet(stylesheet);
        help_menu->setStyleSheet(stylesheet);
        file.close();
    }

    main_layout->addWidget(min_btn);
    main_layout->addWidget(max_btn);
    main_layout->addWidget(close_btn);
    main_layout->setContentsMargins(0, 0, 2, 0);
    main_layout->setSpacing(0);
    setLayout(main_layout);
}

TitleWidget::~TitleWidget()
{
    delete file_menu;
    delete help_menu;
    delete segment_menu;
    delete main_layout;
}

void TitleWidget::setTitleName(const QString& s)
{
    title_name->setText(s);
}

void TitleWidget::min_btn_clicked()
{
    if (par_widget) {
        par_widget->showMinimized();
    }
    else TitleWidget::showMinimized();
}

void TitleWidget::max_btn_clicked()
{
    if (par_widget) {
        if (par_widget->isMaximized()) {
            par_widget->showNormal();//还原
            max_btn->setIcon(QIcon(":/res/background-image/max.png"));
        }
        else {
            par_widget->showMaximized();//最大化
            max_btn->setIcon(QIcon(":/res/background-image/restore.png"));
        }
    }
    else {
        if (isMaximized()) {
            showNormal();//还原
            max_btn->setIcon(QIcon(":/res/background-image/max.png"));
        }
        else {
            showMaximized();//最大化
            max_btn->setIcon(QIcon(":/res/background-image/restore.png"));
        }
    }
}

void TitleWidget::close_btn_clicked()
{
    if (par_widget) {
        par_widget->close();
    }
    else TitleWidget::close();
}

void TitleWidget::on_help_doc_clicked()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile("config/help/README.pdf"));
}

void TitleWidget::setTitleIcon(const QPixmap& icon)
{
    title_icon->setPixmap(icon);
}

void TitleWidget::setParentWidget(Widget* w)
{
    par_widget = w;
    //文件菜单信号绑定
    connect(file_menu->actions()[0], &QAction::triggered, par_widget->getFileView(), &FileView::readImage);
    connect(file_menu->actions()[1], &QAction::triggered, par_widget->getFileView(), &FileView::readImageDir);
    connect(file_menu->actions()[2], &QAction::triggered, par_widget->getFileView(), &FileView::readITKImage);
    connect(file_menu->actions()[3], &QAction::triggered, par_widget->getFileView(), &FileView::readITKImageDir);
    //视图菜单信号绑定
    connect(view_menu->actions()[0], &QAction::triggered, par_widget, &Widget::DimensionTrans);
}

void TitleWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QStyleOption styleOpt;
    styleOpt.initFrom(this);
    style()->drawPrimitive(QStyle::PE_Widget, &styleOpt, &painter, this);
    QWidget::paintEvent(event);
}

void TitleWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        is_left_mouse_pressed = true;
        m_mouse_press = event->globalPos();
        event->accept();
    }
}

void TitleWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (is_left_mouse_pressed) {
        if (!par_widget) return;
        //当窗口最大化或最小化时也不进行触发
        if (par_widget->isMinimized())
            return;
        else if (par_widget->isMaximized()) {
            max_btn_clicked();
        }
        else {
            //当在按钮之类需要鼠标操作的地方不进行触发(防误触)
            if (close_btn->underMouse() || max_btn->underMouse() || min_btn->underMouse())
                return;
            else {
                QPoint cur_mouse_pos = event->globalPos();
                par_widget->move(par_widget->pos() + cur_mouse_pos - m_mouse_press);//移动
                m_mouse_press = cur_mouse_pos;
                return;
            }
        }
    }
    else {
        QWidget::mouseMoveEvent(event);
    }
}

void TitleWidget::mouseReleaseEvent(QMouseEvent* event)
{
    is_left_mouse_pressed = false;
    QWidget::mouseReleaseEvent(event);
}

void TitleWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    max_btn_clicked();
    Q_UNUSED(event);
}
