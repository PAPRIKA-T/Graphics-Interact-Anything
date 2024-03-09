#pragma once
#include <opencv2/opencv.hpp>
class QPixmap;

namespace CVOperation {
	QPixmap matToPixmap(const cv::Mat& cvImage); //Mat数据转换为QPixmap
	cv::Mat QPixmapToMat(const QPixmap& pixmap, bool inCloneImageData); //QPixmap转换为Mat数据
	void setMaskColor(const cv::Mat& mask, cv::Mat& color_mask, 
		cv::Vec3b color = (0, 0, 255), bool random = true); //为Mask上色，random表示是否是随机上色
	cv::Mat getAnnotation(const cv::Mat& org_image, const cv::Mat& mask,
		cv::Vec3b color = (0, 0, 255), bool random = true); 
	//为原始图像org_image添加掩码mask，random表示是否随机给Mask上色

}

