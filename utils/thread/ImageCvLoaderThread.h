#pragma once
#include <QThread>
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
    void setPixmapPath(const QString& path);

private:
    QString pixmapPath{};
    cv::Mat orgin_img{};
};