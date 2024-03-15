#ifndef SAMWIDGET_H
#define SAMWIDGET_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

#include <atomic>
#include <opencv2/opencv.hpp>
#include <thread>
#define STRIP_FLAG_HELP 1
#include <gflags/gflags.h>
#include "sam.h"

class HeaderButton;
class ComboBoxVWidget;

/****************************************Prompt Widget*******************************************/
class GraphicsScene;

class PromptWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PromptWidget(QWidget* parent = nullptr);
    ~PromptWidget();
    void setLabelText(QString s) { label->setText(s); }
    void setLabelTextFont(QFont f) { label->setFont(f); }
    QLabel* getLabel() { return label; }
    QPushButton* getButton() { return prompt_btn; }
private:
    QHBoxLayout* main_layout = nullptr;
    QPushButton* prompt_btn = nullptr;
    QLabel* label = nullptr;
};

/****************************************Sam Widget*******************************************/
class SamWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SamWidget(QWidget *parent = nullptr);
    ~SamWidget();
    void setSam(Sam s) { sam = &s; }
    Sam* getSam() { return sam; }

    QPushButton* getLoadModelBtn() { return load_model_btn; }
    QPushButton* getDeleteAllPromptItemBtn();
    QPushButton* getSegmentAnythingBtn();
    PromptWidget* getPositivePointWidget() { return positive_point_widget; }
    PromptWidget* getNegativePointWidget() { return negative_point_widget; }
    PromptWidget* getBoxPromptWidget() { return box_prompt_widget; }
    PromptWidget* getPPListPromptWidget() { return pp_list_prompt_widget; }
    PromptWidget* getNPListPromptWidget() { return np_list_prompt_widget; }

    ComboBoxVWidget* getModelSelectWidget() { return model_select_widget; }
    ComboBoxVWidget* getInteractionModeWidget() { return interaction_mode_widget; }
    ComboBoxVWidget* getOutputShapeWidget() { return output_shape_widget; }

    virtual void paintEvent(QPaintEvent* event) override;

    void initSamModel();
    void unLoadSamModel();
    void enablePromptWidgets(bool enable);
private:
    QWidget* scroll_area_widget = nullptr;
    QVBoxLayout* scroll_area_layout = nullptr;

    Sam* sam = nullptr;
    QVBoxLayout* main_layout = nullptr;
    ComboBoxVWidget* model_select_widget = nullptr;
    ComboBoxVWidget* interaction_mode_widget = nullptr;
    ComboBoxVWidget* output_shape_widget = nullptr;
    QPushButton* load_model_btn = nullptr;
    QPushButton* delete_all_prompt_items_btn = nullptr;
    PromptWidget* positive_point_widget = nullptr;
    PromptWidget* negative_point_widget = nullptr;
    PromptWidget* box_prompt_widget = nullptr;
    PromptWidget* pp_list_prompt_widget = nullptr;
    PromptWidget* np_list_prompt_widget = nullptr;
    QPushButton* segment_anything_btn = nullptr;

    QPushButton* positive_point = nullptr;
    QPushButton* negative_point = nullptr;
    QPushButton* box_prompt = nullptr;
    QPushButton* pp_list_prompt = nullptr;
    QPushButton* np_list_prompt = nullptr;
};

#endif // SAMWIDGET_H
