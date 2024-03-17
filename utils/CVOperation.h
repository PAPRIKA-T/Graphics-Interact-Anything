#pragma once
#include <opencv2/opencv.hpp>
class QColor;
class QImage;
namespace CVOperation {
	QImage matToQImage(const cv::Mat&); //Mat数据转换为QPixmap
	cv::Mat QImageToMat(const QImage&, bool inCloneImageData); //QPixmap转换为Mat数据

	cv::Mat getAnnotation(const cv::Mat& org_image, const cv::Mat& mask,
		cv::Vec3b color = (0, 0, 255), bool random = true); 
	//为原始图像org_image添加掩码mask，random表示是否随机给Mask上色

	QImage getAnnotation(const QImage& org_image, const cv::Mat& mask,
		QColor color, bool random = true);

	//QImage getAnnotation(const QImage& org_image, const cv::Mat& mask,
	//	QColor color, bool random = true);
}

