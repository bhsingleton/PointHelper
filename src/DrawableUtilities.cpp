//
// File: DrawableUtilities.cpp
//
// Author: Ben Singleton
//

#include "DrawableUtilities.h"


MMatrix DrawableUtilities::composeMatrix(MVector center, MVector normal, MVector up, double scale[3])
/**
Function used to compose a transformation matrix from a position, forward/up vector and scale.

@param center: Center of transform.
@param normal: Vector assigned to x-axis.
@param up: Secondary vector used in cross product.
@param scale: Local space scale.
@return: MMatrix
*/
{

	// Compose matrix list from arguments
	//
	MVector xAxis = normal.normal() * scale[0];
	MVector zAxis = (normal ^ up).normal() * scale[1];
	MVector yAxis = (zAxis ^ xAxis).normal() * scale[2];

	double matrixList[4][4] = 
	{
		{ xAxis.x, xAxis.y, xAxis.z, 0.0 },
		{ yAxis.x, yAxis.y, yAxis.z, 0.0 },
		{ zAxis.x, zAxis.y, zAxis.z, 0.0 },
		{ center.x, center.y, center.z, 1.0 }
	};

	return MMatrix(matrixList);

};


MMatrix DrawableUtilities::composeMatrix(MVector center, MEulerRotation eulerRotation, const double scale[3])
/**
Function used to compose a transformation matrix from a position, rotation and scale.

@param center: Center of transform.
@param rotation: Euler rotation angles.
@param rotateOrder: Order of rotation.
@param scale: Local space scale.
@return: MMatrix
*/
{

	MStatus status;

	// Set translation
	//
	MTransformationMatrix transform;

	status = transform.setTranslation(center, MSpace::kTransform);
	CHECK_MSTATUS(status);

	// Set rotation
	//
	double rotation[3] = { eulerRotation.x, eulerRotation.y, eulerRotation.z };

	status = transform.setRotation(rotation, MTransformationMatrix::RotationOrder(eulerRotation.order + 1));
	CHECK_MSTATUS(status);

	// Set scale
	//
	status = transform.setScale(scale, MSpace::kTransform);
	CHECK_MSTATUS(status);

	return transform.asMatrix();

};


MMatrix DrawableUtilities::composeMatrix(MEulerRotation eulerRotation)
/**
Function used to compose a rotation matrix from an euler rotation.

@param rotation: Euler rotation angles.
@param rotateOrder: Order of rotation.
@return: MMatrix
*/
{

	// Call overload method
	//
	const double scale[3] = { 1.0, 1.0, 1.0 };
	return DrawableUtilities::composeMatrix(MVector::zero, eulerRotation, scale);

};


void DrawableUtilities::decomposeMatrix(MMatrix matrix, MVector& translation, MEulerRotation& eulerRotation, double3& scale)
/**
Decomposes a matrix into it's translate, rotate and scale components.

@param matrix: The matrix to decompose.
@param translation: A passed container to store the translation vector.
@param eulerRotation: A passed container to store the euler rotation.
@param scale: A passed container to store the scale values.
@return: void
*/
{

	MStatus status;

	// Initialize transformation matrix
	//
	MTransformationMatrix transform(matrix);

	// Get translation
	//
	translation = transform.getTranslation(MSpace::kObject, &status);
	CHECK_MSTATUS(status);

	// Get euler rotation
	//
	eulerRotation = transform.eulerRotation();
	
	// Get scale
	//
	status = transform.getScale(scale, MSpace::kObject);
	CHECK_MSTATUS(status);

};


void DrawableUtilities::decomposeMatrix(MMatrix matrix, MVector& xAxis, MVector& yAxis, MVector& zAxis, MPoint& position)
/**
Decomposes a matrix into it's axis vectors.

@param matrix: The matrix to decompose.
@param xAxis: A passed container to store the X axis vector.
@param yAxis: A passed container to store the Y axis vector.
@param zAxis: A passed container to store the Z axis vector.
@param position: A passed container to store the position.
@return: void
*/
{

	xAxis = MVector(matrix[0][0], matrix[0][1], matrix[0][2]);
	yAxis = MVector(matrix[1][0], matrix[1][1], matrix[1][2]);
	zAxis = MVector(matrix[2][0], matrix[2][1], matrix[2][2]);
	position = MPoint(matrix[3][0], matrix[3][1], matrix[3][2], 1.0);

};


