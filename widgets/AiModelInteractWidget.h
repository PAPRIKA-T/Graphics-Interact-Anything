#pragma once
#include <QWidget>

class GenericToolButton;
class GraphicsView;
class QHBoxLayout;
class QButtonGroup;
class ComboBoxHWidget;
class QPushButton;
class GraphicsScene;

class AiModelInteractWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AiModelInteractWidget(QWidget *parent = nullptr);
	~AiModelInteractWidget();
	void setGraphicsView(GraphicsView* view);
    ComboBoxHWidget* getOutputShapeWidget();
    QButtonGroup* getExclusiveButtonGroup() const;
    void promptItemLoad();
    QPushButton* getAcceptBtn() const;
    QPushButton* getClearBtn() const;
private:
    void connectSceneSignal(GraphicsScene*);
    GenericToolButton* accept_btn = nullptr;
    GenericToolButton* clear_btn = nullptr;

    QHBoxLayout* main_layout = nullptr;
    GraphicsView* m_view = nullptr;
    QButtonGroup* exclusive_button_group = nullptr;

    ComboBoxHWidget* output_shape_widget = nullptr;
    GenericToolButton* positive_point = nullptr;
    GenericToolButton* negative_point = nullptr;
    GenericToolButton* box_prompt = nullptr;
};

