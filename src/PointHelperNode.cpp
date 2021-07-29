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

MTypeId PointHelper::id(0x00131800);
MString	PointHelper::drawableCategory("Drawable");
MString	PointHelper::drawDbClassification("drawdb/geometry/PointHelper");
MString	PointHelper::drawRegistrantId("PointHelperNodePlugin");


PointHelper::PointHelper() {}
PointHelper::~PointHelper() {}


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
	MObject attribute = plug.attribute();

	if (attribute == PointHelper::objectMatrix || attribute == PointHelper::objectInverseMatrix) 
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

		double adjustedScale[3] = {
			scale[0] * size,
			scale[1] * size,
			scale[2] * size
		};

		// Define transform matrix
		//
		MTransformationMatrix transform = MTransformationMatrix();

		status = transform.setTranslation(position, MSpace::kTransform);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		status = transform.setRotation(rotation, MTransformationMatrix::RotationOrder::kXYZ);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		status = transform.setScale(adjustedScale, MSpace::kTransform);
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
	else if (attribute == PointHelper::objectWorldMatrix || attribute == PointHelper::objectWorldInverseMatrix) 
	{

		// Get object matrix
		//
		MDataHandle objectMatrixHandle = data.inputValue(PointHelper::objectMatrix, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MMatrix objectMatrix = objectMatrixHandle.asMatrix();

		// Get all dag paths to this node
		//
		MObject node = this->thisMObject();

		MDagPathArray dagPaths;

		status = MDagPath::getAllPathsTo(node, dagPaths);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		unsigned int numPaths = dagPaths.length();

		// Define data builders
		//
		MArrayDataHandle objectWorldMatrixArrayHandle = data.outputArrayValue(PointHelper::objectWorldMatrix, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MArrayDataBuilder objectWorldMatrixBuilder(&data, PointHelper::objectWorldMatrix, numPaths, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MArrayDataHandle objectWorldInverseMatrixArrayHandle = data.outputArrayValue(PointHelper::objectWorldInverseMatrix, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MArrayDataBuilder objectWorldInverseMatrixBuilder(&data, PointHelper::objectWorldInverseMatrix, numPaths, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		for (unsigned int i = 0; i < numPaths; i++) 
		{

			// Create new data handles for elements
			//
			MDataHandle hObjectWorldMatrix = objectWorldMatrixBuilder.addElement(i, &status);
			MDataHandle hObjectWorldInverseMatrix = objectWorldInverseMatrixBuilder.addElement(i, &status);

			// Get inclusive matrix to dag path
			//
			MDagPath dagPath = dagPaths[i];
			MMatrix inclusiveMatrix = dagPath.inclusiveMatrix();

			MMatrix objectWorldMatrix = objectMatrix * inclusiveMatrix;
			MMatrix objectWorldInverseMatrix = objectWorldMatrix.inverse();

			// Assign matrices to elements
			//
			hObjectWorldMatrix.setMMatrix(objectWorldMatrix);
			hObjectWorldInverseMatrix.setMMatrix(objectWorldInverseMatrix);

			// Mark handles as clean
			//
			hObjectWorldMatrix.setClean();
			hObjectWorldInverseMatrix.setClean();

		}

		// Assign builders to data handles
		//
		objectWorldMatrixArrayHandle.set(objectWorldMatrixBuilder);
		objectWorldInverseMatrixArrayHandle.set(objectWorldInverseMatrixBuilder);

		// Mark data handles as clean
		//
		objectWorldMatrixArrayHandle.setAllClean();
		objectWorldInverseMatrixArrayHandle.setAllClean();

		// Mark data block as clean
		//
		status = data.setClean(plug);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		return MS::kSuccess;

	}
	else;

	return MS::kUnknownParameter;

};


MStatus PointHelper::preEvaluation(const MDGContext& context, const MEvaluationNode& evaluationNode)
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

	return MS::kSuccess;

};


void PointHelper::getCacheSetup(const MEvaluationNode& evaluationNode, MNodeCacheDisablingInfo& disablingInfo, MNodeCacheSetupInfo& cacheSetupInfo, MObjectArray& monitoredAttributes) const
{

	// Call parent function
	//
	MPxLocatorNode::getCacheSetup(evaluationNode, disablingInfo, cacheSetupInfo, monitoredAttributes);
	assert(!disablingInfo.getCacheDisabled());

	// Update caching preference
	//
	cacheSetupInfo.setPreference(MNodeCacheSetupInfo::kWantToCacheByDefault, true);
	
};


MMatrix PointHelper::getObjectMatrix() const
/**
Retrieves the object matrix using the supplied context.

@param context: The context to use when retrieving the value.
@return: Object matrix.
*/
{

	MStatus status;

	// Fing matrix plug
	//
	MObject node = this->thisMObject();
	MPlug plug(node, PointHelper::objectMatrix);

	// Get matrix data
	//
	MObject matrixData;

	status = plug.getValue(matrixData);
	CHECK_MSTATUS(status);

	// Get matrix value using function set
	//
	MFnMatrixData fnMatrixData(matrixData, &status);
	CHECK_MSTATUS(status);

	MMatrix matrix = fnMatrixData.matrix(&status);
	CHECK_MSTATUS(status);

	return matrix;

};


MMatrix PointHelper::getObjectWorldMatrix() const
/**
Retrieves the world object matrix for this instance.

@return: Object world matrix.
*/
{

	MStatus status;

	// Get instance number
	//
	MObject node = this->thisMObject();
	
	MDagPath dagPath;

	status = MDagPath::getAPathTo(node, dagPath);
	CHECK_MSTATUS(status);

	unsigned int instanceNumber = dagPath.instanceNumber(&status);
	CHECK_MSTATUS(status);

	// Find plug element
	//
	MPlug plug(node, PointHelper::objectWorldMatrix);

	status = plug.selectAncestorLogicalIndex(instanceNumber, PointHelper::objectWorldMatrix);
	CHECK_MSTATUS(status);

	// Get matrix data
	//
	MObject matrixData;

	status = plug.getValue(matrixData);
	CHECK_MSTATUS(status);

	// Get matrix value using function set
	//
	MFnMatrixData fnMatrixData(matrixData, &status);
	CHECK_MSTATUS(status);

	MMatrix matrix = fnMatrixData.matrix(&status);
	CHECK_MSTATUS(status);

	return matrix;

};


MStatus PointHelper::getControlPoints(const MDagPath& dagPath, MPointArray& points)
/**
Retrieves all of the points to make the user defined custom drawable.

@param objPath: Dag path to the point helper.
@param points: Passed array to store points in.
@return: Return status.
*/
{
	MStatus status;

	// Get control points plug
	//
	MObject node = dagPath.node();
	MPlug controlPointsPlug(node, PointHelper::controlPoints);

	unsigned int numElements = controlPointsPlug.numElements(&status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// Resize point array based on number of elements
	//
	status = points.setLength(numElements);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// Iterate through elements
	//
	MPlug element;
	MPlug xValuePlug, yValuePlug, zValuePlug;


	for (unsigned int i = 0; i < numElements; i++) 
	{

		// Jump to next element
		//
		element = controlPointsPlug.elementByPhysicalIndex(i, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);
		
		xValuePlug = element.child(PointHelper::xValue, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		yValuePlug = element.child(PointHelper::yValue, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		zValuePlug = element.child(PointHelper::zValue, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		// Assign point to passed array
		//
		points[i] = MPoint(xValuePlug.asDouble(), yValuePlug.asDouble(), zValuePlug.asDouble());

	}

	return MS::kSuccess;

};


bool PointHelper::getBooleanValue(const MObject& node, const MObject& attribute)
/**
Retrieves a boolean value from the plug associated with the supplied node and attribute.
No error checking is performed here so be careful!

@param node: Node object
@param attribute: Attribute object
@return: Boolean
*/
{

	return MPlug(node, attribute).asBool();

};


double PointHelper::getDoubleValue(const MObject& node, const MObject& attribute)
/**
Retrieves a double value from the plug associated with the supplied node and attribute.
No error checking is performed here so be careful!

@param node: Node object
@param attribute: Attribute object
@return: Double
*/
{

	return MPlug(node, attribute).asDouble();

};


float PointHelper::getFloatValue(const MObject& node, const MObject& attribute)
/**
Retrieves a float value from the plug associated with the supplied node and attribute.
No error checking is performed here so be careful!

@param node: Node object
@param attribute: Attribute object
@return: Float
*/
{

	return MPlug(node, attribute).asFloat();

};


int PointHelper::getIntegerValue(const MObject& node, const MObject& attribute)
/**
Retrieves an integer value from the plug associated with the supplied node and attribute.
No error checking is performed here so be careful!

@param node: Node object
@param attribute: Attribute object
@return: Integer
*/
{

	return MPlug(node, attribute).asShort();
	
};


MString PointHelper::getTextChoice() const
/**
Retrieve the text value associated with the choice index.
If the index is out of range then an empty string will be returned.

@return: MString
*/
{

	MStatus status;

	// Intialize plugs
	//
	MObject node = this->thisMObject();

	MPlug choicePlug(node, PointHelper::choice);
	MPlug textPlug(node, PointHelper::text);

	// Get plug values
	//
	int choice = choicePlug.asShort();
	MString text;

	int numElements = textPlug.numElements(&status);
	CHECK_MSTATUS(status);

	if (choice < numElements) 
	{

		MPlug element = textPlug.elementByLogicalIndex(choice, &status);
		CHECK_MSTATUS(status);

		text = element.asString();

	}

	return text;

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

	// Create unit scaled bounding box
	//
	MMatrix objectMatrix = this->getObjectMatrix();

	MPoint corner1(-1.0, -1.0, -1.0);
	corner1 *= objectMatrix;

	MPoint corner2(1.0, 1.0, 1.0);
	corner2 *= objectMatrix;

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


MStatus PointHelper::getAttributesByCategory(const MObject& node, const MString& category, MObjectArray& attributes)
/**
Collects all of the attributes from the given node based on the supplied category name.
All drawable attributes are tagged to make retrieving them easier.

@param node: The node to collect from.
@param category: The category to filter for.
@param attributes: Passed container to store the attributes.
@return: MStatus
*/
{

	MStatus status;

	// Initialize function set
	//
	MFnDependencyNode fnDependNode(node, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// Clear passed array
	//
	status = attributes.clear();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// Iterate through attributes
	//
	unsigned int attributeCount = fnDependNode.attributeCount(&status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	MObject attribute;
	MFnAttribute fnAttribute;

	for (unsigned int i = 0; i < attributeCount; i++) 
	{

		// Assign attribute to function set
		//
		attribute = fnDependNode.attribute(i, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		status = fnAttribute.setObject(attribute);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		// Check if attribute belongs to category
		//
		if (fnAttribute.hasCategory(category)) 
		{

			status = attributes.append(attribute);
			CHECK_MSTATUS_AND_RETURN_IT(status);

		}

	}

	return MS::kSuccess;

};


MStatus PointHelper::getAttributesByCategory(const MString& category, MObjectArray& attributes)
/**
Collects all of the attributes based on the given category name from the node.
All drawable attributes are tagged to make retrieving them easier.

@param category: The category to filter for.
@param attributes: Passed container to store the attributes.
@return: MStatus
*/
{

	return PointHelper::getAttributesByCategory(this->thisMObject(), category, attributes);

}


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
	// Define ".localRotateX" attribute
	//
	PointHelper::localRotateX = fnUnitAttr.create("localRotateX", "lorx", MFnUnitAttribute::kAngle, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setChannelBox(true));
	CHECK_MSTATUS(fnUnitAttr.setAffectsWorldSpace(true)); // Enable this to force MPxDrawOverride::transform() to update!

	// Define ".localRotateY" attribute
	//
	PointHelper::localRotateY = fnUnitAttr.create("localRotateY", "lory", MFnUnitAttribute::kAngle, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setChannelBox(true));
	CHECK_MSTATUS(fnUnitAttr.setAffectsWorldSpace(true)); // Enable this to force MPxDrawOverride::transform() to update!

	// Define ".localRotateZ" attribute
	//
	PointHelper::localRotateZ = fnUnitAttr.create("localRotateZ", "lorz", MFnUnitAttribute::kAngle, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setChannelBox(true));
	CHECK_MSTATUS(fnUnitAttr.setAffectsWorldSpace(true)); // Enable this to force MPxDrawOverride::transform() to update!

	// Define ".localRotate"
	//
	PointHelper::localRotate = fnNumericAttr.create("localRotate", "lor", PointHelper::localRotateX, PointHelper::localRotateY, PointHelper::localRotateZ, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setChannelBox(true));
	CHECK_MSTATUS(fnNumericAttr.setAffectsWorldSpace(true)); // Enable this to force MPxDrawOverride::transform() to update!

	// Define ".size" attribute
	//
	PointHelper::size = fnNumericAttr.create("size", "size", MFnNumericData::kDouble, 10.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setAffectsWorldSpace(true)); // Enable this to force MPxDrawOverride::transform() to update!

	// Define ".centerMarker" attribute
	//
	PointHelper::centerMarker = fnNumericAttr.create("centerMarker", "centerMarker", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Define ".axisTripod" attribute
	//
	PointHelper::axisTripod = fnNumericAttr.create("axisTripod", "axisTripod", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Define ".cross" attribute
	//
	PointHelper::cross = fnNumericAttr.create("cross", "cross", MFnNumericData::kBoolean, true, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Define ".square" attribute
	//
	PointHelper::square = fnNumericAttr.create("square", "square", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Define ".box" attribute
	//
	PointHelper::box = fnNumericAttr.create("box", "box", MFnNumericData::kBoolean, true, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Define ".diamond" attribute
	//
	PointHelper::diamond = fnNumericAttr.create("diamond", "diamond", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Define ".circle" attribute
	//
	PointHelper::disc = fnNumericAttr.create("disc", "disc", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Define ".arrow" attribute
	//
	PointHelper::arrow = fnNumericAttr.create("arrow", "arrow", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Define ".notch" attribute
	//
	PointHelper::notch = fnNumericAttr.create("notch", "notch", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Define ".tearDrop" attribute
	//
	PointHelper::tearDrop = fnNumericAttr.create("tearDrop", "tearDrop", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Define ".cylinder" attribute
	//
	PointHelper::cylinder = fnNumericAttr.create("cylinder", "cylinder", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Define ".sphere" attribute
	//
	PointHelper::sphere = fnNumericAttr.create("sphere", "sphere", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// Define ".choice" attribute
	//
	PointHelper::choice = fnNumericAttr.create("choice", "choice", MFnNumericData::kInt, 0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// Define ".text" attribute
	//
	PointHelper::text = fnTypedAttr.create("text", "txt", MFnData::kString, MObject::kNullObj, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnTypedAttr.setArray(true));

	// Define ".fontSize" attribute
	//
	PointHelper::fontSize = fnNumericAttr.create("fontSize", "fontSize", MFnNumericData::kInt, 11, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setMin(1));

	// Define ".custom" attribute
	//
	PointHelper::custom = fnNumericAttr.create("custom", "custom", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setInternal(true)); // Without this we can't assign a callback at runtime!
	CHECK_MSTATUS(fnNumericAttr.addToCategory(PointHelper::drawableCategory));

	// ".xValue" attribute
	//
	PointHelper::xValue = fnNumericAttr.create("xValue", "xValue", MFnNumericData::kDouble, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// ".yValue" attribute
	//
	PointHelper::yValue = fnNumericAttr.create("yValue", "yValue", MFnNumericData::kDouble, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// ".zValue" attribute
	//
	PointHelper::zValue = fnNumericAttr.create("zValue", "zValue", MFnNumericData::kDouble, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// ".controlPoints" attribute
	//
	PointHelper::controlPoints = fnNumericAttr.create("controlPoints", "controlPoints", PointHelper::xValue, PointHelper::yValue, PointHelper::zValue, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	
	CHECK_MSTATUS(fnNumericAttr.setArray(true));

	// Define ".lineWidth" attribute
	//
	PointHelper::lineWidth = fnNumericAttr.create("lineWidth", "ls", MFnNumericData::kDouble, 1.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setMin(0.0));

	// Define ".drawOnTop" attribute
	//
	PointHelper::drawOnTop = fnNumericAttr.create("drawOnTop", "drawOnTop", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// Define ".fill" attribute
	//
	PointHelper::fill = fnNumericAttr.create("fill", "fill", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// Define ".shaded" attribute
	//
	PointHelper::shaded = fnNumericAttr.create("shaded", "shaded", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

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