#include "utility/inc/extended_math.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


WT_API f32 clamp(f32 v)
{
	if(v < 0.0) return 0.0;
	if(v > 1.0) return 1.0;
	return v;
}


WT_API f32 dist(f32 x, f32 y, f32 xp, f32 yp)
{
	return (sqrt((x - xp) * (x - xp) + (y - yp) * (y - yp)));
}

WT_INTERN f32 lineSlope(wt_vec2_t a)
{ 
	if(a[0] == 0)
		return 0;

	return a[1] / a[0]; 
}

WT_API s32 line_cross(wt_vec2_t p0, wt_vec2_t v0, wt_vec2_t p1, wt_vec2_t v1, wt_vec2_t out)
{
	f32 slope_a = lineSlope(v0);
	f32 slope_b = lineSlope(v1);
	f32 del = slope_a - slope_b;

	if(slope_a == slope_b)
		return 0;

	out[0] = (slope_a * p0[0] - slope_b * p1[0] + p1[1] - p0[1]) / del;
	out[1] = slope_b * (out[0] - p1[0]) + p1[1];
 
	return 1;
}





/*
 *
 * 2D-Vector
 *
 */

WT_API void wt_vec2_set(wt_vec2_t out, f32 x, f32 y)
{
	out[0] = x;
	out[1] = y;
}


WT_API void wt_vec2_clr(wt_vec2_t in)
{
	memset(in, 0, WT_VEC2_SIZE);
}


WT_API void wt_vec2_cpy(wt_vec2_t out, wt_vec2_t in)
{
	memcpy(out, in, WT_VEC2_SIZE);
}


WT_API s32 wt_vec2_cmp(wt_vec2_t in1, wt_vec2_t in2)
{
	if(in1[0] != in2[0] || in1[1] != in2[1])
		return 0;

	return 1;
}


WT_API void wt_vec2_add(wt_vec2_t in1, wt_vec2_t in2, wt_vec2_t out)
{
	out[0] = in1[0] + in2[0];
	out[1] = in1[1] + in2[1];
}


WT_API void wt_vec2_sub(wt_vec2_t in1, wt_vec2_t in2, wt_vec2_t out)
{
	out[0] = in1[0] - in2[0];
	out[1] = in1[1] - in2[1];
}


WT_API void wt_vec2_scl(wt_vec2_t in, f32 f, wt_vec2_t out)
{
	out[0] = in[0] * f;
	out[1] = in[1] * f;
}


WT_API void wt_vec2_inv_scl(wt_vec2_t in, f32 f, wt_vec2_t out)
{
	out[0] = in[0] / f;
	out[1] = in[1] / f;
}


WT_API void wt_vec2_mult(wt_vec2_t in1, wt_vec2_t in2, wt_vec2_t out)
{
	out[0] = in1[0] * in2[0];
	out[1] = in1[1] * in2[1];
}


WT_API void wt_vec2_div(wt_vec2_t in1, wt_vec2_t in2, wt_vec2_t out)
{
	out[0] = in1[0] / in2[0];
	out[1] = in1[1] / in2[1];
}


WT_API f32 wt_vec2_len(wt_vec2_t in)
{
	f64 l = (in[0] * in[0]) + (in[1] * in[1]);
	return (f32)sqrt(l);
}


WT_API f32 wt_vec2_sqrlen(wt_vec2_t in)
{
	return (in[0] * in[0]) + (in[1] * in[1]);
}


WT_API void wt_vec2_nrm(wt_vec2_t in, wt_vec2_t out)
{
	f32 len = wt_vec2_len(in);
	if(len == 0.0) {
		out[0] = 0.0;
		out[1] = 0.0;
		return;
	}

	out[0] = in[0] / len;
	out[1] = in[1] / len;
}


WT_API f32 wt_vec2_dot(wt_vec2_t v1, wt_vec2_t v2)
{
	return (v1[0] * v2[0]) + (v1[1] * v2[1]);
}


