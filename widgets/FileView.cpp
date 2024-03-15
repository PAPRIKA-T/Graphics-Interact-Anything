#include "FileView.h"
#include "Model/ViewListContainer.h"
#include "Model/ItkTool/ITKImageIoModel.h"
#include "ForePlayWidget.h"
#include "graphs/ThumbnailPixmapItem.h"
#include "graphs/Graphicspixmapitem.h"
#include "StatusWidget.h"
#include "utils/FilePathOperation.h"
#include "model/StyleSheetConfigModel.h"

/****************************************************导入文件索引的树状控件*********************************************************/

FileViewParentItem::FileViewParentItem(const QString& s)
    :QStandardItem(s)
{
}

FileViewParentItem::~FileViewParentItem()
{
}

FileViewChildItem::FileViewChildItem(const QString& s)
    :QStandardItem(s)
{
}

FileViewChildItem::~FileViewChildItem()
{
}

FileView::FileView(QWidget* parent)
    : QTreeView(parent)
{
    itk_helper = new ITKImageIoModel{};
    setContextMenuPolicy(Qt::CustomContextMenu);
    setEditTriggers(QTreeView::NoEditTriggers);
    tree_model = new QStandardItemModel(this);
    tree_model->setHorizontalHeaderLabels(QStringList() << QStringLiteral("FileView"));
    select_model = new QItemSelectionModel(tree_model);
    setHeaderHidden(true);
    setModel(tree_model);
    setSelectionModel(select_model);
    setIndentation(15);
    setAlternatingRowColors(true);
    connect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(readImageAtIndex(QModelIndex)));
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomContextMenuRequested(QPoint)));
    connect(remove_img, &QAction::triggered, this, &FileView::removeImage);
}

FileView::~FileView()
{
    delete tree_model;
    delete remove_img;
    delete itk_helper;
}

void FileView::setStatusWidget(StatusWidget* s)
{
    status_widget = s;
}

StatusWidget* FileView::getStatusWidget()
{
    return status_widget;
}

void FileView::setForeplayWidget(ForePlayWidget* w)
{
    foreplay_widget = w;
}

ForePlayWidget* FileView::getForePlayWidget()
{
    return foreplay_widget;
}

void FileView::setViewListContainer(ViewListContainer* vlc)
{
    view_list_container = vlc;
}

void FileView::enterEvent(QEvent* event)
{
    Q_UNUSED(event);
    setCursor(Qt::ArrowCursor);
}

void FileView::setItemSelected(const QString& pixmap_path)
{
    if (pixmap_path.isEmpty()) {
        return;
    }
    QString par_path = FilePathOperation::getFileParentPath(pixmap_path);
    par_path = par_path.removeLast();
    for (FileViewParentItem* par_item : parent_item_list) {
        if (par_path == par_item->data()) {
            int row_count = par_item->rowCount();
            for (int i = 0; i < row_count; ++i)
            {
                QStandardItem* childitem = par_item->child(i);
                if (childitem->data() == pixmap_path) {
                    setCurrentIndex(childitem->index());
                    FileViewParentItem* parent_item = dynamic_cast<FileViewParentItem*>(childitem->parent());
                    if (parent_item->getAnnotationReadPath() !=
                        foreplay_widget->getAnnotationReadPath()) {
                        foreplay_widget->setAnnotationReadPath(parent_item->getAnnotationReadPath());
                    }
                    if (parent_item->getAnnotationSavePath() !=
                        foreplay_widget->getAnnotationSavePath()) {
                        foreplay_widget->setAnnotationSavePath(parent_item->getAnnotationSavePath());
                    }
                    return;
                }
            }
        }
    }
}

