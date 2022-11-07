#ifndef _CENTER_MARKER_DRAWABLE
#define _CENTER_MARKER_DRAWABLE
//
// File: CenterMarker.h
//
// Author: Ben Singleton
//

#include "AbstractDrawable.h"


class CenterMarker : public Drawable ::AbstractDrawable
{

public:

						CenterMarker();
	virtual				~CenterMarker();

	virtual	void		prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, PointHelperData* pointHelperData) {};
	virtual	void		draw(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const PointHelperData* pointHelperData);

};
#endif