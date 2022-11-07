#ifndef _SQUARE_DRAWABLE
#define _SQUARE_DRAWABLE
//
// File: Square.h
//
// Author: Ben Singleton
//

#include "AbstractDrawable.h"


class Square : public Drawable::AbstractDrawable
{

public:

							Square();
	virtual					~Square();

	virtual	void			prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, PointHelperData* pointHelperData);
	virtual	void			draw(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const PointHelperData* pointHelperData);

protected:

			MPointArray		triangles;
			MVectorArray	normals;
			MPointArray		lines;

private:

	static	MObject		MESH_DATA;
	static	MIntArray	BOUNDARY;

	const double POINTS[4][4] = 
	{
		{ 0.0, -0.5, 0.5, 1.0 },
		{ 0.0, 0.5, 0.5, 1.0 },
		{ 0.0, 0.5, -0.5, 1.0 },
		{ 0.0, -0.5, -0.5, 1.0 }
	};

	const int POLYGON_CONNECTS[4] = { 0, 3, 2, 1 };
	const int POLYGON_COUNTS[1] = { 4 };
	const int EDGE_SMOOTHINGS[4] = { 0, 0, 0, 0 };

};
#endif