void DrawableUtilities::transform(MMatrix matrix, MPointArray &points)
/**
Function used to transform an array of points using the supplied transform matrix.
This is an in place multiplication performed on a passed array so the original values will be lost!

@param matrix: Transform matrix.
@param points: A passed array of points to be multiplied.
@return: void
*/
{

	// Iterate through points
	//
	unsigned int numPoints = points.length();

	for (unsigned int i = 0; i < numPoints; i++) 
	{

		points[i] *= matrix;

	}

};


MPointArray DrawableUtilities::transform(MMatrix matrix, const double points[][4], int numPoints)
/**
Function used to transform an array of points using the supplied transform matrix.
Unlike the other function, this method will return a brand new array.

@param matrix: Transform matrix.
@param points: An array of points to be multiplied.
@param points: Number of points inside the array.
@return: MPointArray
*/
{

	// Initialize new point array
	//
	MPointArray newPoints(numPoints, MPoint::origin);

	for (int i = 0; i < numPoints; i++) 
	{

		newPoints[i] = MPoint(points[i]) * matrix;

	}

	return newPoints;

};


unsigned int DrawableUtilities::sum(const MIntArray& values)
/**
Function similar to the builtin method in python used to add up all of the values inside an array.
Would be nice to expand support for this but this is all we really need right now.

@param values: Sequence of integers.
@return: Total sum of all values inside array.
*/
{

	// Iterate through values
	//
	unsigned int total = 0;

	for (unsigned int i = 0; i < values.length(); i++) 
	{

		total += values[i];

	}

	return total;

};


MIntArray DrawableUtilities::range(int start, int end, int increment)
/**
Function similar to the builtin method in python used to construct an array of integers.
This is not a 1:1 copy, there is no error checking, so please check your arguments!

@param start: Value to start at.
@param end: Value to end at.
@param increment: Value to increment by.
@return: A squence of integers.
*/
{

	// Calculate length of array
	//
	unsigned int length = (end - start) / increment;
	MIntArray values(length, 0);

	for (unsigned int i = 0; i < length; i += increment) 
	{

		values[i] = start + i;

	}

	return values;

};


MPointArray DrawableUtilities::chain(std::vector<MPointArray>& points)
/**
Function used to take a 2-dimensional array of points and collapse it into a single array.
If you're planning on merging seperate line lists make sure to stagger them before combining!

@param points: A 2D array of points.
@return: A flattened array.
*/
{

	// Get sum of arrays
	//
	int length = 0;

	for (size_t i = 0; i < points.size(); i++) 
	{

		length += points[i].length();

	}

	// Pre-allocate space for all points
	//
	MPointArray newPoints(length, MPoint::origin);
	int counter = 0;

	for (size_t i = 0; i < points.size(); i++) 
	{

		// Assign points to array
		//
		for (unsigned int j = 0; j < points[i].length(); j++) 
		{

			newPoints[counter + j] = points[i][j];

		}

		counter += points[i].length();

	}

	return newPoints;

};


MPointArray DrawableUtilities::stagger(MPointArray& points)
/**
Static function used to take a series of points and stagger them to form a line list.
For example: x[4] = {a, b, c, d}; becomes y[6] = {a, b, b, c, c, d};

@param points: A series of sequential points.
@return: An array of points suitable to pass as a line list.
*/
{

	// Calculate new length
	//
	unsigned int length = points.length();
	unsigned int newLength = (length - 1) * 2;

	MPointArray newPoints(newLength, MPoint::origin);

	// Iterate through points
	//
	unsigned int i, j;

	for (i = 0, j = 0; i < (length - 1); i++, j += 2) 
	{

		newPoints[j] = points[i];
		newPoints[j + 1] = points[i + 1];

	}

	return newPoints;

};


