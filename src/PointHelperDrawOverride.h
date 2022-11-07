#ifndef _POINT_HELPER_DRAW_OVERRIDE
#define _POINT_HELPER_DRAW_OVERRIDE
//
// File: PointHelperDrawOverride.h
//
// Author: Ben Singleton
//

#include "PointHelperNode.h"
#include "PointHelperData.h"
#include "AbstractDrawable.h"

#include <maya/MPxDrawOverride.h>
#include <maya/MObject.h>
#include <maya/MDagPath.h>
#include <maya/MEventMessage.h>
#include <maya/MCallbackIdArray.h>
#include <maya/MFnDependencyNode.h>

#include <maya/MViewport2Renderer.h>
#include <maya/MUIDrawManager.h>
#include <maya/MFrameContext.h>
#include <maya/MHWGeometry.h>
#include <maya/MHWGeometryUtilities.h>

#include <map>
#include <string>


class PointHelperDrawOverride : public MHWRender::MPxDrawOverride 
{

public:

								PointHelperDrawOverride(const MObject& node);
	virtual						~PointHelperDrawOverride();

	static	MPxDrawOverride*	creator(const MObject& node);

	virtual	MHWRender::DrawAPI	supportedDrawAPIs() const;

	virtual bool				isBounded(const MDagPath& objPath, const MDagPath& cameraPath) const;
	virtual	MBoundingBox		boundingBox(const MDagPath& objPath, const MDagPath& cameraPath) const;

	virtual MUserData*			prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, MUserData* userData);

	virtual bool				hasUIDrawables() const;
	virtual void				addUIDrawables(const MDagPath& objPath, MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const MUserData* userData);

	virtual	bool				traceCallSequence() const;
	virtual	void				handleTraceMessage(const MString& message) const;

private:

			PointHelper*										pointHelper;
			std::map<std::string, Drawable::AbstractDrawable*>	drawables;

			MCallbackId							modelEditorChangedCallbackId;
	static	void								onModelEditorChanged(void *clientData);

};
#endif