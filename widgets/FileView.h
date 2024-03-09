#pragma once
#include <QTreeView>
#include <QStandardItem>

/****************************************************导入文件索引的树状控件*********************************************************/
class FileViewParentItem : public QStandardItem
{
public:
    FileViewParentItem(const QString& s);
    ~FileViewParentItem();
    const QString getAnnotationReadPath() { return annotation_read_path; }
    const QString getAnnotationSavePath() { return annotation_save_path; }
    void setAnnotationReadPath(const QString& path) { annotation_read_path = path; }
    void setAnnotationSavePath(const QString& path) { annotation_save_path = path; }
private:
    QString annotation_read_path = "";
    QString annotation_save_path = "";
};

class FileViewChildItem : public QStandardItem
{
public:
    FileViewChildItem(const QString& s);
    ~FileViewChildItem();
    bool getIsItkImageItem() { return is_itk_image_item; }
    void setIsItkImageItem(bool ok) { is_itk_image_item = ok; }
private:
    bool is_itk_image_item = false;
};

class ITKImageIoModel;
class ViewListContainer;
class VTKWidget;
class StatusWidget;
class ForePlayWidget;

class FileView :public QTreeView
{
    Q_OBJECT
public:
    FileView(QWidget* parent = nullptr);
    ~FileView() override;
    void setStatusWidget(StatusWidget* s);
    StatusWidget* getStatusWidget();

    void setForeplayWidget(ForePlayWidget* w);
    ForePlayWidget* getForePlayWidget();

    void setFilePath(const QString& p) { load_filepath = p; }
    QString getFilePath() { return load_filepath; }
    void setViewListContainer(ViewListContainer*);
    QStandardItemModel* getModel() { return tree_model; }
    virtual void enterEvent(QEvent* event);
    QList<FileViewParentItem*>& getViewParentItemList() { return parent_item_list; }
    void setItemSelected(const QString& pixmap_path);
    void setVTKWidget(VTKWidget* v) { vtk_widget = v; }
    void nextIndex(); //index++ 下一张图像
    void PreviousIndex(); //上一张图像

signals:
    void sceneClear();
    void viewClear();

public slots:
    void readImage(); //导入图像
    void readImageDir(); //导入文件夹
    void readITKImage(); //导入nii数据
    void readITKImageDir(); //导入nii数据文件夹
    void slotCustomContextMenuRequested(QPoint); //右键菜单栏槽
    void onSliceChangeOneByOne(bool);

private slots:
    void readImageAtIndex(const QModelIndex& index); //更改图像
private:
    void changeITKImage(const QString filepath);
    void removeImage(); //删除图像

    QStandardItemModel* tree_model; //管理item控件model
    QItemSelectionModel* select_model; //选择model
    ForePlayWidget* foreplay_widget = nullptr;

    ViewListContainer* view_list_container = nullptr; //视图容器指针
    StatusWidget* status_widget = nullptr;
    QString load_filepath = ""; //用于存储当前打开图像路径
    QAction* remove_img = new QAction("remove");
    QModelIndex current_index; // 当前选中的位置
    QList<FileViewParentItem*> parent_item_list; //父节点容器
    ITKImageIoModel* itk_helper = nullptr; //ITKImageHelper指针，管理itk三维数据
    VTKWidget* vtk_widget = nullptr; //vtkWidget指针
};