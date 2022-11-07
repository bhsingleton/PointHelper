//
// File: CenterMarker.cpp
//
// Author: Ben Singleton
//

#include "CenterMarker.h"


CenterMarker::CenterMarker() : Drawable::AbstractDrawable() {};
CenterMarker::~CenterMarker() {}


void CenterMarker::draw(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const PointHelperData* pointHelperData)
/**
Draws a center marker at the origin of this shape.

@param drawManager: Interface used for drawing basic shapes inside the viewport.
@param frameContext: Contains global information for the current render frame.
@return: void
*/
{


	// Draw icon
	//
	drawManager.icon(MVector::zero, MString("CROSS"), 1.0);

};