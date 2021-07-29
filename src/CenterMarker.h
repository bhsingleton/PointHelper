#ifndef _CENTER_MARKER
#define _CENTER_MARKER
//
// File: CenterMarker.h
//
// Author: Ben Singleton
//

#include "Drawable.h"
#include "DrawableUtilities.h"


class CenterMarker : public Drawable 
{

public:

						CenterMarker() {};
	virtual				~CenterMarker() {};

	virtual	void		prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, PointHelperData* pointHelperData) {};
	virtual	void		draw(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const PointHelperData* pointHelperData);

};

#endif