#include "ShapeIntrpltNode.h"
#include "ShapeIntrpltCmd.h"
#include <maya/MFnPlugin.h>

MStatus initializePlugin(MObject obj)
{
	MStatus stat;
	MString errStr;
	MFnPlugin plugin(obj);

	stat = plugin.registerCommand("shapeIntrpl", ShapeIntrpltCmd::creator);
	if (!stat)
	{
		errStr = "registerCommand failed";
		goto error;
	}

	stat = plugin.registerNode("intrpl", ShapeIntrpltNode::id,
		ShapeIntrpltNode::creator, ShapeIntrpltNode::initialize);
	if (!stat)
	{
		errStr = "registerNode failed";
		goto error;
	}

	return stat;

error:

	stat.perror(errStr);
	return stat;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus stat;
	MString errStr;
	MFnPlugin plugin(obj);

	stat = plugin.deregisterCommand("shapeIntrpl");
	if (!stat)
	{
		errStr = "deregisterCommand failed";
		goto error;
	}

	stat = plugin.deregisterNode(ShapeIntrpltNode::id);
	if (!stat)
	{
		errStr = "deregisterNode failed";
		goto error;
	}

	return stat;

error:

	stat.perror(errStr);
	return stat;
}
