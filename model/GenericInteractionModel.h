#pragma once
class GraphicsView;
class GenericInteractionModel
{
public:
    enum class InteractionStatus
    {
        INTERACTION_RUBBER,
        INTERACTION_OBSERVE,
        INTERACTION_PAINT,
        INTERACTION_SELECT,
        INTERACTION_CALCULATE,
        INTERACTION_EDIT_POLYGON,
        INTERACTION_SAM,
        INTERACTION_SPRAY,
    };
    GenericInteractionModel();
    void setGraphicsView(GraphicsView* view);
    InteractionStatus getInteractionStatus(); //获取当前交互模式
    void setObserveInteraction(bool ok); //查看交互模式
    void setRubberInteraction(bool ok); //擦除交互模式
    void setPaintInteraction(bool ok); //绘制交互模式
    void setSelectInteraction(bool ok); //选择交互模式
    void setCalculateInteraction(bool ok); //测量交互模式
    void setEditPolygonInteraction(bool ok); //编辑多边形交互模式
    void setSamModelInteraction(bool ok); //sam模型交互模式
    void setSprayInteraction(bool ok); //喷涂模式
private:
    void setPolygonItemEdit(bool ok); //设置多边形item是否可编辑
    InteractionStatus interaciton_status{ InteractionStatus::INTERACTION_OBSERVE };
    GraphicsView* m_view{ nullptr };
};