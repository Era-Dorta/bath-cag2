/*
 * ClothNode.h
 *
 *  Created on: 27 Mar 2015
 *      Author: gdp24
 */

#ifndef SRC_CLOTHNODE_H_
#define SRC_CLOTHNODE_H_

#include <maya/MPxNode.h>

class ClothNode: public MPxNode {
public:
	ClothNode();
	virtual ~ClothNode();

	virtual MStatus compute(const MPlug&, MDataBlock&);
	virtual void postConstructor();

	static void * creator();
	static MStatus initialize();

private:
	static void setAttributeAffects();
	static void addAttributes();

public:
	static MTypeId id;
	const static  MString m_TypeName;

private:
	static MObject aColor;
	static MObject aTranslucenceCoeff;
	static MObject aDiffuseReflectivity;
	static MObject aIncandescence;
	static MObject aPointCamera;
	static MObject aNormalCamera;
	static MObject aLightDirection;
	static MObject aLightIntensity;
	static MObject aPower;
	static MObject aSpecularity;
	static MObject aLightAmbient;
	static MObject aLightDiffuse;
	static MObject aLightSpecular;
	static MObject aLightShadowFraction;
	static MObject aPreShadowIntensity;
	static MObject aLightBlindData;
	static MObject aLightData;

	static MObject aRayOrigin;
	static MObject aRayDirection;

	static MObject aObjectId;
	static MObject aRaySampler;
	static MObject aRayDepth;

	static MObject aReflectGain;

	static MObject aTriangleNormalCamera;

	static MObject aOutColor;
};

#endif /* SRC_CLOTHNODE_H_ */
