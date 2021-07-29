#ifndef _POINT_HELPER_DATA
#define _POINT_HELPER_DATA
//
// File: PointHelperData.h
//
// Author: Ben Singleton
//

#include <maya/MViewport2Renderer.h>
#include <maya/MUIDrawManager.h>
#include <maya/MUserData.h>

#include <maya/MMatrix.h>
#include <maya/MColor.h>
#include <maya/MString.h>


class PointHelperData : public MUserData 
{

public:

							PointHelperData();
	virtual					~PointHelperData();

public:
			
			MMatrix			objectMatrix;
			MMatrix			objectWorldMatrix;

			bool			fill;
			bool			shaded;
			bool			drawOnTop;
			MString			text;
			int				fontSize;
			MColor			wireColor;
			float			lineWidth;
			unsigned int	depthPriority;

public:

			MHWRender::MUIDrawManager::LineStyle		lineStyle;
			MHWRender::MUIDrawManager::Selectability	selectability;

};

#endif