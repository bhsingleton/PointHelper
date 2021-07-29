#ifndef _POINT_HELPER_NODE
#define _POINT_HELPER_NODE
//
// File: PointHelperNode.h
//
// Dependency Graph Node: PointHelper
//
// Author: Ben Singleton
//

#include <maya/MPxLocatorNode.h>
#include <maya/MPxTransform.h>

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
#include <maya/MMessage.h>
#include <maya/MDagMessage.h>
#include <maya/MNodeMessage.h>
#include <maya/MEventMessage.h>
#include <maya/MCallbackIdArray.h>

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

#include <maya/MGlobal.h>
#include <maya/MTypeId.h>
#include <maya/MEvaluationNode.h>
#include <maya/MNodeCacheDisablingInfo.h>
#include <maya/MNodeCacheSetupInfo.h>
#include <maya/MViewport2Renderer.h>

#include <assert.h>


class PointHelper : public MPxLocatorNode 
{

public:

							PointHelper();
	virtual					~PointHelper();

	virtual MStatus			compute(const MPlug& plug, MDataBlock& data);
	virtual	void			draw(M3dView& view, const MDagPath& dagPath, M3dView::DisplayStyle displayStyle, M3dView::DisplayStatus displayStatus) {};

	virtual	MStatus			preEvaluation(const MDGContext& context, const MEvaluationNode& evaluationNode);
	virtual	void			getCacheSetup(const MEvaluationNode& evaluationNode, MNodeCacheDisablingInfo& disablingInfo, MNodeCacheSetupInfo& cacheSetupInfo, MObjectArray& monitoredAttributes) const;

	virtual	bool			isBounded() const;
	virtual	MBoundingBox	boundingBox() const;

	static  void*			creator();
	static  MStatus			initialize();

	virtual	MMatrix			getObjectMatrix() const;
	virtual	MMatrix			getObjectWorldMatrix() const;

	virtual	MString			getTextChoice() const;
	static	MStatus			getControlPoints(const MDagPath& dagPath, MPointArray& points);

	static	bool			getBooleanValue(const MObject& node, const MObject& attribute);
	static	double			getDoubleValue(const MObject& node, const MObject& attribute);
	static	float			getFloatValue(const MObject& node, const MObject& attributerre);
	static	int				getIntegerValue(const MObject& node, const MObject& attribute);

	static	MStatus			getAttributesByCategory(const MObject& node, const MString& category, MObjectArray& attributes);
	virtual	MStatus			getAttributesByCategory(const MString& category, MObjectArray& attributes);

public:

	static  MObject			localRotate;
	static  MObject			localRotateX;
	static  MObject			localRotateY;
	static  MObject			localRotateZ;

	static  MObject			centerMarker;
	static  MObject			axisTripod;
	static  MObject			cross;
	static	MObject			square;
	static  MObject			box;
	static  MObject			pyramid;
	static	MObject			diamond;
	static  MObject			disc;
	static  MObject			arrow;
	static  MObject			notch;
	static  MObject			tearDrop;
	static  MObject			cylinder;
	static  MObject			sphere;
	static	MObject			custom;

	static  MObject			size;
	static  MObject			choice;
	static  MObject			text;
	static  MObject			fontSize;
	static  MObject			lineWidth;
	static  MObject			drawOnTop;
	static  MObject			fill;
	static  MObject			shaded;
	static	MObject			controlPoints;
	static	MObject			xValue;
	static	MObject			yValue;
	static	MObject			zValue;

	static  MObject			objectMatrix;
	static	MObject			objectInverseMatrix;
	static  MObject			objectWorldMatrix;
	static  MObject			objectWorldInverseMatrix;

	static	MTypeId			id;
	static	MString			drawableCategory;
	static	MString			drawDbClassification;
	static	MString			drawRegistrantId;

};


#endif