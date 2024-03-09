#include "GiantPushButton.h"
#include "style/ToolTipWindow.h"
#include <QToolTip>

GiantPushButton::GiantPushButton(QWidget* parent)
    : QPushButton(parent)
{

}

GiantPushButton::GiantPushButton(const QString& text, QWidget* parent)
    :QPushButton(text, parent)
{
}

GiantPushButton::~GiantPushButton()
{

}

void GiantPushButton::setCustomTooltip(const QString& tooltip)
{
    if (tooltip.isEmpty())return;
    tooltip_text = tooltip;
    is_set_custom_tooltip = true;
}

void GiantPushButton::enterEvent(QEnterEvent* event)
{
    Q_UNUSED(event);
    if (!is_set_custom_tooltip)return;
    // 获取按钮底部中心点的全局坐标
    QPoint bottomCenter = mapToGlobal(rect().bottomLeft());
    bottomCenter.setX(bottomCenter.x() + rect().width() / 2);

    // 获取工具提示文本的宽度
    int tooltipWidth = QFontMetrics(QToolTip::font()).horizontalAdvance(tooltip_text);

    // 计算工具提示文本的左上角位置
    QPoint tooltipPos(bottomCenter.x() - tooltipWidth / 2, bottomCenter.y());

    // 创建 TooltipWindow 实例，并显示
    tooltip_window = new ToolTipWindow(tooltip_text);
    tooltip_window->move(tooltipPos);
    tooltip_window->show();
}

void GiantPushButton::leaveEvent(QEvent* event)
{
    Q_UNUSED(event);
    if (tooltip_window) {
        tooltip_window->close();
        delete tooltip_window;
        tooltip_window = nullptr;
    }
}