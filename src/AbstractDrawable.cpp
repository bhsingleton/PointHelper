//
// File: AbstractDrawable.cpp
//
// Author: Ben Singleton
//

#include "AbstractDrawable.h"

using namespace Drawable;


AbstractDrawable::AbstractDrawable()
/**
Constructor.
*/
{

	this->enabled = false;
	this->objectMatrix = MMatrix::identity;
	this->forward = MVector::xAxis;
	this->right = MVector::zAxis;
	this->up = MVector::yAxis;
	this->center = MPoint::origin;
	this->scale = MVector::one;

};


AbstractDrawable::~AbstractDrawable() {};


void AbstractDrawable::setObjectMatrix(MMatrix objectMatrix)
/**
Updates the object matrix for this drawable.
Additional data will be extrapolated such as axis vectors and scaling.

@param objectMatrix: New matrix value.
@return: void
*/
{

	// Store object matrix
	//
	this->objectMatrix = objectMatrix;

	// Decompose object-matrix
	//
	Drawable::decomposeMatrix(this->objectMatrix, this->forward, this->up, this->right, this->center);
	Drawable::decomposeScale(this->objectMatrix, this->scale);

};


MMatrix AbstractDrawable::getObjectMatrix()
/**
Retrieves the object matrix assigned to this drawable.

@return: MMatrix
*/
{

	return this->objectMatrix;

};


void AbstractDrawable::setEnabled(bool enabled)
/**
Updates the enabled state of this drawable.

@return: void
*/
{

	this->enabled = enabled;

};


bool AbstractDrawable::isEnabled()
/**
Determines if this drawable is enabled.

@return: bool
*/
{

	return this->enabled;

};