WT_API void wt_vec2_print(wt_vec2_t v)
{
	printf("%.2f/%.2f", v[0], v[1]);
}


/*
 *
 * 3D-Vector
 *
 */

WT_API void wt_vec3_set(wt_vec3_t out, f32 x, f32 y, f32 z)
{
	out[0] = x;
	out[1] = y;
	out[2] = z;
}


WT_API void wt_vec3_clr(wt_vec3_t in)
{
	memset(in, 0, WT_VEC3_SIZE);
}


WT_API void wt_vec3_cpy(wt_vec3_t out, wt_vec3_t in)
{
	memcpy(out, in, WT_VEC3_SIZE);
}


WT_API void wt_vec3_cpy_v2(wt_vec3_t out, wt_vec2_t in)
{
	out[0] = in[0];
	out[1] = 0.0;
	out[2] = in[1];
}


WT_API s32 wt_vec3_cmp(wt_vec3_t in1, wt_vec3_t in2)
{
	if(in1[0] != in2[0] || in1[1] != in2[1] || in1[2] != in2[2])
		return 0;

	return 1;
}


WT_API void wt_vec3_add(wt_vec3_t in1, wt_vec3_t in2, wt_vec3_t out)
{
	out[0] = in1[0] + in2[0];
	out[1] = in1[1] + in2[1];
	out[2] = in1[2] + in2[2];
}


WT_API void wt_vec3_sub(wt_vec3_t in1, wt_vec3_t in2, wt_vec3_t out)
{
	out[0] = in1[0] - in2[0];
	out[1] = in1[1] - in2[1];
	out[2] = in1[2] - in2[2];
}


WT_API void wt_vec3_scl(wt_vec3_t in, f32 f, wt_vec3_t out)
{
	out[0] = in[0] * f;
	out[1] = in[1] * f;
	out[2] = in[2] * f;
}


WT_API void wt_vec3_inv_scl(wt_vec3_t in, f32 f, wt_vec3_t out)
{
	out[0] = in[0] / f;
	out[1] = in[1] / f;
	out[2] = in[2] / f;
}


WT_API void wt_vec3_mult(wt_vec3_t in1, wt_vec3_t in2, wt_vec3_t out)
{
	out[0] = in1[0] * in2[0];
	out[1] = in1[1] * in2[1];
	out[2] = in1[2] * in2[2];
}


WT_API void wt_vec3_div(wt_vec3_t in1, wt_vec3_t in2, wt_vec3_t out)
{
	out[0] = in1[0] / in2[0];
	out[1] = in1[1] / in2[1];
	out[2] = in1[2] / in2[2];
}


WT_API f32 wt_vec3_len(wt_vec3_t in)
{
	f64 l = (in[0] * in[0]) + (in[1] * in[1]) + (in[2] * in[2]);
	return (f32)sqrt(l);
}


WT_API f32 wt_vec3_sqrlen(wt_vec3_t in)
{
	return ((in[0] * in[0]) + (in[1] * in[1]) + (in[2] * in[2]));
}


WT_API void wt_vec3_nrm(wt_vec3_t in, wt_vec3_t out)
{
	f32 len = wt_vec3_len(in);
	if(len == 0.0) {
		out[0] = 0.0;
		out[1] = 0.0;
		out[2] = 0.0;
		return;
	}

	out[0] = in[0] / len;
	out[1] = in[1] / len;
	out[2] = in[2] / len;
}


WT_API void wt_vec3_setlen(wt_vec3_t in, f32 l, wt_vec3_t out)
{
	wt_vec3_t v;

	wt_vec3_nrm(in, v);
	wt_vec3_scl(v, l, out);
}


WT_API f32 wt_vec3_dot(wt_vec3_t v1, wt_vec3_t v2)
{
	return (v1[0] * v2[0]) + (v1[1] * v2[1]) + (v1[2] * v2[2]);
}


