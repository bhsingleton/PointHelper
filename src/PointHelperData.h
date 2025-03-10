#ifndef _POINT_HELPER_DATA
#define _POINT_HELPER_DATA
//
// File: PointHelperData.h
//
// Author: Ben Singleton
//

#include "Drawable.h"

#include <maya/MUserData.h>
#include <maya/MPlug.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MVector.h>
#include <maya/MVectorArray.h>
#include <maya/MEulerRotation.h>
#include <maya/MMatrix.h>
#include <maya/MFnMatrixData.h>
#include <maya/MColor.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <maya/MStatus.h>

#include <maya/MViewport2Renderer.h>
#include <maya/MUIDrawManager.h>
#include <maya/MFrameContext.h>
#include <maya/MHWGeometry.h>
#include <maya/MHWGeometryUtilities.h>

#include <string>
#include <map>


class PointHelperData : public MUserData 
{

public:

							PointHelperData();
	virtual					~PointHelperData();

	virtual	MStatus			copyWireColor(const MDagPath& objPath);
	virtual	MStatus			copyDepthPriority(const MDagPath& dagPath);

	virtual	void			invalidate();

public:
			
			MVector			localPosition;
			MVector			localRotate;
			MVector			localScale;
			double			size;
			MMatrix			objectMatrix;

			MStringArray	texts;
			int				choice;
			MString			text;
			unsigned int	fontSize;

			MColor			wireColor;
			float			lineWidth;
			bool			fill;
			bool			shaded;
			bool			drawOnTop;
			unsigned int	depthPriority;

			MVectorArray	controlPoints;

			std::map<std::string, bool>	drawables;

};
#endif