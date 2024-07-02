//
// File: PointHelperDrawOverride.cpp
//
// Author: Ben Singleton
//

#include "PointHelperDrawOverride.h"

#include "Arrow.h"
#include "AxisTripod.h"
#include "AxisView.h"
#include "Box.h"
#include "CenterMarker.h"
#include "Cross.h"
#include "Custom.h"
#include "Cylinder.h"
#include "Triangle.h"
#include "Pyramid.h"
#include "Diamond.h"
#include "Disc.h"
#include "Notch.h"
#include "Sphere.h"
#include "Square.h"
#include "TearDrop.h"


PointHelperDrawOverride::PointHelperDrawOverride(const MObject& node) : MPxDrawOverride(node, NULL, false)
/**
Constructor.

@param node: The Maya object this override draws.
@param callback: The callback function to be invoked at draw time.
@param isAlwaysDirty: If true, this override will always be updated via prepareForDraw() or addUIDrawables() without checking the dirty state of the Maya object.
*/
{

	MStatus status;

	// Create model editor callback
	//
	this->modelEditorChangedCallbackId = MEventMessage::addEventCallback("modelEditorChanged", PointHelperDrawOverride::onModelEditorChanged, this, &status);
	CHECK_MSTATUS(status);

	// Store pointer to MPxLocator
	// This will be useful for getting plug data to pass into our MPxUserData class
	//
	MFnDependencyNode fnNode(node, &status);
	CHECK_MSTATUS(status);

	this->pointHelper = status ? dynamic_cast<PointHelper*>(fnNode.userNode()) : nullptr;

	// Check if drawables have been initialized
	//
	
	size_t numItems = this->drawables.size();

	if (numItems == 0 && this->pointHelper != nullptr)
	{

		// Create new dicationary
		//
		this->drawables = std::map<std::string, Drawable::AbstractDrawable*>();

		// Add drawables to dictionary
		//
		this->drawables.insert(std::make_pair("arrow", new Arrow()));
		this->drawables.insert(std::make_pair("axisTripod", new AxisTripod()));
		this->drawables.insert(std::make_pair("axisView", new AxisView()));
		this->drawables.insert(std::make_pair("box", new Box()));
		this->drawables.insert(std::make_pair("centerMarker", new CenterMarker()));
		this->drawables.insert(std::make_pair("cross", new Cross()));
		this->drawables.insert(std::make_pair("custom", new Custom()));
		this->drawables.insert(std::make_pair("cylinder", new Cylinder()));
		this->drawables.insert(std::make_pair("triangle", new Triangle()));
		this->drawables.insert(std::make_pair("pyramid", new Pyramid()));
		this->drawables.insert(std::make_pair("diamond", new Diamond()));
		this->drawables.insert(std::make_pair("disc", new Disc()));
		this->drawables.insert(std::make_pair("notch", new Notch()));
		this->drawables.insert(std::make_pair("sphere", new Sphere()));
		this->drawables.insert(std::make_pair("square", new Square()));
		this->drawables.insert(std::make_pair("tearDrop", new TearDrop()));

	}
	
};


PointHelperDrawOverride::~PointHelperDrawOverride()
/**
Destructor.
*/
{

	// Clear pointer and drawables
	//
	this->pointHelper = NULL;
	this->drawables.clear();

	// Remove model editor callback
	//
	if (this->modelEditorChangedCallbackId != 0) 
	{

		MMessage::removeCallback(this->modelEditorChangedCallbackId);
		this->modelEditorChangedCallbackId = 0;

	}

};


MHWRender::MPxDrawOverride* PointHelperDrawOverride::creator(const MObject& node)
/**
Static function used to create a new draw override instance.
This function is called via the MDrawRegistry::registerDrawOverrideCreator() method.

@param node: The Maya object this override draws. 
@return: MPxDrawOverride*
*/
{

	return new PointHelperDrawOverride(node);

};


