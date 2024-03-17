#pragma once
#include <opencv2/opencv.hpp>
class QPixmap;
class QColor;
class QImage;
namespace CVOperation {
	QPixmap matToPixmap(const cv::Mat& cvImage); //Mat数据转换为QPixmap
	cv::Mat QPixmapToMat(const QPixmap& pixmap, bool inCloneImageData); //QPixmap转换为Mat数据

	cv::Mat getAnnotation(const cv::Mat& org_image, const cv::Mat& mask,
		cv::Vec3b color = (0, 0, 255), bool random = true); 
	//为原始图像org_image添加掩码mask，random表示是否随机给Mask上色

	QPixmap getAnnotation(const QPixmap& org_image, const cv::Mat& mask,
		QColor color, bool random = true);

	QPixmap getAnnotation(const QImage& org_image, const cv::Mat& mask,
		QColor color, bool random = true);
}

