#ifndef _DRAWABLE_UTILITIES
#define _DRAWABLE_UTILITIES
//
// File: DrawableUtilities.h
//
// Author: Ben Singleton
//

#include <maya/MFn.h>
#include <maya/MFnMesh.h>
#include <maya/MFnMeshData.h>
#include <maya/MFnSingleIndexedComponent.h>

#include <maya/MItMeshVertex.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshEdge.h>

#include <maya/MObject.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MIntArray.h>
#include <maya/MVector.h>
#include <maya/MVectorArray.h>
#include <maya/MQuaternion.h>
#include <maya/MEulerRotation.h>
#include <maya/MMatrix.h>
#include <maya/MTransformationMatrix.h>

#include <vector>
#include <numeric>
#include <map>

constexpr auto PI = 3.141592653589793238462643383279502884197169399375105820974944592307816406286;
constexpr auto UNIT_SQUARE_RADIUS = 0.70710678118654752440084436210485;
constexpr auto MERGE_THRESHOLD = 0.001;


class DrawableUtilities 
{

private:

							DrawableUtilities() {};
	virtual					~DrawableUtilities() {};

public:

	static	void			transform(MMatrix matrix, MPointArray& points);
	static	MPointArray		transform(MMatrix matrix, const double points[][4], int numPoints);

	static	unsigned int	sum(const MIntArray& values);
	static	MIntArray		range(int start, int end, int increment);

	static	MPointArray		chain(std::vector<MPointArray>& points);

	static	MPointArray		stagger(MPointArray& points);
	static	MPointArray		stagger(std::vector<MPointArray>& points);

	static	MMatrix			composeMatrix(MVector center, MVector normal, MVector up, double scale[3]);
	static	MMatrix			composeMatrix(MVector center, MEulerRotation eulerRotation, const double scale[3]);
	static	MMatrix			composeMatrix(MEulerRotation eulerRotation);

	static	void			decomposeMatrix(MMatrix matrix, MVector& translation, MEulerRotation& eulerRotation, double3& scale);
	static	void			decomposeMatrix(MMatrix matrix, MVector& xAxis, MVector& yAxis, MVector& zAxis, MPoint& position);

	static	MPointArray		line(MPoint start, MPoint end);
	static	MPointArray		arc(MVector center, MVector normal, double radius, double startAngle, double endAngle, int numPoints);
	static	MPointArray		square(MVector center, MVector normal);
	static	MPointArray		circle(MVector center, MVector normal, double radius, int numPoints);

	static	MObject			sphere(MVector center, double radius, int subdivisionAxis, int subdivisionHeight, MObject& parent);
	static	MObject			cylinder(MVector center, MVector normal, double radius, double length, int subdivisionAxis, MObject& parent);
	static	MObject			disc(MVector center, MVector normal, double radius, int subdivisionAxis, MObject& parent);
	static	MObject			sector(MVector center, MVector normal, double radius, double startAngle, double endAngle, int subdivisionAxis, MObject& parent);

	static	MObject			createMeshData(MPointArray points, MIntArray polygonCounts, MIntArray polygonConnects, MStatus* status);
	static	MObject			createMeshData(MPointArray points, MIntArray polygonCounts, MIntArray polygonConnects, MIntArray edgeSmoothings, MStatus* status);
	static	MObject			copyMeshData(MObject& source, MMatrix matrix, MStatus* status);

	static	MObject			createComponent(MFn::Type type, MIntArray& elements, MStatus* status);
	static	MObject			createComponent(MFn::Type type, int count, MStatus* status);

	static	MStatus			getPoints(MObject& meshData, MPointArray& points);

	static	MStatus			getBoundary(MObject& meshData, MIntArray& boundary);

	static	MStatus			getLines(MObject& meshData, MPointArray& lines);
	static	MStatus			getLines(MObject& meshData, MIntArray& elements, MPointArray& lines);
	static	MStatus			getLines(MObject& meshData, MObject& component, MPointArray& lines);

	static	MStatus			getTriangles(MObject& meshData, MPointArray& triangles, MVectorArray& normals);

	static	MStatus			autoSmoothEdges(MObject& meshData);
	static	double			getFaceNormalDifference(MObject& meshData, int polygonIndex, int otherPolygonIndex, MStatus* status);

	static	MTransformationMatrix::RotationOrder	rotationOrder(MEulerRotation::RotationOrder rotationOrder);
	static	MEulerRotation::RotationOrder			rotationOrder(MTransformationMatrix::RotationOrder rotationOrder);

};

#endif