WT_API void wt_vec3_cross(wt_vec3_t v1, wt_vec3_t v2, wt_vec3_t out)
{
	out[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]);
	out[1] = (v1[2] * v2[0]) - (v1[0] * v2[2]);
	out[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);
}


WT_API void wt_vec3_rot_x(wt_vec3_t in, f32 angle, wt_vec3_t out)
{
	wt_mat3_t rmat;
	wt_mat3_idt(rmat);
	rmat[0x0] =  1.0;
	rmat[0x4] =  cos(angle);
	rmat[0x5] = -sin(angle);
	rmat[0x7] =  sin(angle);
	rmat[0x8] =  cos(angle);
	wt_vec3_trans(in, rmat, out);
}


WT_API void wt_vec3_rot_y(wt_vec3_t in, f32 angle, wt_vec3_t out)
{
	wt_mat3_t rmat;
	wt_mat3_idt(rmat);
	rmat[0x0] =  cos(angle);
	rmat[0x2] =  sin(angle);
	rmat[0x3] =  1.0;
	rmat[0x6] = -sin(angle);
	rmat[0x8] =  cos(angle);
	wt_vec3_trans(in, rmat, out);
}


WT_API void wt_vec3_rot_z(wt_vec3_t in, f32 angle, wt_vec3_t out)
{
	wt_mat3_t rmat;
	wt_mat3_idt(rmat);
	rmat[0x0] =  cos(angle);
	rmat[0x1] = -sin(angle);
	rmat[0x3] =  sin(angle);
	rmat[0x4] =  cos(angle);
	rmat[0x8] =  1.0;
	wt_vec3_trans(in, rmat, out);
}


WT_API void wt_vec3_rot_axes(wt_vec3_t in, f32 angle, wt_vec3_t axis, wt_vec3_t out)
{
	wt_mat3_t rmat;
	f32 q0;
	f32 q1;
	f32 q2;
	f32 q3;

	wt_vec3_nrm(axis, axis);
	q0 = cos(angle / 2);
	q1 = sin(angle / 2) * axis[0];
	q2 = sin(angle / 2) * axis[1];
	q3 = sin(angle / 2) * axis[2];

	rmat[0x0] = q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3;
	rmat[0x1] = 2 * (q1 * q2 - q0 * q3);
	rmat[0x2] = 2 * (q1 * q3 + q0 * q2);
	rmat[0x3] = 2 * (q2 * q1 + q0 * q3);
	rmat[0x4] = q0 * q0 - q1 * q1 + q2 * q2 - q3 * q3;
	rmat[0x5] = 2 * (q2 * q3 - q0 * q1);
	rmat[0x6] = 2 * (q3 * q1 - q0 * q2);
	rmat[0x7] = 2 * (q3 * q2 + q0 * q1);
	rmat[0x8] = q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3;

	wt_vec3_trans(in, rmat, out);
}


WT_API void wt_vec3_trans(wt_vec3_t in, wt_mat3_t mat, wt_vec3_t out)
{
	wt_vec3_t tmp;
	wt_vec3_cpy(tmp, in);

	out[0] = tmp[0] * mat[0x0] + tmp[1] * mat[0x3] + tmp[2] * mat[0x6];
	out[1] = tmp[0] * mat[0x1] + tmp[1] * mat[0x4] + tmp[2] * mat[0x7];
	out[2] = tmp[0] * mat[0x2] + tmp[1] * mat[0x5] + tmp[2] * mat[0x8];
}


WT_API void wt_vec3_calc_nrm(wt_vec3_t p0, wt_vec3_t p1, wt_vec3_t p2, wt_vec3_t nrm)
{
	wt_vec3_t d1;
	wt_vec3_t d2;

	wt_vec3_sub(p1, p0, d1);
	wt_vec3_sub(p2, p0, d2);

	wt_vec3_cross(d1, d2, nrm);
	wt_vec3_nrm(nrm, nrm);
}


