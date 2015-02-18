#ifndef SHAPEINTRPLCMD_H
#define SHAPEINTRPLCMD_H

#include <maya/MPxCommand.h>
#include <maya/MDaGModifier.h>

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
	MDagModifier dagMod;
};

#endif