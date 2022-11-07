#ifndef _ABSTRACT_DRAWABLE
#define _ABSTRACT_DRAWABLE
//
// File: Drawable.h
//
// Author: Ben Singleton
//

#include "PointHelperData.h"
#include "Drawable.h"

#include <maya/MUIDrawManager.h>
#include <maya/MFrameContext.h>


namespace Drawable
{

	class AbstractDrawable
	{

	public:

							AbstractDrawable();
		virtual				~AbstractDrawable();

		virtual	void		prepareForDraw(const MDagPath& dagPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, PointHelperData* pointHelperData) = 0;
		virtual	void		draw(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const PointHelperData* pointHelperData) = 0;

		virtual	MMatrix		getObjectMatrix();
		virtual	void		setObjectMatrix(MMatrix objectMatrix);

		virtual	bool		isEnabled();
		virtual	void		setEnabled(bool enabled);

	protected:

				bool		enabled;
				MMatrix		objectMatrix;
				MVector		forward, up, right, scale;
				MPoint		center;

	};

};
#endif