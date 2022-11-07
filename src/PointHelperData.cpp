//
// File: PointHelperData.cpp
//
// Author: Ben Singleton
//

#include "PointHelperData.h"


PointHelperData::PointHelperData() : MUserData(false)
/**
Constructor.
*/
{

	this->enabled = { { "cross", true }, { "box", true } };
	this->localPosition = MVector(0.0, 0.0, 0.0);
	this->localRotate = MEulerRotation(0.0, 0.0, 0.0);
	this->localScale = MVector(1.0, 1.0, 1.0);
	this->size = 10;
	this->objectMatrix = MMatrix::identity;
	this->choice = 0;
	this->currentText = MString();
	this->texts = MStringArray();
	this->fontSize = 12;
	this->lineWidth = 1;
	this->wireColor = MColor();
	this->controlPoints = MPointArray();
	this->fill = false;
	this->shaded = false;
	this->drawOnTop = false;
	this->depthPriority = 0;

};


PointHelperData::~PointHelperData()
/**
Destructor.
*/
{

	this->enabled.clear();
	this->texts.clear();
	this->controlPoints.clear();

};


MStatus PointHelperData::cacheText(const unsigned int index, const MString& text)
/**
Caches a text value from the supplied text plug element.

@param element: The plug element to cache.
@return: Return status.
*/
{

	MStatus status;

	// Resize text array based on plug index
	//
	unsigned int textCount = this->texts.length();

	if (index >= textCount)
	{

		status = this->texts.setLength(index + 1);
		CHECK_MSTATUS_AND_RETURN_IT(status);

	}

	// Update text element
	//
	this->texts[index] = text;

	return status;

};


MStatus PointHelperData::cacheControlPoint(const unsigned int index, const int child, const double value)
/**
Caches a control point from the supplied control-point plug element.

@param element: The plug element to cache.
@return: Return status.
*/
{
	MStatus status;

	// Resize point array based on plug index
	//
	unsigned int controlPointCount = this->controlPoints.length();

	if (index >= controlPointCount)
	{

		status = this->controlPoints.setLength(index + 1);
		CHECK_MSTATUS_AND_RETURN_IT(status);

	}

	// Update control point element
	//
	this->controlPoints[index][child] = value;

	return MS::kSuccess;

};


MStatus PointHelperData::cacheWireColor(const MDagPath& dagPath)
/**
Caches the wire-colour from the supplied dag path.

@param dagPath: A path to the point helper.
@return: Return status.
*/
{

	this->wireColor = MHWRender::MGeometryUtilities::wireframeColor(dagPath);
	return MS::kSuccess;

};


MStatus PointHelperData::cacheDepthPriority(const MDagPath& dagPath)
/**
Caches the depth priority from the supplied dag path.

@param dagPath: A path to the point helper.
@return: Return status.
*/
{


	// Get correct depth priority
	//
	MHWRender::DisplayStatus status = MHWRender::MGeometryUtilities::displayStatus(dagPath);

	switch (status)
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


void PointHelperData::dirtyCurrentText()
/**
Updates the current text value based on the associated choice index.
If the index is out of range then an empty string is used.

@return: Return status.
*/
{

	// Check if choice is within range
	//
	int textCount = this->texts.length();

	if (0u <= this->choice < textCount)
	{

		this->currentText = this->texts[this->choice];

	}
	else
	{

		this->currentText = MString();

	}

};


void PointHelperData::dirtyObjectMatrix()
/**
Updates the internal object-matrix.

@return: Null.
*/
{

	MMatrix positionMatrix = Drawable::createPositionMatrix(this->localPosition);
	MMatrix rotateMatrix = this->localRotate.asMatrix();
	MMatrix scaleMatrix = Drawable::createScaleMatrix(this->localScale);
	MMatrix sizeMatrix = Drawable::createScaleMatrix(this->size);

	this->objectMatrix = sizeMatrix * scaleMatrix * rotateMatrix * positionMatrix;

};