// 导入图像
void FileView::readImage()
{
    QString filepath = QFileDialog::getOpenFileName(this, tr("Input File"), "C:/Users/Administrator/Desktop",
        tr("images(*.png; *.jpg; *.bmp; *.jpeg; *.webp;);"));
    if (!filepath.trimmed().isEmpty())
    {
        GraphicsScene* m_scene = view_list_container->getActivedView()->getGraphicsScene();
        foreplay_widget->saveItemToPathAllForm(m_scene);
        m_scene->changePixmap(filepath);
        status_widget->setRightLabelText(filepath);
        setFilePath(filepath);
        QStringList list = filepath.split('/');
        FileViewParentItem* item = new FileViewParentItem(list[list.indexOf(list.last()) - 1]);
        FileViewChildItem* child = new FileViewChildItem(list.last());
        item->setIcon(QIcon(":/res/qss/GenericStyle/background-image/folder_icon.png"));
        child->setData(filepath);
        child->setToolTip(filepath);
        child->setIcon(QIcon(":/res/qss/GenericStyle/background-image/picture_item.png"));
        list.removeAt(list.indexOf(list.last()));
        QString parent_path = list.join("/");
        item->setData(parent_path);
        item->setToolTip(parent_path);
        for (QStandardItem* par_item : parent_item_list) {
            if (par_item->text() != item->text()) continue;
            for (int j = 0; j < par_item->rowCount(); j++) { // 遍历父节点的子节点，寻找是否已经导入
                if (par_item->child(j)->text() == child->text()) { // 如果已经导入
                    setCurrentIndex(par_item->child(j)->index());
                    m_scene->updateRbText(1, par_item->rowCount());
                    delete child;
                    delete item;
                    return;
                }
            }
            delete item;
            par_item->appendRow(child);
            setCurrentIndex(child->index());
            m_scene->updateRbText(1, par_item->rowCount());
            return;
        }
        tree_model->appendRow(item);
        parent_item_list.push_back(item);
        item->appendRow(child);
        expand(tree_model->indexFromItem(item));
        foreplay_widget->setAnnotationReadPath(FilePathOperation::getFileParentPath(filepath));
        foreplay_widget->setAnnotationSavePath(FilePathOperation::getFileParentPath(filepath));
        foreplay_widget->readItemFromPathAllForm(m_scene);
        setCurrentIndex(child->index());
        m_scene->updateRbText(1, item->rowCount());
    }
}

//导入文件夹
void FileView::readImageDir()
{
    QString dirpath = QFileDialog::getExistingDirectory(this, tr("Input Dir"), "C:/Users/Administrator/Desktop", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);  //文件夹路径
    QDir dir(dirpath);  //文件夹
    QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Dirs);
    if (!dirpath.trimmed().isEmpty()) {
        for (QStandardItem* par_item : parent_item_list) {
            if (par_item->data() == dirpath) return;
        }
        GraphicsScene* m_scene = view_list_container->getActivedView()->getGraphicsScene();
        foreplay_widget->saveItemToPathAllForm(m_scene);
        QString pixpath;
        QStringList list = dirpath.split('/');
        FileViewParentItem* item = new FileViewParentItem(list[list.indexOf(list.last())]);
        item->setData(dirpath);
        item->setToolTip(dirpath);
        item->setIcon(QIcon(":/res/qss/GenericStyle/background-image/folder_icon.png"));
        tree_model->appendRow(item);
        foreach(auto fileInfo, fileInfoList) {// 遍历文件夹中的所有文件
            if (fileInfo.isFile()) {
                pixpath = fileInfo.absoluteFilePath();
                if (pixpath.contains(".jpg") || pixpath.contains(".bmp")
                    || pixpath.contains(".png") || pixpath.contains(".jpeg")
                    || pixpath.contains(".JPG")
                    ) {   //判断文件是否为图像
                    list = pixpath.split('/');
                    FileViewChildItem* child = new FileViewChildItem(list.last());
                    child->setData(pixpath);
                    child->setToolTip(pixpath);
                    list.removeAt(list.indexOf(list.last()));
                    child->setIcon(QIcon(":/res/qss/GenericStyle/background-image/picture_item.png"));
                    item->appendRow(child);
                }
            }
        }
        if (!item->hasChildren()) {
            tree_model->removeRow(item->row());
            return;
        }
        parent_item_list.push_back(item);
        QStandardItem* first_child = tree_model->itemFromIndex(tree_model->index(0, 0, item->index()));
        pixpath = first_child->data().toString();
        status_widget->setRightLabelText(pixpath);
        m_scene->changePixmap(pixpath);
        dirpath = dirpath + "/";
        load_filepath = pixpath;
        foreplay_widget->setAnnotationReadPath(dirpath);
        foreplay_widget->setAnnotationSavePath(dirpath);
        foreplay_widget->readItemFromPathAllForm(m_scene);
        setExpanded(tree_model->indexFromItem(item), 1);
        setCurrentIndex(first_child->index());
        m_scene->updateRbText(1, item->rowCount());
    }
}