MPointArray DrawableUtilities::stagger(std::vector<MPointArray>& points)
/**
Static function used to take a series of 2-Dimensional arrays and stagger them to form a line list.
For example: x[4] = {a, b, c, d}; becomes y[6] = {a, b, b, c, c, d};

@param points: A series of sequential points.
@return: An array of points suitable to pass as a line list.
*/
{

	// Calculate new length
	//
	size_t size = points.size();
	std::vector<MPointArray> newPoints(size);

	for (size_t i = 0; i < points.size(); i++) 
	{

		newPoints[i] = DrawableUtilities::stagger(points[i]);

	}

	return DrawableUtilities::chain(newPoints);

};


MPointArray DrawableUtilities::line(MPoint start, MPoint end)
/**
A convenience function used to quickly create a point array for a set of line points.

@param start: The start point on the line.
@param end: The end point on the line.
@return: An array of points that make up a line.
*/
{

	// Define point array
	//
	MPointArray points(2, MPoint::origin);
	points[0] = start;
	points[1] = end;

	return points;

};


MPointArray	DrawableUtilities::arc(MVector center, MVector normal, double radius, double startAngle, double endAngle, int numPoints)
/**
Static function used to quickly generate an arc that can be used as a line strip for drawables.
By default all points are calculated on the yz plane using x as the normal vector.
The number of divisions dictates the number of points returned.

@param center: The center of this arc.
@param normal: The forward vector for this arc.
@param startAngle: The angle to start iterating from in degrees.
@param endAngle: The angle to stop iterating at in degrees.
@param steps: The number of points that make up this arc.
@return: An array of points in object space.
*/
{

	// Derive transform matrix from normal
	//
	MQuaternion quat = MVector::xAxis.rotateTo(normal);
	MMatrix matrix = quat.asMatrix();

	// Iterate through angle range
	//
	MPointArray points(numPoints, MPoint::origin);

	double fraction = 1.0 / (numPoints - 1);
	double step = (endAngle - startAngle) * fraction;
	double y, z, angle;
	MPoint point;

	for (int i = 0; i < numPoints; i++) 
	{

		// Calculate 2D co-ordinates
		//
		angle = startAngle + (step * static_cast<double>(i));

		y = radius * sin(angle * (PI / 180.0));
		z = radius * cos(angle * (PI / 180.0));

		// Convert co-ordinates into 3D space
		//
		point = MPoint(0.0, y, z, 1.0);
		point *= matrix;
		point += center;

		points[i] = point;

	}

	return points;

};


MPointArray DrawableUtilities::square(MVector center, MVector normal)
/**
Static function used to generate a square for drawables.
The diagonal of a unit square is expressed through the square root of 2.

@param center: The center of this square.
@param normal: The forward vector for this square.
@return: An array of points in object space.
*/
{

	return DrawableUtilities::arc(center, normal, UNIT_SQUARE_RADIUS, 45, 405, 5);

};


MPointArray	DrawableUtilities::circle(MVector center, MVector normal, double radius, int numPoints)
/**
Static function used to quickly generate a list of points that make up a circle.

@param center: The center of this circle.
@param normal: The forward vector for this circle.
@param radius: The radius of this circle.
@param numPoints: The number of vertices for this circle.
@return: An array of points in object space.
*/
{

	return DrawableUtilities::arc(center, normal, radius, 0, 360, numPoints);

};


