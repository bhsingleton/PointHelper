//
// File: Sphere.cpp
//
// Author: Ben Singleton
//

#include "Sphere.h"


MObject	Sphere::MESH_DATA = MObject::kNullObj;
double	Sphere::SPHERE_RADIUS = 0.5;
int		Sphere::SUBDIVISIONS_AXIS = 40;
int		Sphere::SUBDIVISIONS_HEIGHT = 40;


Sphere::Sphere()
/**
Constructor.
*/
{

	// Check if mesh data is null
	//
	if (Sphere::MESH_DATA.isNull()) 
	{

		// Create sphere primitive
		//
		MFnMeshData fnMeshData;
		Sphere::MESH_DATA = fnMeshData.create();

		DrawableUtilities::sphere(MVector::zero, Sphere::SPHERE_RADIUS, Sphere::SUBDIVISIONS_AXIS, Sphere::SUBDIVISIONS_HEIGHT, Sphere::MESH_DATA);

	}

};


Sphere::~Sphere() {};


void Sphere::prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, PointHelperData* pointHelperData)
/**
Prepares to draw a cylinder.

@param objPath: The path to the object being drawn.
@param cameraPath: The path to the camera that is being used to draw.
@param frameContext: Frame level context information.
@param PointHelperData: Data cached by the previous draw of the instance.
@return: void
*/
{

	MStatus status;

	// Copy mesh data
	//
	MObject meshData = DrawableUtilities::copyMeshData(Sphere::MESH_DATA, pointHelperData->objectMatrix, &status);
	CHECK_MSTATUS(status);

	// Get mesh triangles
	//
	status = DrawableUtilities::getTriangles(meshData, this->triangles, this->normals);
	CHECK_MSTATUS(status);

	// Build circle array for wireframe
	//
	std::vector<MPointArray> points(3);
	points[0] = DrawableUtilities::circle(MVector::zero, MVector::xAxis, Sphere::SPHERE_RADIUS, Sphere::SUBDIVISIONS_AXIS + 1);
	points[1] = DrawableUtilities::circle(MVector::zero, MVector::yAxis, Sphere::SPHERE_RADIUS, Sphere::SUBDIVISIONS_AXIS + 1);
	points[2] = DrawableUtilities::circle(MVector::zero, MVector::zAxis, Sphere::SPHERE_RADIUS, Sphere::SUBDIVISIONS_AXIS + 1);

	this->lines = DrawableUtilities::stagger(points);
	DrawableUtilities::transform(pointHelperData->objectMatrix, this->lines);

};


void Sphere::draw(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const PointHelperData* pointHelperData) 
/**
Draws a sphere.

@param drawManager: Interface used for drawing basic shapes inside the viewport.
@param frameContext: Contains global information for the current render frame.
@param pointHelperData: Pointer to a data class with draw information from the associated point helper.
@return: void
*/
{

	// Draw mesh
	//
	if (pointHelperData->fill) 
	{

		drawManager.setPaintStyle(pointHelperData->shaded ? MHWRender::MUIDrawManager::kShaded : MHWRender::MUIDrawManager::kFlat);
		drawManager.mesh(MHWRender::MUIDrawManager::kTriangles, this->triangles, &this->normals);

	}

	// Draw lines and points
	//
	drawManager.mesh(MHWRender::MUIDrawManager::kLines, this->lines);

}