WT_API f32 wt_vec3_barry_centric(wt_vec3_t p0, wt_vec3_t p1, wt_vec3_t p2, wt_vec2_t pos)
{
	f32 det;
	f32 l1;
	f32 l2;
	f32 l3;

	det = (p1[2] - p2[2]) * (p0[0]  - p2[0]) + (p2[0] - p1[0]) * (p0[2]  - p2[2]);
	l1 = ((p1[2] - p2[2]) * (pos[0] - p2[0]) + (p2[0] - p1[0]) * (pos[1] - p2[2])) / det;
	l2 = ((p2[2] - p0[2]) * (pos[0] - p2[0]) + (p0[0] - p2[0]) * (pos[1] - p2[2])) / det;
	l3 = 1.0 - l1 - l2;

	return (l1 * p0[1]) + (l2 * p1[1]) + (l3 * p2[1]);
}


WT_API void wt_vec3_flip(wt_vec3_t in, wt_vec3_t out)
{
	out[0] = -in[0];
	out[1] = -in[1];
	out[2] = -in[2];
}


WT_API f32 wt_vec3_angle(wt_vec3_t v1, wt_vec3_t v2)
{
	f32 a;
	f32 b;

	a = wt_vec3_dot(v1, v2);
	b = ABS(wt_vec3_len(v1)) * ABS(wt_vec3_len(v2));

	return acos(a / b);
}


WT_API void wt_vec3_interp(wt_vec3_t v1, wt_vec3_t v2, f32 p, wt_vec3_t out)
{
	wt_vec3_t conv;

	conv[0] = v1[0] + (v2[0] - v1[0]) * p;
	conv[1] = v1[1] + (v2[1] - v1[1]) * p;
	conv[2] = v1[2] + (v2[2] - v1[2]) * p;

	wt_vec3_cpy(out, conv);
}


WT_API void wt_vec3_print(wt_vec3_t v)
{
	printf("%.2f/%.2f/%.2f", v[0], v[1], v[2]);
}


/*
 *
 * 4D-Vector
 *
 */


WT_API void wt_vec4_set(wt_vec4_t out, f32 x, f32 y, f32 z, f32 w)
{
	out[0] = x;
	out[1] = y;
	out[2] = z;
	out[3] = w;
}


WT_API void wt_vec4_clr(wt_vec4_t in)
{
	memset(in, 0, WT_VEC4_SIZE);
}


WT_API void wt_vec4_cpy(wt_vec4_t out, wt_vec4_t in)
{
	memcpy(out, in, WT_VEC4_SIZE);
}


WT_API s32 wt_vec4_cmp(wt_vec4_t in1, wt_vec4_t in2)
{
	if(in1[0] != in2[0] || in1[1] != in2[1] || in1[2] != in2[2] ||
			in1[3] != in2[3])
		return 0;

	return 1;
}


WT_API void wt_vec4_add(wt_vec4_t in1, wt_vec4_t in2, wt_vec4_t out)
{
	out[0] = in1[0] + in2[0];
	out[1] = in1[1] + in2[1];
	out[2] = in1[2] + in2[2];
	out[3] = in1[3] + in2[3];
}


WT_API void wt_vec4_sub(wt_vec4_t in1, wt_vec4_t in2, wt_vec4_t out)
{
	out[0] = in1[0] - in2[0];
	out[1] = in1[1] - in2[1];
	out[2] = in1[2] - in2[2];
	out[3] = in1[3] - in2[3];
}


WT_API void wt_vec4_scl(wt_vec4_t in, f32 f, wt_vec4_t out)
{
	out[0] = in[0] * f;
	out[1] = in[1] * f;
	out[2] = in[2] * f;
	out[3] = in[3] * f;
}


WT_API f32 wt_vec4_len(wt_vec4_t in)
{
	f64 l;
	l = (in[0] * in[0]) + (in[1] * in[1]) + (in[2] * in[2]) + (in[3] * in[3]);
	return (f32)sqrt(l);
}