void PointHelperDrawOverride::onModelEditorChanged(void* clientData)
/**
Callback function used to mark this locator as dirty whenever a model editor change has occurred.
This includes things like toggling wireframe mode, etc.
To explicitly mark an object as being dirty the MRenderer::setGeometryDrawDirty() method can be used!

@param clientData: Pointer to the draw override instance.
@return: void
*/
{

	// Check if client data is null
	//
	if (clientData == NULL)
	{

		return;

	}

	// Check if point helper is null
	//
	PointHelperDrawOverride *drawOverride = static_cast<PointHelperDrawOverride*>(clientData);

	if (drawOverride->pointHelper == NULL)
	{

		return;

	}

	// Mark the node as being dirty so that it can update when the display appearance switches between wireframe and shaded
	//
	MHWRender::MRenderer::setGeometryDrawDirty(drawOverride->pointHelper->thisMObject());

};


MHWRender::DrawAPI PointHelperDrawOverride::supportedDrawAPIs() const
/**
Returns the draw API supported by this override.
The returned value may be formed as the bitwise or of MHWRender::DrawAPI elements to indicate that the override supports multiple draw APIs. 
This method returns MHWRender::kOpenGL by default.

@return: MHWRender::DrawAPI
*/
{

	return (MHWRender::kOpenGL | MHWRender::kDirectX11 | MHWRender::kOpenGLCoreProfile);

};


bool PointHelperDrawOverride::isBounded(const MDagPath& objPath, const MDagPath& cameraPath) const
/**
Called by Maya to determine if the drawable object is bounded or not.
If the object is not bounded then it will never be culled by the current camera frustum used for drawing.
The default implementation will always return true. This method can be overridden in derived classes to customize the behaviour.
Note that if this method returns false then the boundingBox() method will not be called as no bounds are required in this case.

@param objPath: The path to the object being drawn.
@param cameraPath: The path to the camera that is being used to draw.
@return: bool
*/
{

	return this->pointHelper->isBounded();

};


MBoundingBox PointHelperDrawOverride::boundingBox(const MDagPath& objPath, const MDagPath& cameraPath) const
/**
Called by Maya whenever the bounding box of the drawable object is needed.
This method should return the object space bounding box for the object to be drawn.
Note that this method will not be called if the isBounded() method returns a value of false.
Default implementation returns a huge bounding box which will never cull the object.

@param objPath: The path to the object being drawn.
@param cameraPath: The path to the camera that is being used to draw.
@return: MBoundingBox
*/
{

	return this->pointHelper->boundingBox();

};


bool PointHelperDrawOverride::hasUIDrawables() const
/**
In order for any override for the addUIDrawables() method to be called this method must also be overridden to return true.
This method should not be overridden if addUIDrawables() has not also been overridden as there may be associated wasted overhead.

@return: bool
*/
{

	return true;

};


MUserData* PointHelperDrawOverride::prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, MUserData* userData)
/**
Called by Maya whenever the object is dirty and needs to update for draw.
Any data needed from the Maya dependency graph must be retrieved and cached in this stage. 
It is invalid to pull data from the Maya dependency graph in the draw callback method and Maya may become unstable if that is attempted.

@param objPath: The path to the object being drawn.
@param cameraPath: The path to the camera that is being used to draw.
@param frameContext: Frame level context information.
@param userData: Data cached by the previous draw of the instance.
@return: MUserData
*/
{

	MStatus status;

	// Check if an instance of PointHelperData exists
	//
	PointHelperData* pointHelperData = dynamic_cast<PointHelperData*>(userData);

	if (pointHelperData == nullptr)
	{

		pointHelperData = new PointHelperData();
		
	}

	// Copy internal values
	//
	this->pointHelper->copyInternalData(pointHelperData);

	pointHelperData->copyWireColor(objPath);
	pointHelperData->copyDepthPriority(objPath);

	// Iterate through drawable attributes
	//
	Drawable::AbstractDrawable* drawable;
	std::map<std::string, bool>::iterator iter;

	for (iter = pointHelperData->drawables.begin(); iter != pointHelperData->drawables.end(); iter++)
	{
		
		// Check if drawable is enabled
		//
		drawable = this->drawables[iter->first];
		drawable->setEnabled(iter->second);

		if (iter->second)
		{

			drawable->setObjectMatrix(pointHelperData->objectMatrix);
			drawable->prepareForDraw(objPath, cameraPath, frameContext, pointHelperData);

		}
		else
		{

			continue;

		}
		
	}

	return pointHelperData;

};


