#pragma once

#include "vtkAutoInit.h" 
VTK_MODULE_INIT(vtkRenderingOpenGL2); // VTK was built with vtkRenderingOpenGL2
VTK_MODULE_INIT(vtkInteractionStyle);
#include <QVTKOpenGLNativeWidget.h>
#include <vtkNamedColors.h>
#include <vtkPolyDataMapper.h>
#include <vtkCylinderSource.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkProperty.h>
#include <vtkDiscreteMarchingCubes.h>
#include <vtkSmartPointer.h>
#include <vtkNIFTIImageReader.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>
#include <vtkVolumeMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkImageShiftScale.h>

/***********************VTKWidget Class**************************/
class VTKWidget : public QVTKOpenGLNativeWidget
{
	Q_OBJECT
public:
	VTKWidget(QWidget* parent = nullptr);
	~VTKWidget();
	void AddActorFromITK(const vtkNew<vtkDiscreteMarchingCubes>& contour);
	void visualizeNii(const QString& filePath);
private:
	vtkSmartPointer<vtkRenderer> renderer;
};