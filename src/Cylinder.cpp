//
// File: Cylinder.cpp
//
// Author: Ben Singleton
//

#include "Cylinder.h"


MObject	Cylinder::MESH_DATA = MObject::kNullObj;
double	Cylinder::CYLINDER_RADIUS = 0.5;
double	Cylinder::CYLINDER_LENGTH = 1.0;
int		Cylinder::SUBDIVISIONS_AXIS = 40;


Cylinder::Cylinder()
/**
Constructor.
*/
{

	MStatus status;

	// Check if mesh data is null
	//
	if (Cylinder::MESH_DATA.isNull()) 
	{

		// Create cylinder primitive
		//
		MFnMeshData fnMeshData;
		Cylinder::MESH_DATA = fnMeshData.create();

		DrawableUtilities::cylinder(MVector::zero, MVector::xAxis, Cylinder::CYLINDER_RADIUS, Cylinder::CYLINDER_LENGTH, Cylinder::SUBDIVISIONS_AXIS, Cylinder::MESH_DATA);

	}
	

};


Cylinder::~Cylinder() {};


void Cylinder::prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, PointHelperData* pointHelperData)
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
	MObject meshData = DrawableUtilities::copyMeshData(Cylinder::MESH_DATA, pointHelperData->objectMatrix, &status);
	CHECK_MSTATUS(status);

	// Extrapolate data from mesh
	//
	DrawableUtilities::getTriangles(meshData, this->triangles, this->normals);

	// Build array for wireframe
	//
	std::vector<MPointArray> points(6);
	points[0] = DrawableUtilities::circle(MVector(0.5, 0.0, 0.0), MVector::xAxis, CYLINDER_RADIUS, SUBDIVISIONS_AXIS + 1);
	points[1] = DrawableUtilities::circle(MVector(-0.5, 0.0, 0.0), MVector::xAxis, CYLINDER_RADIUS, SUBDIVISIONS_AXIS + 1);
	points[2] = DrawableUtilities::line(MPoint((CYLINDER_LENGTH * 0.5), CYLINDER_RADIUS, 0.0, 1.0), MPoint(-(CYLINDER_LENGTH * 0.5), CYLINDER_RADIUS, 0.0, 1.0));
	points[3] = DrawableUtilities::line(MPoint((CYLINDER_LENGTH * 0.5), -CYLINDER_RADIUS, 0.0, 1.0), MPoint(-(CYLINDER_LENGTH * 0.5), -CYLINDER_RADIUS, 0.0, 1.0));
	points[4] = DrawableUtilities::line(MPoint((CYLINDER_LENGTH * 0.5), 0.0, CYLINDER_RADIUS, 1.0), MPoint(-(CYLINDER_LENGTH * 0.5), 0.0, CYLINDER_RADIUS, 1.0));
	points[5] = DrawableUtilities::line(MPoint((CYLINDER_LENGTH * 0.5), 0.0, -CYLINDER_RADIUS, 1.0), MPoint(-(CYLINDER_LENGTH * 0.5), 0.0, -CYLINDER_RADIUS, 1.0));

	this->lines = DrawableUtilities::stagger(points);
	DrawableUtilities::transform(pointHelperData->objectMatrix, this->lines);

};


void Cylinder::draw(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const PointHelperData* pointHelperData) 
/**
Draws a box.

@param drawManager: Interface used for drawing basic shapes inside the viewport.
@param frameContext: Contains global information for the current render frame.
@return: void
*/
{

	// Check if fill is enabled
	//
	if (pointHelperData->fill) 
	{

		// Draw mesh
		//
		drawManager.setPaintStyle(pointHelperData->shaded ? MHWRender::MUIDrawManager::kShaded : MHWRender::MUIDrawManager::kFlat);
		drawManager.mesh(MHWRender::MUIDrawManager::kTriangles, this->triangles, &this->normals);

	}

	// Draw lines
	//
	drawManager.mesh(MHWRender::MUIDrawManager::kLines, this->lines);

};