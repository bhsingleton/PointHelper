#ifndef _SPHERE
#define _SPHERE
//
// File: Sphere.h
//
// Author: Ben Singleton
//

#include "Drawable.h"
#include "DrawableUtilities.h"


class Sphere : public Drawable 
{

public:

							Sphere();
	virtual					~Sphere();

	virtual	void			prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, PointHelperData* pointHelperData);
	virtual	void			draw(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const PointHelperData* pointHelperData);

protected:

			MPointArray		triangles;
			MVectorArray	normals;
			MPointArray		lines;
	
	static	MObject			MESH_DATA;
	static	double			SPHERE_RADIUS;
	static	int				SUBDIVISIONS_AXIS;
	static	int				SUBDIVISIONS_HEIGHT;

};

#endif