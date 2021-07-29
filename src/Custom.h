#ifndef _CUSTOM
#define _CUSTOM
//
// File: Custom.h
//
// Author: Ben Singleton
//

#include "PointHelperNode.h"

#include "Drawable.h"
#include "DrawableUtilities.h"


class Custom : public Drawable 
{

public:

							Custom();
	virtual					~Custom();

	virtual	void			prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, PointHelperData* pointHelperData);
	virtual	void			draw(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const PointHelperData* pointHelperData);

protected:

			MPointArray		points;

};

#endif