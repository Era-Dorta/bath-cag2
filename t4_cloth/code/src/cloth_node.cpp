#include <iostream>
#include <algorithm>

#include "shader.h"
#include "mayaapi.h"

#define _DEBUG

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

//TODO Move all the data to the custom argument in the state variable
const static miScalar eta = 1.46; // From the first measurements in the paper
const static miScalar r_0 = ((eta - 1) / (eta + 1)) * ((eta - 1) / (eta + 1));
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
			"%f", s, v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8], v[9],
			v[10], v[11], v[12], v[13], v[14], v[15]);
}

miScalar inline clamp(const miScalar &value, const miScalar min = -1,
		const miScalar max = 1) {
	if (value < min) {
		return min;
	}
	if (value > max) {
		return max;
	}
	return value;
}

void computeBRDF(const miVector& tex_inter, const miVector& abc,
		const miVector& def, const miVector& n, const miVector& p, int m,
		int n_l, miState* state, miTag* light, miColor* specular, miColor* diff,
		int uInc, int vInc, miColor* result) {
	// Build a coordinate system, n, t, s as in the paper
	// Get a vector t that lies on the triangle
	miVector t;
	t.x = (tex_inter.x + uInc) * abc.x + (tex_inter.y + vInc) * def.x;
	t.y = (tex_inter.x + uInc) * abc.y + (tex_inter.y + vInc) * def.y;
	t.z = (tex_inter.x + uInc) * abc.z + (tex_inter.y + vInc) * def.z;
	mi_vector_normalize(&t);
	// s has to be orthogonal to n and t
	miVector s;
	mi_vector_prod(&s, &n, &t);
	// Build matrix that transform from current coordinate system to normalised
	// one, where t is [1,0,0], n is [0,1,0] and s is [0,0,1]
	miMatrix trans_to_p, rot_to_axis, trans_to_axis;
	mi_matrix_ident(trans_to_p);
	trans_to_p[12] = -p.x;
	trans_to_p[13] = -p.y;
	trans_to_p[14] = -p.z;
	mi_matrix_ident(rot_to_axis);
	rot_to_axis[0] = t.x;
	rot_to_axis[4] = t.y;
	rot_to_axis[8] = t.z;
	rot_to_axis[1] = n.x;
	rot_to_axis[5] = n.y;
	rot_to_axis[9] = n.z;
	rot_to_axis[2] = s.x;
	rot_to_axis[6] = s.y;
	rot_to_axis[10] = s.z;
	mi_matrix_prod(trans_to_axis, trans_to_p, rot_to_axis);

	int samples;
	miColor color;
	/* Loop over all light sources */
	if (m == 4 || n_l) {
		miColor sum;
		for (mi::shader::LightIterator iter(state, light, n_l); !iter.at_end();
				++iter) {

			sum.r = sum.g = sum.b = 0;
			while (iter->sample()) {

				miVector aux, aux1;
				// w -> omega, t -> theta
				mi_vector_to_world(state, &aux, &(iter->get_direction()));
				mi_vector_transform(&aux1, &aux, trans_to_axis);
				//mi_vector_neg(&aux1);
				const miVector w_i = aux1;

				// Point to camera vector
				mi_vector_to_world(state, &aux, &(state->dir));
				mi_vector_transform(&aux1, &aux, trans_to_axis);
				mi_vector_neg(&aux1);
				const miVector v = aux1;

				// Half way vector
				mi_vector_add(&aux, &w_i, &v);
				mi_vector_mul(&aux, 0.5);
				const miVector h = aux;
				//TODO This computes a a reflection direction according to Lambert's cosine
				//law, in the paper that is not the case
				mi_reflection_dir_diffuse(&aux, state);
				mi_vector_to_world(state, &aux1, &aux);
				mi_vector_transform(&aux, &aux1, trans_to_axis);
				const miVector w_r = aux;
				// Because the vectors are normalised and we set the coordinate system for
				// t,n,s, we can easily compute cos and sin of theta and phi, see page
				// 456 in PBRT book
				// Theta -> angle with respect to s
				// Phi -> angle with respect to n

				const miScalar cos_t_i = w_i.z;
				const miScalar sin_t_i = clamp(
						sqrtf(std::max(0.0f, 1 - cos_t_i * cos_t_i)));
				const miScalar cos_p_i = w_i.y;
				const miScalar sin_p_i = clamp(
						sqrtf(std::max(0.0f, 1 - cos_p_i * cos_p_i)));
				//miScalar t_i = acos(cos_t_i);
				const miScalar cos_t_r = w_r.z;
				const miScalar sin_t_r = clamp(
						sqrtf(std::max(0.0f, 1 - cos_t_r * cos_t_r)));
				const miScalar cos_p_r = w_r.y;
				const miScalar sin_p_r = clamp(
						sqrtf(std::max(0.0f, 1 - cos_p_r * cos_p_r)));
				//miScalar t_r = acos(cos_t_r);
				//cos^2 + sin^2 = 1
				//sin(x) = sqrt(1 - cos^2(x))
				//sin(α + β) = sin(α)cos(β) + cos(α)sin(β)
				//sin(α – β) = sin(α)cos(β) – cos(α)sin(β)
				//cos(α + β) = cos(α)cos(β) – sin(α)sin(β)
				//cos(α – β) = cos(α)cos(β) + sin(α)sin(β)
				//cos(x/2) = sqrt((1 + cos(x))*0.5)
				// d = i - r
				const miScalar cos_p_d = clamp(
						cos_p_i * cos_p_r + sin_p_i * sin_p_r);
				const miScalar cos_2t_d = clamp(
						cos_t_i * cos_t_r + sin_t_i * sin_t_r);
				const miScalar cos_p2_d = clamp(
						sqrtf(std::max(0.0f, (1.0f + cos_p_d) * 0.5f)));
				//const miScalar cos_t_d2_2 = (1 + cos_t_d) * 0.5;

				// cos(x/2) = sqrt((1 + cos(x))*0.5) -> 1 / cos(x/2)^2 = 2 / (1 + cos(x))
				const miScalar inv_cos_t_d_sq = 2.0 / (1 + cos_2t_d);

				const miScalar g_lobe_v = gamma_v * exp(1);
				const miScalar g_lobe_s = gamma_s * exp(1);

				const miScalar F_r = clamp(
						r_0 + (1 - r_0) * powf(1 - mi_vector_dot(&h, &v), 5), 0,
						1);
				//clamp(mi_schlick_scatter(&w_i_chan, &w_r_chan, eta), 0, 1);
				//clamp(mi_fresnel(air_eta, eta, cos_t_i, cos_t_r), 0, 1);
				const miScalar F_t = 1 - F_r;

				//miScalar t_h = (t_i + t_r) * 0.5;
				//miScalar t_d = (t_i - t_r) * 0.5;

				//dot_nl = iter->get_dot_nl();
				iter->get_contribution(&color);
				miScalar vol_scatter = F_t * F_t * ((1 - k_d) * g_lobe_v + k_d)
						/ (cos_t_i + cos_t_r);
				//vol_scatter = vol_scatter * 0.0;
				vol_scatter = clamp(vol_scatter, 0, 1);
				miScalar surf_reflection = F_r * cos_p2_d * g_lobe_s;
				surf_reflection = surf_reflection * 0.000;
				surf_reflection = surf_reflection * 0.01;
				sum.r += (surf_reflection * color.r
						+ vol_scatter * A.x * diff->r) * inv_cos_t_d_sq;
				sum.g += (surf_reflection * color.g
						+ vol_scatter * A.y * diff->g) * inv_cos_t_d_sq;
				sum.b += (surf_reflection * color.b
						+ vol_scatter * A.z * diff->b) * inv_cos_t_d_sq;
			}
			samples = iter->get_number_of_samples();
			if (samples > 0) {
				miScalar inv_samples = 1 / samples;
				result->r += sum.r * inv_samples;
				result->g += sum.g * inv_samples;
				result->b += sum.b * inv_samples;
			}
		}
	}
}

