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
	virtual						~PointHelperData();

	virtual	PointHelperData&	operator=(const PointHelperData* src);

	virtual	MStatus				resizeTexts(const unsigned int size);
	virtual	MString				getText(const unsigned int index);
	virtual	MStatus				setText(const unsigned int index, const MString& text);

	virtual	MStatus				resizeControlPoints(const unsigned int size);
	virtual	MVector				getControlPoint(const unsigned int index);
	virtual	MStatus				setControlPoint(const unsigned int index, const MVector& point);
	virtual	MStatus				setControlPoint(const unsigned int index, const int axis, const double value);

	virtual	MStatus				copyObjectMatrix(const MDagPath& objPath);
	virtual	MStatus				copyWireColor(const MDagPath& objPath);
	virtual	MStatus				copyDepthPriority(const MDagPath& dagPath);

	virtual	void				dirtyCurrentText();
	virtual	void				dirtyObjectMatrix();

public:
			
			std::map<std::string, bool>	enabled;

			MVector				localPosition;
			MVector				localRotate;
			MVector				localScale;
			double				size;
			MMatrix				objectMatrix;

			unsigned int		choice;
			MString				currentText;
			MStringArray		texts;
			int					fontSize;

			float				lineWidth;
			MColor				wireColor;
			MVectorArray		controlPoints;

			bool				fill;
			bool				shaded;
			bool				drawOnTop;
			unsigned int		depthPriority;

};
#endif