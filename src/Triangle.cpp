//
// File: Triangle.cpp
//
// Author: Ben Singleton
//

#include "Triangle.h"


MObject		Triangle::MESH_DATA = MObject::kNullObj;
MIntArray	Triangle::BOUNDARY = MIntArray();


Triangle::Triangle() : Drawable::AbstractDrawable()
/**
Constructor.
*/
{

	MStatus status;

	// Check if mesh data is null
	//
	if (Triangle::MESH_DATA.isNull())
	{

		// Initialize mesh arrays
		//
		int numPoints = sizeof(Triangle::POINTS) / sizeof(Triangle::POINTS[0]);
		int numPolygons = sizeof(Triangle::POLYGON_COUNTS) / sizeof(Triangle::POLYGON_COUNTS[0]);
		int numFaceVertices = sizeof(Triangle::POLYGON_CONNECTS) / sizeof(Triangle::POLYGON_CONNECTS[0]);

		MPointArray points = MPointArray(Triangle::POINTS, numPoints);
		MIntArray polygonCounts = MIntArray(Triangle::POLYGON_COUNTS, numPolygons);
		MIntArray polygonConnects = MIntArray(Triangle::POLYGON_CONNECTS, numFaceVertices);

		// Create new mesh data object
		//
		Triangle::MESH_DATA = Drawable::createMeshData(points, polygonCounts, polygonConnects, &status);
		CHECK_MSTATUS(status);

		// Get edge boundary
		//
		status = Drawable::getBoundary(Triangle::MESH_DATA, Triangle::BOUNDARY);
		CHECK_MSTATUS(status);

	}

};


Triangle::~Triangle() {};


void Triangle::prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, PointHelperData* pointHelperData)
/**
Prepares to draw a triangle.

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
	MObject meshData = Drawable::copyMeshData(Triangle::MESH_DATA, pointHelperData->objectMatrix, &status);
	CHECK_MSTATUS(status);

	// Extrapolate data from mesh
	//
	status = Drawable::getTriangles(meshData, this->triangles, this->normals);
	CHECK_MSTATUS(status);

	// Get lines from boundary
	//
	status = Drawable::getLines(meshData, Triangle::BOUNDARY, this->lines);
	CHECK_MSTATUS(status);

};


void Triangle::draw(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const PointHelperData* pointHelperData)
/**
Draws a triangle.

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