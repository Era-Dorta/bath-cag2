/*
 * ClothShaderOverride.h
 *
 *  Created on: 27 Mar 2015
 *      Author: gdp24
 */

#ifndef SRC_CLOTHSHADEROVERRIDE_H_
#define SRC_CLOTHSHADEROVERRIDE_H_

#include <maya/MPxSurfaceShadingNodeOverride.h>

class ClothShaderOverride: public MHWRender::MPxSurfaceShadingNodeOverride {
public:
	static MHWRender::MPxSurfaceShadingNodeOverride* creator(
			const MObject& obj);

	virtual ~ClothShaderOverride();

	virtual MHWRender::DrawAPI supportedDrawAPIs() const;

	virtual MString fragmentName() const;
	virtual void getCustomMappings(
			MHWRender::MAttributeParameterMappingList& mappings);

	virtual MString primaryColorParameter() const;
	virtual MString bumpAttribute() const;

	virtual void updateDG();
	virtual void updateShader(MHWRender::MShaderInstance& shader,
			const MHWRender::MAttributeParameterMappingList& mappings);

private:
	ClothShaderOverride(const MObject& obj);

	MObject fObject;
	float fSpecularColor[3];
	mutable MString fResolvedSpecularColorName;
};

#endif /* SRC_CLOTHSHADEROVERRIDE_H_ */
