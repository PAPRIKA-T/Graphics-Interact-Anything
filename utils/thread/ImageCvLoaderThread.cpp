#include "ImageCvLoaderThread.h"
#include "utils/CVOperation.h"
#include <QDebug>

void ImageCvLoaderThread::translateQImage2cvMat(const QImage& image)
{
    i = image;
	start(); // 启动线程;
}

void ImageCvLoaderThread::run()
{
    // 在这里放入你的函数体
    if (i.isNull()) {
        emit imageLoaded(cv::Mat()); // 发送一个空的图像信号表示加载失败
        return;
    }

    orgin_image_mat = CVOperation::QImage2cvMat(i);

    if (!orgin_image_mat.empty()) {
        emit imageLoaded(orgin_image_mat); // 发送加载成功的图像信号
    }
    else {
        qDebug() << "Image load fail";
        emit imageLoaded(cv::Mat()); // 发送一个空的图像信号表示加载失败
    }
    deleteLater(); // 加载完成后自动销毁
}
