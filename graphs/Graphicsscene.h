#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H
#include <QWidget>
#include <QThread>
#include <QGraphicsScene>
#include <QFileDialog>
#include <QTime>
#include "Graphicsitem.h"
#include "Graphicspixmapitem.h"
#include "ThumbnailPixmapItem.h"
#include "model/ScenePromptItemModel.h"
#include "model/GiantMaskItemModel.h"

class LabelBoard;
class ItemIndexView;
class GraphicsView;
class InteractionPolygon;
class GraphicsTextItem;
class QTimer;
class GiantMaskItem;
struct SelectedLabelBoardData;

class GraphicsScene: public QGraphicsScene
{
    Q_OBJECT
        friend class GraphicsView;
        friend class SamSegmentRealTimeThread;
public:
    explicit GraphicsScene(QWidget *parent = nullptr); //构造函数
    ~GraphicsScene() override; //析构函数
    void setGraphicsView(GraphicsView* v);
    GraphicsView* getGraphicsView();
    void setItemIndexView(ItemIndexView* i);
    ItemIndexView* getItemIndexView();
    void setLabelBoardWidget(LabelBoard* w);
    LabelBoard* getLabelBoardWidget();
    ScenePromptItemModel* getScenePromptItemModel();
    GiantMaskItemModel* getGiantMaskItemModel();
    bool getIsPaintPromptItem(); //获取是否在绘制提示图元标志
    GraphicsItem* getPaintingItem(); //获取正在绘制的图形对象

    /***********Mask Operation***********/
    GiantMaskItem* getForegroundMaskItem(); //获取前景图层对象
    QList<GiantMaskItem*> getMaskItemList() const;
    void applyForegroundMask2Label(); //将前景mask应用到label
    void applySparyRect2Label(const QRect&); //将喷枪mask应用到label

    /***********图像设置***********/
    bool changeShowImage(const QImage&);//切换显示图像
    bool changeShowImage(const QString&);//切换显示图像

    /***********scene文本、缩略图图元***********/
    GiantImageItem* getPixmapItem(); //获取图像
    ThumbnailPixmapItem* getThumbnailItem(); //获取缩略图
    GraphicsTextItem* getLeftBottomTextItem(); //获取左下文本item
    GraphicsTextItem* getLeftUpTextItem(); //获取左上文本item
    GraphicsTextItem* getRightBottomTextItem(); //获取右下文本item
    GraphicsTextItem* getRightUpTextItem(); //获取右上文本item

    void initTextItem(); //初始化TextItem
    void updateRtText(); //更新右上文本
    void updateRbText(int index, int total_index); //更新右上文本
    void updateTextPos(); //更新文本控件位置
    void updateThumbnailBox(); //更新缩略图
    void updateItemIndexView(); //更新ItemIndexView控件

    /***********用于pixmapItem移动***********/
    void pixmapItemMoveStart();
    void pixmapItemMoveBy(const QPointF&);

    /***********用于GraphicsItem添加***********/
    void addItemAcceptLabelBoardSetting(GraphicsItem*);
    void addItemInit(GraphicsItem*);//scene添加item初始化设置（保存读取、测量等方式添加图元）

    /***********多边形绘制相关(坐标相关的参数，一样需要映射到scene)***********/
    bool getIsCreatePolygon();
    void pushBackPolygonPointConsitantly(const QPointF&);
    void pushBackPolygonPoint(const QPointF&);
    void finishCreatePolygon(); //完成绘制多边形
    void setContinuousPointDrawInterval(int intercal); //设置连续点绘制间隔

    bool isPaintItem(QGraphicsItem *item); //判断是否是继承GraphicsItem
    bool isPaintItemWithChild(QGraphicsItem *item); //判断是否是继承GraphicsItem包含子item
    bool isPaintItemOnScene(); //判断是否有绘制图元在scene上

    void clearSceneGraphicsItem();//删除所有图元
    void resetScene();//重置界面
    void clearPaintCache(); /******每次绘制完图元时，会自动补充
    下一个图元，有时我们需要清除这个图元的补充******/

