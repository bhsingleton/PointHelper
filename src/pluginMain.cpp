//
// File: pluginMain.cpp
//
// Author: Ben Singleton
//

#include "PointHelperNode.h"
#include "PointHelperDrawOverride.h"

#include <maya/MFnPlugin.h>
#include <maya/MDrawRegistry.h>


MStatus initializePlugin(MObject obj) 
{ 

	MStatus   status;

	MFnPlugin plugin(obj, "Ben Singleton", "2017", "Any");
	status = plugin.registerNode("pointHelper", PointHelper::id, &PointHelper::creator, &PointHelper::initialize, MPxNode::kLocatorNode, &PointHelper::drawDbClassification);
	
	if (!status) 
	{

		status.perror("registerNode");
		return status;

	}

	status = MHWRender::MDrawRegistry::registerDrawOverrideCreator(PointHelper::drawDbClassification, PointHelper::drawRegistrantId, PointHelperDrawOverride::creator);

	if (!status) 
	{

		status.perror("registerDrawOverrideCreator");
		return status;

	}

	return status;

}

MStatus uninitializePlugin(MObject obj) 
{

	MStatus   status;

	status = MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(PointHelper::drawDbClassification, PointHelper::drawRegistrantId);

	if (!status) 
	{

		status.perror("deregisterDrawOverrideCreator");
		return status;

	}

	MFnPlugin plugin(obj);
	status = plugin.deregisterNode(PointHelper::id);

	if (!status) 
	{

		status.perror("deregisterNode");
		return status;

	}

	return status;

}
