#ifndef _NOTCH_DRAWABLE
#define _NOTCH_DRAWABLE
//
// File: Notch.h
//
// Author: Ben Singleton
//

#include "AbstractDrawable.h"


class Notch : public Drawable::AbstractDrawable
{

public:

							Notch();
	virtual					~Notch();

	virtual	void			prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, PointHelperData* pointHelperData);
	virtual	void			draw(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const PointHelperData* pointHelperData);

protected:

			MPointArray		triangles;
			MVectorArray	normals;
			MPointArray		lines;

	static	MObject			MESH_DATA;
	static	MIntArray		BOUNDARY;
	static	double			NOTCH_RADIUS;
	static	int				SUBDIVISIONS_AXIS;

};
#endif