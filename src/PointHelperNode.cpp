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


PointHelper::PointHelper()
/**
Constructor.
*/
{

	this->localPositionValue = MVector(0.0, 0.0, 0.0);
	this->localRotateValue = MVector(0.0, 0.0, 0.0);
	this->localScaleValue = MVector(1.0, 1.0, 1.0);
	this->sizeValue = 10.0;

	this->lineWidthValue = 1.0f;
	this->fillValue = false;
	this->shadedValue = false;
	this->drawOnTopValue = false;

	this->textValues = MStringArray();
	this->choiceValue = 0;
	this->fontSizeValue = 11;

	this->controlPointValues = MVectorArray();

	this->drawables = { { "cross", true }, { "box", true } };

};


PointHelper::~PointHelper()
/**
Destructor.
*/
{

	this->drawables.clear();
	this->controlPointValues.clear();
	this->textValues.clear();

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

		// Get values from handles
		//
		MMatrix positionMatrix = Drawable::createPositionMatrix(this->localPositionValue);
		MMatrix rotateMatrix = Drawable::createRotationMatrix(this->localRotateValue);
		MMatrix scaleMatrix = Drawable::createScaleMatrix(this->localScaleValue);
		MMatrix sizeMatrix = Drawable::createScaleMatrix(this->sizeValue);

		MMatrix objectMatrix = sizeMatrix * scaleMatrix * rotateMatrix * positionMatrix;
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


bool PointHelper::getInternalValue(const MPlug& plug, MDataHandle& handle)
/**
This method is overridden by nodes that store attribute data in some internal format.
The internal state of attributes can be set or queried using the setInternal and internal methods of MFnAttribute.
When internal attribute values are queried via getAttr or MPlug::getValue this method is called.

@param plug: The attribute that is being queried.
@param handle: The data handle to store the attribute value.
@return: The attribute was placed in the datablock.
*/
{

	MStatus status;

	// Evaluate attribute category
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

		if (attribute == PointHelper::localPosition)
		{

			handle.setMVector(this->localPositionValue);

		}
		else if (attribute == PointHelper::localPositionX)
		{

			handle.setMDistance(MDistance(this->localPositionValue.x, MDistance::kCentimeters));

		}
		else if (attribute == PointHelper::localPositionY)
		{

			handle.setMDistance(MDistance(this->localPositionValue.y, MDistance::kCentimeters));

		}
		else if (attribute == PointHelper::localPositionZ)
		{

			handle.setMDistance(MDistance(this->localPositionValue.z, MDistance::kCentimeters));

		}
		else;

		return true;

	}
	else if (isLocalRotation)
	{

		if (attribute == PointHelper::localRotate)
		{

			handle.setMVector(this->localRotateValue);

		}
		else if (attribute == PointHelper::localRotateX)
		{

			handle.setMAngle(MAngle(this->localRotateValue.x, MAngle::kRadians));

		}
		else if (attribute == PointHelper::localRotateY)
		{

			handle.setMAngle(MAngle(this->localRotateValue.y, MAngle::kRadians));

		}
		else if (attribute == PointHelper::localRotateZ)
		{

			handle.setMAngle(MAngle(this->localRotateValue.z, MAngle::kRadians));

		}
		else;

		return true;

	}
	else if (isLocalScale)
	{

		if (attribute == PointHelper::localScale)
		{

			handle.setMVector(this->localScaleValue);

		}
		else if (attribute == PointHelper::localScaleX)
		{

			handle.setMDistance(MDistance(this->localScaleValue.x, MDistance::kCentimeters));

		}
		else if (attribute == PointHelper::localScaleY)
		{

			handle.setMDistance(MDistance(this->localScaleValue.y, MDistance::kCentimeters));

		}
		else if (attribute == PointHelper::localScaleZ)
		{

			handle.setMDistance(MDistance(this->localScaleValue.z, MDistance::kCentimeters));

		}
		else;

		return true;

	}
	else if (isDrawable)
	{

		std::string name = fnAttribute.name().asChar();
		handle.setBool(this->drawables[name]);

		return true;

	}
	else if (isText)
	{

		if (attribute == PointHelper::choice)
		{

			handle.setInt(this->choiceValue);

		}
		else if (attribute == PointHelper::text)
		{

			bool isElement = plug.isElement(&status);
			CHECK_MSTATUS_AND_RETURN(status, false);

			if (!isElement)
			{

				return false;

			}

			unsigned int index = plug.logicalIndex();
			unsigned int count = this->textValues.length();
			MString text = (0 <= index && index < count) ? this->textValues[index] : MString("");

			handle.setString(text);

		}
		else if (attribute == PointHelper::fontSize)
		{

			handle.setDouble(this->fontSizeValue);

		}
		else;

		return true;

	}
	else if (isCustom)
	{

		if (attribute == PointHelper::controlPoints)
		{

			bool isElement = plug.isElement(&status);
			CHECK_MSTATUS_AND_RETURN(status, false);

			if (!isElement)
			{

				return false;

			}

			unsigned int index = plug.logicalIndex();
			unsigned int count = this->controlPointValues.length();
			MVector controlPoint = (0 <= index && index < count) ? this->controlPointValues[index] : MVector::zero;

			handle.setMVector(controlPoint);

		}
		else if (attribute == PointHelper::xValue)
		{

			unsigned int index = plug.parent().logicalIndex();
			unsigned int count = this->controlPointValues.length();
			MVector controlPoint = (0 <= index && index < count) ? this->controlPointValues[index] : MVector::zero;

			handle.setDouble(controlPoint.x);

		}
		else if (attribute == PointHelper::yValue)
		{

			unsigned int index = plug.parent().logicalIndex();
			unsigned int count = this->controlPointValues.length();
			MVector controlPoint = (0 <= index && index < count) ? this->controlPointValues[index] : MVector::zero;

			handle.setDouble(controlPoint.y);

		}
		else if (attribute == PointHelper::zValue)
		{

			unsigned int index = plug.parent().logicalIndex();
			unsigned int count = this->controlPointValues.length();
			MVector controlPoint = (0 <= index && index < count) ? this->controlPointValues[index] : MVector::zero;

			handle.setDouble(controlPoint.z);

		}
		else;

		return true;

	}
	else if (isRender)
	{

		if (attribute == PointHelper::size)
		{

			handle.setDouble(this->sizeValue);

		}
		else if (attribute == PointHelper::lineWidth)
		{

			handle.setFloat(this->lineWidthValue);

		}
		else if (attribute == PointHelper::fill)
		{

			handle.setBool(this->fillValue);

		}
		else if (attribute == PointHelper::shaded)
		{

			handle.setBool(this->shadedValue);

		}
		else if (attribute == PointHelper::drawOnTop)
		{

			handle.setBool(this->drawOnTopValue);

		}
		else;

		return true;

	}
	else;

	return MPxLocatorNode::getInternalValue(plug, handle);

};


