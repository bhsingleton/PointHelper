#ifndef _CROSS
#define _CROSS
//
// File: Cross.h
//
// Author: Ben Singleton
//

#include "Drawable.h"
#include "DrawableUtilities.h"


class Cross : public Drawable 
{

public:

							Cross();
	virtual					~Cross();

	virtual	void			prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, PointHelperData* pointHelperData);
	virtual	void			draw(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const PointHelperData* pointHelperData);

protected:

			MPointArray		lines;

private:

	const double LINES[6][4] = {
		{ -1.0, 0.0, 0.0, 1.0 },
		{ 1.0, 0.0, 0.0, 1.0 },
		{ 0.0, -1.0, 0.0, 1.0 },
		{ 0.0, 1.0, 0.0, 1.0 },
		{ 0.0, 0.0, -1.0, 1.0 },
		{ 0.0, 0.0, 1.0, 1.0 }
	};

};

#endif