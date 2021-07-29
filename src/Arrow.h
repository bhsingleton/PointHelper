#ifndef _ARROW
#define _ARROW
//
// File: Arrow.h
//
// Author: Ben Singleton
//

#include "Drawable.h"
#include "DrawableUtilities.h"


class Arrow : public Drawable 
{

public:

							Arrow();
	virtual					~Arrow();

	virtual	void			prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, PointHelperData* pointHelperData);
	virtual	void			draw(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const PointHelperData* pointHelperData);

protected:

			MPointArray		triangles;
			MVectorArray	normals;
			MPointArray		lines;

	static	MObject			MESH_DATA;
	static	MIntArray		BOUNDARY;

private:

	const double POINTS[35][4] = 
	{
		{ 0.4904, 0.0975, 0.0, 1.0 },
		{ 0.4619, 0.1913, 0.0, 1.0 },
		{ 0.4157, 0.2778, 0.0, 1.0 },
		{ 0.3536, 0.3536, 0.0, 1.0 },
		{ 0.2778, 0.4157, 0.0, 1.0 },
		{ 0.1913, 0.4619, 0.0, 1.0 },
		{ 0.0975, 0.4904, 0.0, 1.0 },
		{ 0.0, 0.5, 0.0, 1.0 },
		{ -0.0975, 0.4904, 0.0, 1.0 },
		{ -0.1913, 0.4619, 0.0, 1.0 },
		{ -0.2778, 0.4157, 0.0, 1.0 },
		{ -0.3536, 0.3536, 0.0, 1.0 },
		{ -0.4157, 0.2778, 0.0, 1.0 },
		{ -0.4619, 0.1913, 0.0, 1.0 },
		{ -0.4904, 0.0975, 0.0, 1.0 },
		{ -0.5, 0.0, 0.0, 1.0 },
		{ -0.4904, -0.0975, 0.0, 1.0 },
		{ -0.4619, -0.1913, 0.0, 1.0 },
		{ -0.4157, -0.2778, 0.0, 1.0 },
		{ -0.3536, -0.3536, 0.0, 1.0 },
		{ -0.2778, -0.4157, 0.0, 1.0 },
		{ -0.1913, -0.4619, 0.0, 1.0 },
		{ 0.1913, -0.4619, 0.0, 1.0 },
		{ 0.2778, -0.4157, 0.0, 1.0 },
		{ 0.3536, -0.3536, 0.0, 1.0 },
		{ 0.4157, -0.2778, 0.0, 1.0 },
		{ 0.4619, -0.1913, 0.0, 1.0 },
		{ 0.4904, -0.0975, 0.0, 1.0 },
		{ 0.5, 0.0, 0.0, 1.0 },
		{ 0.0, 0.0, 0.0, 1.0 },
		{ 0.0, -1.0, 0.0, 1.0 },
		{ 0.375, -0.625, 0.0, 1.0 },
		{ -0.375, -0.625, 0.0, 1.0 },
		{ 0.1913, -0.625, 0.0, 1.0 },
		{ -0.1913, -0.625, 0.0, 1.0 }
	};

	const int POLYGON_CONNECTS[100] = { 0, 1, 29, 1, 2, 29, 2, 3, 29, 3, 4, 29, 4, 5, 29, 5, 6, 29, 6, 7, 29, 7, 8, 29, 8, 9, 29, 9, 10, 29, 10, 11, 29, 11, 12, 29, 12, 13, 29, 13, 14, 29, 14, 15, 29, 15, 16, 29, 16, 17, 29, 17, 18, 29, 18, 19, 29, 19, 20, 29, 20, 21, 29, 29, 21, 22, 22, 23, 29, 23, 24, 29, 24, 25, 29, 25, 26, 29, 26, 27, 29, 27, 28, 29, 28, 0, 29, 30, 34, 32, 30, 31, 33, 30, 33, 34, 22, 21, 34, 33 };
	const int POLYGON_COUNTS[33] = { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4 };

};

#endif