#ifndef SHAPEINTRPLCMD_H
#define SHAPEINTRPLCMD_H

#include <vector>

#include <maya/MPxCommand.h>
#include <maya/MDGModifier.h>
#include <maya/MDaGPath.h>

class ShapeIntrpltCmd : public MPxCommand 
{
public:
	virtual MStatus	doIt ( const MArgList& );
	virtual MStatus undoIt();
 	virtual MStatus redoIt();
	virtual bool isUndoable() const { return true; }

	static void *creator() { return new ShapeIntrpltCmd; }
	static MSyntax newSyntax();

private:
	MDGModifier dgMod;

	// The stack of undo to perform.
	std::vector<MDGModifier*> mUndo;

	// The intermediate mesh.
	MObject newMesh;
	bool doMeshUndo;
};

#endif