bool PointHelper::setInternalValue(const MPlug& plug, const MDataHandle& handle)
/**
This method is overridden by nodes that store attribute specs in some internal format.
The internal state of attributes can be set or queried using the setInternal and internal methods of MFnAttribute.
When internal attribute values are set via setAttr or MPlug::setValue this method is called.
Another use for this method is to impose attribute limits.

@param plug: The attribute that is being set.
@param handle: The data handle containing the value to set.
@return: The attribute was handled internally.
*/
{

	MStatus status;

	// Evaluate attribute category
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

		if (attribute == PointHelper::localPosition)
		{

			this->localPositionValue = handle.asVector();

		}
		else if (attribute == PointHelper::localPositionX)
		{

			this->localPositionValue.x = handle.asDistance().asCentimeters();

		}
		else if (attribute == PointHelper::localPositionY)
		{

			this->localPositionValue.y = handle.asDistance().asCentimeters();

		}
		else if (attribute == PointHelper::localPositionZ)
		{

			this->localPositionValue.z = handle.asDistance().asCentimeters();

		}
		else;

		return true;

	}
	else if (isLocalRotation)
	{

		if (attribute == PointHelper::localRotate)
		{

			this->localRotateValue = handle.asVector();

		}
		else if (attribute == PointHelper::localRotateX)
		{

			this->localRotateValue.x = handle.asAngle().asRadians();

		}
		else if (attribute == PointHelper::localRotateY)
		{

			this->localRotateValue.y = handle.asAngle().asRadians();

		}
		else if (attribute == PointHelper::localRotateZ)
		{

			this->localRotateValue.z = handle.asAngle().asRadians();

		}
		else;

		return true;

	}
	else if (isLocalScale)
	{

		if (attribute == PointHelper::localScale)
		{

			this->localScaleValue = handle.asVector();

		}
		else if (attribute == PointHelper::localScaleX)
		{

			this->localScaleValue.x = handle.asDouble();

		}
		else if (attribute == PointHelper::localScaleY)
		{

			this->localScaleValue.y = handle.asDouble();

		}
		else if (attribute == PointHelper::localScaleZ)
		{

			this->localScaleValue.z = handle.asDouble();

		}
		else;

		return true;

	}
	else if (isDrawable)
	{

		std::string name = fnAttribute.name().asChar();
		this->drawables[name] = handle.asBool();

		return true;

	}
	else if (isText)
	{

		if (attribute == PointHelper::choice)
		{

			this->choiceValue = handle.asInt();

		}
		else if (attribute == PointHelper::text)
		{

			this->setText(plug.logicalIndex(), handle.asString());

		}
		else if (attribute == PointHelper::fontSize)
		{

			this->fontSizeValue = handle.asDouble();

		}
		else;

		return true;

	}
	else if (isCustom)
	{

		if (attribute == PointHelper::controlPoints)
		{

			this->setControlPoint(plug.logicalIndex(), handle.asVector());

		}
		else if (attribute == PointHelper::xValue)
		{

			this->setControlPoint(plug.parent().logicalIndex(), Axis::X, handle.asDouble());

		}
		else if (attribute == PointHelper::yValue)
		{

			this->setControlPoint(plug.parent().logicalIndex(), Axis::Y, handle.asDouble());

		}
		else if (attribute == PointHelper::zValue)
		{

			this->setControlPoint(plug.parent().logicalIndex(), Axis::Z, handle.asDouble());

		}
		else;

		return true;

	}
	else if (isRender)
	{
		
		if (attribute == PointHelper::size)
		{

			this->sizeValue = handle.asDouble();

		}
		else if (attribute == PointHelper::lineWidth)
		{

			this->lineWidthValue = handle.asFloat();

		}
		else if (attribute == PointHelper::fill)
		{

			this->fillValue = handle.asBool();

		}
		else if (attribute == PointHelper::shaded)
		{

			this->shadedValue = handle.asBool();

		}
		else if (attribute == PointHelper::drawOnTop)
		{

			this->drawOnTopValue = handle.asBool();

		}
		else;

		return true;

	}
	else;

	return MPxLocatorNode::setInternalValue(plug, handle);

};


