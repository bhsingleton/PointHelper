#ifndef _DRAWABLE
#define _DRAWABLE
//
// File: Drawable.h
//
// Author: Ben Singleton
//

#include "PointHelperData.h"

#include <maya/MUIDrawManager.h>
#include <maya/MFrameContext.h>

#include <maya/MGlobal.h>
#include <maya/MMatrix.h>
#include <maya/MVector.h>
#include <maya/MVectorArray.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MIntArray.h>

#include "stdlib.h"
#include "map"
#include "typeinfo"


class Drawable
/**
Abstract base class used for defining basic shapes to be drawn via the MUIDrawManager.
This class expects all shapes to be self contained within a unit sized bounding box.
The reason for this is because the object matrix is returned via the PointHelperDrawOverride::transform() function in order to reduce the number of recomputations.
Any form of inverse scaling needs to be performed via the Drawable::prepareForDraw function.
*/
{

public:

							Drawable();
	virtual					~Drawable();

	virtual	void			prepareForDraw(const MDagPath& dagPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, PointHelperData* pointHelperData) = 0;
	virtual	void			draw(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const PointHelperData* pointHelperData) = 0;

	virtual	MMatrix			getObjectMatrix();
	virtual	void			setObjectMatrix(MMatrix objectMatrix);
	
	virtual	bool			isEnabled();
	virtual	void			setEnabled(bool enabled);

protected:
		
			MMatrix			objectMatrix;
			MVector			forward, up, right, center;
			double			scale[3];
			bool			enabled;

};

#endif