WT_API void wt_vec4_nrm(wt_vec4_t in, wt_vec4_t out)
{
	f32 len = wt_vec4_len(in);
	if(len == 0.0) {
		out[0] = 0.0;
		out[1] = 0.0;
		out[2] = 0.0;
		out[3] = 0.0;
		return;
	}

	out[0] = in[0] / len;
	out[1] = in[1] / len;
	out[2] = in[2] / len;
	out[3] = in[3] / len;
}


WT_API f32 wt_vec4_dot(wt_vec4_t in1, wt_vec4_t in2)
{
	return (in1[0] * in2[0]) + (in1[1] * in2[1]) + (in1[2] * in2[2]) +
		(in1[3] * in2[3]);
}


WT_API void wt_vec4_trans(wt_vec4_t in, wt_mat4_t mat, wt_vec4_t out)
{
	wt_vec4_t tmp;
	wt_vec4_cpy(tmp, in);

	out[0] = tmp[0] * mat[0x0] + tmp[1] * mat[0x4] + tmp[2] * mat[0x8] + tmp[3] * mat[0xc];
	out[1] = tmp[0] * mat[0x1] + tmp[1] * mat[0x5] + tmp[2] * mat[0x9] + tmp[3] * mat[0xd];
	out[2] = tmp[0] * mat[0x2] + tmp[1] * mat[0x6] + tmp[2] * mat[0xa] + tmp[3] * mat[0xe];
	out[3] = tmp[0] * mat[0x3] + tmp[1] * mat[0x7] + tmp[2] * mat[0xb] + tmp[3] * mat[0xf];
}


WT_API void wt_vec4_print(wt_vec4_t in)
{
	printf("%.2f/%.2f/%.2f/%.2f", in[0], in[1], in[2], in[3]);
}





WT_API void wt_mat3_zero(wt_mat3_t m) 
{
	memset(m, 0, MAT3_SIZE);
}


WT_API void wt_mat3_idt(wt_mat3_t m) 
{
	wt_mat3_zero(m);
	m[0x0] = 1.0;
	m[0x4] = 1.0;
	m[0x8] = 1.0;
}


WT_API void wt_mat3_cpy(wt_mat3_t out, wt_mat3_t in) 
{
	memcpy(out, in, MAT3_SIZE);
}


WT_API void wt_mat3_mult(wt_mat3_t m1, wt_mat3_t m2, wt_mat3_t res)
{
	s32 i, j, k;
	wt_mat3_t ret;
	wt_mat3_zero(ret);
	wt_mat3_zero(res);

	for(i = 0; i < 3; i++) {
		for(j = 0; j < 3; j++) {
			ret[j * 4 + i] = 0;

			for(k = 0; k < 3; k++)
				ret[j * 3 + i] += m1[k * 3 + i] * 
					m2[j * 3 + k];
		}
	}

	wt_mat3_cpy(res, ret);
}


WT_API void wt_mat3_print(wt_mat3_t m)
{
	s32 r, c;

	for(r = 0; r < 3; r++) {
		for(c = 0; c < 3; c++)
			printf("%6.2f ", m[c * 3 + r]);

		printf("\n");
	}
}


WT_API void wt_mat4_zero(wt_mat4_t m)
{
	s32 i;

	for(i = 0; i < 16; i++)
		m[i] = 0.0;
}


WT_API void wt_mat4_idt(wt_mat4_t m)
{
	wt_mat4_zero(m);
	m[0x00] = 1.0;
	m[0x05] = 1.0;
	m[0x0a] = 1.0;
	m[0x0f] = 1.0;
}


WT_API void wt_mat4_cpy(wt_mat4_t out, wt_mat4_t in)
{
	if(in)
		memcpy(out, in, MAT4_SIZE);
}


WT_API void wt_mat4_mult(wt_mat4_t m1, wt_mat4_t m2, wt_mat4_t out)
{
	s32 i, j, k;
	f32 a, b;
	wt_mat4_t conv;
	wt_mat4_zero(conv);

	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			for(k = 0; k < 4; k++) {
				a = m1[k * 4 + i];
				b = m2[j * 4 + k];

				conv[j * 4 + i] += a * b;
			}
		}
	}

	wt_mat4_cpy(out, conv);
}