MObject DrawableUtilities::sphere(MVector center, double radius, int subdivisionAxis, int subdivisionHeight, MObject& parent)
/**
Creates a sphere primitive in the form of a mesh data object.

@param center: The center of the sphere.
@param radius: The radius of the sphere.
@param subdivisionAxis: The number of faces around the x-axis.
@param subdivisionHeight: The number of faces along the x-axis.
@param parent: The parent container to store this data.
@return: MObject
*/
{

	MStatus status;

	// Check if parent has been initialized
	//
	if (parent.apiType() != MFn::kMeshData) 
	{

		MFnMeshData fnMeshData;
		parent = fnMeshData.create();

	}

	// Assign mesh data to function set
	//
	MFnMesh fnMesh;

	status = fnMesh.setObject(parent);
	CHECK_MSTATUS(status);

	// Add polygons to mesh data
	//
	double fraction = 1.0 / static_cast<double>(subdivisionHeight);
	double endAngle = 360.0 - (360.0 / static_cast<double>(subdivisionAxis));

	double x1, x2, y1, y2;
	int startIndex, endIndex;
	MPointArray p1, p2;
	MPointArray polygon;

	for (int i = 0; i < subdivisionHeight; i++) 
	{

		// Get center and radius of edge loops
		//
		x1 = radius * cos((fraction * static_cast<double>(i)) * PI);
		y1 = radius * sin((fraction * static_cast<double>(i)) * PI);

		x2 = radius * cos((fraction * static_cast<double>(i + 1)) * PI);
		y2 = radius * sin((fraction * static_cast<double>(i + 1)) * PI);

		// Get edge loop points
		//
		p1 = DrawableUtilities::arc(MVector(center.x + x1, center.y, center.z), MVector::xAxis, y1, 0.0, endAngle, subdivisionAxis);
		p2 = DrawableUtilities::arc(MVector(center.x + x2, center.y, center.z), MVector::xAxis, y2, 0.0, endAngle, subdivisionAxis);

		for (int j = 0; j < subdivisionAxis; j++) 
		{

			// Determine start and end index
			//
			startIndex = j;
			endIndex = (j < (subdivisionAxis - 1)) ? (startIndex + 1) : 0;

			if (i == 0) 
			{

				polygon = MPointArray(3);
				polygon[0] = p1[startIndex];
				polygon[1] = p2[endIndex];
				polygon[2] = p2[startIndex];

			}
			else if (i < (subdivisionHeight - 1)) 
			{

				polygon = MPointArray(4);
				polygon[0] = p1[startIndex];
				polygon[1] = p1[endIndex];
				polygon[2] = p2[endIndex];
				polygon[3] = p2[startIndex];

			}
			else 
			{

				polygon = MPointArray(3);
				polygon[0] = p1[startIndex];
				polygon[1] = p1[endIndex];
				polygon[2] = p2[startIndex];

			}

			// Add polygon to mesh
			//
			fnMesh.addPolygon(polygon, true, MERGE_THRESHOLD, parent, &status);
			CHECK_MSTATUS(status);

		}

	}

	return parent;

};


MObject DrawableUtilities::disc(MVector center, MVector normal, double radius, int subdivisionAxis, MObject& parent) 
/**
Static function used to generate a disc for drawables.

@param center: The center of this disc.
@param normal: The forward vector for this sector.
@param radius: The radius of this disc.
@param subdivisionAxis: The number of faces around the x-axis.
@param parent: The parent container to store this data.
@return: An array of points in object space.
*/
{

	return DrawableUtilities::sector(center, normal, radius, 0, 360, subdivisionAxis, parent);

};


MObject DrawableUtilities::sector(MVector center, MVector normal, double radius, double startAngle, double endAngle, int subdivisionAxis, MObject& parent)
/**
Static function used to generate a sector for drawables.
This is essentially a disc but with a start and end angle.

@param center: The center of this sector.
@param normal: The forward vector for this sector.
@param radius: The radius of this sector.
@param startAngle: The start angle of this sector.
@param endAngle: The end angle of this sector.
@param subdivisionAxis: The number of faces around the x-axis.
@param parent: The parent container to store this data.
@return: An array of points in object space.
*/
{

	MStatus status;

	// Check if parent has been initialized
	//
	if (parent.apiType() != MFn::kMeshData) 
	{

		MFnMeshData fnMeshData;
		parent = fnMeshData.create();

	}

	// Initialize function set
	//
	MFnMesh fnMesh(parent, &status);
	CHECK_MSTATUS(status);

	// Create point array
	//
	MPointArray points = DrawableUtilities::arc(center, normal, radius, startAngle, endAngle, subdivisionAxis + 1);
	unsigned int numPoints = points.length();

	MPointArray polygon(3, MPoint::origin);

	for (unsigned int i = 0; i < (numPoints - 1); i++) 
	{

		// Add points to polygon
		//
		polygon[0] = points[i];
		polygon[1] = points[i + 1];
		polygon[2] = center;

		// Create polygon
		//
		fnMesh.addPolygon(polygon, true, MERGE_THRESHOLD, parent, &status);
		CHECK_MSTATUS(status);

	}

	return parent;

};


