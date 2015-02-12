#include <memory>

#include <maya/MIOStream.h>
#include <maya/MPxCommand.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>

#include "ErrorCheck.h"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT " " __FILE__ ": " TOSTRING(__LINE__) 

/*DeclareSimpleCommand(HelloMaya, PLUGIN_COMPANY, "4.5");

MStatus HelloMaya::doIt(const MArgList&){
MGlobal::displayInfo("Hello this is a maya plugin test 1");

return MS::kSuccess;
}*/

class HelloMaya : public MPxCommand{
public:
	virtual MStatus doIt(const MArgList&){
		std::shared_ptr<int> var = std::make_shared<int>(2);
		MString txt("Hello World ");
		txt += *var;
		MGlobal::displayInfo(txt);
		return MS::kSuccess;
	}
	static void *creator(){
		return new HelloMaya;
	}
};

MStatus initializePlugin(MObject obj) {
	// obj is a handler to the plugin, assign it to a function class to be able
	// to call methods on it
	MFnPlugin pluginFn(obj, "HelloMaya", "1.0");
	MStatus stat;
	// Set command with name HelloMaya that executes creator method 
	stat = pluginFn.registerCommand("HelloMaya", HelloMaya::creator);
	checkStatus(MS::kFailure, "registerCommand failed" AT);
	//if (stat.error()) {
	// Will print on the output window 
	// registerCommand failed: <the type of error>
	//stat.perror("registerCommand failed");
	//}
	return stat;
}

MStatus uninitializePlugin(MObject obj) {
	MFnPlugin pluginFn(obj);
	MStatus stat;
	stat = pluginFn.deregisterCommand("HelloMaya");
	checkStatus(stat, "deregisterCommand failed" AT);
	checkObject(obj, "object" AT);
	//if (stat.error()) {
	//	stat.perror("deregisterCommand failed");
	//}
	return stat;
}