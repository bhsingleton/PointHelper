#ifndef _DISC
#define _DISC
//
// File: Disc.h
//
// Author: Ben Singleton
//

#include "Drawable.h"
#include "DrawableUtilities.h"


class Disc : public Drawable 
{

public:

							Disc();
	virtual					~Disc();

	virtual	void			prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, PointHelperData* pointHelperData);
	virtual	void			draw(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const PointHelperData* pointHelperData);

protected:

			MPointArray		triangles;
			MVectorArray	normals;
			MPointArray		lines;
	
	static	MObject			MESH_DATA;
	static	MIntArray		BOUNDARY;
	static	double			DISC_RADIUS;
	static	int				SUBDIVISIONS_AXIS;

};

#endif