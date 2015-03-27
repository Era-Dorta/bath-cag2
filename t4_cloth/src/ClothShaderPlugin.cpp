/*
 * ClothShaderPlugin.cpp
 *
 *  Created on: 27 Mar 2015
 *      Author: gdp24
 */

#include <math.h>

#include <maya/MFnPlugin.h>
#include <maya/MDrawRegistry.h>

#include "ClothNode.h"
#include "ClothShaderOverride.h"

static const MString sRegistrantId("ClothShaderPlugin");

//
// DESCRIPTION:
///////////////////////////////////////////////////////
MStatus initializePlugin(MObject obj) {
	const MString UserClassify(
			"shader/surface:drawdb/shader/surface/" + ClothNode::m_TypeName);

	MFnPlugin plugin(obj, PLUGIN_COMPANY, "4.5", "Any");
	CHECK_MSTATUS(
			plugin.registerNode(ClothNode::m_TypeName, ClothNode::id, ClothNode::creator,
					ClothNode::initialize, MPxNode::kDependNode, &UserClassify));

	CHECK_MSTATUS(
			MHWRender::MDrawRegistry::registerSurfaceShadingNodeOverrideCreator(
					"drawdb/shader/surface/" + ClothNode::m_TypeName, sRegistrantId,
					ClothShaderOverride::creator));

	return MS::kSuccess;
}

//
// DESCRIPTION:
///////////////////////////////////////////////////////
MStatus uninitializePlugin(MObject obj) {
	MFnPlugin plugin(obj);
	CHECK_MSTATUS(plugin.deregisterNode(ClothNode::id));

	CHECK_MSTATUS(
			MHWRender::MDrawRegistry::deregisterSurfaceShadingNodeOverrideCreator(
					"drawdb/shader/surface/" + ClothNode::m_TypeName, sRegistrantId));

	return MS::kSuccess;
}
