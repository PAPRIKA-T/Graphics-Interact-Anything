#include "CVOperation.h"
#include <QPixmap>
#include <QColor>
#include <QDebug>

QPixmap CVOperation::matToPixmap(const cv::Mat& cvImage)
{
    // 转换为 QImage
    QImage qImage(cvImage.data, cvImage.cols, cvImage.rows, cvImage.step, QImage::Format_RGB888);
    // 将 QImage 转换为 QPixmap
    QPixmap pixmap = QPixmap::fromImage(qImage);
    return pixmap;
}

cv::Mat CVOperation::QPixmapToMat(const QPixmap& pixmap, bool inCloneImageData)
{
    QImage org_image = pixmap.toImage();
    switch (org_image.format())
    {
        // 8-bit, 4 channel
    case QImage::Format_RGB32:
    {
        cv::Mat mat(org_image.height(), org_image.width(), CV_8UC4, const_cast<uchar*>(org_image.bits()), org_image.bytesPerLine());
        return (inCloneImageData ? mat.clone() : mat);
    }

    // 8-bit, 3 channel
    case QImage::Format_RGB888:
    {
        if (!inCloneImageData) {
            qWarning() << "ASM::QImageToCvMat() - Conversion requires cloning since we use a temporary QImage";
        }
        QImage swapped = org_image.rgbSwapped();
        return cv::Mat(swapped.height(), swapped.width(), CV_8UC3, const_cast<uchar*>(swapped.bits()), swapped.bytesPerLine()).clone();
    }

    // 8-bit, 1 channel
    case QImage::Format_Indexed8:
    {
        cv::Mat  mat(org_image.height(), org_image.width(), CV_8UC1, const_cast<uchar*>(org_image.bits()), org_image.bytesPerLine());

        return (inCloneImageData ? mat.clone() : mat);
    }

    default:
        qDebug("Image format is not supported: depth=%d and %d format\n", org_image.depth(), org_image.format());
        break;
    }
    return cv::Mat();
}

cv::Mat CVOperation::getAnnotation(const cv::Mat& org_image, const cv::Mat& mask, cv::Vec3b color, bool random)
{
    if (org_image.empty() || mask.empty()) {
        std::cerr << "SamWidget::getAnnotation Failed to load images" << std::endl;
        cv::Mat emptyMat{};
        return emptyMat;
    }
    // 创建一个彩色版本的掩码（在掩码上应用伪彩色，以便与原始图像叠加）
    cv::Mat result = org_image;
    if (random) {
        // generate random color
        cv::RNG rng(time(0));
        color = cv::Vec3b (rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
    }
    // 设置被掩膜遮住的部分的颜色
    for (int i = 0; i < mask.rows; ++i) {
        for (int j = 0; j < mask.cols; ++j) {
            if (mask.at<uchar>(i, j) != 0) {
                result.at<cv::Vec3b>(i, j) = color*0.5;
            }
        }
    }
    cv::cvtColor(result, result, cv::COLOR_BGR2RGB);
    return result;
}

QPixmap CVOperation::getAnnotation(const QPixmap& org_image, const cv::Mat& mask, QColor color, bool random)
{
    if (org_image.isNull() || mask.empty()) {
        std::cerr << "SamWidget::getAnnotation Failed to load images" << std::endl;
        QPixmap emptyPixmap{};
        return emptyPixmap;
    }
    // 创建一个彩色版本的掩码（在掩码上应用伪彩色，以便与原始图像叠加）

    QImage image = org_image.toImage();
    if (random) {
        // generate random color
        cv::RNG rng(time(0));
        cv::Vec3b cv_color = cv::Vec3b(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
        color = QColor(cv_color[0], cv_color[1], cv_color[2]);
    }
    // 设置被掩膜遮住的部分的颜色
    for (int i = 0; i < mask.rows; ++i) {
        for (int j = 0; j < mask.cols; ++j) {
            if (mask.at<uchar>(i, j) != 0) {
                image.setPixel(j, i, color.rgb());
            }
        }
    }

    return  QPixmap::fromImage(image);
}

QPixmap CVOperation::getAnnotation(const QImage& org_image, const cv::Mat& mask, QColor color, bool random)
{
    if (org_image.isNull() || mask.empty()) {
        std::cerr << "SamWidget::getAnnotation Failed to load images" << std::endl;
        QPixmap emptyPixmap{};
        return emptyPixmap;
    }
    // 创建一个彩色版本的掩码（在掩码上应用伪彩色，以便与原始图像叠加）

    QImage image{ org_image.size(), QImage::Format_ARGB32};
    image = org_image;
    
    if (random) {
        // generate random color
        cv::RNG rng(time(0));
        cv::Vec3b cv_color = cv::Vec3b(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
        color = QColor(cv_color[0], cv_color[1], cv_color[2]);
    }
    color.setAlpha(200);
    // 设置被掩膜遮住的部分的颜色
    for (int i = 0; i < mask.rows; ++i) {
        for (int j = 0; j < mask.cols; ++j) {
            if (mask.at<uchar>(i, j) != 0) {
                image.setPixelColor(j, i, color);
            }
        }
    }
    return  QPixmap::fromImage(image);
}
