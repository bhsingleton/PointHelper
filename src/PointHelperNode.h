#ifndef _POINT_HELPER_NODE
#define _POINT_HELPER_NODE
//
// File: PointHelperNode.h
//
// Dependency Graph Node: PointHelper
//
// Author: Ben Singleton
//

#include "Drawable.h"
#include "PointHelperData.h"

#include <maya/MPxLocatorNode.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MArrayDataBuilder.h>
#include <maya/MObject.h>
#include <maya/MObjectArray.h>
#include <maya/MDagPath.h>
#include <maya/MDagPathArray.h>
#include <maya/MString.h>
#include <maya/MMatrix.h>
#include <maya/MMatrixArray.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MBoundingBox.h>
#include <maya/MDistance.h>
#include <maya/MEulerRotation.h>
#include <maya/MAngle.h>
#include <maya/MColor.h>
#include <maya/MDoubleArray.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MVector.h>
#include <maya/MVectorArray.h>

#include <maya/MFnDependencyNode.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnTransform.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnNumericData.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnTypedAttribute.h>

#include <maya/MEvaluationNode.h>
#include <maya/MNodeCacheDisablingInfo.h>
#include <maya/MNodeCacheSetupInfo.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MDGContext.h>
#include <maya/M3dView.h>
#include <maya/MGlobal.h>
#include <maya/MTypeId.h>

#include <assert.h>


class PointHelper : public MPxLocatorNode 
{

public:

								PointHelper();
	virtual						~PointHelper();

	virtual MStatus				compute(const MPlug& plug, MDataBlock& data);
	virtual	void				draw(M3dView& view, const MDagPath& dagPath, M3dView::DisplayStyle displayStyle, M3dView::DisplayStatus displayStatus) {};

	virtual	MStatus				preEvaluation(const MDGContext& context, const MEvaluationNode& evaluationNode);
	virtual	void				getCacheSetup(const MEvaluationNode& evaluationNode, MNodeCacheDisablingInfo& disablingInfo, MNodeCacheSetupInfo& cacheSetupInfo, MObjectArray& monitoredAttributes) const;

	virtual	bool				setInternalValue(const MPlug& plug, const MDataHandle& handle);
	virtual	void				copyInternalData(MPxNode* node);
	virtual PointHelperData*	getUserData();

	virtual	bool				isBounded() const;
	virtual	MBoundingBox		boundingBox() const;

	static  void*				creator();
	static  MStatus				initialize();

public:

	static  MObject				localRotate;
	static  MObject				localRotateX;
	static  MObject				localRotateY;
	static  MObject				localRotateZ;

	static  MObject				centerMarker;
	static  MObject				axisTripod;
	static  MObject				cross;
	static	MObject				square;
	static  MObject				box;
	static  MObject				pyramid;
	static	MObject				diamond;
	static  MObject				disc;
	static  MObject				arrow;
	static  MObject				notch;
	static  MObject				tearDrop;
	static  MObject				cylinder;
	static  MObject				sphere;
	static	MObject				custom;

	static  MObject				size;
	static  MObject				choice;
	static  MObject				text;
	static  MObject				fontSize;
	static  MObject				lineWidth;
	static	MObject				controlPoints;
	static	MObject				xValue;
	static	MObject				yValue;
	static	MObject				zValue;
	static  MObject				fill;
	static  MObject				shaded;
	static  MObject				drawOnTop;

	static  MObject				objectMatrix;
	static	MObject				objectInverseMatrix;
	static  MObject				objectWorldMatrix;
	static  MObject				objectWorldInverseMatrix;

	static	MString				localPositionCategory;
	static	MString				localRotationCategory;
	static	MString				localScaleCategory;
	static	MString				drawableCategory;
	static	MString				textCategory;
	static	MString				customCategory;
	static	MString				renderCategory;

	static	MString				drawDbClassification;
	static	MString				drawRegistrantId;
	static	MTypeId				id;

protected:

			PointHelperData*	data;

};


#endif