#ifndef _TEAR_DROP
#define _TEAR_DROP
//
// File: TearDrop.h
//
// Author: Ben Singleton
//

#include "Drawable.h"
#include "DrawableUtilities.h"


class TearDrop : public Drawable 
{

public:

							TearDrop();
	virtual					~TearDrop();

	virtual	void			prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, PointHelperData* pointHelperData);
	virtual	void			draw(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const PointHelperData* pointHelperData);

protected:

			MPointArray		triangles;
			MVectorArray	normals;
			MPointArray		lines;

	static	MObject			MESH_DATA;
	static	MIntArray		BOUNDARY;
	static	double			TEARDROP_RADIUS;
	static	int				SUBDIVISIONS_AXIS;

};

#endif