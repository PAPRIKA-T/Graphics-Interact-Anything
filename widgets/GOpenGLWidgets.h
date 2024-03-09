#pragma once
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QtOpenGL/QOpenGLFunctions_3_3_Core>

class OpenGLWidget : public QOpenGLWidget, QOpenGLFunctions_3_3_Core
{
	Q_OBJECT
public:
	OpenGLWidget(QWidget *parent = nullptr);
	~OpenGLWidget();

protected:
	virtual void initializeGL();
	virtual void paintGL();

private:
};

