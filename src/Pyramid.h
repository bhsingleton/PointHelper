#ifndef _PYRAMID_DRAWABLE
#define _PYRAMID_DRAWABLE
//
// File: Pyramid.h
//
// Author: Ben Singleton
//

#include "AbstractDrawable.h"


class Pyramid : public Drawable::AbstractDrawable
{

public:

							Pyramid();
	virtual					~Pyramid();

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
		{ 0.0, 0.0, -0.5, 1.0 },
		{ 0.5, 0.0, 0.0, 1.0 },
		{ 0.0, 0.0, 0.5, 1.0 },
		{ 0.0, 0.5, 0.0, 1.0 }
	};

	const int POLYGON_CONNECTS[24] = { 0, 1, 2, 2, 4, 3, 1, 4, 2, 3, 0, 2, 0, 3, 4, 1 };
	const int POLYGON_COUNTS[5] = { 3, 3, 3, 3, 4 };
	const int NUM_EDGES = 8;

};
#endif