//导入ITK数据
void FileView::readITKImage()
{
    QString filepath = QFileDialog::getOpenFileName(this, tr("Input Image"), "C:/Users/Administrator/Desktop",
        tr("image(*.nii *.nii.gz);"));
    QList<GraphicsView*> view_list =  view_list_container->getViewList();
    if (view_list.size() < 3) {
        return;
    }
    if (!filepath.trimmed().isEmpty())
    {
        itk_helper->readNiiImage(filepath.toStdString().c_str());
        //vtk_widget->AddActorFromITK(itk_helper->ITK2VTKactor());
        //vtk_widget->visualizeNii(filepath);
        QPixmap pix{};
        for (int i = 0; i < 3; ++i) {
            itk_helper->processAndConvertToPixmap(pix, i);
            view_list[i]->getGraphicsScene()->changePixmap(pix);
            view_list[i]->getGraphicsScene()->getPixmapItem()->setPixmapPath(filepath);
            view_list[i]->getGraphicsScene()->updateRbText(itk_helper->getCurSliceIndex(i) + 1, itk_helper->getDimensionSize(i));
        }
        setFilePath(filepath);
        status_widget->setRightLabelText(filepath);

        QStringList list = filepath.split('/');
        FileViewParentItem* item = new FileViewParentItem(list[list.indexOf(list.last()) - 1]);
        FileViewChildItem* child = new FileViewChildItem(list.last());
        child->setIsItkImageItem(true);
        item->setIcon(QIcon(":/res/qss/GenericStyle/background-image/folder_icon.png"));
        child->setData(filepath);
        child->setToolTip(filepath);
        child->setIcon(QIcon(":/res/qss/GenericStyle/background-image/picture_item.png"));
        list.removeAt(list.indexOf(list.last()));
        QString parent_path = list.join("/");
        item->setData(parent_path);
        item->setToolTip(parent_path);
        for (QStandardItem* par_item : parent_item_list) {
            if (par_item->text() != item->text()) continue;
            for (int j = 0; j < par_item->rowCount(); j++) { // 遍历父节点的子节点，寻找是否已经导入
                if (par_item->child(j)->text() == child->text()) { // 如果已经导入
                    setCurrentIndex(par_item->child(j)->index());
                    delete child;
                    delete item;
                    return;
                }
            }
            delete item;
            par_item->appendRow(child);
            setCurrentIndex(child->index());
            return;
        }
        tree_model->appendRow(item);
        parent_item_list.push_back(item);
        item->appendRow(child);
        expand(tree_model->indexFromItem(item));
        
        foreplay_widget->setAnnotationReadPath(FilePathOperation::getFileParentPath(filepath));
        foreplay_widget->setAnnotationSavePath(FilePathOperation::getFileParentPath(filepath));
        setCurrentIndex(child->index());
    }
}

//导入nii数据文件夹
void FileView::readITKImageDir()
{
    QString dirpath = QFileDialog::getExistingDirectory(this, tr("Input Dir"), "C:/Users/Administrator/Desktop", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);  //文件夹路径
    QDir dir(dirpath);  //文件夹
    QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Dirs);
    if (view_list_container->getViewList().size() < 3) {
        return;
    }
    if (!dirpath.trimmed().isEmpty()) {
        for (QStandardItem* par_item : parent_item_list) {
            if (par_item->data() == dirpath) return;
        }
        foreplay_widget->saveItemToPathAllFormAllScene();
        QString pixpath;
        QStringList list = dirpath.split('/');
        FileViewParentItem* item = new FileViewParentItem(list[list.indexOf(list.last())]);
        item->setData(dirpath);
        item->setToolTip(dirpath);
        item->setIcon(QIcon(":/res/background-image/folder_icon.png"));
        tree_model->appendRow(item);
        foreach(auto fileInfo, fileInfoList) {// 遍历文件夹中的所有文件
            if (fileInfo.isFile()) {
                pixpath = fileInfo.absoluteFilePath();
                if (pixpath.contains(".nii") || pixpath.contains(".nii.gz")
                    ) {   //判断文件是否为图像
                    list = pixpath.split('/');
                    FileViewChildItem* child = new FileViewChildItem(list.last());
                    child->setIsItkImageItem(true);
                    child->setData(pixpath);
                    child->setToolTip(pixpath);
                    list.removeAt(list.indexOf(list.last()));
                    child->setIcon(QIcon(":/res/background-image/picture_item.png"));
                    item->appendRow(child);
                }
            }
        }
        if (!item->hasChildren()) {
            tree_model->removeRow(item->row());
            return;
        }
        parent_item_list.push_back(item);
        FileViewChildItem* first_child = dynamic_cast<FileViewChildItem*>(tree_model->itemFromIndex(tree_model->index(0, 0, item->index())));
        first_child->setIsItkImageItem(true);
        pixpath = first_child->data().toString();
        setFilePath(pixpath);
        status_widget->setRightLabelText(load_filepath);
        itk_helper->readNiiImage(load_filepath.toStdString().c_str());
        QPixmap pix{};
        for (int i = 0; i < 3; ++i) {
            itk_helper->processAndConvertToPixmap(pix, i);
            view_list_container->getViewList()[i]->getGraphicsScene()->changePixmap(pix);
            view_list_container->getViewList()[i]->getGraphicsScene()->getPixmapItem()->setPixmapPath(load_filepath);
            view_list_container->getViewList()[i]->getGraphicsScene()->updateRbText(itk_helper->getCurSliceIndex(i) + 1, itk_helper->getDimensionSize(i));
        }
        dirpath = dirpath + "/";
        foreplay_widget->setAnnotationReadPath(dirpath);
        foreplay_widget->setAnnotationSavePath(dirpath);
        setExpanded(tree_model->indexFromItem(item), 1);
        setCurrentIndex(first_child->index());
    }
}