extern "C" DLLEXPORT miBoolean cloth_node(miColor *result, miState *state,
		struct cloth_node *paras) {

	/*A Photon map does not record the first bounce, so we compute the direct
	 * ilumination here and the indirect one will be computed in the photon
	 * shader and added in mi_compute_avg_radiance*/

	miColor *diff, *specular;
	miTag *light; /* tag of light instance */
	int n_l; /* number of light sources */
	int i_l; /* offset of light sources */
	int m; /* light mode: 0=all, 1=incl, 2=excl, 4=native mental ray */
	miColor color; /* color from light source */

	/* check for illegal calls */
	if (state->type == miRAY_SHADOW || state->type == miRAY_DISPLACE) {
		return (miFALSE);
	}

	diff = mi_eval_color(&paras->diffuse);
	specular = mi_eval_color(&paras->specular);
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

	// Get number of vertices in the primitive
	miUint num = 0;
	if (!mi_query(miQ_PRI_NUM_VERTICES, state, miNULLTAG, &num)) {
		mi_error("Cloth shader could not get number of vertices");
		return miFALSE;
	}

	if (num != 3) {
		mi_error("Cloth shader can not handle non triangular primitives");
		return miFALSE;
	}

	miVector aux;
	// Get intersection point, for some reason to world gives a wrong point and
	// to object gives a good one
	mi_point_to_object(state, &aux, &(state->point));
	const miVector p = aux;

	// Get intersection normal
	mi_point_to_world(state, &aux, &(state->normal));
	const miVector n = aux;

	// Get vertex positions in the triangle
	miVector vert_p[3];
	miVector *q[] = { &vert_p[0], &vert_p[1], &vert_p[2] };
	if (!mi_query(miQ_PRI_VERTICES_POINTS, state, miNULLTAG, q)) {
		mi_error("Could not recover vertices points in cloth shader");
		return miFALSE;
	}

	mi_point_to_world(state, &aux, &vert_p[0]);
	vert_p[0] = aux;

	mi_point_to_world(state, &aux, &vert_p[1]);
	vert_p[1] = aux;

	mi_point_to_world(state, &aux, &vert_p[2]);
	vert_p[2] = aux;

	miUint num_tex = 0;
	if (!mi_query(miQ_NUM_TEXTURES, state, miNULLTAG, &num_tex)) {
		mi_error("Cloth shader could not get number of textures");
		return miFALSE;
	}

	miVector tex_p[3] = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };
	q[0] = &tex_p[0];
	q[1] = &tex_p[1];
	q[2] = &tex_p[2];
	if (!mi_query(miQ_PRI_VERTICES_TEX, state, miNULLTAG, q, 0)) {
		mi_error("Could not recover texture points in cloth shader");
		return miFALSE;
	}

	// Translate all textures 1,1,1 so that there are no division by zero when
	// computing the vectors t and s
	const miVector ones = { 1, 1, 0 };
	mi_vector_add(&tex_p[0], &tex_p[0], &ones);

	aux.x = state->bary[0] * tex_p[0].x + state->bary[1] * tex_p[1].x
			+ state->bary[2] * tex_p[2].x;
	aux.y = state->bary[0] * tex_p[0].y + state->bary[1] * tex_p[1].y
			+ state->bary[2] * tex_p[2].y;
	aux.z = state->bary[0] * tex_p[0].z + state->bary[1] * tex_p[1].z
			+ state->bary[2] * tex_p[2].z;
	const miVector tex_inter = aux;

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

	miScalar u2v1_u1v2_inv = 1.0
			/ (tex_p[1].x * tex_p[0].y - tex_p[0].x * tex_p[1].y);
	miScalar u1_inv = 1.0 / tex_p[0].x;
	miVector abc, def;
	def.x = (tex_p[1].x * vert_p[0].x - tex_p[0].x * vert_p[1].x)
			* u2v1_u1v2_inv;
	def.y = (tex_p[1].x * vert_p[0].y - tex_p[0].x * vert_p[1].y)
			* u2v1_u1v2_inv;
	def.z = (tex_p[1].x * vert_p[0].z - tex_p[0].x * vert_p[1].z)
			* u2v1_u1v2_inv;
	abc.x = (vert_p[0].x - tex_p[0].y * def.x) * u1_inv;
	abc.y = (vert_p[0].y - tex_p[0].y * def.y) * u1_inv;
	abc.z = (vert_p[0].z - tex_p[0].y * def.z) * u1_inv;

