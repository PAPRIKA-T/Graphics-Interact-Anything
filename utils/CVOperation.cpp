#include "CVOperation.h"
#include <QPixmap>
#include <QColor>
#include <QBitmap>
#include <QDebug>

QImage CVOperation::cvMat2QImage(const cv::Mat& mat)
{
    // 转换为 QImage
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if (mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for (int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar* pSrc = mat.data;
        for (int row = 0; row < mat.rows; row++)
        {
            uchar* pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if (mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar* pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if (mat.type() == CV_8UC4)
    {
        qDebug() << "CV_8UC4";
        // Copy input Mat
        const uchar* pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}

cv::Mat CVOperation::QImage2cvMat(const QImage& image, bool inCloneImageData)
{
    cv::Mat mat{};
    qDebug() << image.format();
    switch (image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    }
    return mat;
}

cv::Mat CVOperation::setMaskOnImage(const cv::Mat& org_image, const cv::Mat& mask, cv::Vec3b color, bool random)
{
    if (org_image.empty() || mask.empty()) {
        std::cerr << "SamWidget::setMaskOnImage Failed to load images" << std::endl;
        cv::Mat emptyMat{};
        return emptyMat;
    }
    // 创建一个彩色版本的掩码（在掩码上应用伪彩色，以便与原始图像叠加）
    cv::Mat result = org_image;
    if (random) {
        color = generateRandomVec3bColor();
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

QImage CVOperation::setMaskOnImage(const QImage& org_image, const cv::Mat& mask, QColor color, bool random)
{
    if (org_image.isNull() || mask.empty()) {
        std::cerr << "SamWidget::setMaskOnImage Failed to load images" << std::endl;
        QImage emptyImage{};
        return emptyImage;
    }
    // 创建一个彩色版本的掩码（在掩码上应用伪彩色，以便与原始图像叠加）

    QImage image = org_image;
    if (random) {
        color = generateRandomQColor();
    }
    // 设置被掩膜遮住的部分的颜色
    for (int i = 0; i < mask.rows; ++i) {
        for (int j = 0; j < mask.cols; ++j) {
            if (mask.at<uchar>(i, j) != 0) {
                image.setPixel(j, i, color.rgb());
            }
        }
    }
    return image;
}

QColor CVOperation::generateRandomQColor()
{
    cv::RNG rng(time(0));
    cv::Vec3b cv_color = cv::Vec3b(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
    return QColor(cv_color[0], cv_color[1], cv_color[2]);
}

cv::Vec3b CVOperation::generateRandomVec3bColor()
{
    cv::RNG rng(time(0));
    return cv::Vec3b(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
}