//切换图像
void FileView::readImageAtIndex(const QModelIndex& index)
{
    QStandardItem* item = tree_model->itemFromIndex(index);
    if (!item->hasChildren())
    {
        GraphicsScene* m_scene = view_list_container->getActivedView()->getGraphicsScene();
        //保存切换图像前item
        if (foreplay_widget->getAutoSave())
            foreplay_widget->saveItemToPathAllForm(m_scene);
        //读取切换图像后item
        FileViewChildItem* m_child = dynamic_cast<FileViewChildItem*>(item);
        load_filepath = item->data().toString();

        FileViewParentItem* parent_item = dynamic_cast<FileViewParentItem*>(item->parent());

        if (parent_item->getAnnotationReadPath() !=
            foreplay_widget->getAnnotationReadPath()) {
            foreplay_widget->setAnnotationReadPath(parent_item->getAnnotationReadPath());
        }
        if (parent_item->getAnnotationSavePath() !=
            foreplay_widget->getAnnotationSavePath()) {
            foreplay_widget->setAnnotationSavePath(parent_item->getAnnotationSavePath());
        }
        if (m_child->getIsItkImageItem()) {
            changeITKImage(load_filepath);
        }
        else {
            m_scene->changePixmap(load_filepath);
            foreplay_widget->readItemFromPathAllForm(m_scene);
            status_widget->setRightLabelText(load_filepath);
            m_scene->updateRbText(item->row() + 1, parent_item->rowCount());
        }
    }
    else {
        QItemSelection selection(index, index);
        select_model->select(selection, QItemSelectionModel::Select);
    }
}

void FileView::changeITKImage(const QString filepath)
{
    if (!filepath.trimmed().isEmpty())
    {
        itk_helper->readNiiImage(filepath.toStdString().c_str());
        QPixmap pix{};
        for (int i = 0; i < 3; ++i) {
            itk_helper->processAndConvertToPixmap(pix, i);
            view_list_container->getViewList()[i]->getGraphicsScene()->changePixmap(pix);
            view_list_container->getViewList()[i]->getGraphicsScene()->getPixmapItem()->setPixmapPath(filepath);
            view_list_container->getViewList()[i]->getGraphicsScene()->updateRbText(itk_helper->getCurSliceIndex(i) + 1, itk_helper->getDimensionSize(i));
        }
        setFilePath(filepath);
        status_widget->setRightLabelText(filepath);
    }
}

//右键菜单栏
void FileView::slotCustomContextMenuRequested(QPoint pos)
{
    QTreeView* treeView = qobject_cast<QTreeView*>(sender());  // sender()是QObejct的函数，用来获取连接时信号的发射指针
    if (!treeView)
    {
        return;
    }
    current_index = treeView->indexAt(pos);
    if (current_index.isValid())
    {
        QMenu menu;
        StyleSheetConfigModel style_sheet;
        style_sheet.setMenuStyle(&menu);
        menu.addAction(remove_img);
        menu.exec(QCursor::pos());  //QCursor::pos()让menu的位置在鼠标点击的的位置
    }
}