MObject DrawableUtilities::cylinder(MVector center, MVector normal, double radius, double length, int subdivisionAxis, MObject& parent)
/**
Static function used to generate a cylinder for drawables.

@param center: The center of this cylinder.
@param normal: The forward vector for this cylinder.
@param radius: The radius of this cylinder.
@param length: The length of this cylinder.
@param subdivisionAxis: The number of faces around the x-axis.
@param parent: The parent container to store this data.
@return: An array of points in object space.
*/
{

	MStatus status;

	// Check if parent has been initialized
	//
	if (parent.apiType() != MFn::kMeshData) 
	{

		MFnMeshData fnMeshData;
		parent = fnMeshData.create();

	}

	// Initialize function set
	//
	MFnMesh fnMesh(parent, &status);
	CHECK_MSTATUS(status);

	// Create point array
	//
	MPointArray points = DrawableUtilities::circle(center, normal, radius, subdivisionAxis + 1);
	unsigned int numPoints = points.length();

	MPointArray frontCap(3, MPoint::origin);
	MPointArray backCap(3, MPoint::origin);
	MPointArray bridge(4, MPoint::origin);

	double halfLength = length * 0.5;

	for (unsigned int i = 0; i < (numPoints - 1); i++) 
	{

		// Add fron cap segment
		//
		frontCap[0] = points[i] + (normal * halfLength);
		frontCap[1] = points[i + 1] + (normal * halfLength);
		frontCap[2] = center + (normal * halfLength);

		fnMesh.addPolygon(frontCap, true, MERGE_THRESHOLD, parent, &status);
		CHECK_MSTATUS(status);

		// Add back cap segment
		//
		backCap[0] = points[i] + (-normal * halfLength);
		backCap[1] = points[i + 1] + (-normal * halfLength);
		backCap[2] = center + (-normal * halfLength);

		fnMesh.addPolygon(backCap, true, MERGE_THRESHOLD, parent, &status);
		CHECK_MSTATUS(status);

		// Bridge caps
		//
		bridge[0] = points[i] + (normal * halfLength);
		bridge[1] = points[i + 1] + (normal * halfLength);
		bridge[2] = points[i + 1] + (-normal * halfLength);;
		bridge[3] = points[i] + (-normal * halfLength);

		fnMesh.addPolygon(bridge, true, MERGE_THRESHOLD, parent, &status);
		CHECK_MSTATUS(status);

	}

	// Auto smooth primitive
	//
	status = DrawableUtilities::autoSmoothEdges(parent);
	CHECK_MSTATUS(status);

	return parent;

};


MObject DrawableUtilities::createMeshData(MPointArray points, MIntArray polygonCounts, MIntArray polygonConnects, MStatus* status)
/**
Static function used to create a new mesh data object based on the supplied constructors.
No error checking is performed to test if the incoming data is valid!

@param points: List of vertex points.
@param polygonCounts: List of number of vertices per polygon face.
@param polygonConnects: List of vertices that make up face vertices.
@param status: Return status.
@return: MObject
*/
{

	// Create new mesh data object
	//
	MFnMeshData fnMeshData;
	MObject meshData = fnMeshData.create(status);

	if (!status) 
	{

		return meshData;

	}

	// Create mesh and assign to data object
	//
	MFnMesh fnMesh;
	fnMesh.create(points.length(), polygonCounts.length(), points, polygonCounts, polygonConnects, meshData, status);

	return meshData;

};


