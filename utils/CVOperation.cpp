#include "CVOperation.h"
#include <QPixmap>
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

void CVOperation::setMaskColor(const cv::Mat& mask, cv::Mat& color_mask, cv::Vec3b color, bool random)
{
    if (random) {
        // generate random color
        cv::RNG rng(time(0));
        cv::Vec3b color_rng(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
        // 遍历图像的每个像素，根据掩膜来应用颜色
        for (int y = 0; y < mask.rows; y++) {
            for (int x = 0; x < mask.cols; x++) {
                if (mask.at<uchar>(y, x) > 0) {
                    color_mask.at<cv::Vec3b>(y, x) = cv::Vec3b(color_rng[0], color_rng[1], color_rng[2]);
                }
            }
        }return;
    }
    for (int y = 0; y < mask.rows; y++) {
        for (int x = 0; x < mask.cols; x++) {
            if (mask.at<uchar>(y, x) > 0) {
                color_mask.at<cv::Vec3b>(y, x) = cv::Vec3b(color[0], color[1], color[2]);
            }
        }
    }
}

cv::Mat CVOperation::getAnnotation(const cv::Mat& org_image, const cv::Mat& mask, cv::Vec3b color, bool random)
{
    if (org_image.empty() || mask.empty()) {
        std::cerr << "SamWidget::getAnnotation Failed to load images" << std::endl;
        cv::Mat emptyMat;
        return emptyMat;
    }
    // 创建一个彩色版本的掩码（在掩码上应用伪彩色，以便与原始图像叠加）
    cv::Mat result;
    cv::Mat coloredMask = cv::Mat::zeros(org_image.size(), org_image.type());

    // generate random color
    setMaskColor(mask, coloredMask, color, random);
    cv::cvtColor(coloredMask, coloredMask, cv::COLOR_BGR2RGB);
    cv::cvtColor(org_image, result, cv::COLOR_BGR2RGB);
    result = result + coloredMask * 0.8;
    return result;
}