WT_API void wt_mat4_inv(wt_mat4_t out, wt_mat4_t in)
{
	f64 inv[16], det;
	s32 i;

	inv[0] = in[5]  * in[10] * in[15] - 
		in[5]  * in[11] * in[14] - 
		in[9]  * in[6]  * in[15] + 
		in[9]  * in[7]  * in[14] +
		in[13] * in[6]  * in[11] - 
		in[13] * in[7]  * in[10];

	inv[4] = -in[4]  * in[10] * in[15] + 
		in[4]  * in[11] * in[14] + 
		in[8]  * in[6]  * in[15] - 
		in[8]  * in[7]  * in[14] - 
		in[12] * in[6]  * in[11] + 
		in[12] * in[7]  * in[10];

	inv[8] = in[4]  * in[9] * in[15] - 
		in[4]  * in[11] * in[13] - 
		in[8]  * in[5] * in[15] + 
		in[8]  * in[7] * in[13] + 
		in[12] * in[5] * in[11] - 
		in[12] * in[7] * in[9];

	inv[12] = -in[4]  * in[9] * in[14] + 
		in[4]  * in[10] * in[13] +
		in[8]  * in[5] * in[14] - 
		in[8]  * in[6] * in[13] - 
		in[12] * in[5] * in[10] + 
		in[12] * in[6] * in[9];

	inv[1] = -in[1]  * in[10] * in[15] + 
		in[1]  * in[11] * in[14] + 
		in[9]  * in[2] * in[15] - 
		in[9]  * in[3] * in[14] - 
		in[13] * in[2] * in[11] + 
		in[13] * in[3] * in[10];

	inv[5] = in[0]  * in[10] * in[15] - 
		in[0]  * in[11] * in[14] - 
		in[8]  * in[2] * in[15] + 
		in[8]  * in[3] * in[14] + 
		in[12] * in[2] * in[11] - 
		in[12] * in[3] * in[10];

	inv[9] = -in[0]  * in[9] * in[15] + 
		in[0]  * in[11] * in[13] + 
		in[8]  * in[1] * in[15] - 
		in[8]  * in[3] * in[13] - 
		in[12] * in[1] * in[11] + 
		in[12] * in[3] * in[9];

	inv[13] = in[0]  * in[9] * in[14] - 
		in[0]  * in[10] * in[13] - 
		in[8]  * in[1] * in[14] + 
		in[8]  * in[2] * in[13] + 
		in[12] * in[1] * in[10] - 
		in[12] * in[2] * in[9];

	inv[2] = in[1]  * in[6] * in[15] - 
		in[1]  * in[7] * in[14] - 
		in[5]  * in[2] * in[15] + 
		in[5]  * in[3] * in[14] + 
		in[13] * in[2] * in[7] - 
		in[13] * in[3] * in[6];

	inv[6] = -in[0]  * in[6] * in[15] + 
		in[0]  * in[7] * in[14] + 
		in[4]  * in[2] * in[15] - 
		in[4]  * in[3] * in[14] - 
		in[12] * in[2] * in[7] + 
		in[12] * in[3] * in[6];

	inv[10] = in[0]  * in[5] * in[15] - 
		in[0]  * in[7] * in[13] - 
		in[4]  * in[1] * in[15] + 
		in[4]  * in[3] * in[13] + 
		in[12] * in[1] * in[7] - 
		in[12] * in[3] * in[5];

	inv[14] = -in[0]  * in[5] * in[14] + 
		in[0]  * in[6] * in[13] + 
		in[4]  * in[1] * in[14] - 
		in[4]  * in[2] * in[13] - 
		in[12] * in[1] * in[6] + 
		in[12] * in[2] * in[5];

	inv[3] = -in[1] * in[6] * in[11] + 
		in[1] * in[7] * in[10] + 
		in[5] * in[2] * in[11] - 
		in[5] * in[3] * in[10] - 
		in[9] * in[2] * in[7] + 
		in[9] * in[3] * in[6];

	inv[7] = in[0] * in[6] * in[11] - 
		in[0] * in[7] * in[10] - 
		in[4] * in[2] * in[11] + 
		in[4] * in[3] * in[10] + 
		in[8] * in[2] * in[7] - 
		in[8] * in[3] * in[6];

	inv[11] = -in[0] * in[5] * in[11] + 
		in[0] * in[7] * in[9] + 
		in[4] * in[1] * in[11] - 
		in[4] * in[3] * in[9] - 
		in[8] * in[1] * in[7] + 
		in[8] * in[3] * in[5];

	inv[15] = in[0] * in[5] * in[10] - 
		in[0] * in[6] * in[9] - 
		in[4] * in[1] * in[10] + 
		in[4] * in[2] * in[9] + 
		in[8] * in[1] * in[6] - 
		in[8] * in[2] * in[5];

	det = in[0] * inv[0] + in[1] * inv[4] + in[2] * inv[8] + in[3] * inv[12];

	if(det == 0)
		return;

	det = 1.0 / det;

	for(i = 0; i < 16; i++)
		out[i] = (f32)(inv[i] * det);
}