#ifdef _DEBUG
	if (tex_p[0].x == 0
			|| (tex_p[1].x * tex_p[0].y - tex_p[0].x * tex_p[1].y) == 0) {
		mi_warning("Divide by zero in cloth node");
	}
#endif

	// Build a coordinate system, n, t, s as in the paper
	// Compute for first thread
	computeBRDF(tex_inter, abc, def, n, p, m, n_l, state, light, specular, diff,
			0, 1, result);

	// Compute for second thread
	computeBRDF(tex_inter, abc, def, n, p, m, n_l, state, light, specular, diff,
			1, 0, result);

	// TODO How to compute the Gaussian lobe

	/*color.r = energy->r * vol_scatter * A.x * m.diffuse_color.r;
	 color.g = energy->g * vol_scatter * A.y * m.diffuse_color.g;
	 color.b = energy->b * vol_scatter * A.z * m.diffuse_color.b;

	 color.r = energy->r * m.diffuse_color.r;
	 color.g = energy->g * m.diffuse_color.g;
	 color.b = energy->b * m.diffuse_color.b;*/

	/* Compute indirect illumination */
	mi_compute_avg_radiance(&color, state, 'f', NULL);

	/* add contribution from indirect illumination (caustics) */
	//mi_compute_irradiance(&color, state);
	//result->r += color.r * diff->r;
	//result->g += color.g * diff->g;
	//result->b += color.b * diff->b;
	result->a = 1;

	//mi_compute_irradiance(&irrad, state);
	// If the surface got hit by any photon, the final color is the surface
	// color plus the received color
	return (miTRUE);
}

