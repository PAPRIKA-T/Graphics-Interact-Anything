#include "GiantITKImageReadModel.h"
#include<itkNiftiImageIO.h>
#include<itkImageRegionConstIterator.h>
#include<itkRescaleIntensityImageFilter.h>
#include <itkExtractImageFilter.h>
#include <itkImageToVTKImageFilter.h>

#include<QPixmap>

GiantITKImageReadModel::GiantITKImageReadModel()
{
	cur_slice_index.push_back(-1);
	cur_slice_index.push_back(-1);
	cur_slice_index.push_back(-1);
}

void GiantITKImageReadModel::readNiiImage(const char* file_path)
{
	using ReaderType = itk::ImageFileReader<OrgImageType>;
	// 设置读取的文件类型
	using ImageIoType = itk::NiftiImageIO;
	ReaderType::Pointer reader = ReaderType::New();
	ImageIoType::Pointer niftiIO = ImageIoType::New();
	reader->SetImageIO(niftiIO);
	reader->SetFileName(file_path);
	reader->Update(); // 读取文件结束
	// 得到文件
	org_image = reader->GetOutput();
	origin_size = org_image->GetLargestPossibleRegion().GetSize();
	image_path = file_path;
	normalizeImageByFilter(org_image, gray_image, 0, 255);

	// 查看spacing
	itk::Image<double, 3>::SpacingType spacing = org_image->GetSpacing();
	cout << "spacing:\n " << spacing << endl;
	// 查看direction
	itk::Image<double, 3>::DirectionType direction = org_image->GetDirection();
	cout << "direction:\n " << direction << endl;
	// 查看origin
	itk::Image<double, 3>::PointType origin = org_image->GetOrigin();
	cout << "origin: \n" << origin << endl;
	// 查看大小: x,y,z
	itk::Image<double, 3>::SizeType size = org_image->GetLargestPossibleRegion().GetSize();
	cout << "size: \n" << size << endl;
}

void GiantITKImageReadModel::processAndConvertToQImage(QImage& out_image, int dimension, int slice_index)
{
	if (dimension < 0 || dimension>2) {
		std::cout <<"ITKImageHelper::processAndConvertToPixmap dimension wrong!" << std::endl;
		return ;
	}
	if (slice_index > 0 && slice_index > origin_size[dimension]-1) {
		std::cout << "ITKImageHelper::processAndConvertToPixmap slice_index wrong!" << std::endl;
		return ;
	}
	OrgImageType::RegionType inputRegion = gray_image->GetLargestPossibleRegion();
	OrgImageType::SizeType size = inputRegion.GetSize();
	size[dimension] = 0;// 提取垂直于Z轴的切片，size的Z方向为0，图像大小是size[0]*size[1]；当然也可以沿着x,y方向切割
	// 设置切片在Z轴的位置及切片大小与起始索引
	OrgImageType::IndexType start = inputRegion.GetIndex();
	if (slice_index>=0) {
		cur_slice_index[dimension] = slice_index;
		start[dimension] = slice_index;   //提取切片号，比如第几张
	}
	else {
		cur_slice_index[dimension] = origin_size[dimension] / 2 - 1;
		start[dimension] = origin_size[dimension] / 2 - 1;
	}
	OrgImageType::RegionType desiredRegion;
	desiredRegion.SetSize(size);
	desiredRegion.SetIndex(start);
	// 提取切片滤波器
	typedef itk::ExtractImageFilter<GrayImageType, GrayImage2DType> FilterType;
	FilterType::Pointer filter = FilterType::New();
	filter->SetDirectionCollapseToIdentity();
	filter->SetInput(gray_image);
	filter->SetExtractionRegion(desiredRegion); // 设置切片提取区域
	filter->Update();
	// 转换为Qt的QImage
	QImage qImage{};
	ITKImageToQImage(qImage, filter->GetOutput(), !dimension);
	// 转换为QPixmap
	out_image = qImage;
	return;
}

void GiantITKImageReadModel::normalizeImageByFilter(const OrgImagePointerType& input_image, GrayImagePointerType& out_image, const double dst_min, const double dst_max)
{
	using rescaleFilterType = itk::RescaleIntensityImageFilter<OrgImageType, GrayImageType>;
	auto rescaleFilter = rescaleFilterType::New();
	rescaleFilter->SetInput(input_image);
	rescaleFilter->SetOutputMinimum(dst_min);
	rescaleFilter->SetOutputMaximum(dst_max);
	try {
		rescaleFilter->Update();
	}
	catch (const itk::ExceptionObject& e) {
		std::cout << e.what() << std::endl;
	}
	out_image = rescaleFilter->GetOutput();
}

void GiantITKImageReadModel::reset()
{
	org_image = nullptr;
	gray_image = nullptr;
	origin_size.Fill(0);
	cur_slice_index.clear();
	cur_slice_index.push_back(-1);
	cur_slice_index.push_back(-1);
	cur_slice_index.push_back(-1);
	image_path.clear();
}

vtkNew<vtkDiscreteMarchingCubes> GiantITKImageReadModel::ITK2VTKactor()
{
	// Convert itkImage to vtkImage
	using ConvertFilter = itk::ImageToVTKImageFilter<OrgImageType>;
	ConvertFilter::Pointer convertFilter = ConvertFilter::New();
	convertFilter->SetInput(org_image);
	convertFilter->Update();

	// Extract vtkImageData contour to vtkPolyData
	vtkNew<vtkDiscreteMarchingCubes> contour;
	contour->SetInputData(convertFilter->GetOutput());
	return contour;
}

void GiantITKImageReadModel::ITKImageToQImage(QImage& out_image, const GrayImage2DPointerType& itkImage2D, bool reverse)
{
	const OrgImage2DType::RegionType& region = itkImage2D->GetLargestPossibleRegion();
	OrgImage2DType::SizeType size = region.GetSize();
	if (reverse) {
		std::swap(size[0], size[1]);  // 在反转时交换宽高
	}
	// 转换为QImage
	out_image = QImage(size[0], size[1], QImage::Format_Grayscale8);
	// 获取QImage的bits指针
	uchar* bits = out_image.bits();
	// 获取ITK图像的buffer指针
	GrayPixelType* itkBuffer = itkImage2D->GetBufferPointer();
	// 将ITK图像的像素复制到QImage中
	if (!reverse) {
		for (unsigned int y = 0; y < size[1]; ++y) {
			for (unsigned int x = 0; x < size[0]; ++x) {
				bits[y * size[0] + x] = itkBuffer[y * size[0] + x];
			}
		}
	}
	else {
		for (unsigned int y = 0; y < size[0]; ++y) {
			for (unsigned int x = 0; x < size[1]; ++x) {
				bits[x * size[0] + y] = itkBuffer[y * size[1] + x];
			}
		}
	}
}
