#include <iostream>
using std::cout;
using std::endl;

#include "shader.h"
#include "mayaapi.h"

// To initialize the diffuse color, for each node in maya execute
// createNode cloth_photon
// connectAttr cloth_node1.diffuse cloth_photon1.diffuse_color;
// connectAttr cloth_photon1.outValue cloth_node1SG.miPhotonShader;

static const miScalar eta = 1.46; // From the first measurements in the paper
static const miScalar k_d = 0.3;
static const miScalar gamma_s = 12;
static const miScalar gamma_v = 24;
static const miScalar a = 0.33;
static const miVector A = { 0.2 * 0.3, 0.8 * 0.3, 0.3 };
static const miScalar a1 = 0.5;
static const miScalar a2 = 0.5;
static const miScalar air_eta = 1;

struct cloth_photon {
	miColor diffuse_color; /* diffuse color */
	miColor specular_color; /* diffuse color */
	miScalar ior; /* diffuse color */
};

extern "C" DLLEXPORT int cloth_photon_version(void) {
	return (1);
}

extern "C" DLLEXPORT miBoolean cloth_photon(miColor *energy, miState *state,
		struct cloth_photon *paras) {
	struct cloth_photon m;
	miColor color;
	miVector dir;
	miScalar ior_in = 0.9, ior_out = 0.1;
	miRay_type type;

	m.diffuse_color = *mi_eval_color(&paras->diffuse_color);
	m.specular_color = *mi_eval_color(&paras->specular_color);
	m.ior = *mi_eval_scalar(&paras->ior);

	// Save the current photon with its color in this intersection
	mi_store_photon(energy, state);

	/*
	 * Choose scatter type for new photon
	 */

	/* We are only interested in reflection and transmission of specular and
	 * Diffuse components */
	//type = mi_choose_scatter_type(state, 1, &m.diffuse_color, &other,
	//	&m.specular_color);
	type = miPHOTON_TRANSMIT_DIFFUSE;

	/*
	 * Shoot new photon: Compute new photon color
	 * (compensating for Russian roulette) and shoot
	 * new photon in a direction determined by the
	 * scattering type
	 */

	switch (type) {
	/* no reflection. or transmission */
	case miPHOTON_ABSORB: {
		return (miTRUE);
	}
		/* specular reflection (mirror) */
	case miPHOTON_REFLECT_SPECULAR: {
		color.r = energy->r * m.specular_color.r;
		color.g = energy->g * m.specular_color.g;
		color.b = energy->b * m.specular_color.b;
		mi_reflection_dir_specular(&dir, state);
		return mi_photon_reflection_specular(&color, state, &dir);
	}
		/* diffuse (Lamberts cosine law) */
	case miPHOTON_REFLECT_DIFFUSE: {
		color.r = energy->r * m.diffuse_color.r;
		color.g = energy->g * m.diffuse_color.g;
		color.b = energy->b * m.diffuse_color.b;
		mi_reflection_dir_diffuse(&dir, state);
		return (mi_photon_reflection_diffuse(&color, state, &dir));
	}
		/* specular transmission */
	case miPHOTON_TRANSMIT_SPECULAR: {
		color.r = energy->r * m.specular_color.r;
		color.g = energy->g * m.specular_color.g;
		color.b = energy->b * m.specular_color.b;
		return (ior_out == ior_in ? mi_photon_transparent(&color, state) :
				mi_transmission_dir_specular(&dir, state, ior_in, ior_out) ?
						mi_photon_transmission_specular(&color, state, &dir) :
						miFALSE);
	}
		/* diffuse transm. (translucency), so far only this one gets executed */
	case miPHOTON_TRANSMIT_DIFFUSE: {
		// w -> omega, t -> theta
		miVector w_i = dir;
		mi_transmission_dir_diffuse(&dir, state);
		miVector w_r = dir;
		const miVector &n = state->normal_geom;

		/* Vectors are in columnwise
		 * Matrices in mental ray are a row of 16 values, using the convention
		 * r = v * M, where the translation component in the matrix in the last
		 * column, and they are store columnwise
		 * 0 4  8 12
		 * 1 5  9 13
		 * 2 6 10 14
		 * 3 7 11 15
		 * Rotations are counterclockwise, and the functions angles are in
		 * radians */
		miScalar cos_t_i = fabs(
				mi_vector_dot(&(state->normal_geom), &(state->dir)));
		miScalar t_i = acos(cos_t_i);

		miScalar cos_t_r = fabs(mi_vector_dot(&(state->normal_geom), &dir));
		miScalar t_r = acos(cos_t_r);


		miScalar t_h = (t_i + t_r) * 0.5;
		miScalar t_d = (t_i - t_r) * 0.5;

		// TODO How to compute the Gaussian lobe
		miScalar g_lobe = 1;
		miScalar F_r = mi_fresnel(air_eta, eta, cos_t_i, cos_t_r);
		miScalar F = 1 - F_r;
		miScalar vol_scatter = F * ((1 - k_d) + g_lobe + k_d)
				/ (cos_t_i + cos_t_r);

		color.r = energy->r * vol_scatter * A.x * m.diffuse_color.r;
		color.g = energy->g * vol_scatter * A.y * m.diffuse_color.g;
		color.b = energy->b * vol_scatter * A.z * m.diffuse_color.b;

		//color.r = energy->r * m.diffuse_color.r;
		//color.g = energy->g * m.diffuse_color.g;
		//color.b = energy->b * m.diffuse_color.b;

		return (mi_photon_transmission_diffuse(&color, state, &dir));
	}
	default: { /* Unknown scatter type */
		mi_error("unknown scatter type in dgs photon shader");
		return (miFALSE);
	}
	}
}

