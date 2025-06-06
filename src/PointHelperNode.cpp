//
// File: PointHelperNode.cpp
//
// Author: Ben Singleton
//

#include "PointHelperNode.h"


MObject	PointHelper::localRotate;
MObject	PointHelper::localRotateX;
MObject	PointHelper::localRotateY;
MObject	PointHelper::localRotateZ;

MObject	PointHelper::centerMarker;
MObject	PointHelper::axisTripod;
MObject	PointHelper::axisView;
MObject	PointHelper::cross;
MObject	PointHelper::square;
MObject	PointHelper::box;
MObject	PointHelper::triangle;
MObject	PointHelper::pyramid;
MObject	PointHelper::diamond;
MObject	PointHelper::disc;
MObject	PointHelper::arrow;
MObject	PointHelper::notch;
MObject	PointHelper::tearDrop;
MObject	PointHelper::cylinder;
MObject	PointHelper::sphere;

MObject	PointHelper::size;
MObject	PointHelper::choice;
MObject	PointHelper::text;
MObject	PointHelper::fontSize;
MObject	PointHelper::custom;
MObject	PointHelper::controlPoints;
MObject	PointHelper::xValue;
MObject	PointHelper::yValue;
MObject	PointHelper::zValue;
MObject	PointHelper::lineWidth;
MObject	PointHelper::drawOnTop;
MObject	PointHelper::fill;
MObject	PointHelper::shaded;

MObject	PointHelper::objectMatrix;
MObject	PointHelper::objectInverseMatrix;
MObject	PointHelper::objectWorldMatrix;
MObject	PointHelper::objectWorldInverseMatrix;

MString	PointHelper::localPositionCategory("LocalPosition");
MString	PointHelper::localRotationCategory("LocalRotation");
MString	PointHelper::localScaleCategory("LocalScale");
MString	PointHelper::drawableCategory("Drawable");
MString	PointHelper::textCategory("Text");
MString	PointHelper::customCategory("Custom");
MString	PointHelper::renderCategory("Render");

MString	PointHelper::drawDbClassification("drawdb/geometry/PointHelper");
MString	PointHelper::drawRegistrantId("PointHelperPlugin");
MTypeId PointHelper::id(0x0013b1c0);


PointHelper::PointHelper() {};
PointHelper::~PointHelper() {};


