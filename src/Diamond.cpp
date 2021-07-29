//
// File: Diamond.cpp
//
// Author: Ben Singleton
//

#include "Diamond.h"


MObject	Diamond::MESH_DATA = MObject::kNullObj;


Diamond::Diamond()
/**
Constructor.
*/
{

	MStatus status;

	// Check if mesh data is null
	//
	if (Diamond::MESH_DATA.isNull())
	{

		// Initialize mesh arrays
		//
		int numPoints = (sizeof(Diamond::POINTS) / sizeof(Diamond::POINTS[0]));
		int numPolygons = (sizeof(Diamond::POLYGON_COUNTS) / sizeof(Diamond::POLYGON_COUNTS[0]));
		int numFaceVertices = (sizeof(Diamond::POLYGON_CONNECTS) / sizeof(Diamond::POLYGON_CONNECTS[0]));

		MPointArray points(Diamond::POINTS, numPoints);
		MIntArray polygonCounts(Diamond::POLYGON_COUNTS, numPolygons);
		MIntArray polygonConnects(Diamond::POLYGON_CONNECTS, numFaceVertices);
		MIntArray edgeSmoothings(Diamond::NUM_EDGES, 0);

		// Create diamond primitive
		//
		Diamond::MESH_DATA = DrawableUtilities::createMeshData(points, polygonCounts, polygonConnects, edgeSmoothings, &status);
		CHECK_MSTATUS(status);

	}

};


Diamond::~Diamond() {};


void Diamond::prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, PointHelperData* pointHelperData)
/**
Prepares to draw a diamond.

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
	MObject meshData = DrawableUtilities::copyMeshData(Diamond::MESH_DATA, pointHelperData->objectMatrix, &status);
	CHECK_MSTATUS(status);

	// Extrapolate data from mesh
	//
	status = DrawableUtilities::getTriangles(meshData, this->triangles, this->normals);
	CHECK_MSTATUS(status);

	status = DrawableUtilities::getLines(meshData, this->lines);
	CHECK_MSTATUS(status);

};


void Diamond::draw(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const PointHelperData* pointHelperData) 
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