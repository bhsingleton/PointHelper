//
// File: AxisTripod.cpp
//
// Author: Ben Singleton
//

#include "AxisView.h"


AxisView::AxisView() : Drawable::AbstractDrawable() {};
AxisView::~AxisView() {};


void AxisView::prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, PointHelperData* pointHelperData)
/**
Prepares to draw an axis view.

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
	int numPoints = sizeof(AxisView::LINES) / sizeof(AxisView::LINES[0]);
	this->lines = MPointArray(AxisView::LINES, numPoints);

	Drawable::transform(pointHelperData->objectMatrix, this->lines);

	// Calculate sphere radius
	//
	this->radius = pointHelperData->size * AxisView::RADIUS;

};


void AxisView::draw(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const PointHelperData* pointHelperData)
/**
Draws an axis view.

@param drawManager: Interface used for drawing basic shapes inside the viewport.
@param frameContext: Contains global information for the current render frame.
@param pointHelperData: Pointer to a data class with draw information from the associated point helper.
@return: void
*/
{

	// Draw lines
	//
	drawManager.lineList(this->lines, false);

	// Draw spheres
	//
	drawManager.sphere(this->lines[0], this->radius, AxisView::SUBDIVISION_AXIS, AxisView::SUBDIVISION_HEIGHT, true);
	drawManager.sphere(this->lines[1], this->radius, AxisView::SUBDIVISION_AXIS, AxisView::SUBDIVISION_HEIGHT, true);
	
	drawManager.sphere(this->lines[2], this->radius, AxisView::SUBDIVISION_AXIS, AxisView::SUBDIVISION_HEIGHT, true);
	drawManager.sphere(this->lines[3], this->radius, AxisView::SUBDIVISION_AXIS, AxisView::SUBDIVISION_HEIGHT, true);

	drawManager.sphere(this->lines[4], this->radius, AxisView::SUBDIVISION_AXIS, AxisView::SUBDIVISION_HEIGHT, true);
	drawManager.sphere(this->lines[5], this->radius, AxisView::SUBDIVISION_AXIS, AxisView::SUBDIVISION_HEIGHT, true);

};