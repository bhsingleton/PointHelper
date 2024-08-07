#ifndef _POINT_HELPER_NODE
#define _POINT_HELPER_NODE
//
// File: PointHelperNode.h
//
// Dependency Graph Node: pointHelper
//
// Author: Ben Singleton
//

#include "Drawable.h"
#include "PointHelperData.h"

#include <maya/MPxLocatorNode.h>
#include <maya/MObject.h>
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
#include <maya/MFnVectorArrayData.h>
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


enum class Axis
{

	X = 0,
	Y = 1,
	Z = 2

};


class PointHelper : public MPxLocatorNode 
{

public:

								PointHelper();
	virtual						~PointHelper();

	virtual MStatus				compute(const MPlug& plug, MDataBlock& data);
	virtual	void				draw(M3dView& view, const MDagPath& dagPath, M3dView::DisplayStyle displayStyle, M3dView::DisplayStatus displayStatus) {};

	virtual	MStatus				preEvaluation(const MDGContext& context, const MEvaluationNode& evaluationNode);
	virtual	void				getCacheSetup(const MEvaluationNode& evaluationNode, MNodeCacheDisablingInfo& disablingInfo, MNodeCacheSetupInfo& cacheSetupInfo, MObjectArray& monitoredAttributes) const;

	virtual	bool				getInternalValue(const MPlug& plug, MDataHandle& handle);
	virtual	bool				setInternalValue(const MPlug& plug, const MDataHandle& handle);
	virtual int					internalArrayCount(const MPlug& plug) const;
	virtual	void				copyInternalData(PointHelperData* data);

	virtual	MStatus				setText(const unsigned int index, const MString& text);
	virtual	MStatus				setControlPoint(const unsigned int index, const MVector& vector);
	virtual	MStatus				setControlPoint(const unsigned int index, const Axis axis, const double value);

	static	MMatrix				getMatrixData(const MObject& data);
	virtual	MDagPath			thisMDagPath() const;

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
	static  MObject				axisView;
	static  MObject				cross;
	static	MObject				square;
	static  MObject				box;
	static  MObject				triangle;
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
			
			double				sizeValue;
			MVector				localPositionValue;
			MVector				localRotateValue;
			MVector				localScaleValue;

			float				lineWidthValue;
			bool				fillValue;
			bool				shadedValue;
			bool				drawOnTopValue;

			MStringArray		textValues;
			unsigned int		choiceValue;
			unsigned int		fontSizeValue;

			MVectorArray		controlPointValues;

			std::map<std::string, bool>	drawables;

};


#endif