//
// File: PointHelperData.cpp
//
// Author: Ben Singleton
//

#include "PointHelperData.h"


PointHelperData::PointHelperData() : MUserData()
/**
Constructor.
*/
{

	this->localPosition = MVector(0.0, 0.0, 0.0);
	this->localRotate = MVector(0.0, 0.0, 0.0);
	this->localScale = MVector(1.0, 1.0, 1.0);
	this->size = 10.0;
	this->objectMatrix = Drawable::createScaleMatrix(this->size);

	this->lineWidth = 1;
	this->wireColor = MColor();
	this->fill = false;
	this->shaded = false;
	this->drawOnTop = false;
	this->depthPriority = 0;

	this->text = MString("");
	this->fontSize = 11;

	this->controlPoints = MVectorArray();

	this->drawables = { { "cross", true }, { "box", true } };

};


PointHelperData::~PointHelperData()
/**
Destructor.
*/
{

	this->controlPoints.clear();
	this->drawables.clear();

};


MStatus PointHelperData::copyWireColor(const MDagPath& dagPath)
/**
Copies the wire-colour from the supplied dag path.

@param dagPath: A path to the point helper.
@return: Return status.
*/
{

	MStatus status;

	// Check if path is valid
	//
	bool isValid = dagPath.isValid(&status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	if (!isValid)
	{

		return MS::kFailure;

	}

	// Evaluate wire color
	//
	this->wireColor = MHWRender::MGeometryUtilities::wireframeColor(dagPath);
	return MS::kSuccess;

};


MStatus PointHelperData::copyDepthPriority(const MDagPath& dagPath)
/**
Copies the depth priority from the supplied dag path.

@param dagPath: A path to the point helper.
@return: Return status.
*/
{

	MStatus status;

	// Check if path is valid
	//
	bool isValid = dagPath.isValid(&status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	if (!isValid)
	{

		return MS::kFailure;

	}

	// Evaluate display status
	//
	MHWRender::DisplayStatus displayStatus = MHWRender::MGeometryUtilities::displayStatus(dagPath);

	switch (displayStatus)
	{

	case MHWRender::DisplayStatus::kActiveComponent:

		this->depthPriority = MHWRender::MRenderItem::sActiveWireDepthPriority;
		break;

	default:

		this->depthPriority = MHWRender::MRenderItem::sDormantFilledDepthPriority;
		break;

	}

	return MS::kSuccess;

};


void PointHelperData::invalidate()
{

	MMatrix positionMatrix = Drawable::createPositionMatrix(this->localPosition);
	MMatrix rotateMatrix = Drawable::createRotationMatrix(this->localRotate);
	MMatrix scaleMatrix = Drawable::createScaleMatrix(this->localScale);
	MMatrix sizeMatrix = Drawable::createScaleMatrix(this->size);

	this->objectMatrix = sizeMatrix * scaleMatrix * rotateMatrix * positionMatrix;

};