MObject DrawableUtilities::createMeshData(MPointArray points, MIntArray polygonCounts, MIntArray polygonConnects, MIntArray smoothings, MStatus* status)
/**
Overload function used to create a new mesh data object based on the supplied constructors.
No error checking is performed to test if the incoming data is valid!

@param points: List of vertex points.
@param polygonCounts: List of number of vertices per polygon face.
@param polygonConnects: List of vertices that make up face vertices.
@param edgeSmoothings: List of smoothing values per edge.
@param status: Return status.
@return: MObject
*/
{

	// Call base function
	//
	MObject meshData = DrawableUtilities::createMeshData(points, polygonCounts, polygonConnects, status);

	if (!status) 
	{

		return meshData;

	}

	// Initialize function set
	//
	MFnMesh fnMesh(meshData, status);

	// Define element and smoothing arrays
	//
	int numEdges = smoothings.length();
	MIntArray elements = DrawableUtilities::range(0, numEdges, 1);

	// Assign edge smoothing values
	//
	*status = fnMesh.setEdgeSmoothings(elements, smoothings);

	if (!status) 
	{

		return meshData;

	}

	// Cleanup edge smoothings
	//
	*status = fnMesh.cleanupEdgeSmoothing();

	return meshData;

};


MObject DrawableUtilities::copyMeshData(MObject& source, MMatrix matrix, MStatus* status)
/**
Copies the supplied mesh data while simultaneously transforming the control points.

@param source: The source mesh data to copy.
@param matrix: The transformation matrix.
@param status: Return status.
@return: The copied mesh data.
*/
{

	// Create new mesh data object
	//
	MFnMeshData fnMeshData;
	MObject meshData = fnMeshData.create(status);

	if (!status)
	{

		return meshData;

	}

	// Copy source mesh to data object
	//
	MFnMesh fnMesh;
	fnMesh.copy(source, meshData, status);

	if (!status)
	{

		return meshData;

	}

	// Get control points
	//
	unsigned int numVertices = fnMesh.numVertices();

	MPointArray points = MPointArray(numVertices);
	*status = fnMesh.getPoints(points);

	if (!status)
	{

		return meshData;

	}

	// Modify control points
	//
	DrawableUtilities::transform(matrix, points);
	*status = fnMesh.setPoints(points);

	return meshData;

};


MObject DrawableUtilities::createComponent(MFn::Type type, MIntArray& elements, MStatus* status)
/**
Static function used to create a component object from a list of elements.

@param type: Component type to create.
@param elements: List of indexed elements to add to component.
@param status: Return status.
@return: MObject
*/
{

	// Initialize component object
	//
	MFnSingleIndexedComponent fnSingleIndexedComponent;
	MObject component = fnSingleIndexedComponent.create(type, status);

	if (!status) 
	{

		return component;

	}

	// Add elements to component
	//
	*status = fnSingleIndexedComponent.addElements(elements);

	return component;

};


MObject DrawableUtilities::createComponent(MFn::Type type, int count, MStatus* status)
/**
Static function used to create a component object from a list of elements.

@param type: Component type to create.
@param elements: List of indexed elements to add to component.
@param status: Return status.
@return: MObject
*/
{

	// Initialize component object
	//
	MFnSingleIndexedComponent fnSingleIndexedComponent;
	MObject component = fnSingleIndexedComponent.create(type, status);

	if (!status) 
	{

		return component;

	}

	// Add elements to component
	//
	MIntArray elements = DrawableUtilities::range(0, count, 1);
	*status = fnSingleIndexedComponent.addElements(elements);

	return component;

};


