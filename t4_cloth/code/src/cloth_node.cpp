#include <iostream>
using std::cout;
using std::endl;

#include "shader.h"
#include "mayaapi.h"

struct cloth_node {
	miColor ambience; /* ambient color multiplier */
	miColor ambient; /* ambient color */
	miColor diffuse; /* diffuse color */
	int mode; /* light mode: 0..2, 4 */
	int i_light; /* index of first light */
	int n_light; /* number of lights */
	miTag light[1]; /* list of lights */
};

extern "C" DLLEXPORT int cloth_node_version(void) {
	return (1);
}

extern "C" DLLEXPORT miBoolean cloth_node(miColor *result, miState *state,
		struct cloth_node *paras) {

	/* check for illegal calls */
	if (state->type == miRAY_SHADOW || state->type == miRAY_DISPLACE) {
		return (miFALSE);
	}

	miColor *diffuse = mi_eval_color(&paras->diffuse);

	/* Compute indirect illumination */
	miColor irrad;
	mi_compute_avg_radiance(&irrad, state, 'f', NULL);
	//mi_compute_irradiance(&irrad, state);
	// If the surface got hit by any photon, the final color is the surface
	// color plus the received color
	if (irrad.r != 0 || irrad.g != 0 || irrad.b != 0) {
		result->r = irrad.r + diffuse->r;
		result->g = irrad.g + diffuse->g;
		result->b = irrad.b + diffuse->b;
	}
	result->a = 1;
	return (miTRUE);
}

