#include "TitleWidget.h"
#include "widget.h"
#include "HelpDialog.h"
#include "GenericToolButton.h"
#include "model/StyleSheetConfigModel.h"
#include "FileView.h"
#include <QLabel>
#include <QDesktopServices>
#include <QMenu>

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
    QAction* theme_divert = new QAction("Theme Divert");

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
    help_menu->addAction(theme_divert);
    help_menu_btn->setMenu(help_menu);

    main_layout->addSpacing(8);
    main_layout->addWidget(title_icon);
    main_layout->addSpacing(0);
    main_layout->addWidget(title_name);
    main_layout->addSpacing(0);
    main_layout->addWidget(file_menu_btn);
    main_layout->addWidget(segment_menu_btn);
    main_layout->addWidget(edit_menu_btn);
    main_layout->addWidget(view_menu_btn);
    main_layout->addWidget(help_menu_btn);
    main_layout->addStretch();

    main_layout->setContentsMargins(0, 0, 2, 0);
    main_layout->setSpacing(0);
    setLayout(main_layout);
    setMenuStyle();
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

    //主题风格设置信号绑定
    connect(help_menu->actions()[4], &QAction::triggered, [=]() {
        StyleSheetConfigModel style_model;
        if (style_model.getStyleType() == StyleSheetConfigModel::StyleSheetType::Dark) {
            style_model.setStyleType(StyleSheetConfigModel::StyleSheetType::Light);
        }
        else {
            style_model.setStyleType(StyleSheetConfigModel::StyleSheetType::Dark);
        }
        style_model.setGlobalStyleSheet(par_widget);
        setMenuStyle();
        });
}

void TitleWidget::setMenuStyle()
{
    StyleSheetConfigModel style_model;
    style_model.setMenuStyle(file_menu);
    style_model.setMenuStyle(edit_menu);
    style_model.setMenuStyle(segment_menu);
    style_model.setMenuStyle(view_menu);
    style_model.setMenuStyle(help_menu);
}

void TitleWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QStyleOption styleOpt;
    styleOpt.initFrom(this);
    style()->drawPrimitive(QStyle::PE_Widget, &styleOpt, &painter, this);
    QWidget::paintEvent(event);
}
