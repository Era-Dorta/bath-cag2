#ifndef EXTRAFN_H
#define EXTRAFN_H

#include <string.h>
#include <iostream>

#include <maya/MTypes.h>
#include <maya/MString.h>
#include <maya/MGlobal.h>

inline void displayError(const char* msg) {
	std::cerr << std::endl << "Error: " << MString(msg);
}

inline void checkBool(bool result) {
	if (!result)  {
		displayError("bool");
	}
}

inline void checkStat(const MStatus& stat, const char* msg) {
	if (stat.error()) {
		displayError(msg);
	}
}

inline void checkObject(const MObject& obj, const char* msg) {
	if (obj.isNull()) {
		displayError(msg);
	}
}

#endif