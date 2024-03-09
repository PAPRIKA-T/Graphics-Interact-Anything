#pragma once

#include <QPushButton>

class ColorButton : public QPushButton
{
    Q_OBJECT
public:
    ColorButton(QColor c = Qt::white, QWidget* parent = nullptr);
    ~ColorButton();
    void setBackgrondColor(QColor c); //设置背景颜色
    QColor getBackgroundColor() { return background_color; } //获取背景颜色
    const QString colorToQString(const QColor& c, const QString type);
signals:
    void sentSelf(ColorButton*);
    void sentSelfColor(QColor c); //信号:传递自身背景颜色
public slots:
    void sentColor() { emit sentSelfColor(background_color); } //槽:发射信号
    void openColorDialog(); //打开默认调色板
private:
    QColor background_color; //背景颜色
};
