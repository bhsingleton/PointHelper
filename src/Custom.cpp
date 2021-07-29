//
// File: Custom.cpp
//
// Author: Ben Singleton
//

#include "Custom.h"


Custom::Custom() {};
Custom::~Custom() {};


void Custom::prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, PointHelperData* pointHelperData)
/**
Collects all of the control points required for this drawable.

@param objPath: The path to the object being drawn.
@param cameraPath: The path to the camera that is being used to draw.
@param frameContext: Frame level context information.
@param pointHelperData: Data cached by the previous draw of the instance.
@return: void
*/
{
	
	// Get custom drawable
	//
	MStatus status = PointHelper::getControlPoints(objPath, this->points);
	CHECK_MSTATUS(status);

};


void Custom::draw(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const PointHelperData* pointHelperData)
/**
Draws a custom shape stored inside the data pointer.

@param drawManager: Interface used for drawing basic shapes inside the viewport.
@param frameContext: Contains global information for the current render frame.
@return: void
*/
{

	// Check if there are enough points
	//
	unsigned int numPoints = this->points.length();

	if (numPoints < 2) 
	{

		return;

	}

	// Draw line through points
	//
	for (unsigned int i = 0; i < (numPoints - 1); i++) 
	{

		drawManager.line(this->points[i], this->points[i + 1]);

	}

};