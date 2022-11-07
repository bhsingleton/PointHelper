//
// File: Cross.cpp
//
// Author: Ben Singleton
//

#include "Cross.h"


Cross::Cross() : Drawable::AbstractDrawable() {};
Cross::~Cross() {};


void Cross::prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, PointHelperData* pointHelperData)
/**
Prepares to draw a cross.

@param objPath: The path to the object being drawn.
@param cameraPath: The path to the camera that is being used to draw.
@param frameContext: Frame level context information.
@param PointHelperData: Data cached by the previous draw of the instance.
@return: void
*/
{

	MStatus status;

	// Initialize point array
	//
	int numPoints = sizeof(Cross::LINES) / sizeof(Cross::LINES[0]);
	this->lines = MPointArray(Cross::LINES, numPoints);

	Drawable::transform(pointHelperData->objectMatrix, this->lines);

};


void Cross::draw(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const PointHelperData* pointHelperData)
/**
Draws a cross.

@param drawManager: Interface used for drawing basic shapes inside the viewport.
@param frameContext: Contains global information for the current render frame.
@return: void
*/
{

	// Draw line list
	//
	drawManager.lineList(this->lines, false);

};