MStatus DrawableUtilities::getPoints(MObject& meshData, MPointArray& points)
/**
Static function used to populate a point array from a mesh data object.

@param meshData: Mesh data object to copy points from.
@param matrix: Point array to copy points to.
@return: Status.
*/
{

	MStatus status;

	// Initialize function set
	//
	MFnMesh fnMesh(meshData, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// Copy point data over to array
	//
	int numVertices = fnMesh.numVertices(&status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	status = fnMesh.getPoints(points);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	return MS::kSuccess;

};


MStatus DrawableUtilities::getLines(MObject& meshData, MPointArray& lines)
/**
Static function used to populate a point array from a mesh data object.

@param meshData: Mesh data object to copy points from.
@param matrix: Point array to copy points to.
@return: Status.
*/
{

	MStatus status;

	// Initialize function set
	//
	MFnMesh fnMesh(meshData, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// Create complete component
	//
	int numEdges = fnMesh.numEdges(&status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	MObject component = DrawableUtilities::createComponent(MFn::kMeshEdgeComponent, numEdges, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// Call overload function
	//
	return DrawableUtilities::getLines(meshData, component, lines);

};


MStatus DrawableUtilities::getLines(MObject& meshData, MObject& component, MPointArray& lines)
/**
Static function used to populate a point array from a mesh data and component pair.

@param meshData: Mesh data object to copy points from.
@param matrix: Point array to copy points to.
@return: Status.
*/
{

	MStatus status;

	// Initialize function set
	//
	MItMeshEdge iterEdges(meshData, component, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// Resize array to fit points
	//
	int numEdges = iterEdges.count(&status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	int length = numEdges * 2;

	status = lines.setLength(length);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// Iterate through edges
	//
	int insertAt, count = 0;
	MPoint start, end;

	for (iterEdges.reset(); !iterEdges.isDone(); iterEdges.next(), count++) 
	{

		// Get start and end points
		//
		start = iterEdges.point(0, MSpace::kObject, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		end = iterEdges.point(1, MSpace::kObject, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		// Add points to array
		//
		insertAt = count * 2;

		lines[insertAt] = start;
		lines[insertAt + 1] = end;

	}

	return MS::kSuccess;

};


MStatus DrawableUtilities::getLines(MObject& meshData, MIntArray& elements, MPointArray& lines)
/**
Static function used to populate a point array from a mesh data and edge elements pair.

@param meshData: Mesh data object to copy points from.
@param matrix: Point array to copy points to.
@return: Status.
*/
{

	MStatus status;

	// Create edge component
	//
	MObject component = DrawableUtilities::createComponent(MFn::kMeshEdgeComponent, elements, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// Call overload
	//
	return DrawableUtilities::getLines(meshData, component, lines);

}


MStatus DrawableUtilities::getTriangles(MObject& meshData, MPointArray& triangles, MVectorArray& normals)
/**
Static function used to populate a point array from a mesh data object.

@param meshData: Mesh data object to copy points from.
@param matrix: Point array to copy points to.
@return: Status.
*/
{

	MStatus status;

	// Initialize function set
	//
	MFnMesh fnMesh(meshData, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// Get triangles from mesh data
	//
	MIntArray triangleCounts, triangleVertices;

	status = fnMesh.getTriangles(triangleCounts, triangleVertices);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	MPointArray points;

	status = fnMesh.getPoints(points);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// Resize trianlge arrays
	//
	int triangleSum = DrawableUtilities::sum(triangleCounts);
	int length = triangleSum * 3;

	status = triangles.setLength(length);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	status = normals.setLength(length);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// Iterate through polygons
	//
	MItMeshPolygon iterPolygons(meshData, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	int polygonIndex, triangleCount;
	int index0, index1, index2;
	int counter = 0;

	for (iterPolygons.reset(); !iterPolygons.isDone(); iterPolygons.next()) 
	{

		// Iterate through triangles
		//
		polygonIndex = iterPolygons.index();
		triangleCount = triangleCounts[polygonIndex];

		for (int triangleIndex = 0; triangleIndex < triangleCount; triangleIndex++) {

			// Update triangle points
			//
			index0 = counter * 3;
			index1 = index0 + 1;
			index2 = index0 + 2;

			triangles[index0] = points[triangleVertices[index0]];
			triangles[index1] = points[triangleVertices[index1]];
			triangles[index2] = points[triangleVertices[index2]];

			// Update triangle normals
			//
			status = fnMesh.getFaceVertexNormal(polygonIndex, triangleVertices[index0], normals[index0], MSpace::kObject);
			CHECK_MSTATUS_AND_RETURN_IT(status);

			status = fnMesh.getFaceVertexNormal(polygonIndex, triangleVertices[index1], normals[index1], MSpace::kObject);
			CHECK_MSTATUS_AND_RETURN_IT(status);

			status = fnMesh.getFaceVertexNormal(polygonIndex, triangleVertices[index2], normals[index2], MSpace::kObject);
			CHECK_MSTATUS_AND_RETURN_IT(status);

			// Increment insertion index
			//
			counter += 1;

		}

	}

	return MS::kSuccess;

};


MStatus DrawableUtilities::getBoundary(MObject& meshData, MIntArray& boundary)
/**
Static function used to populate an element array from a mesh data object.
Since passed arrays can be reused this function has been structured to only resize arrays when needed.

@param meshData: Mesh data object to copy points from.
@param matrix: Point array to copy points to.
@return: Return status.
*/
{

	MStatus status;

	// Initialize iterator
	//
	MItMeshEdge iterEdges(meshData, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// Set length to maximum possible number of edges
	//
	int count = iterEdges.count(&status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	status = boundary.setLength(count);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// Iterate through elements
	//
	bool onBoundary;
	MPoint start, end;

	int insertAt = 0;

	for (iterEdges.reset(); !iterEdges.isDone(); iterEdges.next()) 
	{

		// Check if edge is on boundary
		//
		onBoundary = iterEdges.onBoundary(&status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		if (onBoundary) 
		{

			boundary[insertAt] = iterEdges.index(&status);
			CHECK_MSTATUS_AND_RETURN_IT(status);

			insertAt += 1;

		}

	}

	// Check if array requires pruning
	//
	if (insertAt < count) {

		status = boundary.setLength(insertAt);
		CHECK_MSTATUS_AND_RETURN_IT(status);

	}

	return MS::kSuccess;

};


MStatus DrawableUtilities::autoSmoothEdges(MObject& meshData)
/**
Auto smooths the edges on the supplied mesh based on a 45 degree angle.

@param meshData: The mesh data object to auto smooth.
@return: MStatus
*/
{

	MStatus status;

	// Initialize iterator
	//
	MItMeshEdge iterEdges(meshData, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// Iterate through edges
	//
	int numConnectedFaces;
	MIntArray connectedFaces;

	double normalAngle;

	for (iterEdges.reset(); !iterEdges.isDone(); iterEdges.next()) 
	{

		// Get connected faces
		//
		numConnectedFaces = iterEdges.getConnectedFaces(connectedFaces, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		switch (numConnectedFaces) 
		{

			case 1: 
			{

				// Assume hard edge for boundaries
				//
				iterEdges.setSmoothing(false);
				break;

			}

			case 2: 
			{

				// Calculate normal difference to determine edge smoothing
				//
				normalAngle = DrawableUtilities::getFaceNormalDifference(meshData, connectedFaces[0], connectedFaces[1], &status);
				CHECK_MSTATUS_AND_RETURN_IT(status);

				if (normalAngle > 45.0) 
				{

					iterEdges.setSmoothing(false);

				}
				else 
				{

					iterEdges.setSmoothing(true);

				}

				break;

			}
			
			default: 
			{

				break;

			}

		}

	}

	return MS::kSuccess;

};


double DrawableUtilities::getFaceNormalDifference(MObject& meshData, int polygonIndex, int otherPolygonIndex, MStatus* status)
/**
Gets the angle between two face normals.

@param meshData: Mesh data object.
@param polygonIndex: A polygon index.
@param otherPolygonIndex: The other polygon index.
@param status: MStatus
@return: Angle in degrees.
*/
{

	// Initialize function set
	//
	double angle = 0.0;
	MFnMesh fnMesh(meshData, status);

	if (!status) 
	{

		return angle;

	}

	// Get polygon normal
	//
	MVector normal;
	*status = fnMesh.getPolygonNormal(polygonIndex, normal, MSpace::kObject);

	if (!status) 
	{

		return angle;

	}

	// Get other polygon normal
	//
	MVector otherNormal;
	*status = fnMesh.getPolygonNormal(otherPolygonIndex, otherNormal, MSpace::kObject);

	if (!status) 
	{

		return angle;

	}

	// Return angle in degrees
	//
	return normal.angle(otherNormal) * (180.0 / PI);

};