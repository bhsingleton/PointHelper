//
// File: Drawable.cpp
//
// Author: Ben Singleton
//

#include "Drawable.h"


Drawable::Drawable() {}
Drawable::~Drawable() {}


void Drawable::setObjectMatrix(MMatrix objectMatrix)
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

	// Decompose matrix
	//
	this->center = MVector(this->objectMatrix[3]);
	this->up = MVector(this->objectMatrix[0]).normal();
	this->right = MVector(this->objectMatrix[1]).normal();

	this->scale[0] = MVector(this->objectMatrix[0]).length() / 2.0;
	this->scale[1] = MVector(this->objectMatrix[1]).length() / 2.0;
	this->scale[2] = MVector(this->objectMatrix[2]).length() / 2.0;

};


MMatrix Drawable::getObjectMatrix()
/**
Retrieves the object matrix assigned to this drawable.

@return: MMatrix
*/
{

	return this->objectMatrix;

};


void Drawable::setEnabled(bool enabled)
/**
Updates the enabled state of this drawable.

@return: void
*/
{

	this->enabled = enabled;

};


bool Drawable::isEnabled()
/**
Determines if this drawable is enabled.

@return: bool
*/
{

	return this->enabled;

};