//移除图像
void FileView::removeImage()
{
    bool is_clear_all_scene = false;

    if (foreplay_widget->getAutoSave())
        foreplay_widget->saveItemToPathAllForm(view_list_container->getActivedView()->getGraphicsScene());

    QString remove_path = tree_model->itemFromIndex(current_index)->data().toString();
    QModelIndex par_index = current_index.parent();
    FileViewChildItem* child_item{};
    FileViewParentItem* parent_item{};
    if (par_index.row() >= 0) {     // 父级目录索引值是-1。当大于等于0时，说明选择的是目录下的某张图片
        parent_item = dynamic_cast<FileViewParentItem*>(tree_model->item(par_index.row()));
        child_item = dynamic_cast<FileViewChildItem*>(tree_model->itemFromIndex(current_index));
        parent_item->removeRow(current_index.row()); // 索引到所选行数的父级目录，精确删除某一目录下的某一行
        if (parent_item->rowCount() == 0) { // 如果该目录下没有对象时，如果没有这一行，还会留一个目录名在列表上
            int index = 0;
            foreach(FileViewParentItem * item, parent_item_list) {
                if (parent_item->data() == item->data()) break;
                index++;
            }
            parent_item_list.removeAt(index);
            tree_model->removeRow(par_index.row());
            is_clear_all_scene = true;
            remove_path = FilePathOperation::getFileParentPath(remove_path);
            remove_path = remove_path.removeLast();
        }
        else {
            if (load_filepath == remove_path) {
                FileViewChildItem* child_item = dynamic_cast<FileViewChildItem*>(tree_model->itemFromIndex(current_index));
                load_filepath = "";
                readImageAtIndex(par_index);
                if (child_item->getIsItkImageItem()) {
                    for (GraphicsView* view : view_list_container->getViewList()) {
                        GraphicsScene* scene = view->getGraphicsScene();
                        scene->resetScene();
                    }
                }
                else emit sceneClear();
            }
        }
    }
    else {
        parent_item = dynamic_cast<FileViewParentItem*>(tree_model->item(current_index.row()));
        int index = 0;
        is_clear_all_scene = true;
        foreach(FileViewParentItem * item, parent_item_list) {
            if (parent_item->data() == item->data()) break;
            index++;
        }
        parent_item_list.removeAt(index);
        tree_model->removeRow(current_index.row());
    } // 如果选择的是父级目录，则删除该目录下全部内容
    if (is_clear_all_scene)
    {
        for (GraphicsView* view : view_list_container->getViewList()) {
            GraphicsScene* scene = view->getGraphicsScene();
            if (load_filepath == scene->getPixmapItem()->getPixmapPath()) {
                load_filepath = "";
                emit viewClear();
            }
            QString par_path = FilePathOperation::getFileParentPath(scene->getPixmapItem()->getPixmapPath());
            par_path = par_path.removeLast();
            if (par_path == remove_path) {
                scene->resetScene();
            }
        }
        status_widget->setRightLabelText("");
        itk_helper->reset();
        return;
    }
}

//下一张
void FileView::nextIndex()
{
    QModelIndex current = currentIndex();
    QStandardItem* item = tree_model->itemFromIndex(current.parent());
    if (current.parent().row() >= 0 && current.row() + 1 < item->rowCount())
    {
        readImageAtIndex(item->child(current.row() + 1)->index());
        setCurrentIndex(item->child(current.row() + 1)->index());
    }
}

//上一张
void FileView::PreviousIndex()
{
    QModelIndex current = currentIndex();
    QStandardItem* item = tree_model->itemFromIndex(current.parent());
    if (current.parent().row() >= 0 && current.row() - 1 >= 0)
    {
        readImageAtIndex(item->child(current.row() - 1)->index());
        setCurrentIndex(item->child(current.row() - 1)->index());
    }
}

void FileView::onSliceChangeOneByOne(bool dir)
{
    if (!itk_helper)return;
    GraphicsView* emit_view = dynamic_cast<GraphicsView*>(sender());
    QPixmap pix{};
    QList<GraphicsView*> view_list = view_list_container->getViewList();
    for (int i = 0; i < 3; ++i) {
        if (view_list[i] == emit_view) {
            int slice = itk_helper->getCurSliceIndex(i);
            if (dir) {
                ++slice;
                if (slice > itk_helper->getDimensionSize(i) - 1)return;
            }
            else {
                --slice;
                if (slice < 0)return;
            }
            itk_helper->processAndConvertToPixmap(pix, i, slice);
            emit_view->getGraphicsScene()->getPixmapItem()->updatePixmap(pix);
            emit_view->getGraphicsScene()->getThumbnailItem()->updatePixmap(pix);
            emit_view->getGraphicsScene()->updateRbText(
                itk_helper->getCurSliceIndex(i) + 1, itk_helper->getDimensionSize(i));
            return;
        }
    }
}
