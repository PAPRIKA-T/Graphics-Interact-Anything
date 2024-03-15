#pragma once
#include <QWidget>

class GenericToolButton;
class GraphicsView;
class QHBoxLayout;
class QButtonGroup;

class AiModelInteractWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AiModelInteractWidget(QWidget *parent = nullptr);
	~AiModelInteractWidget();
	void setGraphicsView(GraphicsView* view);
	//bool eventFilter(QObject* object, QEvent* event) override;
private:
    QHBoxLayout* main_layout = nullptr;
    GraphicsView* m_view = nullptr;
    QButtonGroup* exclusive_button_group = nullptr;

    GenericToolButton* positive_point = nullptr;
    GenericToolButton* negative_point = nullptr;
    GenericToolButton* box_prompt = nullptr;
};