void PointHelperDrawOverride::addUIDrawables(const MDagPath& objPath, MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const MUserData* userData)
/**
Provides access to the MUIDrawManager, which can be used to queue up operations to draw simple UI shapes like lines, circles, text, etc.
This method will only be called when this override is dirty and its hasUIDrawables() is overridden to return true. 
This method is called after prepareForDraw() and carries the same restrictions on the sorts of operations it can perform.

@param objPath: The path to the object being drawn.
@param cameraPath: The path to the camera that is being used to draw.
@param drawManager: The UI draw manager, it can be used to draw some simple geometry including text.
@param frameContext: Frame level context information.
@param userData: Data cached by prepareForDraw().
@return: void
*/
{

	// Retrieve the cached data from the prepareForDraw() method
	//
	PointHelperData* pointHelperData = (PointHelperData*)userData;

	if (!pointHelperData) 
	{

		return;

	}

	// Iterate through drawables
	//
	bool isEnabled;

	for (auto const& x : this->drawables) 
	{

		// Check if drawable is enabled
		//
		isEnabled = x.second->isEnabled();

		if (!isEnabled) 
		{

			continue;

		}

		// Begin drawable
		//
		drawManager.beginDrawable();

		// Edit draw properties
		//
		drawManager.setColor(pointHelperData->wireColor);
		drawManager.setDepthPriority(pointHelperData->depthPriority);
		drawManager.setLineWidth(pointHelperData->lineWidth);
		drawManager.setLineStyle(MHWRender::MUIDrawManager::kSolid);

		if (pointHelperData->drawOnTop) 
		{

			drawManager.beginDrawInXray();

		}

		// Request draw
		//
		x.second->draw(drawManager, frameContext, pointHelperData);

		if (pointHelperData->drawOnTop) 
		{

			drawManager.endDrawInXray();

		}

		// End drawable
		//
		drawManager.endDrawable();

	}

	// Check if text is valid
	//
	if (pointHelperData->text.length() > 0) 
	{

		// Begin drawable
		//
		drawManager.beginDrawable();

		// Edit draw properties
		//
		drawManager.setColor(pointHelperData->wireColor);
		drawManager.setDepthPriority(pointHelperData->depthPriority);
		drawManager.setFontSize(pointHelperData->fontSize);

		// Draw text
		//
		drawManager.text(MPoint::origin, pointHelperData->text, MHWRender::MUIDrawManager::TextAlignment::kCenter);

		// End drawable
		//
		drawManager.endDrawable();

	}

};


bool PointHelperDrawOverride::traceCallSequence() const
/**
This method allows a way for a plug-in to examine the basic call sequence for a draw override.
The default implementation returns false meaning no tracing will occur.

@return: bool
*/
{

	return false; // Toggle for debugging!!!

};


void PointHelperDrawOverride::handleTraceMessage(const MString& message) const
/**
When debug tracing is enabled via MPxDrawOverride::traceCallSequence(), this method will be called for each trace message.
The default implementation will print the message to stderr.

@param message: A string which will provide feedback on either an internal or plug-in call location.
@return: void
*/
{

	MGlobal::displayInfo("PointHelperDrawOverride::handleTraceMessage() : " + message);

}