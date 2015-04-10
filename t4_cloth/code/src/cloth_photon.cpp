#include <iostream>
using std::cout;
using std::endl;

#include "shader.h"
#include "mayaapi.h"

// To initialize the diffuse color, for each node in maya execute
// connectAttr cloth_node1.diffuse cloth_photon1.diffuse_color;

struct cloth_photon {
	miColor diffuse_color; /* diffuse color */
};

extern "C" DLLEXPORT int cloth_photon_version(void) {
	return (1);
}

extern "C" DLLEXPORT miBoolean cloth_photon(miColor *result, miState *state,
		struct cloth_photon *params) {

	miVector diffuse_direction;
	miColor *diffuse_color = mi_eval_color(&params->diffuse_color);

	mi_store_photon(result, state);

	result->r *= diffuse_color->r;
	result->g *= diffuse_color->g;
	result->b *= diffuse_color->b;

	mi_reflection_dir_diffuse(&diffuse_direction, state);
	return mi_photon_reflection_diffuse(result, state, &diffuse_direction);
}

