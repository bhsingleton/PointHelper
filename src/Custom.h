#ifndef _CUSTOM_DRAWABLE
#define _CUSTOM_DRAWABLE
//
// File: Custom.h
//
// Author: Ben Singleton
//

#include "AbstractDrawable.h"


class Custom : public Drawable::AbstractDrawable
{

public:

							Custom();
	virtual					~Custom();

	virtual	void			prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, PointHelperData* pointHelperData) {};
	virtual	void			draw(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const PointHelperData* pointHelperData);

protected:

			MVectorArray	points;

};
#endif