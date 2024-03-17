#include "ImageCvLoaderThread.h"
#include <QDebug>

void ImageCvLoaderThread::setPixmapPath(const QString& path)
{
	pixmapPath = path;
	start(); // 启动线程;
}

void ImageCvLoaderThread::run()
{
    // 在这里放入你的函数体
    if (pixmapPath.isEmpty()) {
        emit imageLoaded(cv::Mat()); // 发送一个空的图像信号表示加载失败
        return;
    }

    orgin_mat = cv::imread(pixmapPath.toStdString(), -1);

    if (!orgin_mat.empty()) {
        emit imageLoaded(orgin_mat); // 发送加载成功的图像信号
    }
    else {
        qDebug() << "Image load fail";
        emit imageLoaded(cv::Mat()); // 发送一个空的图像信号表示加载失败
    }
    deleteLater(); // 加载完成后自动销毁
}
