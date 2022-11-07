//
// File: Box.cpp
//
// Author: Ben Singleton
//

#include "Box.h"


MObject	Box::MESH_DATA = MObject::kNullObj;


Box::Box() : Drawable::AbstractDrawable()
/**
Constructor.
*/
{
	
	MStatus status;

	// Check if mesh data is null
	//
	if (Box::MESH_DATA.isNull())
	{

		// Initialize mesh constructors
		//
		int numPoints = (sizeof(Box::POINTS) / sizeof(Box::POINTS[0]));
		int numPolygons = (sizeof(Box::POLYGON_COUNTS) / sizeof(Box::POLYGON_COUNTS[0]));
		int numFaceVertices = (sizeof(Box::POLYGON_CONNECTS) / sizeof(Box::POLYGON_CONNECTS[0]));

		MPointArray points(Box::POINTS, numPoints);
		MIntArray polygonCounts(Box::POLYGON_COUNTS, numPolygons);
		MIntArray polygonConnects(Box::POLYGON_CONNECTS, numFaceVertices);
		MIntArray edgeSmoothings(Box::NUM_EDGES, 0);

		// Create box primitive
		//
		Box::MESH_DATA = Drawable::createMeshData(points, polygonCounts, polygonConnects, edgeSmoothings, &status);
		CHECK_MSTATUS(status);

	}

};


Box::~Box() {};


void Box::prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, PointHelperData* pointHelperData)
/**
Prepares to draw a box.

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
	MObject meshData = Drawable::copyMeshData(Box::MESH_DATA, pointHelperData->objectMatrix, &status);
	CHECK_MSTATUS(status);

	// Extrapolate data from mesh
	//
	status = Drawable::getTriangles(meshData, this->triangles, this->normals);
	CHECK_MSTATUS(status);

	status = Drawable::getLines(meshData, this->lines);
	CHECK_MSTATUS(status);

};


void Box::draw(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const PointHelperData* pointHelperData)
/**
Draws a box.

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