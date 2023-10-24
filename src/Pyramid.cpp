//
// File: Pyramid.cpp
//
// Author: Ben Singleton
//

#include "Pyramid.h"


MObject	Pyramid::MESH_DATA = MObject::kNullObj;


Pyramid::Pyramid() : Drawable::AbstractDrawable()
/**
Constructor.
*/
{

	MStatus status;

	// Check if mesh data is null
	//
	if (Pyramid::MESH_DATA.isNull())
	{

		// Initialize mesh arrays
		//
		int numPoints = (sizeof(Pyramid::POINTS) / sizeof(Pyramid::POINTS[0]));
		int numPolygons = (sizeof(Pyramid::POLYGON_COUNTS) / sizeof(Pyramid::POLYGON_COUNTS[0]));
		int numFaceVertices = (sizeof(Pyramid::POLYGON_CONNECTS) / sizeof(Pyramid::POLYGON_CONNECTS[0]));

		MPointArray points(Pyramid::POINTS, numPoints);
		MIntArray polygonCounts(Pyramid::POLYGON_COUNTS, numPolygons);
		MIntArray polygonConnects(Pyramid::POLYGON_CONNECTS, numFaceVertices);
		MIntArray edgeSmoothings(Pyramid::NUM_EDGES, 0);

		// Create Pyramid primitive
		//
		Pyramid::MESH_DATA = Drawable::createMeshData(points, polygonCounts, polygonConnects, edgeSmoothings, &status);
		CHECK_MSTATUS(status);

	}

};


Pyramid::~Pyramid() {};


void Pyramid::prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, PointHelperData* pointHelperData)
/**
Prepares to draw a pyramid.

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
	MObject meshData = Drawable::copyMeshData(Pyramid::MESH_DATA, pointHelperData->objectMatrix, &status);
	CHECK_MSTATUS(status);

	// Extrapolate data from mesh
	//
	status = Drawable::getTriangles(meshData, this->triangles, this->normals);
	CHECK_MSTATUS(status);

	status = Drawable::getLines(meshData, this->lines);
	CHECK_MSTATUS(status);

};


void Pyramid::draw(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const PointHelperData* pointHelperData)
/**
Draws a pyramid.

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