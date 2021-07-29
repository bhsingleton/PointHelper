#ifndef _DIAMOND
#define _DIAMOND
//
// File: Diamond.h
//
// Author: Ben Singleton
//

#include "Drawable.h"
#include "DrawableUtilities.h"


class Diamond : public Drawable 
{

public:

							Diamond();
	virtual					~Diamond();

	virtual	void			prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, PointHelperData* pointHelperData);
	virtual	void			draw(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const PointHelperData* pointHelperData);

protected:

	static	MObject			MESH_DATA;

			MPointArray		triangles;
			MVectorArray	normals;
			MPointArray		lines;

private:

	const double POINTS[6][4] = 
	{
		{ 0.0, -0.5, 0.0, 1.0 },
		{ 0.5, 0.0, 0.0, 1.0 },
		{ 0.0, 0.0, 0.5, 1.0 },
		{ -0.5, 0.0, 0.0, 1.0 },
		{ 0.0, 0.5, 0.0, 1.0 },
		{ 0.0, 0.0, -0.5, 1.0 }
	};

	const int POLYGON_CONNECTS[24] = { 0, 1, 2, 2, 4, 3, 3, 4, 5, 5, 1, 0, 1, 4, 2, 3, 0, 2, 1, 5, 4, 5, 0, 3 };
	const int POLYGON_COUNTS[8] = { 3, 3, 3, 3, 3, 3, 3, 3 };
	const int NUM_EDGES = 12;

};

#endif