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
MObject	PointHelper::cross;
MObject	PointHelper::square;
MObject	PointHelper::box;
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

double PointHelper::DEFAULT_SIZE(10.0);
int PointHelper::DEFAULT_FONT_SIZE(11);
double PointHelper::DEFAULT_LINE_WIDTH(1.0);

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


PointHelper::PointHelper()
/**
Constructor.
*/
{

	this->data = new PointHelperData();

};


PointHelper::~PointHelper()
/**
Destructor.
*/
{

	this->data = nullptr;

};


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

		// Get input data handles
		//
		MDataHandle localPositionHandle = data.inputValue(PointHelper::localPosition, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle localRotateHandle = data.inputValue(PointHelper::localRotate, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle localScaleHandle = data.inputValue(PointHelper::localScale, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle sizeHandle = data.inputValue(PointHelper::size, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		// Get values from handles
		//
		MVector position = localPositionHandle.asVector();
		double3 &rotation = localRotateHandle.asDouble3();
		double3 &scale = localScaleHandle.asDouble3();
		
		// Adjust scale based on size
		//
		double size = sizeHandle.asDouble();
		double offsetScale[3] = { scale[0] * size, scale[1] * size, scale[2] * size };

		// Define transform matrix
		//
		MTransformationMatrix transform = MTransformationMatrix();

		status = transform.setTranslation(position, MSpace::kTransform);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		status = transform.setRotation(rotation, MTransformationMatrix::RotationOrder::kXYZ);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		status = transform.setScale(offsetScale, MSpace::kTransform);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		// Compose object matrix
		//
		MMatrix objectMatrix = transform.asMatrix();
		MMatrix objectInverseMatrix = transform.asMatrixInverse();

		// Get output data handles
		//
		MDataHandle objectMatrixHandle = data.outputValue(PointHelper::objectMatrix, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle objectInverseMatrixHandle = data.outputValue(PointHelper::objectInverseMatrix, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		// Commit values to data handle
		//
		objectMatrixHandle.setMMatrix(objectMatrix);
		objectInverseMatrixHandle.setMMatrix(objectInverseMatrix);

		// Mark handles as clean
		//
		objectMatrixHandle.setClean();
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

		for (unsigned int i = 0; i < numPaths; i++) 
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

		if (evaluationNode.dirtyPlugExists(PointHelper::size, &status) && status || 
			evaluationNode.dirtyPlugExists(PointHelper::localRotate, &status) && status ||
			evaluationNode.dirtyPlugExists(PointHelper::localRotateX, &status) && status || 
			evaluationNode.dirtyPlugExists(PointHelper::localRotateY, &status) && status || 
			evaluationNode.dirtyPlugExists(PointHelper::localRotateZ, &status) && status)
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


bool PointHelper::setInternalValue(const MPlug& plug, const MDataHandle& handle)
/**
This method is overridden by nodes that store attribute data in some internal format.
The internal state of attributes can be set or queried using the setInternal and internal methods of MFnAttribute.
When internal attribute values are set via setAttr or MPlug::setValue this method is called.
Another use for this method is to impose attribute limits.

@param plug: The attribute that is being set.
@param handle: The dataHandle containing the value to set.
@return: 
*/
{

	MStatus status;
	
	// Inspect attribute
	//
	MObject attribute = plug.attribute(&status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	MFnAttribute fnAttribute(attribute, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	bool isLocalPosition = fnAttribute.hasCategory(PointHelper::localPositionCategory);
	bool isLocalRotation = fnAttribute.hasCategory(PointHelper::localRotationCategory);
	bool isLocalScale = fnAttribute.hasCategory(PointHelper::localScaleCategory);
	bool isDrawable = fnAttribute.hasCategory(PointHelper::drawableCategory);
	bool isText = fnAttribute.hasCategory(PointHelper::textCategory);
	bool isCustom = fnAttribute.hasCategory(PointHelper::customCategory);
	bool isRender = fnAttribute.hasCategory(PointHelper::renderCategory);
	
	if (isLocalPosition)
	{

		if (attribute == PointHelper::localPositionX)
		{

			this->data->localPosition.x = handle.asDistance().asCentimeters();

		}
		else if (attribute == PointHelper::localPositionY)
		{

			this->data->localPosition.y = handle.asDistance().asCentimeters();

		}
		else if (attribute == PointHelper::localPositionZ)
		{

			this->data->localPosition.z = handle.asDistance().asCentimeters();

		}
		else;

		this->data->dirtyObjectMatrix();

	}
	else if (isLocalRotation)
	{
		
		if (attribute == PointHelper::localRotateX)
		{
			
			this->data->localRotate.x = handle.asAngle().asRadians();

		}
		else if (attribute == PointHelper::localRotateY)
		{
			
			this->data->localRotate.y = handle.asAngle().asRadians();

		}
		else if (attribute == PointHelper::localRotateZ)
		{
			
			this->data->localRotate.z = handle.asAngle().asRadians();

		}
		else;

		this->data->dirtyObjectMatrix();

	}
	else if (isLocalScale)
	{

		if (attribute == PointHelper::localScaleX)
		{

			this->data->localScale.x = handle.asDistance().asCentimeters();

		}
		else if (attribute == PointHelper::localScaleY)
		{

			this->data->localScale.y = handle.asDistance().asCentimeters();

		}
		else if (attribute == PointHelper::localScaleZ)
		{

			this->data->localScale.z = handle.asDistance().asCentimeters();

		}
		else;

		this->data->dirtyObjectMatrix();

	}
	else if (isDrawable)
	{

		std::string name = fnAttribute.name().asChar();
		this->data->enabled[name] = handle.asBool();

	}
	else if (isText)
	{

		if (attribute == PointHelper::choice)
		{

			this->data->choice = handle.asInt();
			this->data->dirtyCurrentText();

		}
		else if (attribute == PointHelper::text)
		{

			this->data->cacheText(plug.logicalIndex(), handle.asString());
			this->data->dirtyCurrentText();

		}
		else if (attribute == PointHelper::fontSize)
		{

			this->data->fontSize = handle.asDouble();

		}
		else;

	}
	else if (isCustom)
	{
		
		if (attribute == PointHelper::xValue)
		{

			this->data->cacheControlPoint(plug.parent().logicalIndex(), 0, handle.asDouble());

		}
		else if (attribute == PointHelper::yValue)
		{

			this->data->cacheControlPoint(plug.parent().logicalIndex(), 1, handle.asDouble());

		}
		else if (attribute == PointHelper::zValue)
		{

			this->data->cacheControlPoint(plug.parent().logicalIndex(), 2, handle.asDouble());

		}
		else;

	}
	else if (isRender)
	{
		
		if (attribute == PointHelper::size)
		{

			this->data->size = handle.asDouble();
			this->data->dirtyObjectMatrix();

		}
		else if (attribute == PointHelper::lineWidth)
		{

			this->data->lineWidth = handle.asDouble();

		}
		else if (attribute == PointHelper::fill)
		{

			this->data->fill = handle.asBool();

		}
		else if (attribute == PointHelper::shaded)
		{

			this->data->shaded = handle.asBool();

		}
		else if (attribute == PointHelper::drawOnTop)
		{

			this->data->drawOnTop = handle.asBool();

		}
		else;

	}
	else;

	return MPxLocatorNode::setInternalValue(plug, handle);

};


PointHelperData* PointHelper::getUserData()
/**
Returns a pointer to the internal point helper data.

@return: The point helper data pointer.
*/
{

	return this->data;

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

	// Create unit scaled bounding box
	//
	MPoint corner1(-1.0, -1.0, -1.0);
	corner1 *= this->data->objectMatrix;

	MPoint corner2(1.0, 1.0, 1.0);
	corner2 *= this->data->objectMatrix;

	return MBoundingBox(corner1, corner2);

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
	// Edit ".localPositionX" attribute
	//
	status = fnUnitAttr.setObject(PointHelper::localPositionX);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setInternal(true));
	CHECK_MSTATUS(fnUnitAttr.setAffectsWorldSpace(true));
	CHECK_MSTATUS(fnUnitAttr.addToCategory(PointHelper::localPositionCategory));

	// Edit ".localPositionY" attribute
	//
	status = fnUnitAttr.setObject(PointHelper::localPositionY);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setInternal(true));
	CHECK_MSTATUS(fnUnitAttr.setAffectsWorldSpace(true));
	CHECK_MSTATUS(fnUnitAttr.addToCategory(PointHelper::localPositionCategory));

	// Edit ".localPositionZ" attribute
	//
	status = fnUnitAttr.setObject(PointHelper::localPositionZ);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setInternal(true));
	CHECK_MSTATUS(fnUnitAttr.setAffectsWorldSpace(true));
	CHECK_MSTATUS(fnUnitAttr.addToCategory(PointHelper::localPositionCategory));

	// Define ".localRotateX" attribute
	//
	PointHelper::localRotateX = fnUnitAttr.create("localRotateX", "lorx", MFnUnitAttribute::kAngle, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setInternal(true));
	CHECK_MSTATUS(fnUnitAttr.setChannelBox(true));
	CHECK_MSTATUS(fnUnitAttr.setAffectsWorldSpace(true));
	CHECK_MSTATUS(fnUnitAttr.addToCategory(PointHelper::localRotationCategory));

	// Define ".localRotateY" attribute
	//
	PointHelper::localRotateY = fnUnitAttr.create("localRotateY", "lory", MFnUnitAttribute::kAngle, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setInternal(true));
	CHECK_MSTATUS(fnUnitAttr.setChannelBox(true));
	CHECK_MSTATUS(fnUnitAttr.setAffectsWorldSpace(true));
	CHECK_MSTATUS(fnUnitAttr.addToCategory(PointHelper::localRotationCategory));

	// Define ".localRotateZ" attribute
	//
	PointHelper::localRotateZ = fnUnitAttr.create("localRotateZ", "lorz", MFnUnitAttribute::kAngle, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setInternal(true));
	CHECK_MSTATUS(fnUnitAttr.setChannelBox(true));
	CHECK_MSTATUS(fnUnitAttr.setAffectsWorldSpace(true));
	CHECK_MSTATUS(fnUnitAttr.addToCategory(PointHelper::localRotationCategory));

	// Define ".localRotate"
	//
	PointHelper::localRotate = fnNumericAttr.create("localRotate", "lor", PointHelper::localRotateX, PointHelper::localRotateY, PointHelper::localRotateZ, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// Edit ".localScaleX" attribute
	//
	status = fnUnitAttr.setObject(PointHelper::localScaleX);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setInternal(true));
	CHECK_MSTATUS(fnUnitAttr.setAffectsWorldSpace(true));
	CHECK_MSTATUS(fnUnitAttr.addToCategory(PointHelper::localScaleCategory));

	// Edit ".localScaleY" attribute
	//
	status = fnUnitAttr.setObject(PointHelper::localScaleY);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setInternal(true));
	CHECK_MSTATUS(fnUnitAttr.setAffectsWorldSpace(true));
	CHECK_MSTATUS(fnUnitAttr.addToCategory(PointHelper::localScaleCategory));

	// Edit ".localScaleZ" attribute
	//
	status = fnUnitAttr.setObject(PointHelper::localScaleZ);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setInternal(true));
	CHECK_MSTATUS(fnUnitAttr.setAffectsWorldSpace(true));
	CHECK_MSTATUS(fnUnitAttr.addToCategory(PointHelper::localScaleCategory));

	// Define ".size" attribute
	//
	PointHelper::size = fnNumericAttr.create("size", "size", MFnNumericData::kDouble, DEFAULT_SIZE, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.setAffectsWorldSpace(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::renderCategory));

	// Define ".centerMarker" attribute
	//
	PointHelper::centerMarker = fnNumericAttr.create("centerMarker", "centerMarker", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Define ".axisTripod" attribute
	//
	PointHelper::axisTripod = fnNumericAttr.create("axisTripod", "axisTripod", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Define ".cross" attribute
	//
	PointHelper::cross = fnNumericAttr.create("cross", "cross", MFnNumericData::kBoolean, true, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Define ".square" attribute
	//
	PointHelper::square = fnNumericAttr.create("square", "square", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Define ".box" attribute
	//
	PointHelper::box = fnNumericAttr.create("box", "box", MFnNumericData::kBoolean, true, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Define ".diamond" attribute
	//
	PointHelper::diamond = fnNumericAttr.create("diamond", "diamond", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Define ".circle" attribute
	//
	PointHelper::disc = fnNumericAttr.create("disc", "disc", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Define ".arrow" attribute
	//
	PointHelper::arrow = fnNumericAttr.create("arrow", "arrow", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Define ".notch" attribute
	//
	PointHelper::notch = fnNumericAttr.create("notch", "notch", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Define ".tearDrop" attribute
	//
	PointHelper::tearDrop = fnNumericAttr.create("tearDrop", "tearDrop", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Define ".cylinder" attribute
	//
	PointHelper::cylinder = fnNumericAttr.create("cylinder", "cylinder", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Define ".sphere" attribute
	//
	PointHelper::sphere = fnNumericAttr.create("sphere", "sphere", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Define ".choice" attribute
	//
	PointHelper::choice = fnNumericAttr.create("choice", "choice", MFnNumericData::kInt, 0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::textCategory));

	// Define ".text" attribute
	//
	PointHelper::text = fnTypedAttr.create("text", "txt", MFnData::kString, MObject::kNullObj, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnTypedAttr.setInternal(true));
	CHECK_MSTATUS(fnTypedAttr.setArray(true));
	CHECK_MSTATUS(fnTypedAttr.addToCategory(PointHelper::textCategory));

	// Define ".fontSize" attribute
	//
	PointHelper::fontSize = fnNumericAttr.create("fontSize", "fontSize", MFnNumericData::kInt, DEFAULT_FONT_SIZE, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setMin(1));
	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::textCategory));

	// Define ".custom" attribute
	//
	PointHelper::custom = fnNumericAttr.create("custom", "custom", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// ".xValue" attribute
	//
	PointHelper::xValue = fnNumericAttr.create("xValue", "xValue", MFnNumericData::kDouble, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::customCategory));

	// ".yValue" attribute
	//
	PointHelper::yValue = fnNumericAttr.create("yValue", "yValue", MFnNumericData::kDouble, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::customCategory));

	// ".zValue" attribute
	//
	PointHelper::zValue = fnNumericAttr.create("zValue", "zValue", MFnNumericData::kDouble, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::customCategory));

	// ".controlPoints" attribute
	//
	PointHelper::controlPoints = fnNumericAttr.create("controlPoints", "controlPoints", PointHelper::xValue, PointHelper::yValue, PointHelper::zValue, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	
	CHECK_MSTATUS(fnNumericAttr.setArray(true));

	// Define ".lineWidth" attribute
	//
	PointHelper::lineWidth = fnNumericAttr.create("lineWidth", "lw", MFnNumericData::kDouble, DEFAULT_LINE_WIDTH, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setMin(0.0));
	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::renderCategory));

	// Define ".fill" attribute
	//
	PointHelper::fill = fnNumericAttr.create("fill", "fill", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::renderCategory));

	// Define ".shaded" attribute
	//
	PointHelper::shaded = fnNumericAttr.create("shaded", "shaded", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::renderCategory));

	// Define ".drawOnTop" attribute
	//
	PointHelper::drawOnTop = fnNumericAttr.create("drawOnTop", "drawOnTop", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::renderCategory));

	// Output attributes:
	// Define ".objectMatrix" attribute
	//
	PointHelper::objectMatrix = fnMatrixAttr.create("objectMatrix", "om", MFnMatrixAttribute::kDouble, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnMatrixAttr.setWritable(false));
	CHECK_MSTATUS(fnMatrixAttr.setStorable(false));
	CHECK_MSTATUS(fnMatrixAttr.setCached(false));

	// Define ".objectInverseMatrix" attribute
	//
	PointHelper::objectInverseMatrix = fnMatrixAttr.create("objectInverseMatrix", "oim", MFnMatrixAttribute::kDouble, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnMatrixAttr.setWritable(false));
	CHECK_MSTATUS(fnMatrixAttr.setStorable(false));
	CHECK_MSTATUS(fnMatrixAttr.setCached(false));

	// Define ".objectWorldMatrix" attribute
	//
	PointHelper::objectWorldMatrix = fnMatrixAttr.create("objectWorldMatrix", "owm", MFnMatrixAttribute::kDouble, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnMatrixAttr.setWritable(false));
	CHECK_MSTATUS(fnMatrixAttr.setStorable(false));
	CHECK_MSTATUS(fnMatrixAttr.setCached(false));
	CHECK_MSTATUS(fnMatrixAttr.setArray(true));
	CHECK_MSTATUS(fnMatrixAttr.setUsesArrayDataBuilder(true));
	CHECK_MSTATUS(fnMatrixAttr.setWorldSpace(true));

	// Define ".objectWorldInverseMatrix" attribute
	//
	PointHelper::objectWorldInverseMatrix = fnMatrixAttr.create("objectWorldInverseMatrix", "owim", MFnMatrixAttribute::kDouble, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnMatrixAttr.setWritable(false));
	CHECK_MSTATUS(fnMatrixAttr.setStorable(false));
	CHECK_MSTATUS(fnMatrixAttr.setCached(false));
	CHECK_MSTATUS(fnMatrixAttr.setArray(true));
	CHECK_MSTATUS(fnMatrixAttr.setUsesArrayDataBuilder(true));
	CHECK_MSTATUS(fnMatrixAttr.setWorldSpace(true));

	// Add attributes
	//
	CHECK_MSTATUS(PointHelper::addAttribute(PointHelper::localRotate));
	
	CHECK_MSTATUS(PointHelper::addAttribute(PointHelper::centerMarker));
	CHECK_MSTATUS(PointHelper::addAttribute(PointHelper::axisTripod));
	CHECK_MSTATUS(PointHelper::addAttribute(PointHelper::cross));
	CHECK_MSTATUS(PointHelper::addAttribute(PointHelper::square));
	CHECK_MSTATUS(PointHelper::addAttribute(PointHelper::box));
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