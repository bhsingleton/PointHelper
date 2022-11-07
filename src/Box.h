#ifndef _BOX_DRAWABLE
#define _BOX_DRAWABLE
//
// File: Box.h
//
// Author: Ben Singleton
//

#include "AbstractDrawable.h"


class Box : public Drawable::AbstractDrawable
{

public:

							Box();
	virtual					~Box();

	virtual	void			prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, PointHelperData* pointHelperData);
	virtual	void			draw(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const PointHelperData* pointHelperData);

protected:

	static	MObject			MESH_DATA;

			MPointArray		triangles;
			MVectorArray	normals;
			MPointArray		lines;

private:

	const double POINTS[8][4] = 
	{
		{-0.5, -0.5, 0.5, 1.0},
		{0.5, -0.5, 0.5, 1.0},
		{-0.5, 0.5, 0.5, 1.0},
		{0.5, 0.5, 0.5, 1.0},
		{-0.5, 0.5, -0.5, 1.0},
		{0.5, 0.5, -0.5, 1.0},
		{-0.5, -0.5, -0.5, 1.0},
		{0.5, -0.5, -0.5, 1.0}
	};

	const int POLYGON_COUNTS[6] = { 4, 4, 4, 4, 4, 4 };
	const int POLYGON_CONNECTS[24] = { 0, 1, 3, 2, 2, 3, 5, 4, 4, 5, 7, 6, 6, 7, 1, 0, 1, 7, 5, 3, 6, 0, 2, 4 };
	const int NUM_EDGES = 12;

};
#endif