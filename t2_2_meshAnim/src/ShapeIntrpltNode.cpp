#include "ShapeIntrpltNode.h"

#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MGlobal.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNurbsSurfaceData.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MPointArray.h>
#include <maya/MAngle.h>
#include <maya/MItGeometry.h>
#include <maya/MMatrix.h>
#include <assert.h>
#include <float.h>

const MTypeId ShapeIntrpltNode::id(0x00334);

MObject ShapeIntrpltNode::inputSurface;
MObject ShapeIntrpltNode::outputSurface;
      
MStatus ShapeIntrpltNode::compute( const MPlug& plug, MDataBlock& data )
{
	MStatus stat;

	// If asked for the outputSurface then compute it
	if (plug == outputSurface)
	{
		// Get handlers for the two meshes
		MDataHandle inputSurfaceHnd = data.inputValue(inputSurface);
		MDataHandle outputSurfaceHnd = data.outputValue(outputSurface);

		// Copy input into output
		outputSurfaceHnd.copy(inputSurfaceHnd);

		// Dummy interpolation, just translate each vertex 1 on x
		MPoint surfPoint, translation(1, 0, 0);
		MItGeometry iter(outputSurfaceHnd, false);
		for (; !iter.isDone(); iter.next())
		{
			surfPoint = iter.position(MSpace::kWorld);
			iter.setPosition(surfPoint + translation, MSpace::kWorld);
		}

		// Tell de DG that we have updated the outputSurface
		data.setClean(plug);
	}
	else
		stat = MS::kUnknownParameter;

	return stat;
}

void *ShapeIntrpltNode::creator()
{
return new ShapeIntrpltNode();
}

MStatus ShapeIntrpltNode::initialize()
{
MFnTypedAttribute tAttr;

inputSurface = tAttr.create("inputSurface", "is", MFnData::kMesh);
tAttr.setHidden( true );

outputSurface = tAttr.create("outputSurface", "os", MFnData::kMesh);
tAttr.setStorable( false );
tAttr.setHidden( true );

addAttribute( inputSurface );
addAttribute( outputSurface );

attributeAffects( inputSurface, outputSurface );

return MS::kSuccess;
}