//
// File: TearDrop.cpp
//
// Author: Ben Singleton
//

#include "TearDrop.h"


MObject		TearDrop::MESH_DATA = MObject::kNullObj;
MIntArray	TearDrop::BOUNDARY = MIntArray();
double		TearDrop::TEARDROP_RADIUS = 0.5;
int			TearDrop::SUBDIVISIONS_AXIS = 40;


TearDrop::TearDrop()
/**
Constructor.
*/
{

	MStatus status;

	// Check if mesh data is null
	//
	if (TearDrop::MESH_DATA.isNull()) 
	{

		// Create new mesh data
		//
		MFnMeshData fnMeshData;
		TearDrop::MESH_DATA = fnMeshData.create();

		DrawableUtilities::sector(MVector::zero, MVector::xAxis, TearDrop::TEARDROP_RADIUS, 45, 315, TearDrop::SUBDIVISIONS_AXIS, TearDrop::MESH_DATA);

		// Add tear drop
		//
		MFnMesh fnMesh(TearDrop::MESH_DATA, &status);
		CHECK_MSTATUS(status);

		MPointArray points;

		status = fnMesh.getPoints(points, MSpace::kObject);
		CHECK_MSTATUS(status);

		unsigned int length = points.length();

		MPointArray polygon(4, MPoint::origin);
		polygon[0] = points[0];
		polygon[1] = MPoint::origin;
		polygon[2] = points[length - 1];
		polygon[3] = MPoint(0.0, 0.0, 0.7, 1.0);

		fnMesh.addPolygon(polygon, true, MERGE_THRESHOLD, TearDrop::MESH_DATA, &status);
		CHECK_MSTATUS(status);

		// Get edge boundary
		//
		MIntArray boundary;

		status = DrawableUtilities::getBoundary(TearDrop::MESH_DATA, TearDrop::BOUNDARY);
		CHECK_MSTATUS(status);

		MObject component = DrawableUtilities::createComponent(MFn::kMeshEdgeComponent, boundary, &status);
		CHECK_MSTATUS(status);

	}

};


TearDrop::~TearDrop() {};


void TearDrop::prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, PointHelperData* pointHelperData)
/**
Prepares to draw a tear drop.

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
	MObject meshData = DrawableUtilities::copyMeshData(TearDrop::MESH_DATA, pointHelperData->objectMatrix, &status);
	CHECK_MSTATUS(status);

	// Extrapolate data from mesh
	//
	status = DrawableUtilities::getTriangles(meshData, this->triangles, this->normals);
	CHECK_MSTATUS(status);

	// Get lines from boundary
	//
	status = DrawableUtilities::getLines(meshData, TearDrop::BOUNDARY, this->lines);
	CHECK_MSTATUS(status);

};


void TearDrop::draw(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const PointHelperData* pointHelperData) 
/**
Draws a box.

@param drawManager: Interface used for drawing basic shapes inside the viewport.
@param frameContext: Contains global information for the current render frame.
@return: void
*/
{

	MStatus status;

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