MStatus PointHelper::compute(const MPlug& plug, MDataBlock& data)
/**
This method should be overridden in user defined nodes.
Recompute the given output based on the nodes inputs. 
The plug represents the data value that needs to be recomputed, and the data block holds the storage for all of the node's attributes.
The MDataBlock will provide smart handles for reading and writing this node's attribute values. 
Only these values should be used when performing computations!

@param plug: Plug representing the attribute that needs to be recomputed.
@param data: Data block containing storage for the node's attributes.
@return: Return status.
*/
{

	MStatus status;

	// Check requested attribute
	//
	if (plug == PointHelper::objectMatrix || plug == PointHelper::objectInverseMatrix)
	{

		// Get data handles
		//
		MDataHandle localPositionHandle = data.inputValue(PointHelper::localPosition, &status);
		MDataHandle localPositionXHandle = localPositionHandle.child(PointHelper::localPositionX);
		MDataHandle localPositionYHandle = localPositionHandle.child(PointHelper::localPositionY);
		MDataHandle localPositionZHandle = localPositionHandle.child(PointHelper::localPositionZ);

		MDataHandle localRotateHandle = data.inputValue(PointHelper::localRotate, &status);
		MDataHandle localRotateXHandle = localPositionHandle.child(PointHelper::localRotateX);
		MDataHandle localRotateYHandle = localPositionHandle.child(PointHelper::localRotateY);
		MDataHandle localRotateZHandle = localPositionHandle.child(PointHelper::localRotateZ);

		MDataHandle localScaleHandle = data.inputValue(PointHelper::localScale, &status);
		MDataHandle localScaleXHandle = localScaleHandle.child(PointHelper::localScaleX);
		MDataHandle localScaleYHandle = localScaleHandle.child(PointHelper::localScaleY);
		MDataHandle localScaleZHandle = localScaleHandle.child(PointHelper::localScaleZ);

		MDataHandle sizeHandle = data.inputValue(PointHelper::size, &status);

		// Get values from handles
		//
		MVector localPosition = MVector(localPositionXHandle.asDouble(), localPositionYHandle.asDouble(), localPositionZHandle.asDouble());
		MVector localRotate = MVector(localRotateXHandle.asAngle().asRadians(), localRotateYHandle.asAngle().asRadians(), localRotateZHandle.asAngle().asRadians());
		MVector localScale = MVector(localScaleXHandle.asDouble(), localScaleYHandle.asDouble(), localScaleZHandle.asDouble());
		double size = sizeHandle.asDouble();

		MMatrix localPositionMatrix = Drawable::createPositionMatrix(localPosition);
		MMatrix localRotateMatrix = Drawable::createRotationMatrix(localRotate);
		MMatrix localScaleMatrix = Drawable::createScaleMatrix(localScale);
		MMatrix sizeMatrix = Drawable::createScaleMatrix(size);

		MMatrix objectMatrix = sizeMatrix * localScaleMatrix * localRotateMatrix * localPositionMatrix;
		MMatrix objectInverseMatrix = objectMatrix.inverse();

		// Get output data handles
		//
		MDataHandle objectMatrixHandle = data.outputValue(PointHelper::objectMatrix, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle objectInverseMatrixHandle = data.outputValue(PointHelper::objectInverseMatrix, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		// Update data handle values
		//
		objectMatrixHandle.setMMatrix(objectMatrix);
		objectMatrixHandle.setClean();

		objectInverseMatrixHandle.setMMatrix(objectInverseMatrix);
		objectInverseMatrixHandle.setClean();

		// Mark plug as clean
		//
		status = data.setClean(plug);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		return MS::kSuccess;

	}
	else if (plug == PointHelper::objectWorldMatrix || plug == PointHelper::objectWorldInverseMatrix)
	{

		// Get object matrix
		//
		MDataHandle objectMatrixHandle = data.inputValue(PointHelper::objectMatrix, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MMatrix objectMatrix = objectMatrixHandle.asMatrix();

		// Get all dag paths to this node
		//
		MDagPathArray dagPaths;

		status = MDagPath::getAllPathsTo(this->thisMObject(), dagPaths);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		unsigned int numPaths = dagPaths.length();

		// Define data builders
		//
		MArrayDataHandle objectWorldMatrixArrayHandle = data.outputArrayValue(PointHelper::objectWorldMatrix, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MArrayDataBuilder objectWorldMatrixBuilder = objectWorldMatrixArrayHandle.builder(&status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MArrayDataHandle objectWorldInverseMatrixArrayHandle = data.outputArrayValue(PointHelper::objectWorldInverseMatrix, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MArrayDataBuilder objectWorldInverseMatrixBuilder = objectWorldInverseMatrixArrayHandle.builder(&status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle objectWorldMatrixHandle, objectWorldInverseMatrixHandle;
		MDagPath dagPath;
		MMatrix inclusiveMatrix, objectWorldMatrix, objectWorldInverseMatrix;

		for (unsigned int i = 0u; i < numPaths; i++) 
		{

			// Create new data handles for elements
			//
			objectWorldMatrixHandle = objectWorldMatrixBuilder.addElement(i, &status);
			CHECK_MSTATUS_AND_RETURN_IT(status);

			objectWorldInverseMatrixHandle = objectWorldInverseMatrixBuilder.addElement(i, &status);
			CHECK_MSTATUS_AND_RETURN_IT(status);

			// Get inclusive matrix to dag path
			//
			dagPath = dagPaths[i];
			inclusiveMatrix = dagPath.inclusiveMatrix();

			objectWorldMatrix = objectMatrix * inclusiveMatrix;
			objectWorldInverseMatrix = objectWorldMatrix.inverse();

			// Assign matrices to elements
			//
			objectWorldMatrixHandle.setMMatrix(objectWorldMatrix);
			objectWorldMatrixHandle.setClean();

			objectWorldInverseMatrixHandle.setMMatrix(objectWorldInverseMatrix);
			objectWorldInverseMatrixHandle.setClean();

		}

		// Assign builders to data handles
		//
		objectWorldMatrixArrayHandle.set(objectWorldMatrixBuilder);
		objectWorldMatrixArrayHandle.setAllClean();

		objectWorldInverseMatrixArrayHandle.set(objectWorldInverseMatrixBuilder);
		objectWorldInverseMatrixArrayHandle.setAllClean();

		// Mark data block as clean
		//
		status = data.setClean(plug);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		return MS::kSuccess;

	}
	else
	{
		
		return MS::kUnknownParameter;

	}

};


MStatus PointHelper::preEvaluation(const MDGContext& context, const MEvaluationNode& evaluationNode)
/**
Prepare a node's internal state for threaded evaluation.
During the evaluation graph execution each node gets a chance to reset its internal states just before being evaluated.
This code has to be thread safe, non-blocking and work only on data owned by the node.
The timing of this callback is at the discretion of evaluation graph dependencies and individual evaluators. This means, it should be used purely to prepare this node for evaluation and no particular order should be assumed.
This call will most likely happen from a worker thread.
When using Evaluation Caching or VP2 Custom Caching, preEvaluation() is called as part of the evaluation process. This function is not called as part of the cache restore process because no evaluation takes place in that case.

@param context: Context in which the evaluation is happening. This should be respected and only internal state information pertaining to it should be modified.
@param evaluationNode: Evaluation node which contains information about the dirty plugs that are about to be evaluated for the context. Should be only used to query information.
@return: Return status.
*/
{

	MStatus status;

	if (context.isNormal())
	{

		if (evaluationNode.dirtyPlugExists(PointHelper::size, &status) && status || evaluationNode.dirtyPlugExists(PointHelper::localRotate, &status) && status)
		{

			MHWRender::MRenderer::setGeometryDrawDirty(this->thisMObject(), true);

		}

	}

	return MPxLocatorNode::preEvaluation(context, evaluationNode);

};


void PointHelper::getCacheSetup(const MEvaluationNode& evaluationNode, MNodeCacheDisablingInfo& disablingInfo, MNodeCacheSetupInfo& cacheSetupInfo, MObjectArray& monitoredAttributes) const
/**
Provide node-specific setup info for the Cached Playback system.

@param evaluationNode: This node's evaluation node, contains animated plug information.
@param disablingInfo: Information about why the node disables Cached Playback to be reported to the user.
@param cacheSetupInfo: Preferences and requirements this node has for Cached Playback.
@param monitoredAttributes: Attributes impacting the behavior of this method that will be monitored for change.
@return: Void.
*/
{

	// Call parent function
	//
	MPxLocatorNode::getCacheSetup(evaluationNode, disablingInfo, cacheSetupInfo, monitoredAttributes);
	assert(!disablingInfo.getCacheDisabled());

	// Update caching preference
	//
	cacheSetupInfo.setPreference(MNodeCacheSetupInfo::kWantToCacheByDefault, true);

};


void PointHelper::copyInternalData(PointHelperData* data)
/**
Copies the data from this node to the supplied data object.

@param node: The data to copy to
@return: Void.
*/
{

	MObject node = this->thisMObject();

	data->localPosition = PointHelper::getVectorData(MPlug(node, PointHelper::localPosition));
	data->localRotate = PointHelper::getVectorData(MPlug(node, PointHelper::localRotate));
	data->localScale = PointHelper::getVectorData(MPlug(node, PointHelper::localScale));
	data->size = PointHelper::getDoubleData(MPlug(node, PointHelper::size));

	data->drawables = this->drawables();
	data->controlPoints = PointHelper::getVectorArrayData(MPlug(node, PointHelper::controlPoints));
	data->lineWidth = PointHelper::getFloatData(MPlug(node, PointHelper::lineWidth));
	data->fill = PointHelper::getBoolData(MPlug(node, PointHelper::fill));
	data->shaded = PointHelper::getBoolData(MPlug(node, PointHelper::shaded));
	data->drawOnTop = PointHelper::getBoolData(MPlug(node, PointHelper::drawOnTop));

	data->texts = PointHelper::getStringArrayData(MPlug(node, PointHelper::text));
	data->choice = PointHelper::getIntData(MPlug(node, PointHelper::choice));
	data->text = (0 <= data->choice && data->choice < data->texts.length()) ? data->texts[data->choice] : MString("");
	data->fontSize = PointHelper::getIntData(MPlug(node, PointHelper::fontSize));

	data->invalidate();

};


MMatrix PointHelper::getMatrixData(const MPlug& plug)
/**
Returns the matrix value from the supplied plug.

@param plug: The plug to access.
@return: The matrix value
*/
{

	return PointHelper::getMatrixData(plug.asMObject());

};


MMatrix PointHelper::getMatrixData(const MObject& data)
/**
Returns the matrix value from the supplied data object.

@param data: The matrix data object.
@return: The matrix value.
*/
{
	
	MStatus status;

	MMatrix matrix = MMatrix::identity;

	MFnMatrixData fnMatrixData(data, &status);
	CHECK_MSTATUS_AND_RETURN(status, matrix);

	matrix = fnMatrixData.matrix(&status);
	CHECK_MSTATUS_AND_RETURN(status, matrix);

	return matrix;

};


bool PointHelper::getBoolData(const MPlug& plug)
/**
Returns the bool value from the supplied plug.

@param plug: The plug to access.
@return: The boolean value.
*/
{

	return plug.asBool();

};


int PointHelper::getIntData(const MPlug& plug)
/**
Returns the int value from the supplied plug.

@param plug: The plug to access.
@return: The int value.
*/
{

	return plug.asInt();

};


float PointHelper::getFloatData(const MPlug& plug)
/**
Returns the float value from the supplied plug.

@param plug: The plug to access.
@return: The int value.
*/
{

	return plug.asFloat();

};


double PointHelper::getDoubleData(const MPlug& plug)
/**
Returns the double value from the supplied plug.

@param plug: The plug to access.
@return: The double value.
*/
{

	return plug.asDouble();

};


MVector PointHelper::getVectorData(const MPlug& plug)
/**
Returns the vector value from the supplied plug.

@param plug: The plug to access.
@return: The vector value.
*/
{

	return MVector(plug.child(0).asDouble(), plug.child(1).asDouble(), plug.child(2).asDouble());

};


MVectorArray PointHelper::getVectorArrayData(const MPlug& plug)
/**
Returns the vector array from the supplied plug.

@param plug: The plug to access.
@return: The vector array.
*/
{

	unsigned int numElements = plug.numElements();
	MVectorArray vectors = MVectorArray(numElements, MVector::zero);

	for (unsigned int i = 0; i < numElements; i++)
	{

		vectors[i] = PointHelper::getVectorData(plug.elementByPhysicalIndex(i));

	}

	return vectors;

};


MString PointHelper::getStringData(const MPlug& plug)
/**
Returns the string value from the supplied plug.

@param plug: The plug to access.
@return: The string value.
*/
{

	return plug.asString();

};


MStringArray PointHelper::getStringArrayData(const MPlug& plug)
/**
Returns the string array from the supplied plug.

@param plug: The plug to access.
@return: The string array.
*/
{

	unsigned int numElements = plug.numElements();
	MStringArray strings = MStringArray(numElements, MString(""));

	for (unsigned int i = 0; i < numElements; i++)
	{

		strings[i] = PointHelper::getStringData(plug.elementByPhysicalIndex(i));

	}

	return strings;

};


MDagPath PointHelper::thisMDagPath() const
/**
Returns a dag path to this node.

@return: Dag path to this node.
*/
{

	return MDagPath::getAPathTo(this->thisMObject());

};


std::map<std::string, bool> PointHelper::drawables()
/**
Returns the drawable flags from this node.

@return: The drawable flags.
*/
{

	MObject node = this->thisMObject();

	return std::map<std::string, bool> {
		{ "centerMarker", PointHelper::getBoolData(MPlug(node, PointHelper::centerMarker)) },
		{ "axisTripod", PointHelper::getBoolData(MPlug(node, PointHelper::axisTripod)) },
		{ "axisView", PointHelper::getBoolData(MPlug(node, PointHelper::axisView)) },
		{ "cross", PointHelper::getBoolData(MPlug(node, PointHelper::cross)) },
		{ "square", PointHelper::getBoolData(MPlug(node, PointHelper::square)) },
		{ "box", PointHelper::getBoolData(MPlug(node, PointHelper::box)) },
		{ "triangle", PointHelper::getBoolData(MPlug(node, PointHelper::triangle)) },
		{ "pyramid", PointHelper::getBoolData(MPlug(node, PointHelper::pyramid)) },
		{ "diamond", PointHelper::getBoolData(MPlug(node, PointHelper::diamond)) },
		{ "disc", PointHelper::getBoolData(MPlug(node, PointHelper::disc)) },
		{ "arrow", PointHelper::getBoolData(MPlug(node, PointHelper::arrow)) },
		{ "notch", PointHelper::getBoolData(MPlug(node, PointHelper::notch)) },
		{ "tearDrop", PointHelper::getBoolData(MPlug(node, PointHelper::tearDrop)) },
		{ "cylinder", PointHelper::getBoolData(MPlug(node, PointHelper::cylinder)) },
		{ "sphere", PointHelper::getBoolData(MPlug(node, PointHelper::sphere)) },
		{ "custom", PointHelper::getBoolData(MPlug(node, PointHelper::custom)) }
	};

};


bool PointHelper::isBounded() const
/**
This function indicates if the bounding method will be overrided by the user.
Supplying a bounding box will make selection calculation more efficient!

@return: bool
*/
{

	return true;

};


MBoundingBox PointHelper::boundingBox() const
/**
This function is used to calculate a bounding box based on the object transform.
Supplying a bounding box will make selection calculation more efficient!

@return: MBoundingBox
*/
{
	
	MStatus status;

	// Initialize default bounding-box
	//
	MBoundingBox boundingBox = MBoundingBox(MPoint(-1.0, -1.0, -1.0), MPoint(1.0, 1.0, 1.0));

	// Append any control points
	//
	MVectorArray controlPoints = PointHelper::getVectorArrayData(MPlug(this->thisMObject(), PointHelper::controlPoints));
	unsigned int count = controlPoints.length();

	for (unsigned int i = 0; i < count; i++)
	{

		boundingBox.expand(controlPoints[i]);

	}

	// Transform bounding-box with object-matrix
	//
	MVector localPosition = PointHelper::getVectorData(MPlug(this->thisMObject(), PointHelper::localPosition));
	MVector localRotate = PointHelper::getVectorData(MPlug(this->thisMObject(), PointHelper::localRotate));
	MVector localScale = PointHelper::getVectorData(MPlug(this->thisMObject(), PointHelper::localScale));
	double size = PointHelper::getDoubleData(MPlug(this->thisMObject(), PointHelper::size));

	MMatrix localPositionmatrix = Drawable::createPositionMatrix(localPosition);
	MMatrix localRotateMatrix = Drawable::createRotationMatrix(localRotate);
	MMatrix localScaleMatrix = Drawable::createScaleMatrix(localScale);
	MMatrix sizeMatrix = Drawable::createScaleMatrix(size);
	MMatrix objectMatrix = sizeMatrix * localScaleMatrix * localRotateMatrix * localPositionmatrix;

	boundingBox.transformUsing(objectMatrix);

	return boundingBox;

};


void* PointHelper::creator()
/**
This function is called by Maya when a new instance is requested.
See pluginMain.cpp for details.

@return: PointHelper
*/
{

	return new PointHelper();

};


MStatus PointHelper::initialize()
/**
This function is called by Maya after a plugin has been loaded.
Use this function to define any static attributes.

@return: MStatus
*/
{

	MStatus status;

	// Declare attribute function sets
	//
	MFnNumericAttribute fnNumericAttr;
	MFnUnitAttribute fnUnitAttr;
	MFnCompoundAttribute fnCompoundAttr;
	MFnMatrixAttribute fnMatrixAttr;
	MFnTypedAttribute fnTypedAttr;
	
	// Input attributes:
	// Edit ".localPositionX` attribute
	//
	status = fnUnitAttr.setObject(PointHelper::localPositionX);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.addToCategory(PointHelper::localPositionCategory));

	// Edit ".localPositionY` attribute
	//
	status = fnUnitAttr.setObject(PointHelper::localPositionY);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.addToCategory(PointHelper::localPositionCategory));

	// Edit ".localPositionZ` attribute
	//
	status = fnUnitAttr.setObject(PointHelper::localPositionZ);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.addToCategory(PointHelper::localPositionCategory));

	// Edit ".localPosition` attribute
	//
	status = fnNumericAttr.setObject(PointHelper::localPosition);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::localPositionCategory));
	
	// Initialize `localRotateX` attribute
	//
	PointHelper::localRotateX = fnUnitAttr.create("localRotateX", "lorx", MFnUnitAttribute::kAngle, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setChannelBox(true));
	CHECK_MSTATUS(fnUnitAttr.addToCategory(PointHelper::localRotationCategory));

	// Initialize `localRotateY` attribute
	//
	PointHelper::localRotateY = fnUnitAttr.create("localRotateY", "lory", MFnUnitAttribute::kAngle, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setChannelBox(true));
	CHECK_MSTATUS(fnUnitAttr.addToCategory(PointHelper::localRotationCategory));

	// Initialize `localRotateZ` attribute
	//
	PointHelper::localRotateZ = fnUnitAttr.create("localRotateZ", "lorz", MFnUnitAttribute::kAngle, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setChannelBox(true));
	CHECK_MSTATUS(fnUnitAttr.addToCategory(PointHelper::localRotationCategory));

	// Initialize `localRotate"
	//
	PointHelper::localRotate = fnNumericAttr.create("localRotate", "lor", PointHelper::localRotateX, PointHelper::localRotateY, PointHelper::localRotateZ, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::localRotationCategory));

	// Edit ".localScaleX` attribute
	//
	status = fnUnitAttr.setObject(PointHelper::localScaleX);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.addToCategory(PointHelper::localScaleCategory));

	// Edit ".localScaleY` attribute
	//
	status = fnUnitAttr.setObject(PointHelper::localScaleY);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.addToCategory(PointHelper::localScaleCategory));

	// Edit ".localScaleZ` attribute
	//
	status = fnUnitAttr.setObject(PointHelper::localScaleZ);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.addToCategory(PointHelper::localScaleCategory));

	// Edit ".localScale` attribute
	//
	status = fnNumericAttr.setObject(PointHelper::localScale);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::localScaleCategory));

	// Initialize `size` attribute
	//
	PointHelper::size = fnNumericAttr.create("size", "size", MFnNumericData::kDouble, 10.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setChannelBox(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::renderCategory));

	// Initialize `centerMarker` attribute
	//
	PointHelper::centerMarker = fnNumericAttr.create("centerMarker", "centerMarker", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Initialize `axisTripod` attribute
	//
	PointHelper::axisTripod = fnNumericAttr.create("axisTripod", "axisTripod", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Initialize `axisView` attribute
	//
	PointHelper::axisView = fnNumericAttr.create("axisView", "axisView", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Initialize `cross` attribute
	//
	PointHelper::cross = fnNumericAttr.create("cross", "cross", MFnNumericData::kBoolean, true, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Initialize `square` attribute
	//
	PointHelper::square = fnNumericAttr.create("square", "square", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Initialize `box` attribute
	//
	PointHelper::box = fnNumericAttr.create("box", "box", MFnNumericData::kBoolean, true, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Initialize `triangle` attribute
	//
	PointHelper::triangle = fnNumericAttr.create("triangle", "triangle", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Initialize `pyramid` attribute
	//
	PointHelper::pyramid = fnNumericAttr.create("pyramid", "pyramid", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Initialize `diamond` attribute
	//
	PointHelper::diamond = fnNumericAttr.create("diamond", "diamond", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Initialize `circle` attribute
	//
	PointHelper::disc = fnNumericAttr.create("disc", "disc", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Initialize `arrow` attribute
	//
	PointHelper::arrow = fnNumericAttr.create("arrow", "arrow", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Initialize `notch` attribute
	//
	PointHelper::notch = fnNumericAttr.create("notch", "notch", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Initialize `tearDrop` attribute
	//
	PointHelper::tearDrop = fnNumericAttr.create("tearDrop", "tearDrop", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Initialize `cylinder` attribute
	//
	PointHelper::cylinder = fnNumericAttr.create("cylinder", "cylinder", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Initialize `sphere` attribute
	//
	PointHelper::sphere = fnNumericAttr.create("sphere", "sphere", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Initialize `choice` attribute
	//
	PointHelper::choice = fnNumericAttr.create("choice", "choice", MFnNumericData::kInt, 0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::textCategory));

	// Initialize `text` attribute
	//
	PointHelper::text = fnTypedAttr.create("text", "txt", MFnData::kString, MObject::kNullObj, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnTypedAttr.setArray(true));
	CHECK_MSTATUS(fnTypedAttr.addToCategory(PointHelper::textCategory));

	// Initialize `fontSize` attribute
	//
	PointHelper::fontSize = fnNumericAttr.create("fontSize", "fontSize", MFnNumericData::kInt, 11, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setMin(1));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::textCategory));

	// Initialize `custom` attribute
	//
	PointHelper::custom = fnNumericAttr.create("custom", "custom", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// ".xValue` attribute
	//
	PointHelper::xValue = fnNumericAttr.create("xValue", "xValue", MFnNumericData::kDouble, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::customCategory));

	// ".yValue` attribute
	//
	PointHelper::yValue = fnNumericAttr.create("yValue", "yValue", MFnNumericData::kDouble, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::customCategory));

	// ".zValue` attribute
	//
	PointHelper::zValue = fnNumericAttr.create("zValue", "zValue", MFnNumericData::kDouble, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::customCategory));

	// ".controlPoints` attribute
	//
	PointHelper::controlPoints = fnNumericAttr.create("controlPoints", "controlPoints", PointHelper::xValue, PointHelper::yValue, PointHelper::zValue, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	
	CHECK_MSTATUS(fnNumericAttr.setArray(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::customCategory));

	// Initialize `lineWidth` attribute
	//
	PointHelper::lineWidth = fnNumericAttr.create("lineWidth", "lw", MFnNumericData::kFloat, 1.0f, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setMin(0.0));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::renderCategory));

	// Initialize `fill` attribute
	//
	PointHelper::fill = fnNumericAttr.create("fill", "fill", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::renderCategory));

	// Initialize `shaded` attribute
	//
	PointHelper::shaded = fnNumericAttr.create("shaded", "shaded", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::renderCategory));

	// Initialize `drawOnTop` attribute
	//
	PointHelper::drawOnTop = fnNumericAttr.create("drawOnTop", "drawOnTop", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::renderCategory));

	// Output attributes:
	// Initialize `objectMatrix` attribute
	//
	PointHelper::objectMatrix = fnMatrixAttr.create("objectMatrix", "om", MFnMatrixAttribute::kDouble, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnMatrixAttr.setWritable(false));
	CHECK_MSTATUS(fnMatrixAttr.setStorable(false));

	// Initialize `objectInverseMatrix` attribute
	//
	PointHelper::objectInverseMatrix = fnMatrixAttr.create("objectInverseMatrix", "oim", MFnMatrixAttribute::kDouble, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnMatrixAttr.setWritable(false));
	CHECK_MSTATUS(fnMatrixAttr.setStorable(false));

	// Initialize `objectWorldMatrix` attribute
	//
	PointHelper::objectWorldMatrix = fnMatrixAttr.create("objectWorldMatrix", "owm", MFnMatrixAttribute::kDouble, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnMatrixAttr.setWritable(false));
	CHECK_MSTATUS(fnMatrixAttr.setStorable(false));
	CHECK_MSTATUS(fnMatrixAttr.setArray(true));
	CHECK_MSTATUS(fnMatrixAttr.setUsesArrayDataBuilder(true));
	CHECK_MSTATUS(fnMatrixAttr.setWorldSpace(true));

	// Initialize `objectWorldInverseMatrix` attribute
	//
	PointHelper::objectWorldInverseMatrix = fnMatrixAttr.create("objectWorldInverseMatrix", "owim", MFnMatrixAttribute::kDouble, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnMatrixAttr.setWritable(false));
	CHECK_MSTATUS(fnMatrixAttr.setStorable(false));
	CHECK_MSTATUS(fnMatrixAttr.setArray(true));
	CHECK_MSTATUS(fnMatrixAttr.setUsesArrayDataBuilder(true));
	CHECK_MSTATUS(fnMatrixAttr.setWorldSpace(true));

	// Add attributes
	//
	CHECK_MSTATUS(PointHelper::addAttribute(PointHelper::localRotate));
	
	CHECK_MSTATUS(PointHelper::addAttribute(PointHelper::centerMarker));
	CHECK_MSTATUS(PointHelper::addAttribute(PointHelper::axisTripod));
	CHECK_MSTATUS(PointHelper::addAttribute(PointHelper::axisView));
	CHECK_MSTATUS(PointHelper::addAttribute(PointHelper::cross));
	CHECK_MSTATUS(PointHelper::addAttribute(PointHelper::square));
	CHECK_MSTATUS(PointHelper::addAttribute(PointHelper::box));
	CHECK_MSTATUS(PointHelper::addAttribute(PointHelper::triangle));
	CHECK_MSTATUS(PointHelper::addAttribute(PointHelper::pyramid));
	CHECK_MSTATUS(PointHelper::addAttribute(PointHelper::diamond));
	CHECK_MSTATUS(PointHelper::addAttribute(PointHelper::disc));
	CHECK_MSTATUS(PointHelper::addAttribute(PointHelper::arrow));
	CHECK_MSTATUS(PointHelper::addAttribute(PointHelper::notch));
	CHECK_MSTATUS(PointHelper::addAttribute(PointHelper::tearDrop));
	CHECK_MSTATUS(PointHelper::addAttribute(PointHelper::cylinder));
	CHECK_MSTATUS(PointHelper::addAttribute(PointHelper::sphere));

	CHECK_MSTATUS(PointHelper::addAttribute(PointHelper::size));
	CHECK_MSTATUS(PointHelper::addAttribute(PointHelper::choice));
	CHECK_MSTATUS(PointHelper::addAttribute(PointHelper::text));
	CHECK_MSTATUS(PointHelper::addAttribute(PointHelper::fontSize));
	CHECK_MSTATUS(PointHelper::addAttribute(PointHelper::custom));
	CHECK_MSTATUS(PointHelper::addAttribute(PointHelper::controlPoints));
	CHECK_MSTATUS(PointHelper::addAttribute(PointHelper::lineWidth));
	CHECK_MSTATUS(PointHelper::addAttribute(PointHelper::drawOnTop));
	CHECK_MSTATUS(PointHelper::addAttribute(PointHelper::fill));
	CHECK_MSTATUS(PointHelper::addAttribute(PointHelper::shaded));

	CHECK_MSTATUS(PointHelper::addAttribute(PointHelper::objectMatrix));
	CHECK_MSTATUS(PointHelper::addAttribute(PointHelper::objectInverseMatrix));
	CHECK_MSTATUS(PointHelper::addAttribute(PointHelper::objectWorldMatrix));
	CHECK_MSTATUS(PointHelper::addAttribute(PointHelper::objectWorldInverseMatrix));

	// Define attribute relationships
	//
	CHECK_MSTATUS(PointHelper::attributeAffects(PointHelper::size, PointHelper::objectMatrix));
	CHECK_MSTATUS(PointHelper::attributeAffects(PointHelper::size, PointHelper::objectInverseMatrix));
	CHECK_MSTATUS(PointHelper::attributeAffects(PointHelper::size, PointHelper::objectWorldMatrix));
	CHECK_MSTATUS(PointHelper::attributeAffects(PointHelper::size, PointHelper::objectWorldInverseMatrix));

	CHECK_MSTATUS(PointHelper::attributeAffects(PointHelper::localPosition, PointHelper::objectMatrix));
	CHECK_MSTATUS(PointHelper::attributeAffects(PointHelper::localPosition, PointHelper::objectInverseMatrix));
	CHECK_MSTATUS(PointHelper::attributeAffects(PointHelper::localPosition, PointHelper::objectWorldMatrix));
	CHECK_MSTATUS(PointHelper::attributeAffects(PointHelper::localPosition, PointHelper::objectWorldInverseMatrix));

	CHECK_MSTATUS(PointHelper::attributeAffects(PointHelper::localRotate, PointHelper::objectMatrix));
	CHECK_MSTATUS(PointHelper::attributeAffects(PointHelper::localRotate, PointHelper::objectInverseMatrix));
	CHECK_MSTATUS(PointHelper::attributeAffects(PointHelper::localRotate, PointHelper::objectWorldMatrix));
	CHECK_MSTATUS(PointHelper::attributeAffects(PointHelper::localRotate, PointHelper::objectWorldInverseMatrix));

	CHECK_MSTATUS(PointHelper::attributeAffects(PointHelper::localScale, PointHelper::objectMatrix));
	CHECK_MSTATUS(PointHelper::attributeAffects(PointHelper::localScale, PointHelper::objectInverseMatrix));
	CHECK_MSTATUS(PointHelper::attributeAffects(PointHelper::localScale, PointHelper::objectWorldMatrix));
	CHECK_MSTATUS(PointHelper::attributeAffects(PointHelper::localScale, PointHelper::objectWorldInverseMatrix));

	CHECK_MSTATUS(PointHelper::attributeAffects(PointHelper::worldMatrix, PointHelper::objectWorldMatrix));
	CHECK_MSTATUS(PointHelper::attributeAffects(PointHelper::worldInverseMatrix, PointHelper::objectWorldInverseMatrix));

	return MS::kSuccess;

};