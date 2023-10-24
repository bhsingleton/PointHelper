#ifndef _TRIANGLE_DRAWABLE
#define _TRIANGLE_DRAWABLE
//
// File: Triangle.h
//
// Author: Ben Singleton
//

#include "AbstractDrawable.h"


class Triangle : public Drawable::AbstractDrawable
{

public:

							Triangle();
	virtual					~Triangle();

	virtual	void			prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, PointHelperData* pointHelperData);
	virtual	void			draw(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const PointHelperData* pointHelperData);

protected:

			MPointArray		triangles;
			MVectorArray	normals;
			MPointArray		lines;

private:

	static	MObject		MESH_DATA;
	static	MIntArray	BOUNDARY;

	const double POINTS[3][4] =
	{
		{ 0.0, 0.0, 0.5, 1.0 },
		{ 0.5, 0.0, 0.0, 1.0 },
		{ 0.0, 0.0, -0.5, 1.0 }
	};

	const int POLYGON_CONNECTS[3] = { 0, 2, 1 };
	const int POLYGON_COUNTS[1] = { 3 };
	const int EDGE_SMOOTHINGS[4] = { 0, 0, 0 };

};
#endif