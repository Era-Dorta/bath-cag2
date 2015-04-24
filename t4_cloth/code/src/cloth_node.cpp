#include <iostream>

#include "shader.h"
#include "mayaapi.h"

struct cloth_node {
	miColor ambience; /* ambient color multiplier */
	miColor diffuse; /* diffuse color */
	miColor specular;
	int mode; /* light mode: 0..2, 4 */
	int i_light; /* index of first light */
	int n_light; /* number of lights */
	miTag light[1]; /* list of lights */
};

extern "C" DLLEXPORT int cloth_node_version(void) {
	return (1);
}

const static miScalar eta = 1.46; // From the first measurements in the paper
const static miScalar k_d = 0.3;
const static miScalar gamma_s = 12;
const static miScalar gamma_v = 24;
const static miScalar a = 0.33;
const static miVector A = { 0.2 * 0.3, 0.8 * 0.3, 0.3 };
const static miScalar a1 = 0.5;
const static miScalar a2 = 0.5;
const static miScalar air_eta = 1;
const static miScalar ior_in = 1;

static int do_print = 0;

void mi_vector_info(const char* s, const miVector& v) {
	mi_warning("%s %f, %f, %f", s, v.x, v.y, v.z);
}

void mi_vector_info(const char* s, const miGeoVector& v) {
	mi_warning("%s %f, %f, %f", s, v.x, v.y, v.z);
}

void mi_matrix_info(const char* s, const miMatrix& v) {
	mi_warning("%s %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, "
				"%f", s, v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8],
				 v[9], v[10], v[11], v[12], v[13], v[14],  v[15]);
}

extern "C" DLLEXPORT miBoolean cloth_node(miColor *result, miState *state,
		struct cloth_node *paras) {

	/*A Photon map does not record the first bounce, so we compute the direct
	 * ilumination here and the indirect one will be computed in the photon
	 * shader and added in mi_compute_avg_radiance*/

	miColor *diff;
	miTag *light; /* tag of light instance */
	int n_l; /* number of light sources */
	int i_l; /* offset of light sources */
	int m; /* light mode: 0=all, 1=incl, 2=excl, 4=native mental ray */
	int samples; /* # of samples taken */
	miColor color; /* color from light source */
	miColor sum; /* summed sample colors */
	miScalar dot_nl; /* dot prod of normal and dir*/

	/* check for illegal calls */
	if (state->type == miRAY_SHADOW || state->type == miRAY_DISPLACE) {
		return (miFALSE);
	}

	diff = mi_eval_color(&paras->diffuse);
	m = *mi_eval_integer(&paras->mode);

	//*result = *mi_eval_color(&paras->ambience); /* ambient term */

	n_l = *mi_eval_integer(&paras->n_light);
	i_l = *mi_eval_integer(&paras->i_light);
	light = mi_eval_tag(paras->light) + i_l;

	if (m == 1) /* modify light list (inclusive mode) */
		mi_inclusive_lightlist(&n_l, &light, state);
	else if (m == 2) /* modify light list (exclusive mode) */
		mi_exclusive_lightlist(&n_l, &light, state);
	else if (m == 4) {
		n_l = 0;
		light = 0;
	}

	/* Loop over all light sources */
	if (m == 4 || n_l)
		for (mi::shader::LightIterator iter(state, light, n_l); !iter.at_end();
				++iter) {
			sum.r = sum.g = sum.b = 0;

			while (iter->sample()) {
				dot_nl = iter->get_dot_nl();
				iter->get_contribution(&color);
				sum.r += dot_nl * diff->r * color.r;
				sum.g += dot_nl * diff->g * color.g;
				sum.b += dot_nl * diff->b * color.b;
			}
			samples = iter->get_number_of_samples();
			if (samples > 0) {
				result->r += sum.r / samples;
				result->g += sum.g / samples;
				result->b += sum.b / samples;
			}
		}

	// w -> omega, t -> theta
	const miVector &w_i = state->dir;
	miVector w_r;
	mi_reflection_dir_diffuse(&w_r, state);
	const miVector &n = state->normal;
	const miVector &p = state->point;

	// Get a new point in the triangle
	miVector new_p = { 1, 0, 0 };
	new_p.z = (-n.x * (new_p.x - p.x) - n.y * (new_p.y - p.y)) / n.z + p.z;
	// Get vector from intersection to new point and normalize
	miVector t;
	mi_vector_sub(&t, &new_p, &p);
	mi_vector_normalize_d(&t);
	miVector s;
	mi_vector_prod(&s, &n, &t);

	// Build matrix that transform from current coordinate system to normalised
	// one, where t is [1,0,0], n is [0,1,0] and s is [0,0,1]
	miMatrix trans_to_p, rot_to_axis, trans_to_axis;

	mi_matrix_ident(rot_to_axis);
	rot_to_axis[0] = t.x;
	rot_to_axis[1] = t.y;
	rot_to_axis[2] = t.z;
	rot_to_axis[4] = n.x;
	rot_to_axis[5] = n.y;
	rot_to_axis[6] = n.z;
	rot_to_axis[8] = s.x;
	rot_to_axis[9] = s.y;
	rot_to_axis[10] = s.z;

	mi_matrix_ident(trans_to_p);
	trans_to_p[12] = -p.x;
	trans_to_p[13] = -p.y;
	trans_to_p[14] = -p.z;

	mi_matrix_prod(trans_to_axis, trans_to_p, rot_to_axis);

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
	miMatrix rot_m;
	//mi_matrix_rotate(rot_m, 0, M_PI / 5, 0);
	mi_matrix_ident(rot_m);
	rot_m[12] = 1;
	rot_m[13] = 2;
	rot_m[14] = 3;
	miVector resv, tv = { 1, 1, 1 };
	mi_point_transform(&resv, &tv, rot_m);
	//rotation[1] =

	miScalar cos_t_i = fabs(mi_vector_dot(&n, &w_i));
	miScalar t_i = acos(cos_t_i);

	miScalar cos_t_r = fabs(mi_vector_dot(&n, &w_r));
	miScalar t_r = acos(cos_t_r);
	/* Compute indirect illumination */
	miColor irrad;
	mi_compute_avg_radiance(&color, state, 'f', NULL);

	/* add contribution from indirect illumination (caustics) */
	//mi_compute_irradiance(&color, state);
	result->r += color.r * diff->r;
	result->g += color.g * diff->g;
	result->b += color.b * diff->b;
	result->a = 1;

	//mi_compute_irradiance(&irrad, state);
	// If the surface got hit by any photon, the final color is the surface
	// color plus the received color
	return (miTRUE);
}

