//
// File: Disc.cpp
//
// Author: Ben Singleton
//

#include "Disc.h"


MObject		Disc::MESH_DATA = MObject::kNullObj;
MIntArray	Disc::BOUNDARY = MIntArray();
double		Disc::DISC_RADIUS = 0.5;
int			Disc::SUBDIVISIONS_AXIS = 32;


Disc::Disc() : Drawable() 
{

	MStatus status;

	// Check if mesh data is null
	//
	if (Disc::MESH_DATA.isNull()) 
	{

		// Create new mesh data
		//
		MFnMeshData fnMeshData;
		Disc::MESH_DATA = fnMeshData.create();

		DrawableUtilities::disc(MVector::zero, MVector::xAxis, Disc::DISC_RADIUS, Disc::SUBDIVISIONS_AXIS, Disc::MESH_DATA);

		// Get edge boundary
		//
		status = DrawableUtilities::getBoundary(Disc::MESH_DATA, Disc::BOUNDARY);
		CHECK_MSTATUS(status);

	}

};


Disc::~Disc() {};


void Disc::prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, PointHelperData* pointHelperData)
/**
Prepares to draw a disc.

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
	MObject meshData = DrawableUtilities::copyMeshData(Disc::MESH_DATA, pointHelperData->objectMatrix, &status);
	CHECK_MSTATUS(status);

	// Get mesh triangles
	//
	status = DrawableUtilities::getTriangles(meshData, this->triangles, this->normals);
	CHECK_MSTATUS(status);

	// Get lines from boundary
	//
	status = DrawableUtilities::getLines(meshData, Disc::BOUNDARY, this->lines);
	CHECK_MSTATUS(status);

};


void Disc::draw(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const PointHelperData* pointHelperData)
/**
Draws a disc.

@param drawManager: Interface used for drawing basic shapes inside the viewport.
@param frameContext: Contains global information for the current render frame.
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

};