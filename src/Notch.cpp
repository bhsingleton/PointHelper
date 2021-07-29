//
// File: Notch.cpp
//
// Author: Ben Singleton
//

#include "Notch.h"


MObject		Notch::MESH_DATA = MObject::kNullObj;
MIntArray	Notch::BOUNDARY = MIntArray();
double		Notch::NOTCH_RADIUS = 0.5;
int			Notch::SUBDIVISIONS_AXIS = 40;


Notch::Notch() 
{

	MStatus status;

	// Check if mesh data is null
	//
	if (Notch::MESH_DATA.isNull()) 
	{

		// Create sector primitive
		//
		MFnMeshData fnMeshData;
		Notch::MESH_DATA = fnMeshData.create();

		DrawableUtilities::sector(MVector::zero, MVector::xAxis, Notch::NOTCH_RADIUS, 10, 350, Notch::SUBDIVISIONS_AXIS, Notch::MESH_DATA);

		// Add notch
		//
		MFnMesh fnMesh(Notch::MESH_DATA, &status);
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

		fnMesh.addPolygon(polygon, true, MERGE_THRESHOLD, Notch::MESH_DATA, &status);
		CHECK_MSTATUS(status);

		// Get edge boundary
		//
		status = DrawableUtilities::getBoundary(Notch::MESH_DATA, Notch::BOUNDARY);
		CHECK_MSTATUS(status);

	}


};


Notch::~Notch() {};


void Notch::prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, PointHelperData* pointHelperData)
/**
Prepares to draw a notched disc.

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
	MObject meshData = DrawableUtilities::copyMeshData(Notch::MESH_DATA, pointHelperData->objectMatrix, &status);
	CHECK_MSTATUS(status);

	// Extrapolate data from mesh
	//
	status = DrawableUtilities::getTriangles(meshData, this->triangles, this->normals);
	CHECK_MSTATUS(status);

	// Get lines from edges
	//
	status = DrawableUtilities::getLines(meshData, Notch::BOUNDARY, this->lines);
	CHECK_MSTATUS(status);

};


void Notch::draw(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const PointHelperData* pointHelperData) {
	/**
	Draws a notch.

	@param drawManager: Interface used for drawing basic shapes inside the viewport.
	@param frameContext: Contains global information for the current render frame.
	@return: void
	*/

	MStatus status;

	// Check if fill is enabled
	//
	if (pointHelperData->fill) {

		// Draw mesh
		//
		drawManager.setPaintStyle(pointHelperData->shaded ? MHWRender::MUIDrawManager::kShaded : MHWRender::MUIDrawManager::kFlat);
		drawManager.mesh(MHWRender::MUIDrawManager::kTriangles, this->triangles, &this->normals);

	}

	// Draw lines
	//
	drawManager.mesh(MHWRender::MUIDrawManager::kLines, this->lines);

};