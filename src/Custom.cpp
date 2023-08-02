//
// File: Custom.cpp
//
// Author: Ben Singleton
//

#include "Custom.h"


Custom::Custom() : Drawable::AbstractDrawable() {};
Custom::~Custom() {};


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
	unsigned int numPoints = pointHelperData->controlPoints.length();

	if (numPoints < 2) 
	{

		return;

	}

	// Draw line through points
	//
	MPoint startPoint, endPoint;

	for (unsigned int i = 0; i < (numPoints - 1); i++) 
	{

		startPoint = pointHelperData->controlPoints[i];
		endPoint = pointHelperData->controlPoints[i + 1];

		drawManager.line(startPoint, endPoint);

	}

};