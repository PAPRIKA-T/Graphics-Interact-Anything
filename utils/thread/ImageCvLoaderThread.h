#pragma once
#include <QThread>
#include <QImage>
#include <opencv2/opencv.hpp>
class ImageCvLoaderThread : public QThread
{
    Q_OBJECT

public:
    ImageCvLoaderThread(QObject* parent = nullptr) : QThread(parent) {}
signals:
    void imageLoaded(const cv::Mat& image);

protected:
    void run() override;

public slots:
    void translateQImage2cvMat(const QImage&);

private:
    QImage i{};
    cv::Mat orgin_image_mat{};
};