int PointHelper::internalArrayCount(const MPlug& plug) const
/**
This method is overridden by nodes that have internal array attributes which are not stored in Maya's datablock.
This method is used by Maya to determine the non-sparse count of array elements during file io. If the internal array is stored sparsely, you should return the maximum index of the array plus one. If the internal array is non-sparse then return the length of the array.
This method does not need to be implemented for attributes that are stored in the datablock since Maya will use the datablock size.
If this method is overridden, it should return -1 for attributes which it does not handle. Maya will use the datablock size to determine the array length when -1 is returned.

@param plug: The array plug.
@return: The array count.
*/
{

	MObject attribute = plug.attribute();

	if (attribute == PointHelper::controlPoints)
	{

		return this->controlPointValues.length();

	}
	else if (attribute == PointHelper::text)
	{

		return this->textValues.length();

	}
	else;

	return -1;

};


void PointHelper::copyInternalData(PointHelperData* data)
/**
This method is overridden by nodes that store attribute data in some internal format.
On duplication this method is called on the duplicated node with the node being duplicated passed as the parameter.
Overriding this method gives your node a chance to duplicate any internal data you've been storing and manipulating outside of normal attribute data.

@param node: The node that is being duplicated.
@return: Void.
*/
{

	data->localPosition = this->localPositionValue;
	data->localRotate = this->localRotateValue;
	data->localScale = this->localScaleValue;
	data->size = this->sizeValue;

	data->lineWidth = this->lineWidthValue;
	data->fill = this->fillValue;
	data->shaded = this->shadedValue;
	data->drawOnTop = this->drawOnTopValue;

	data->text = (0 <= this->choiceValue && this->choiceValue < this->textValues.length()) ? this->textValues[this->choiceValue] : MString("");
	data->fontSize = this->fontSizeValue;

	data->controlPoints = this->controlPointValues;

	data->drawables = this->drawables;

	data->invalidate();

};


MStatus PointHelper::setText(const unsigned int index, const MString& text)
/**
Updates the internal text string at the specified index.

@param index: The index to update.
@param text: The text string to assign.
@return: Return status.
*/
{

	MStatus status;

	unsigned int count = this->textValues.length();

	if (index >= count)
	{

		status = this->textValues.setLength(index + 1);
		CHECK_MSTATUS_AND_RETURN_IT(status);

	}

	this->textValues[index] = text;

	return status;

};


MStatus PointHelper::setControlPoint(const unsigned int index, const MVector& vector)
/**
Updates the internal control point at the specified index.

@param index: The index to update.
@param vector: The vector point to assign.
@return: Return status.
*/
{

	MStatus status;

	unsigned int count = this->controlPointValues.length();

	if (index >= count)
	{

		status = this->controlPointValues.setLength(index + 1);
		CHECK_MSTATUS_AND_RETURN_IT(status);

	}

	this->controlPointValues[index] = vector;

	return status;

};


