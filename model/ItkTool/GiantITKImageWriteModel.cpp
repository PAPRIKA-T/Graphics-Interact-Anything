#include "GiantITKImageWriteModel.h"

int GiantITKImageWriteModel::saveCvMatAsNii(const cv::Mat& mat, const std::string& write_path)
{
    // 转换为ITK图像类型
    using PixelType = unsigned char;
    constexpr unsigned int Dimension = 3;

    using ImageType = itk::Image<PixelType, Dimension>;
    using ImportFilterType = itk::ImportImageFilter<PixelType, Dimension>;

    ImportFilterType::Pointer importFilter = ImportFilterType::New();
    using ImageIOType = itk::NiftiImageIO;
    ImageIOType::Pointer niiIO = ImageIOType::New();
    // 设置图像尺寸和像素数据
    ImageType::SizeType size;
    size[0] = mat.cols;
    size[1] = mat.rows;
    size[2] = 1; // 假设是2D图像

    ImageType::IndexType start;
    start.Fill(0);
    ImageType::RegionType region;
    region.SetSize(size);
    region.SetIndex(start);
    importFilter->SetRegion(region);

    itk::SizeValueType numberOfPixels = mat.rows * mat.cols;
    const bool importImageFilterWillOwnTheBuffer = false;
    importFilter->SetImportPointer(mat.data, numberOfPixels, importImageFilterWillOwnTheBuffer);

    // 保存为NII文件
    using WriterType = itk::ImageFileWriter<ImageType>;
    WriterType::Pointer writer = WriterType::New();
    writer->SetInput(importFilter->GetOutput());
    writer->SetFileName(write_path);
    writer->SetImageIO(niiIO);
    try{
        writer->Update();
    }
    catch (const itk::ExceptionObject& err){
        std::cerr << "ExceptionObject caught !" << std::endl;
        std::cerr << err << std::endl;
        return EXIT_FAILURE;
    }
    return 0;
}
