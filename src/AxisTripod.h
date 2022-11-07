#ifndef _AXIS_TRIPOD_DRAWABLE
#define _AXIS_TRIPOD_DRAWABLE
//
// File: AxisTripod.h
//
// Author: Ben Singleton
//

#include "AbstractDrawable.h"


class AxisTripod : public Drawable::AbstractDrawable
{

public:

						AxisTripod();
	virtual				~AxisTripod();

	virtual	void		prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, PointHelperData* pointHelperData);
	virtual	void		draw(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const PointHelperData* pointHelperData);

protected:

			MPointArray	lines;

private:

	const double LINES[18][4] = 
	{
		{ 0.0, 0.0, 0.0, 1.0 },
		{ 2.0, 0.0, 0.0, 1.0 },
		{ 0.0, 0.0, 0.0, 1.0 },
		{ 0.0, 2.0, 0.0, 1.0 },
		{ 0.0, 0.0, 0.0, 1.0 },
		{ 0.0, 0.0, 2.0, 1.0 },
		{ 1.8, 0.2, 0.0, 1.0 },
		{ 2.0, 0.0, 0.0, 1.0 },
		{ 2.0, 0.0, 0.0, 1.0 },
		{ 1.8, -0.2, 0.0, 1.0 },
		{ 0.2, 1.8, 0.0, 1.0 },
		{ 0.0, 2.0, 0.0, 1.0 },
		{ 0.0, 2.0, 0.0, 1.0 },
		{ -0.2, 1.8, 0.0, 1.0 },
		{ 0.0, 0.2, 1.8, 1.0 },
		{ 0.0, 0.0, 2.0, 1.0 },
		{ 0.0, 0.0, 2.0, 1.0 },
		{ 0.0, -0.2, 1.8, 1.0 }
	};

};
#endif