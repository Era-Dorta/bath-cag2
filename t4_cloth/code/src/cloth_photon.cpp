#include <iostream>
using std::cout;
using std::endl;

#include "shader.h"
#include "mayaapi.h"

// To initialize the diffuse color, for each node in maya execute
// connectAttr cloth_node1.diffuse cloth_photon1.diffuse_color;

static const miScalar eta = 1.46; // From the first measurements in the paper
static const miScalar k_d = 0.3;
static const miScalar gamma_s = 12;
static const miScalar gamma_v = 24;
static const miScalar a = 0.33;
static const miVector A = { 0.2 * 0.3, 0.8 * 0.3, 0.3 };

struct cloth_photon {
	miColor diffuse_color; /* diffuse color */
};

extern "C" DLLEXPORT int cloth_photon_version(void) {
	return (1);
}

static bool do_print = true;

extern "C" DLLEXPORT miBoolean cloth_photon(miColor *result, miState *state,
		struct cloth_photon *params) {

	miVector diffuse_direction;
	miColor *diffuse_color = mi_eval_color(&params->diffuse_color);

	mi_store_photon(result, state);

	miScalar theta_i = 1.5;
	miScalar theta_r = 1.1;
	miScalar g_lobe = 0.1;
	miScalar F = 1;
	miScalar vol_scatter = F * ((1 - k_d) + g_lobe + k_d)
			/ (cos(theta_i) + cos(theta_r));

	result->r *= vol_scatter * A.x * diffuse_color->r;
	result->g *= vol_scatter * A.y * diffuse_color->g;
	result->b *= vol_scatter * A.z * diffuse_color->b;

	mi_reflection_dir_diffuse(&diffuse_direction, state);
	return mi_photon_reflection_diffuse(result, state, &diffuse_direction);
}