WT_API void wt_mat4_transp(wt_mat4_t out, wt_mat4_t in)
{
	wt_mat4_t tmp;

	tmp[0x0] = in[0x0];
	tmp[0x1] = in[0x4];
	tmp[0x2] = in[0x8];
	tmp[0x3] = in[0xc];
	tmp[0x4] = in[0x1];
	tmp[0x5] = in[0x5];
	tmp[0x6] = in[0x9];
	tmp[0x7] = in[0xd];
	tmp[0x8] = in[0x2];
	tmp[0x9] = in[0x6];
	tmp[0xa] = in[0xa];
	tmp[0xb] = in[0xe];
	tmp[0xc] = in[0x3];
	tmp[0xd] = in[0x7];
	tmp[0xe] = in[0xb];
	tmp[0xf] = in[0xf];

	wt_mat4_cpy(out, tmp);
}


WT_API void wt_mat4_print(wt_mat4_t m)
{
	s32 r, c;

	for(r = 0; r < 4; r++) {
		for(c = 0; c < 4; c++)
			printf("%.6f ", m[r + c * 4]);

		printf("\n");
	}
}


WT_API void wt_mat4_std(wt_mat4_t m)
{
	s32 i;

	for(i = 0; i < 16; i++) {
		if(m[i] == 0.0) {
			m[i] = 0.0;
		}
	}
}


WT_API void wt_mat4_pfpos(wt_mat4_t m, wt_vec3_t v)
{	
	m[0xc] = v[0];
	m[0xd] = v[1];
	m[0xe] = v[2];

	m[0xf] = 1.0;
}


WT_API void wt_mat4_rfqat(wt_mat4_t m, wt_vec4_t v)
{
	m[0x0] = 1.0 - 2.0 * (v[2] * v[2] + v[3] * v[3]);
	m[0x1] = 2.0 * (v[1] * v[2]) + 2.0 * (v[3] * v[0]);
	m[0x2] = 2.0 * (v[1] * v[3]) - 2.0 * (v[2] * v[0]);
	m[0x3] = 0.0;

	m[0x4] = 2.0 * (v[1] * v[2]) - 2.0 * (v[3] * v[0]);
	m[0x5] = 1.0 - 2.0 * (v[0] * v[0]) - 2.0 * (v[3] * v[3]);
	m[0x6] = 2.0 * (v[2] * v[3]) + 2.0 * (v[1] * v[0]);
	m[0x7] = 0.0;

	m[0x8] = 2.0 * (v[1] * v[3]) + 2.0 * (v[2] * v[0]);
	m[0x9] = 2.0 * (v[2] * v[3]) - 2.0 * (v[1] * v[0]);
	m[0xa] = 1.0 - 2.0 * (v[1] * v[1]) - 2.0 * (v[2] * v[2]);
	m[0xb] = 0.0;

	m[0xf] = 1.0;
}


