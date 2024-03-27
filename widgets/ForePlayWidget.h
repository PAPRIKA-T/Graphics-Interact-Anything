#pragma once
#include <QWidget>
#include <QVBoxLayout>

class FileView;
class ChosePathWidget;
class ViewListContainer;
class GraphicsScene;

class ForePlayWidget : public QWidget
{
    Q_OBJECT
public:
    ForePlayWidget(QWidget* parent = nullptr);
    ~ForePlayWidget();
    void setFileView(FileView* view) { file_view = view; }
    FileView* getFileView() { return file_view; }
    void setViewListContainer(ViewListContainer*);

    const QString getAnnotationReadPath();
    const QString getAnnotationSavePath();
    void setAnnotationReadPath(const QString& path);
    void setAnnotationSavePath(const QString& path);
    const QString getAnnotationReadForm();
    const QString getAnnotationSaveForm();

    ChosePathWidget* getReadPathWidget() { return annotation_read_path_widget; }
    ChosePathWidget* getSavePathWidget() { return annotation_save_path_widget; }

    void saveItemToPathAllForm(GraphicsScene*); //保存标注图形到指定路径(获取参数面板上的保存路径)
    void readItemFromPathAllForm(GraphicsScene*); //读取标注图形到指定路径(读取参数面板上的保存路径)

    void setAutoSave(bool ok) { is_auto_save = ok; }
    bool getAutoSave() { return is_auto_save; }

public slots:
    void onPathClear();
    /***************IO Function*****************/
    void saveItemToPathAllFormAllScene(); //保存所有视图图元
    void readItemFromPathAllFormAllScene(); //读取所有视图图元

private:
    void readItemFromPathXml(const QString& ,GraphicsScene*); //pascalVoc Input
    void saveItemToPathXml(const QString&, GraphicsScene*); //pascalVoc Output

    void saveMaskToPathNii(const QString&, GraphicsScene*); //nii Output

    void annotationReadPathChange(const QString& path);
    void annotationSavePathChange(const QString& path);

    ViewListContainer* view_list_container = nullptr; //视图容器指针
    FileView* file_view = nullptr;
    ChosePathWidget* annotation_read_path_widget = nullptr;
    ChosePathWidget* annotation_save_path_widget = nullptr;
    QVBoxLayout* main_layout = nullptr;

    bool is_auto_save = true; //是否自动保存
};

