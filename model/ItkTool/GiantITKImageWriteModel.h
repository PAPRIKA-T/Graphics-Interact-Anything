#pragma once
#include<itkImageFileWriter.h>
#include<itkNiftiImageIO.h>
#include <itkImportImageFilter.h>
#include <opencv2/opencv.hpp>

class GiantITKImageWriteModel
{
public:
    int saveCvMatAsNii(const cv::Mat&, const std::string&);
private:
    template<typename image_type, typename image_pointer>
        void writeData(const image_pointer& write_image, const std::string& write_path) {
        using writerType = itk::ImageFileWriter<image_type>;
        auto writer = writerType::New();

        using niiIOType = itk::NiftiImageIO;
        auto niiIO = niiIOType::New();
        writer->SetImageIO(niiIO);
        writer->SetInput(write_image);
        writer->SetFileName(write_path);
        writer->Update();
    }
};
