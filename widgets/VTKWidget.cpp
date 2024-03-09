#include "VTKWidget.h"
/***********************VTKWidget Class**************************/
VTKWidget::VTKWidget(QWidget* parent)
	: QVTKOpenGLNativeWidget(parent)
{
	setMinimumSize(250, 250);
	vtkNew<vtkNamedColors> colors;
	// Set the background color.
	std::array<unsigned char, 4> bkg{ {25, 25, 25, 255} };
	colors->SetColor("BkgColor", bkg.data());

	// The renderer generates the image
	// which is then displayed on the render window.
	// It can be thought of as a scene to which the actor is added
	renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->SetBackground(colors->GetColor3d("BkgColor").GetData());
	// Zoom in a little by accessing the camera and invoking its "Zoom" method.
	renderer->ResetCamera();
	renderWindow()->AddRenderer(renderer);
}

VTKWidget::~VTKWidget()
{
}

void VTKWidget::AddActorFromITK(const vtkNew<vtkDiscreteMarchingCubes>& contour)
{
	vtkNew<vtkPolyDataMapper> mapper;
	mapper->SetInputConnection(contour->GetOutputPort());
	vtkNew<vtkActor> actor;
	actor->SetMapper(mapper);
	renderer->ResetCamera();
	renderer->AddActor(actor);
}

void VTKWidget::visualizeNii(const QString& filePath)
{
	// 使用VTK读取NII图像
	//vtkSmartPointer<vtkNIFTIImageReader> reader =
	//	vtkSmartPointer<vtkNIFTIImageReader>::New();
	//reader->SetFileName(filePath.toStdString().c_str());
	//reader->Update();

 //   // 创建体绘制的映射器
 //   vtkSmartPointer<vtkGPUVolumeRayCastMapper> mapper =
 //       vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
 //   mapper->SetInputData(reader->GetOutput());

 //   // 创建体绘制的对象
 //   vtkSmartPointer<vtkVolume> volume =
 //       vtkSmartPointer<vtkVolume>::New();
 //   volume->SetMapper(mapper);

 //   // 创建体绘制的属性
 //   vtkSmartPointer<vtkVolumeProperty> property =
 //       vtkSmartPointer<vtkVolumeProperty>::New();

 //   // 创建灰度传递函数
 //   vtkSmartPointer<vtkPiecewiseFunction> popacity =
 //       vtkSmartPointer<vtkPiecewiseFunction>::New();
 //   popacity->AddPoint(1000, 0.0);
 //   popacity->AddPoint(4000, 0.68);
 //   popacity->AddPoint(7000, 0.83);

 //   // 创建颜色传递函数
 //   vtkSmartPointer<vtkColorTransferFunction> color =
 //       vtkSmartPointer<vtkColorTransferFunction>::New();
 //   color->AddHSVPoint(1000, 0.042, 0.73, 0.55);
 //   color->AddHSVPoint(2500, 0.042, 0.73, 0.55, 0.5, 0.92);
 //   color->AddHSVPoint(4000, 0.088, 0.67, 0.88);
 //   color->AddHSVPoint(5500, 0.088, 0.67, 0.88, 0.33, 0.45);
 //   color->AddHSVPoint(7000, 0.95, 0.063, 1.0);

 //   property->SetColor(color);
 //   property->SetScalarOpacity(popacity);
 //   property->ShadeOn();
 //   property->SetInterpolationTypeToLinear();
 //   property->SetShade(0, 1);
 //   property->SetDiffuse(0.9);
 //   property->SetAmbient(0.1);
 //   property->SetSpecular(0.2);
 //   property->SetSpecularPower(10.0);
 //   property->SetComponentWeight(0, 1);
 //   property->SetDisableGradientOpacity(1);
 //   property->DisableGradientOpacityOn();
 //   property->SetScalarOpacityUnitDistance(0.891927);

 //   volume->SetProperty(property);

	//// 将体绘制添加到渲染器中
	//renderer->AddActor(volume);
	//renderer->ResetCamera();
	//renderWindow->Render();
}