WT_API void wt_mat4_rfvec(wt_mat4_t m, wt_vec3_t v)
{
	wt_vec3_t up = {0, 0, 1};
	wt_vec3_t xaxis;
	wt_vec3_t yaxis;

	wt_vec3_cross(up, v, xaxis);
	wt_vec3_nrm(xaxis, xaxis);

	wt_vec3_cross(v, xaxis, yaxis);
	wt_vec3_nrm(yaxis, yaxis);

	wt_mat4_idt(m);

	m[0x0] = xaxis[0];
	m[0x1] = yaxis[0];
	m[0x2] = v[0];

	m[0x4] = xaxis[1];
	m[0x5] = yaxis[1];
	m[0x6] = v[1];

	m[0x8] = xaxis[2];
	m[0x9] = yaxis[2];
	m[0xa] = v[2];
}


/* x: pitch, y: roll, z: yaw; angles in euler */
WT_API void wt_mat4_rfagl(wt_mat4_t m, wt_vec3_t v)
{
	f32 x = DEG_TO_RAD(v[0]);
	f32 y = DEG_TO_RAD(v[1]);
	f32 z = DEG_TO_RAD(v[2]);

	/* Roll */
	f32 cosY = cos(y);
	f32 sinY = sin(y);

	/* Pitch */
	f32 cosP = cos(x);
	f32 sinP = sin(x);

	/* Yaw */
	f32 cosR = cos(z);
	f32 sinR = sin(z);

	wt_mat4_idt(m);
	m[0x0] = cosY * cosR + sinY * sinP * sinR;
	m[0x1] = cosR * sinY * sinP - sinR * cosY;
	m[0x2] = cosP * sinY;

	m[0x4] = cosP * sinR;
	m[0x5] = cosR * cosP;
	m[0x6] = -sinP;

	m[0x8] = sinR * cosY * sinP - sinY * cosR;
	m[0x9] = sinY * sinR + cosR * cosY * sinP;
	m[0xa] = cosP * cosY;


}


WT_API void wt_mat4_pfpos_s(wt_mat4_t m, f32 x, f32 y, f32 z)
{	
	m[0xc] = x;
	m[0xd] = y;
	m[0xe] = z;

	m[0xf] = 1.0;
}


WT_API void wt_mat4_rfqat_s(wt_mat4_t m, f32 w, f32 x, f32 y, f32 z)
{
	m[0x0] = 1.0 - 2.0 * (y * y + z * z);
	m[0x1] = 2.0 * (x * y) + 2.0 * (z * w);
	m[0x2] = 2.0 * (x * z) - 2.0 * (y * w);
	m[0x3] = 0.0;

	m[0x4] = 2.0 * (x * y) - 2.0 * (z * w);
	m[0x5] = 1.0 - 2.0 * (x * x) - 2.0 * (z * z);
	m[0x6] = 2.0 * (y * z) + 2.0 * (x * w);
	m[0x7] = 0.0;

	m[0x8] = 2.0 * (x * z) + 2.0 * (y * w);
	m[0x9] = 2.0 * (y * z) - 2.0 * (x * w);
	m[0xa] = 1.0 - 2.0 * (x * x) - 2.0 * (y * y);
	m[0xb] = 0.0;

	m[0xf] = 1.0;
}


/* see wt_mat4_rfagl() for parameter info */
WT_API void wt_mat4_rfagl_s(wt_mat4_t m, f32 x, f32 y, f32 z)
{
	wt_vec3_t v;
	wt_mat4_t c;

	wt_vec3_set(v, x, y, z);
	wt_mat4_rfagl(c, v);
	wt_mat4_cpy(m, c);
}


