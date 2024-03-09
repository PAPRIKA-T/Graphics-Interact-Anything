#pragma once
#include<itkImageFileReader.h>
#include<itkNiftiImageIO.h>
#include<itkImageRegionConstIterator.h>
#include<itkRescaleIntensityImageFilter.h>
#include <itkExtractImageFilter.h>
#include <itkImageToVTKImageFilter.h>
#include<QPixmap>
#include <QDebug>
#include <vtkDiscreteMarchingCubes.h>

class ITKImageIoModel
{
	using GrayPixelType = unsigned char;
	using OrgPixelType = short;

	using OrgImageType = itk::Image<OrgPixelType, 3>;
	using OrgImagePointerType = OrgImageType::Pointer;

	using GrayImageType = itk::Image<GrayPixelType, 3>;
	using GrayImagePointerType = GrayImageType::Pointer;

	using OrgImage2DType = itk::Image<OrgPixelType, 2>;
	using OrgImage2DPointerType = OrgImage2DType::Pointer;

	using GrayImage2DType = itk::Image<GrayPixelType, 2>;
	using GrayImage2DPointerType = GrayImage2DType::Pointer;
public:
	ITKImageIoModel();
	void readNiiImage(const char* file_path);
	void processAndConvertToPixmap(QPixmap& out_pixmap, int dimension, int slice_index = -1);
	int getCurSliceIndex(int dimension) { return cur_slice_index[dimension]; }
	int getDimensionSize(int dimension) { return origin_size[dimension]; }
	void normalizeImageByFilter(
		const OrgImagePointerType& input_image,
		GrayImagePointerType& out_image,
		const double dst_min, const double dst_max);
	void reset();
	vtkNew<vtkDiscreteMarchingCubes> ITK2VTKactor();
private:
	void ITKImageToQImage(QImage& out_image, const GrayImage2DPointerType& itkImage2D, bool reverse);
	OrgImagePointerType org_image{};
	GrayImagePointerType gray_image{};
	OrgImageType::SizeType origin_size;
	std::vector<int> cur_slice_index{};
	QString image_path = "";
};

