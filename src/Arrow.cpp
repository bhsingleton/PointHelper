//
// File: Arrow.cpp
//
// Author: Ben Singleton
//

#include "Arrow.h"


MObject		Arrow::MESH_DATA = MObject::kNullObj;
MIntArray	Arrow::BOUNDARY = MIntArray();


Arrow::Arrow() : Drawable::AbstractDrawable()
/**
Constructor.
*/
{

	MStatus status;

	// Check if mesh data is null
	//
	if (Arrow::MESH_DATA.isNull()) 
	{

		// Initialize mesh arrays
		//
		int numPoints = sizeof(Arrow::POINTS) / sizeof(Arrow::POINTS[0]);
		int numPolygons = sizeof(Arrow::POLYGON_COUNTS) / sizeof(Arrow::POLYGON_COUNTS[0]);
		int numFaceVertices = sizeof(Arrow::POLYGON_CONNECTS) / sizeof(Arrow::POLYGON_CONNECTS[0]);

		MPointArray points = MPointArray(Arrow::POINTS, numPoints);
		MIntArray polygonCounts = MIntArray(Arrow::POLYGON_COUNTS, numPolygons);
		MIntArray polygonConnects = MIntArray(Arrow::POLYGON_CONNECTS, numFaceVertices);

		// Create new mesh data object
		//
		Arrow::MESH_DATA = Drawable::createMeshData(points, polygonCounts, polygonConnects, &status);
		CHECK_MSTATUS(status);

		// Get edge boundary
		//
		status = Drawable::getBoundary(Arrow::MESH_DATA, Arrow::BOUNDARY);
		CHECK_MSTATUS(status);

	}
	
	
};


Arrow::~Arrow() {};


void Arrow::prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, PointHelperData* pointHelperData)
/**
Prepares to draw an arrow.

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
	MObject meshData = Drawable::copyMeshData(Arrow::MESH_DATA, pointHelperData->objectMatrix, &status);
	CHECK_MSTATUS(status);

	// Extrapolate data from mesh
	//
	status = Drawable::getTriangles(meshData, this->triangles, this->normals);
	CHECK_MSTATUS(status);

	// Get lines from boundary
	//
	status = Drawable::getLines(meshData, Arrow::BOUNDARY, this->lines);
	CHECK_MSTATUS(status);

};


void Arrow::draw(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const PointHelperData* pointHelperData)
/**
Draws an arrow.

@param drawManager: Interface used for drawing basic shapes inside the viewport.
@param frameContext: Contains global information for the current render frame.
@return: void
*/
{

	MStatus status;

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