//
// File: AxisTripod.cpp
//
// Author: Ben Singleton
//

#include "AxisTripod.h"


AxisTripod::AxisTripod() : Drawable::AbstractDrawable() {};
AxisTripod::~AxisTripod() {};


void AxisTripod::prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, PointHelperData* pointHelperData)
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
	int numPoints = sizeof(AxisTripod::LINES) / sizeof(AxisTripod::LINES[0]);
	this->lines = MPointArray(AxisTripod::LINES, numPoints);

	Drawable::transform(pointHelperData->objectMatrix, this->lines);

};


void AxisTripod::draw(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const PointHelperData* pointHelperData)
/**
Draws an axis tripod with text for each axis.

@param drawManager: Interface used for drawing basic shapes inside the viewport.
@param frameContext: Contains global information for the current render frame.
@param pointHelperData: Pointer to a data class with draw information from the associated point helper.
@return: void
*/
{

	// Draw line list
	//
	drawManager.lineList(this->lines, false);

	// Draw axis text
	//
	drawManager.setFontSize(MHWRender::MUIDrawManager::kDefaultFontSize);
	drawManager.text(this->lines[1], MString("x"), MHWRender::MUIDrawManager::kCenter);
	drawManager.text(this->lines[3], MString("y"), MHWRender::MUIDrawManager::kCenter);
	drawManager.text(this->lines[5], MString("z"), MHWRender::MUIDrawManager::kCenter);

};