    void initPaintFinishPromptItem(); //取消提示图元绘制操作
signals:
    void updatePoint(const QPointF &p,bool isCenter); //传递多边形点链表进item信号
    void paintContinue(); //继续绘画
    void promptContinue(); //继续绘制模型提示图元
    void createItemIndex(GraphicsItem* item); //在item索引控件中添加index元素
    void zoom3DLayout(bool); //true为放大，false为缩小

public slots:
    void pointClicked(int checked); //生成点item
    void ellipseClicked(int checked); //生成椭圆item
    void roundClicked(int checked); //生成圆形item
    void lineClicked(int checked); //生成直线item
    void rectClicked(int checked); //生成矩形item
    void polygonClicked(int checked); //生成多边形item
    void angleClicked(int checked); //生成角度item
    void parallelLineClicked(int checked); //生成平行线item
    void pieClicked(int checked); //生成圆饼item
    void lineSegmentClicked(int checked); //生成连续线段

    //模型提示图元
    void positivePointClicked(int checked);
    void negativePointClicked(int checked);
    void promptRectClicked(int checked);
    void PPlineSegmentClicked(int checked);
    void NPlineSegmentClicked(int checked);

private:
    void initImageShowSetting(); //初始化图像显示设置
    void addItemInitAfterPaint(GraphicsItem* item);//scene添加item初始化设置(通过绘制方式)
    void labelBoardAutoSelectNextRow(); //labelBoard选择下一行
    void initPaintGraphicsItem(); //初始化绘画操作
    void initPaintFinishGraphicsItem(); //取消绘画操作
    void initPaintPromptItem(); //初始化提示图元绘制操作
    void initItemSettingAfterPaint(GraphicsItem* item);//item初始化设置
    void startCreatePolygon(); //开始绘制多边形
    void cancelCreatePolygon(); //取消绘制多边形
    void createPromptItem(); //生成模型提示图元（一次只允许输入一个提示矩形框）
    void startAiModelSegment(); //使用自动分割模块,添加完提示图元之后，更新mask

    void samSegmentRealTime(); //sam实时自动分割
    /***********用于绘制graphicsItem, 参数点坐标需要映射到scene上,
  三条函数基本配合使用，目前用在鼠标事件中***********/
    void createPaintItemAtPoint(const QPointF&);
    void setPaintItemPoint(const QPointF&);
    void afterSetPaintItemPoint(const QPointF&);
    void afterSetPromptItemPoint(const QPointF&);

    GraphicsView* m_view = nullptr;
    LabelBoard* label_board_widget = nullptr;
    ItemIndexView* item_index_view = nullptr;
    ScenePromptItemModel scene_prompt_model{}; //scene提示图元模型
    GiantMaskItemModel mask_item_model; //mask图元模型

    GraphicsItem *painting_item = nullptr; //指向正在绘制的图形对象
    InteractionPolygon* painting_pol_item = nullptr; //指向正在绘制的多边形对象
    QList<QPointF> polygon_list; //多边形点链表
    bool is_creating_polygon = false; //判断是否在绘制多边形
    bool is_paint_new_item = false; //判断是否开始绘制一个新的item
    bool is_paint_prompt_item = false; //是否在绘制模型提示图元
    int Continuous_point_draw_interval = 20; //连续点绘制间隔

    QPointF image_pos_before_move{}; //图像移动前位置
    GiantImageItem pixmap_item{ QImage{} }; //图像项
    ThumbnailPixmapItem thumbnail_item{ QImage{} }; //图像缩略图
    GraphicsTextItem* text_left_bottom = nullptr; //左下文本
    GraphicsTextItem* text_left_up = nullptr; //左上文本
    GraphicsTextItem* text_right_bottom = nullptr; //右下文本
    GraphicsTextItem* text_right_up = nullptr; //右上文本
};

#endif // GRAPHICSSCENE_H
