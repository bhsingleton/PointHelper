#ifndef _AXIS_VIEW_DRAWABLE
#define _AXIS_VIEW_DRAWABLE
//
// File: AxisView.h
//
// Author: Ben Singleton
//

#include "AbstractDrawable.h"


class AxisView : public Drawable::AbstractDrawable
{

public:

						AxisView();
	virtual				~AxisView();

	virtual	void		prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, PointHelperData* pointHelperData);
	virtual	void		draw(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const PointHelperData* pointHelperData);

protected:

			MPointArray	lines;
			double		radius;

private:

	const double LINES[6][4] = 
	{
		{ -0.5, 0.0, 0.0, 1.0 },
		{ 0.5, 0.0, 0.0, 1.0 },
		{ 0.0, -0.5, 0.0, 1.0 },
		{ 0.0, 0.5, 0.0, 1.0 },
		{ 0.0, 0.0, -0.5, 1.0 },
		{ 0.0, 0.0, 0.5, 1.0 }
	};

	const double RADIUS = 0.1;
	const int SUBDIVISION_AXIS = 12;
	const int SUBDIVISION_HEIGHT = 12;

	const MColor RED = MColor(1.0, 0.0, 0.0);
	const MColor GREEN = MColor(0.0, 1.0, 0.0);
	const MColor BLUE = MColor(0.0, 0.0, 1.0);

};
#endif