#include "ForePlayWidget.h"
#include "FileView.h"
#include "utils/FileOperation.h"
#include "ChosePathWidget.h"
#include "Model/XmlIOstreamModel.h"
#include "Model/ViewListContainer.h"
#include "model/ItkTool/GiantITKImageWriteModel.h"
#include "graphs/Graphicsscene.h"
#include <QPainter>
#include <QLineEdit>

/****************************************************ForePlayWidget*********************************************************/
ForePlayWidget::ForePlayWidget(QWidget* parent)
    :QWidget(parent)
{
    annotation_read_path_widget = new ChosePathWidget();
    annotation_read_path_widget->setLabelText("Read Annotation Path: ");
    annotation_read_path_widget->setSubtitleText("Read From: ");
    annotation_save_path_widget = new ChosePathWidget();
    annotation_save_path_widget->setLabelText("Save Annotation Path: ");
    annotation_save_path_widget->setSubtitleText("Save From: ");
    main_layout = new QVBoxLayout();
    main_layout->addWidget(annotation_read_path_widget);
    main_layout->addWidget(annotation_save_path_widget);
    main_layout->addStretch();
    setLayout(main_layout);

    connect(getReadPathWidget()->getPathChoseEdit(),
        &QLineEdit::textChanged, this, &ForePlayWidget::annotationReadPathChange);
    connect(getSavePathWidget()->getPathChoseEdit(),
        &QLineEdit::textChanged, this, &ForePlayWidget::annotationSavePathChange);
}

ForePlayWidget::~ForePlayWidget()
{
    delete annotation_read_path_widget;
    delete annotation_save_path_widget;
    delete main_layout;
}

void ForePlayWidget::setViewListContainer(ViewListContainer* vlc)
{
    view_list_container = vlc;
}

const QString ForePlayWidget::getAnnotationReadPath()
{
    return annotation_read_path_widget->getPathChoseEditContent();
}

const QString ForePlayWidget::getAnnotationSavePath()
{
    return annotation_save_path_widget->getPathChoseEditContent();
}

void ForePlayWidget::setAnnotationReadPath(const QString& path)
{
    annotation_read_path_widget->setPathChoseEditContent(path);
}

void ForePlayWidget::setAnnotationSavePath(const QString& path)
{
    annotation_save_path_widget->setPathChoseEditContent(path);
}

const QString ForePlayWidget::getAnnotationReadForm()
{
    return annotation_read_path_widget->getFormComboxContent();
}

const QString ForePlayWidget::getAnnotationSaveForm()
{
    return annotation_save_path_widget->getFormComboxContent();
}

void ForePlayWidget::annotationReadPathChange(const QString& path)
{
    foreach(FileViewParentItem * parent_item, file_view->getViewParentItemList()) {
        if (parent_item->data().toString() + "/" ==
            FilePathOperation::getFileParentPath(file_view->getFilePath())) {
            parent_item->setAnnotationReadPath(path);
            return;
        }
    }
    readItemFromPathAllForm(view_list_container->getActivedView()->getGraphicsScene());
}

void ForePlayWidget::annotationSavePathChange(const QString& path)
{
    foreach(FileViewParentItem * parent_item, file_view->getViewParentItemList()) {
        if (parent_item->data().toString() + "/" ==
            FilePathOperation::getFileParentPath(file_view->getFilePath())) {
            parent_item->setAnnotationSavePath(path);
            return;
        }
    }
}

void ForePlayWidget::readItemFromPathXml(const QString& readpath, GraphicsScene* scene)
{
    XmlIOstreamModel xml_reader = XmlIOstreamModel();
    xml_reader.readItemFromXmlFile(readpath, scene);
}

void ForePlayWidget::saveItemToPathXml(const QString& savepath, GraphicsScene* scene)
{
    XmlIOstreamModel xml_saver = XmlIOstreamModel();
    xml_saver.saveItemToXmlFile(savepath, scene);
}

void ForePlayWidget::saveMaskToPathNii(const QString& savepath, GraphicsScene* scene)
{
    GiantITKImageWriteModel itk_saver = GiantITKImageWriteModel();
    itk_saver.saveCvMatAsNii(scene->getMaskItemList()[0]->getOriginalMask(), savepath.toStdString());
}

void ForePlayWidget::saveItemToPathAllForm(GraphicsScene* scene)
{
    QString save_fold = getAnnotationSavePath();
    if (save_fold == "") {
        //qDebug() << "Save FilePath is null";
        return;
    }
    QString save_pixmap_name = FilePathOperation::getFileBaseName(scene->getPixmapItem()->getImagePath());
    if (save_pixmap_name == "") {
        //qDebug() << "Save PixmapPath is null";
        return;
    }

    scene->getScenePromptItemModel()->onDeleteAllPromptItemBtn();
    if (scene->getIsCreatePolygon()) {
        scene->finishCreatePolygon();
    }
    QString save_form = getAnnotationSaveForm();
    QString save_item_full_path{};
    /********************保存图元信息*************************/
    if (save_form == "pascalVoc")
    {
        save_item_full_path = save_fold + save_pixmap_name + "_label.xml";
        if (!scene->isPaintItemOnScene()) {
        }
        saveItemToPathXml(save_item_full_path, scene);
    }
    else if (save_form == "YOLO")
    {
    }
    else if (save_form == "COCO")
    {
    }
    
    std::vector<std::string> to_zip_file_list{};
    to_zip_file_list.push_back(save_item_full_path.toStdString());

    /********************保存掩膜信息*************************/
    save_item_full_path = save_fold + save_pixmap_name + "_label.nii.gz";
    to_zip_file_list.push_back(save_item_full_path.toStdString());
    saveMaskToPathNii(save_item_full_path, scene);

    //将图元信息和掩膜信息文件压缩成一个zip文件
    std::string zipped_file_path = QString(save_fold + save_pixmap_name + "_label.tar").toStdString();
    FileZipOperation::compressFilesToZip(to_zip_file_list, zipped_file_path);
    return;
}

void ForePlayWidget::readItemFromPathAllForm(GraphicsScene* scene)
{
    QString read_fold = getAnnotationReadPath();
    if (read_fold == "") {
        //qDebug() << "Read FilePath is null";
        return;
    }
    QString read_pixmap_name = FilePathOperation::getFileBaseName(scene->getPixmapItem()->getImagePath());
    if (read_pixmap_name == "") {
        //qDebug() << "Read PixmapPath is null";
        return;
    }

    QString read_form = getAnnotationReadForm();
    if (read_form == "pascalVoc")
    {
        scene->clearSceneGraphicsItem();
        readItemFromPathXml(read_fold + read_pixmap_name + "_label.xml", scene);
    }
    else if (read_form == "YOLO")
    {
    }
    else if (read_form == "COCO")
    {
    }
}

void ForePlayWidget::saveItemToPathAllFormAllScene()
{
    for (GraphicsView* v : view_list_container->getViewList()) {
        saveItemToPathAllForm(v->getGraphicsScene());
    }
}

void ForePlayWidget::readItemFromPathAllFormAllScene()
{
    for (GraphicsView* v : view_list_container->getViewList()) {
        readItemFromPathAllForm(v->getGraphicsScene());
    }
}

void ForePlayWidget::onPathClear()
{
    setAnnotationReadPath("");
    setAnnotationSavePath("");
}