MStatus PointHelper::setControlPoint(const unsigned int index, const Axis axis, const double value)
/**
Updates the internal control point at the specified index.

@param index: The index to update.
@param axis: The vector axis to update.
@param value: The axis value to assign.
@return: Return status.
*/
{

	MStatus status;

	unsigned int count = this->controlPointValues.length();

	if (index >= count)
	{

		status = this->controlPointValues.setLength(index + 1);
		CHECK_MSTATUS_AND_RETURN_IT(status);

	}

	this->controlPointValues[index][(int)axis] = value;

	return status;

};


MMatrix PointHelper::getMatrixData(const MObject& data)
/**
Returns the matrix value from the supplied data object.

@param data: Matrix data object.
@return: Matrix value.
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


MDagPath PointHelper::thisMDagPath() const
/**
Returns a dag path to this node.

@return: Dag path to this node.
*/
{

	return MDagPath::getAPathTo(this->thisMObject());

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
	unsigned int count = this->controlPointValues.length();

	for (unsigned int i = 0; i < count; i++)
	{

		boundingBox.expand(this->controlPointValues[i]);

	}

	// Get object-matrix
	//
	MPlug objectMatrixPlug = MPlug(this->thisMObject(), PointHelper::objectMatrix);

	MObject objectMatrixData = objectMatrixPlug.asMObject(&status);
	CHECK_MSTATUS_AND_RETURN(status, boundingBox);

	MMatrix objectMatrix = PointHelper::getMatrixData(objectMatrixData);

	// Transform bounding-box with object-matrix
	//
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

	CHECK_MSTATUS(fnUnitAttr.setInternal(true));
	CHECK_MSTATUS(fnUnitAttr.addToCategory(PointHelper::localPositionCategory));

	// Edit ".localPositionY` attribute
	//
	status = fnUnitAttr.setObject(PointHelper::localPositionY);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setInternal(true));
	CHECK_MSTATUS(fnUnitAttr.addToCategory(PointHelper::localPositionCategory));

	// Edit ".localPositionZ` attribute
	//
	status = fnUnitAttr.setObject(PointHelper::localPositionZ);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setInternal(true));
	CHECK_MSTATUS(fnUnitAttr.addToCategory(PointHelper::localPositionCategory));

	// Edit ".localPosition` attribute
	//
	status = fnNumericAttr.setObject(PointHelper::localPosition);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::localPositionCategory));
	
	// Initialize `localRotateX` attribute
	//
	PointHelper::localRotateX = fnUnitAttr.create("localRotateX", "lorx", MFnUnitAttribute::kAngle, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setChannelBox(true));
	CHECK_MSTATUS(fnUnitAttr.setInternal(true));
	CHECK_MSTATUS(fnUnitAttr.addToCategory(PointHelper::localRotationCategory));

	// Initialize `localRotateY` attribute
	//
	PointHelper::localRotateY = fnUnitAttr.create("localRotateY", "lory", MFnUnitAttribute::kAngle, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setChannelBox(true));
	CHECK_MSTATUS(fnUnitAttr.setInternal(true));
	CHECK_MSTATUS(fnUnitAttr.addToCategory(PointHelper::localRotationCategory));

	// Initialize `localRotateZ` attribute
	//
	PointHelper::localRotateZ = fnUnitAttr.create("localRotateZ", "lorz", MFnUnitAttribute::kAngle, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setChannelBox(true));
	CHECK_MSTATUS(fnUnitAttr.setInternal(true));
	CHECK_MSTATUS(fnUnitAttr.addToCategory(PointHelper::localRotationCategory));

	// Initialize `localRotate"
	//
	PointHelper::localRotate = fnNumericAttr.create("localRotate", "lor", PointHelper::localRotateX, PointHelper::localRotateY, PointHelper::localRotateZ, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::localRotationCategory));

	// Edit ".localScaleX` attribute
	//
	status = fnUnitAttr.setObject(PointHelper::localScaleX);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setInternal(true));
	CHECK_MSTATUS(fnUnitAttr.addToCategory(PointHelper::localScaleCategory));

	// Edit ".localScaleY` attribute
	//
	status = fnUnitAttr.setObject(PointHelper::localScaleY);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setInternal(true));
	CHECK_MSTATUS(fnUnitAttr.addToCategory(PointHelper::localScaleCategory));

	// Edit ".localScaleZ` attribute
	//
	status = fnUnitAttr.setObject(PointHelper::localScaleZ);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setInternal(true));
	CHECK_MSTATUS(fnUnitAttr.addToCategory(PointHelper::localScaleCategory));

	// Edit ".localScale` attribute
	//
	status = fnNumericAttr.setObject(PointHelper::localScale);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::localScaleCategory));

	// Initialize `size` attribute
	//
	PointHelper::size = fnNumericAttr.create("size", "size", MFnNumericData::kDouble, 10.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setChannelBox(true));
	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::renderCategory));

	// Initialize `centerMarker` attribute
	//
	PointHelper::centerMarker = fnNumericAttr.create("centerMarker", "centerMarker", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Initialize `axisTripod` attribute
	//
	PointHelper::axisTripod = fnNumericAttr.create("axisTripod", "axisTripod", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Initialize `axisView` attribute
	//
	PointHelper::axisView = fnNumericAttr.create("axisView", "axisView", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Initialize `cross` attribute
	//
	PointHelper::cross = fnNumericAttr.create("cross", "cross", MFnNumericData::kBoolean, true, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Initialize `square` attribute
	//
	PointHelper::square = fnNumericAttr.create("square", "square", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Initialize `box` attribute
	//
	PointHelper::box = fnNumericAttr.create("box", "box", MFnNumericData::kBoolean, true, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Initialize `triangle` attribute
	//
	PointHelper::triangle = fnNumericAttr.create("triangle", "triangle", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Initialize `pyramid` attribute
	//
	PointHelper::pyramid = fnNumericAttr.create("pyramid", "pyramid", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Initialize `diamond` attribute
	//
	PointHelper::diamond = fnNumericAttr.create("diamond", "diamond", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Initialize `circle` attribute
	//
	PointHelper::disc = fnNumericAttr.create("disc", "disc", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Initialize `arrow` attribute
	//
	PointHelper::arrow = fnNumericAttr.create("arrow", "arrow", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Initialize `notch` attribute
	//
	PointHelper::notch = fnNumericAttr.create("notch", "notch", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Initialize `tearDrop` attribute
	//
	PointHelper::tearDrop = fnNumericAttr.create("tearDrop", "tearDrop", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Initialize `cylinder` attribute
	//
	PointHelper::cylinder = fnNumericAttr.create("cylinder", "cylinder", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Initialize `sphere` attribute
	//
	PointHelper::sphere = fnNumericAttr.create("sphere", "sphere", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Initialize `choice` attribute
	//
	PointHelper::choice = fnNumericAttr.create("choice", "choice", MFnNumericData::kInt, 0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::textCategory));

	// Initialize `text` attribute
	//
	PointHelper::text = fnTypedAttr.create("text", "txt", MFnData::kString, MObject::kNullObj, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnTypedAttr.setArray(true));
	CHECK_MSTATUS(fnTypedAttr.setInternal(true));
	CHECK_MSTATUS(fnTypedAttr.addToCategory(PointHelper::textCategory));

	// Initialize `fontSize` attribute
	//
	PointHelper::fontSize = fnNumericAttr.create("fontSize", "fontSize", MFnNumericData::kInt, 11, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setMin(1));
	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::textCategory));

	// Initialize `custom` attribute
	//
	PointHelper::custom = fnNumericAttr.create("custom", "custom", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// ".xValue` attribute
	//
	PointHelper::xValue = fnNumericAttr.create("xValue", "xValue", MFnNumericData::kDouble, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::customCategory));

	// ".yValue` attribute
	//
	PointHelper::yValue = fnNumericAttr.create("yValue", "yValue", MFnNumericData::kDouble, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::customCategory));

	// ".zValue` attribute
	//
	PointHelper::zValue = fnNumericAttr.create("zValue", "zValue", MFnNumericData::kDouble, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::customCategory));

	// ".controlPoints` attribute
	//
	PointHelper::controlPoints = fnNumericAttr.create("controlPoints", "controlPoints", PointHelper::xValue, PointHelper::yValue, PointHelper::zValue, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	
	CHECK_MSTATUS(fnNumericAttr.setArray(true));
	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::customCategory));

	// Initialize `lineWidth` attribute
	//
	PointHelper::lineWidth = fnNumericAttr.create("lineWidth", "lw", MFnNumericData::kFloat, 1.0f, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setMin(0.0));
	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::renderCategory));

	// Initialize `fill` attribute
	//
	PointHelper::fill = fnNumericAttr.create("fill", "fill", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::renderCategory));

	// Initialize `shaded` attribute
	//
	PointHelper::shaded = fnNumericAttr.create("shaded", "shaded", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::renderCategory));

	// Initialize `drawOnTop` attribute
	//
	PointHelper::drawOnTop = fnNumericAttr.create("drawOnTop", "drawOnTop", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true));
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