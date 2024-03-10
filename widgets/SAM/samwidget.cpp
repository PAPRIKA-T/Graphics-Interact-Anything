#include "samwidget.h"
#include "graphs/Graphicsscene.h"
#include "widgets/ComboBoxVWidget.h"
#include "widgets/GenericToolButton.h"
#include <QWidget>
#include <locale>
#include <codecvt>
#include <QStyleOption>
#include <QComboBox>
#include <QScrollArea>

/**************************************************Point Prompt Widget********************************************************/
PromptWidget::PromptWidget(QWidget* parent)
    :QWidget(parent)
{
    prompt_btn = new GenericToolButton();
    prompt_btn->setFixedSize(35, 35);
    label = new QLabel(this);
    label->setObjectName("sam_label");

    main_layout = new QHBoxLayout(this);
    main_layout->addWidget(prompt_btn);
    main_layout->addWidget(label);
    main_layout->setContentsMargins(2, 2, 2, 2);
    main_layout->addStretch();
}

PromptWidget::~PromptWidget()
{
    delete main_layout;
}

/**************************************************Sam Widget********************************************************/

SamWidget::SamWidget(QWidget* parent)
    : QWidget{ parent }
{
    scroll_area_widget = new QWidget(this);
    scroll_area_widget->setObjectName("scroll_area_widget");
    scroll_area_layout = new QVBoxLayout(this);
    scroll_area_layout->setContentsMargins(0, 0, 0, 0);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setWidget(scroll_area_widget);

    scroll_area_layout->addWidget(scrollArea);

    main_layout = new QVBoxLayout(scroll_area_widget);
    model_select_widget = new ComboBoxVWidget(this);
    model_select_widget->setLabelText("Select a Model:");
    model_select_widget->getComboBox()->addItem("mobile_sam(faster)");
    model_select_widget->getComboBox()->addItem("sam(More expenses)");

    interaction_mode_widget = new ComboBoxVWidget(this);
    interaction_mode_widget->setLabelText("Interaction Mode:");
    interaction_mode_widget->getComboBox()->addItem("Single Input");
    interaction_mode_widget->getComboBox()->addItem("Multiple Input");

    output_shape_widget = new ComboBoxVWidget(this);
    output_shape_widget->setLabelText("Output Shape:");
    output_shape_widget->getComboBox()->addItem("Polygon");
    output_shape_widget->getComboBox()->addItem("Rect");
    
    load_model_btn = new GenericToolButton("Load Model");
    load_model_btn->setFixedHeight(30);
    //load_model_btn->setCheckable(true);

    positive_point_widget = new PromptWidget(this);
    positive_point_widget->setLabelText("PPoint Prompt");

    negative_point_widget = new PromptWidget(this);
    negative_point_widget->setLabelText("NPoint Prompt");

    box_prompt_widget = new PromptWidget(this);
    box_prompt_widget->setLabelText("Box Prompt");

    pp_list_prompt_widget = new PromptWidget(this);
    pp_list_prompt_widget->setLabelText("PP List Prompt");

    np_list_prompt_widget = new PromptWidget(this);
    np_list_prompt_widget->setLabelText("NP List Prompt");

    segment_anything_btn = new GenericToolButton("Segment Anything", this);
    segment_anything_btn->setFixedHeight(30);

    delete_all_prompt_items_btn = new GenericToolButton("Delete All Prompt Items", this);
    delete_all_prompt_items_btn->setFixedHeight(30);

    main_layout->addWidget(model_select_widget);
    main_layout->addWidget(load_model_btn);
    main_layout->addWidget(interaction_mode_widget);
    main_layout->addWidget(output_shape_widget);
    main_layout->addWidget(positive_point_widget);
    main_layout->addWidget(negative_point_widget);
    main_layout->addWidget(box_prompt_widget);
    main_layout->addWidget(pp_list_prompt_widget);
    main_layout->addWidget(np_list_prompt_widget);
    main_layout->addWidget(delete_all_prompt_items_btn);
    main_layout->addWidget(segment_anything_btn);

    main_layout->addStretch();

    positive_point = positive_point_widget->getButton();
    negative_point = negative_point_widget->getButton();
    box_prompt = box_prompt_widget->getButton();
    pp_list_prompt = pp_list_prompt_widget->getButton();
    np_list_prompt = np_list_prompt_widget->getButton();

    positive_point->setObjectName("Positive Point");
    negative_point->setObjectName("Negative Point");
    box_prompt->setObjectName("Box Prompt");

    positive_point->setIconSize(QSize(30, 30));
    negative_point->setIconSize(QSize(30, 30));
    box_prompt->setIconSize(QSize(20, 20));

    positive_point->setIcon(QIcon(":/res/background-image/positive_point.png"));
    negative_point->setIcon(QIcon(":/res/background-image/negative_point.png"));
    box_prompt->setIcon(QIcon(":/res/background-image/prompt_rect.png"));
    pp_list_prompt->setIcon(QIcon(":/res/background-image/pos_point_list.png"));
    np_list_prompt->setIcon(QIcon(":/res/background-image/neg_point_list.png"));

    positive_point->setCheckable(true);
    negative_point->setCheckable(true);
    box_prompt->setCheckable(true);
    pp_list_prompt->setCheckable(true);
    np_list_prompt->setCheckable(true);
    enablePromptWidgets(false);
}

SamWidget::~SamWidget()
{
    delete sam;
    main_layout->deleteLater();
}

QPushButton* SamWidget::getDeleteAllPromptItemBtn()
{
    return delete_all_prompt_items_btn;
}

QPushButton* SamWidget::getSegmentAnythingBtn()
{
    return segment_anything_btn;
}

void SamWidget::initSamModel()
{
    if (sam) return;
    int selected_model = model_select_widget->getComboBox()->currentIndex();
    std::string pre_model_path{};
    std::string model_path{};
    if (selected_model == 0) {
        pre_model_path = "ai_models/sam/mobile_sam_preprocess.onnx";
        model_path = "ai_models/sam/mobile_sam.onnx";
    }
    else if (selected_model == 1) {
        pre_model_path = "ai_models/sam/sam_preprocess.onnx";
        model_path = "ai_models/sam/sam_vit_h_4b8939.onnx";
    }

    Sam::Parameter param(pre_model_path, model_path, std::thread::hardware_concurrency());
    param.providers[0].deviceType = 0; // cpu for preprocess
    param.providers[1].deviceType = 0; // CUDA for sam
    sam = new Sam(param);
    if (!sam) {
        qDebug() << "load model fail!";
        return;
    }
    enablePromptWidgets(true);
}

void SamWidget::unLoadSamModel()
{
    delete sam;
    sam = nullptr;
    enablePromptWidgets(false);
}

void SamWidget::enablePromptWidgets(bool enable)
{
    positive_point->setEnabled(enable);
    negative_point->setEnabled(enable);
    box_prompt->setEnabled(enable);
    pp_list_prompt->setEnabled(enable);
    np_list_prompt->setEnabled(enable);
}

void SamWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QStyleOption styleOpt;
    styleOpt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &styleOpt, &painter, this);
}
