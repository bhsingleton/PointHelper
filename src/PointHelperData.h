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

	virtual	MStatus			cacheText(const unsigned int index, const MString& text);
	virtual	MStatus			cacheControlPoint(const unsigned int index, const int child, const double value);
	virtual	MStatus			cacheWireColor(const MDagPath& objPath);
	virtual	MStatus			cacheDepthPriority(const MDagPath& dagPath);

	virtual	void			dirtyCurrentText();
	virtual	void			dirtyObjectMatrix();

public:
			
			std::map<std::string, bool>		enabled;

			MVector			localPosition;
			MEulerRotation	localRotate;
			MVector			localScale;
			double			size;
			MMatrix			objectMatrix;

			int				choice;
			MString			currentText;
			MStringArray	texts;
			int				fontSize;

			float			lineWidth;
			MColor			wireColor;
			MPointArray		controlPoints;

			bool			fill;
			bool			shaded;
			bool			drawOnTop;
			unsigned int	depthPriority;

};
#endif