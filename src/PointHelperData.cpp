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
	this->localRotate = MVector(0.0, 0.0, 0.0);
	this->localScale = MVector(1.0, 1.0, 1.0);
	this->size = 10;
	this->objectMatrix = Drawable::createScaleMatrix(this->size);
	this->choice = 0;
	this->currentText = MString();
	this->texts = MStringArray();
	this->fontSize = 11;
	this->lineWidth = 1;
	this->wireColor = MColor();
	this->controlPoints = MVectorArray();
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


PointHelperData& PointHelperData::operator=(const PointHelperData* src)
/**
Assignment operator.

@param src: Point helper data to be copied.
@return: Self.
*/
{

	this->enabled = src->enabled;
	this->localPosition = src->localPosition;
	this->localRotate = src->localRotate;
	this->localScale = src->localScale;
	this->size = src->size;
	this->objectMatrix = src->objectMatrix;
	this->choice = src->choice;
	this->currentText = src->currentText;
	this->texts = src->texts;
	this->fontSize = src->fontSize;
	this->lineWidth = src->lineWidth;
	this->wireColor = src->wireColor;
	this->controlPoints = src->controlPoints;
	this->fill = src->fill;
	this->shaded = src->shaded;
	this->drawOnTop = src->drawOnTop;
	this->depthPriority = src->depthPriority;

	return *this;

};


MStatus PointHelperData::resizeTexts(const unsigned int size)
/**
Resizes the text array to ensure there is enough room for specified size.

@param size: The minimum size of the array.
@return: Return status.
*/
{

	MStatus status;

	// Redundancy check
	//
	unsigned int count = this->texts.length();

	if (size > count)
	{

		status = this->texts.setLength(size);
		CHECK_MSTATUS_AND_RETURN_IT(status);

	}

	return status;

};


MStatus PointHelperData::allocateText(const unsigned int index, const MString& text)
/**
Caches a text value from the supplied text plug element.

@param element: The plug element to cache.
@return: Return status.
*/
{

	MStatus status;

	// Update text element
	//
	status = this->resizeTexts(index + 1);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	this->texts[index] = text;

	return status;

};


MStatus PointHelperData::resizeControlPoints(const unsigned int size)
/**
Resizes the control point array to ensure there is enough room for specified size.

@param size: The minimum size of the array.
@return: Return status.
*/
{

	MStatus status;

	// Redundancy check
	//
	unsigned int count = this->controlPoints.length();

	if (size > count)
	{

		status = this->controlPoints.setLength(size);
		CHECK_MSTATUS_AND_RETURN_IT(status);

	}

	return status;

};


MStatus PointHelperData::allocateControlPoint(const unsigned int index, const MVector& point)
/**
Caches a control point from the supplied control-point plug element.

@param element: The plug element to cache.
@return: Return status.
*/
{
	MStatus status;

	// Update control point element
	//
	this->resizeControlPoints(index + 1);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	this->controlPoints[index] = point;

	return MS::kSuccess;

};


MStatus PointHelperData::allocateControlPoint(const unsigned int index, const int child, const double value)
/**
Caches a control point from the supplied control-point plug element.

@param element: The plug element to cache.
@return: Return status.
*/
{
	MStatus status;

	// Update control point element
	//
	this->resizeControlPoints(index + 1);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	this->controlPoints[index][child] = value;

	return MS::kSuccess;

};


MStatus PointHelperData::copyWireColor(const MDagPath& dagPath)
/**
Caches the wire-colour from the supplied dag path.

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
Caches the depth priority from the supplied dag path.

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


void PointHelperData::dirtyCurrentText()
/**
Updates the current text value based on the associated choice index.
If the index is out of range then an empty string is used.

@return: Return status.
*/
{

	// Check if choice is within range
	//
	unsigned int textCount = this->texts.length();

	if (0u <= this->choice && this->choice < textCount)
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
	MMatrix rotateMatrix = Drawable::createRotationMatrix(this->localRotate);
	MMatrix scaleMatrix = Drawable::createScaleMatrix(this->localScale);
	MMatrix sizeMatrix = Drawable::createScaleMatrix(this->size);

	this->objectMatrix = sizeMatrix * scaleMatrix * rotateMatrix * positionMatrix;

};