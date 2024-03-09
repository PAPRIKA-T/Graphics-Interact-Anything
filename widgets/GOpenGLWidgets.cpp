#include "GOpenGLWidgets.h"

OpenGLWidget::OpenGLWidget(QWidget *parent)
	: QOpenGLWidget(parent)
{
	// 设置OpenGL双缓冲
	QSurfaceFormat format;
	format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
	setFormat(format);
}

OpenGLWidget::~OpenGLWidget()
{
}

void OpenGLWidget::initializeGL()
{
	// 1.初始化OpenGL函数，否则OpenGL函数不可调用
	initializeOpenGLFunctions();
}

void OpenGLWidget::paintGL()
{
	// 2.initializeOpenGLFunctions();执行后，下面的函数才有执行的意义
	// 设置窗口颜色
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}
