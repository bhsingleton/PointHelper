#ifndef _CYLINDER
#define _CYLINDER
//
// File: Cylinder.h
//
// Author: Ben Singleton
//

#include "Drawable.h"
#include "DrawableUtilities.h"


class Cylinder : public Drawable 
{

public:

							Cylinder();
	virtual					~Cylinder();

	virtual	void			prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, PointHelperData* pointHelperData);
	virtual	void			draw(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const PointHelperData* pointHelperData);

protected:

	static	MObject			MESH_DATA;

			MPointArray		triangles;
			MVectorArray	normals;
			MPointArray		lines;
	
	static	double			CYLINDER_RADIUS;
	static	double			CYLINDER_LENGTH;
	static	int				SUBDIVISIONS_AXIS;

};

#endif