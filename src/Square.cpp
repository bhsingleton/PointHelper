//
// File: Square.cpp
//
// Author: Ben Singleton
//

#include "Square.h"


MObject		Square::MESH_DATA = MObject::kNullObj;
MIntArray	Square::BOUNDARY = MIntArray();


Square::Square() : Drawable::AbstractDrawable()
/**
Constructor.
*/
{

	MStatus status;

	// Check if mesh data is null
	//
	if (Square::MESH_DATA.isNull()) 
	{

		// Initialize mesh arrays
		//
		int numPoints = sizeof(Square::POINTS) / sizeof(Square::POINTS[0]);
		int numPolygons = sizeof(Square::POLYGON_COUNTS) / sizeof(Square::POLYGON_COUNTS[0]);
		int numFaceVertices = sizeof(Square::POLYGON_CONNECTS) / sizeof(Square::POLYGON_CONNECTS[0]);

		MPointArray points = MPointArray(Square::POINTS, numPoints);
		MIntArray polygonCounts = MIntArray(Square::POLYGON_COUNTS, numPolygons);
		MIntArray polygonConnects = MIntArray(Square::POLYGON_CONNECTS, numFaceVertices);

		// Create new mesh data object
		//
		Square::MESH_DATA = Drawable::createMeshData(points, polygonCounts, polygonConnects, &status);
		CHECK_MSTATUS(status);

		// Get edge boundary
		//
		status = Drawable::getBoundary(Square::MESH_DATA, Square::BOUNDARY);
		CHECK_MSTATUS(status);

	}

};


Square::~Square() {};


void Square::prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, PointHelperData* pointHelperData)
/**
Prepares to draw a square.

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
	MObject meshData = Drawable::copyMeshData(Square::MESH_DATA, pointHelperData->objectMatrix, &status);
	CHECK_MSTATUS(status);

	// Extrapolate data from mesh
	//
	status = Drawable::getTriangles(meshData, this->triangles, this->normals);
	CHECK_MSTATUS(status);

	// Get lines from boundary
	//
	status = Drawable::getLines(meshData, Square::BOUNDARY, this->lines);
	CHECK_MSTATUS(status);

};


void Square::draw(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const PointHelperData* pointHelperData)
/**
Draws a box.

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