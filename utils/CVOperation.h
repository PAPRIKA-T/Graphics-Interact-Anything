#pragma once
#include <opencv2/opencv.hpp>
class QColor;
class QImage;

namespace CVOperation {
	//Mat数据与QImage数据互相转换
	QImage cvMat2QImage(const cv::Mat&);
	cv::Mat QImage2cvMat(const QImage&);
	
	//为原始图像org_image添加掩码mask，random表示是否随机给Mask上色,会返回一张被掩膜覆盖的图像
	cv::Mat setMaskOnImage(const cv::Mat& org_image, const cv::Mat& mask,
		cv::Vec3b color = (0, 0, 255), bool random = true);
	QImage setMaskOnImage(const QImage& org_image, const cv::Mat& mask,
		QColor color, bool random = true);

	QColor generateRandomQColor(); //生成随机颜色QColor
	cv::Vec3b generateRandomVec3bColor(); //生成随机颜色Vec3b
}

