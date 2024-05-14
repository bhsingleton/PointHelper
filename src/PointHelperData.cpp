//
// File: PointHelperData.cpp
//
// Author: Ben Singleton
//

#include "PointHelperData.h"
#include "PointHelperNode.h"


PointHelperData::PointHelperData() : MUserData(true)
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
	this->text = MString();
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
	this->controlPoints.clear();

};


MStatus PointHelperData::copyInternalData(const MDagPath& dagPath)
/**
Assignment operator.

@param src: Point helper data to be copied.
@return: Self.
*/
{

	// Get associated node from path
	//
	MObject node = dagPath.node();
	
	// Copy drawable properties
	//
	this->enabled["centerMarker"] = MPlug(node, PointHelper::centerMarker).asBool();
	this->enabled["axisTripod"] = MPlug(node, PointHelper::axisTripod).asBool();
	this->enabled["axisView"] = MPlug(node, PointHelper::axisView).asBool();
	this->enabled["cross"] = MPlug(node, PointHelper::cross).asBool();
	this->enabled["square"] = MPlug(node, PointHelper::square).asBool();
	this->enabled["box"] = MPlug(node, PointHelper::box).asBool();
	this->enabled["triangle"] = MPlug(node, PointHelper::triangle).asBool();
	this->enabled["pyramid"] = MPlug(node, PointHelper::pyramid).asBool();
	this->enabled["diamond"] = MPlug(node, PointHelper::diamond).asBool();
	this->enabled["disc"] = MPlug(node, PointHelper::disc).asBool();
	this->enabled["arrow"] = MPlug(node, PointHelper::arrow).asBool();
	this->enabled["notch"] = MPlug(node, PointHelper::notch).asBool();
	this->enabled["tearDrop"] = MPlug(node, PointHelper::tearDrop).asBool();
	this->enabled["cylinder"] = MPlug(node, PointHelper::cylinder).asBool();
	this->enabled["sphere"] = MPlug(node, PointHelper::sphere).asBool();
	this->enabled["custom"] = MPlug(node, PointHelper::custom).asBool();

	// Copy text properties
	//
	MPlug textPlug = MPlug(node, PointHelper::text);
	unsigned int choice = MPlug(node, PointHelper::choice).asInt();

	this->text = textPlug.elementByLogicalIndex(choice).asString();
	this->fontSize = MPlug(node, PointHelper::fontSize).asInt();
	
	// Copy render properties
	//
	this->fill = MPlug(node, PointHelper::fill).asBool();
	this->shaded = MPlug(node, PointHelper::shaded).asBool();
	this->drawOnTop = MPlug(node, PointHelper::drawOnTop).asBool();
	this->lineWidth = MPlug(node, PointHelper::lineWidth).asDouble();

	// Copy control points
	//
	MPlug controlPointsPlug = MPlug(node, PointHelper::controlPoints);

	unsigned int numElements = controlPointsPlug.evaluateNumElements();
	this->controlPoints.setLength(numElements);

	MPlug controlPointElement;

	for (unsigned int i = 0; i < numElements; i++)
	{

		controlPointElement = controlPointsPlug.elementByPhysicalIndex(i);
		this->controlPoints[i] = MVector(controlPointElement.child(0).asDouble(), controlPointElement.child(1).asDouble(), controlPointElement.child(2).asDouble());

	}

	return MS::kSuccess;

};


MStatus PointHelperData::copyObjectMatrix(const MDagPath& dagPath)
/**
Copies the object-matrix from the supplied dag path.

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

	// Get object matrix
	//
	MObject node = dagPath.node(&status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	MPlug positionPlug = MPlug(node, PointHelper::localPosition);
	MPlug rotatePlug = MPlug(node, PointHelper::localRotate);
	MPlug scalePlug = MPlug(node, PointHelper::localScale);
	MPlug sizePlug = MPlug(node, PointHelper::size);

	this->localPosition = MVector(positionPlug.child(0).asDouble(), positionPlug.child(1).asDouble(), positionPlug.child(2).asDouble());
	this->localRotate = MVector(rotatePlug.child(0).asDouble(), rotatePlug.child(1).asDouble(), rotatePlug.child(2).asDouble());
	this->localScale = MVector(scalePlug.child(0).asDouble(), scalePlug.child(1).asDouble(), scalePlug.child(2).asDouble());
	this->size = sizePlug.asDouble();

	MMatrix positionMatrix = Drawable::createPositionMatrix(this->localPosition);
	MMatrix rotateMatrix = Drawable::createRotationMatrix(this->localRotate);
	MMatrix scaleMatrix = Drawable::createScaleMatrix(this->localScale);
	MMatrix sizeMatrix = Drawable::createScaleMatrix(this->size);

	this->objectMatrix = sizeMatrix * scaleMatrix * rotateMatrix * positionMatrix;

	return status;

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
