#include "source/math/inc/vector.h"

#include "source/math/inc/functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 * -=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-
 *
 * FLOAT VECTORS
 */

/*
 *
 * 2D-Vector
 *
 */

WUT_API void wut_vec2_set(wut_Vec2 out, f32 x, f32 y)
{
	out[0] = x;
	out[1] = y;
}


WUT_API void wut_vec2_clr(wut_Vec2 in)
{
	memset(in, 0, WUT_VEC2_SIZE);
}


WUT_API void wut_vec2_cpy(wut_Vec2 out, wut_Vec2 in)
{
	memcpy(out, in, WUT_VEC2_SIZE);
}


WUT_API s32 wut_vec2_cmp(wut_Vec2 in1, wut_Vec2 in2)
{
	if(in1[0] != in2[0] || in1[1] != in2[1])
		return 0;

	return 1;
}


WUT_API void wut_vec2_add(wut_Vec2 in1, wut_Vec2 in2, wut_Vec2 out)
{
	out[0] = in1[0] + in2[0];
	out[1] = in1[1] + in2[1];
}


WUT_API void wut_vec2_sub(wut_Vec2 in1, wut_Vec2 in2, wut_Vec2 out)
{
	out[0] = in1[0] - in2[0];
	out[1] = in1[1] - in2[1];
}


WUT_API void wut_vec2_scl(wut_Vec2 in, f32 f, wut_Vec2 out)
{
	out[0] = in[0] * f;
	out[1] = in[1] * f;
}


WUT_API void wut_vec2_inv_scl(wut_Vec2 in, f32 f, wut_Vec2 out)
{
	out[0] = in[0] / f;
	out[1] = in[1] / f;
}


WUT_API void wut_vec2_mult(wut_Vec2 in1, wut_Vec2 in2, wut_Vec2 out)
{
	out[0] = in1[0] * in2[0];
	out[1] = in1[1] * in2[1];
}


WUT_API void wut_vec2_div(wut_Vec2 in1, wut_Vec2 in2, wut_Vec2 out)
{
	out[0] = in1[0] / in2[0];
	out[1] = in1[1] / in2[1];
}


WUT_API f32 wut_vec2_len(wut_Vec2 in)
{
	f64 l = (in[0] * in[0]) + (in[1] * in[1]);
	return (f32)sqrt(l);
}


WUT_API f32 wut_vec2_sqrlen(wut_Vec2 in)
{
	return (in[0] * in[0]) + (in[1] * in[1]);
}


WUT_API void wut_vec2_nrm(wut_Vec2 in, wut_Vec2 out)
{
	f32 len = wut_vec2_len(in);
	if(len == 0.0) {
		out[0] = 0.0;
		out[1] = 0.0;
		return;
	}

	out[0] = in[0] / len;
	out[1] = in[1] / len;
}


WUT_API f32 wut_vec2_dot(wut_Vec2 v1, wut_Vec2 v2)
{
	return (v1[0] * v2[0]) + (v1[1] * v2[1]);
}


WUT_API void wut_vec2_print(wut_Vec2 v)
{
	printf("%.2f/%.2f", v[0], v[1]);
}


/*
 *
 * 3D-Vector
 *
 */

WUT_API void wut_vec3_set(wut_Vec3 out, f32 x, f32 y, f32 z)
{
	out[0] = x;
	out[1] = y;
	out[2] = z;
}


WUT_API void wut_vec3_clr(wut_Vec3 in)
{
	memset(in, 0, WUT_VEC3_SIZE);
}


WUT_API void wut_vec3_cpy(wut_Vec3 out, wut_Vec3 in)
{
	memcpy(out, in, WUT_VEC3_SIZE);
}


WUT_API void wut_vec3_cpy_v2(wut_Vec3 out, wut_Vec2 in)
{
	out[0] = in[0];
	out[1] = 0.0;
	out[2] = in[1];
}


WUT_API s32 wut_vec3_cmp(wut_Vec3 in1, wut_Vec3 in2)
{
	if(in1[0] != in2[0] || in1[1] != in2[1] || in1[2] != in2[2])
		return 0;

	return 1;
}


WUT_API void wut_vec3_add(wut_Vec3 in1, wut_Vec3 in2, wut_Vec3 out)
{
	out[0] = in1[0] + in2[0];
	out[1] = in1[1] + in2[1];
	out[2] = in1[2] + in2[2];
}


WUT_API void wut_vec3_sub(wut_Vec3 in1, wut_Vec3 in2, wut_Vec3 out)
{
	out[0] = in1[0] - in2[0];
	out[1] = in1[1] - in2[1];
	out[2] = in1[2] - in2[2];
}


WUT_API void wut_vec3_scl(wut_Vec3 in, f32 f, wut_Vec3 out)
{
	out[0] = in[0] * f;
	out[1] = in[1] * f;
	out[2] = in[2] * f;
}


WUT_API void wut_vec3_inv_scl(wut_Vec3 in, f32 f, wut_Vec3 out)
{
	out[0] = in[0] / f;
	out[1] = in[1] / f;
	out[2] = in[2] / f;
}


WUT_API void wut_vec3_mult(wut_Vec3 in1, wut_Vec3 in2, wut_Vec3 out)
{
	out[0] = in1[0] * in2[0];
	out[1] = in1[1] * in2[1];
	out[2] = in1[2] * in2[2];
}


WUT_API void wut_vec3_div(wut_Vec3 in1, wut_Vec3 in2, wut_Vec3 out)
{
	out[0] = in1[0] / in2[0];
	out[1] = in1[1] / in2[1];
	out[2] = in1[2] / in2[2];
}


WUT_API f32 wut_vec3_len(wut_Vec3 in)
{
	f64 l = (in[0] * in[0]) + (in[1] * in[1]) + (in[2] * in[2]);
	return (f32)sqrt(l);
}


WUT_API f32 wut_vec3_sqrlen(wut_Vec3 in)
{
	return ((in[0] * in[0]) + (in[1] * in[1]) + (in[2] * in[2]));
}


WUT_API void wut_vec3_nrm(wut_Vec3 in, wut_Vec3 out)
{
	f32 len = wut_vec3_len(in);
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


WUT_API void wut_vec3_setlen(wut_Vec3 in, f32 l, wut_Vec3 out)
{
	wut_Vec3 v;

	wut_vec3_nrm(in, v);
	wut_vec3_scl(v, l, out);
}


WUT_API f32 wut_vec3_dot(wut_Vec3 v1, wut_Vec3 v2)
{
	return (v1[0] * v2[0]) + (v1[1] * v2[1]) + (v1[2] * v2[2]);
}


WUT_API void wut_vec3_cross(wut_Vec3 v1, wut_Vec3 v2, wut_Vec3 out)
{
	out[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]);
	out[1] = (v1[2] * v2[0]) - (v1[0] * v2[2]);
	out[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);
}


WUT_API void wut_vec3_rot_x(wut_Vec3 in, f32 angle, wut_Vec3 out)
{
	wut_Mat3 rmat;
	wut_mat3_idt(rmat);
	rmat[0x0] =  1.0;
	rmat[0x4] =  cos(angle);
	rmat[0x5] = -sin(angle);
	rmat[0x7] =  sin(angle);
	rmat[0x8] =  cos(angle);
	wut_vec3rans(in, rmat, out);
}


WUT_API void wut_vec3_rot_y(wut_Vec3 in, f32 angle, wut_Vec3 out)
{
	wut_Mat3 rmat;
	wut_mat3_idt(rmat);
	rmat[0x0] =  cos(angle);
	rmat[0x2] =  sin(angle);
	rmat[0x3] =  1.0;
	rmat[0x6] = -sin(angle);
	rmat[0x8] =  cos(angle);
	wut_vec3rans(in, rmat, out);
}


WUT_API void wut_vec3_rot_z(wut_Vec3 in, f32 angle, wut_Vec3 out)
{
	wut_Mat3 rmat;
	wut_mat3_idt(rmat);
	rmat[0x0] =  cos(angle);
	rmat[0x1] = -sin(angle);
	rmat[0x3] =  sin(angle);
	rmat[0x4] =  cos(angle);
	rmat[0x8] =  1.0;
	wut_vec3rans(in, rmat, out);
}


WUT_API void wut_vec3_rot_axes(wut_Vec3 in, f32 angle, wut_Vec3 axis, wut_Vec3 out)
{
	wut_Mat3 rmat;
	f32 q0;
	f32 q1;
	f32 q2;
	f32 q3;

	wut_vec3_nrm(axis, axis);
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

	wut_vec3rans(in, rmat, out);
}


WUT_API void wut_vec3rans(wut_Vec3 in, wut_Mat3 mat, wut_Vec3 out)
{
	wut_Vec3 tmp;
	wut_vec3_cpy(tmp, in);

	out[0] = tmp[0] * mat[0x0] + tmp[1] * mat[0x3] + tmp[2] * mat[0x6];
	out[1] = tmp[0] * mat[0x1] + tmp[1] * mat[0x4] + tmp[2] * mat[0x7];
	out[2] = tmp[0] * mat[0x2] + tmp[1] * mat[0x5] + tmp[2] * mat[0x8];
}


WUT_API void wut_vec3_calc_nrm(wut_Vec3 p0, wut_Vec3 p1, wut_Vec3 p2, wut_Vec3 nrm)
{
	wut_Vec3 d1;
	wut_Vec3 d2;

	wut_vec3_sub(p1, p0, d1);
	wut_vec3_sub(p2, p0, d2);

	wut_vec3_cross(d1, d2, nrm);
	wut_vec3_nrm(nrm, nrm);
}


WUT_API f32 wut_vec3_barry_centric(wut_Vec3 p0, wut_Vec3 p1, wut_Vec3 p2, wut_Vec2 pos)
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


WUT_API void wut_vec3_flip(wut_Vec3 in, wut_Vec3 out)
{
	out[0] = -in[0];
	out[1] = -in[1];
	out[2] = -in[2];
}


WUT_API f32 wut_vec3_angle(wut_Vec3 v1, wut_Vec3 v2)
{
	f32 a;
	f32 b;

	a = wut_vec3_dot(v1, v2);
	b = WUT_ABS(wut_vec3_len(v1)) * WUT_ABS(wut_vec3_len(v2));

	return acos(a / b);
}


WUT_API void wut_vec3_interp(wut_Vec3 v1, wut_Vec3 v2, f32 p, wut_Vec3 out)
{
	wut_Vec3 conv;

	conv[0] = v1[0] + (v2[0] - v1[0]) * p;
	conv[1] = v1[1] + (v2[1] - v1[1]) * p;
	conv[2] = v1[2] + (v2[2] - v1[2]) * p;

	wut_vec3_cpy(out, conv);
}


WUT_API void wut_vec3_print(wut_Vec3 v)
{
	printf("%.2f/%.2f/%.2f", v[0], v[1], v[2]);
}


/*
 *
 * 4D-Vector
 *
 */


WUT_API void wut_vec4_set(wut_Vec4 out, f32 x, f32 y, f32 z, f32 w)
{
	out[0] = x;
	out[1] = y;
	out[2] = z;
	out[3] = w;
}


WUT_API void wut_vec4_clr(wut_Vec4 in)
{
	memset(in, 0, WUT_VEC4_SIZE);
}


WUT_API void wut_vec4_cpy(wut_Vec4 out, wut_Vec4 in)
{
	memcpy(out, in, WUT_VEC4_SIZE);
}


WUT_API s32 wut_vec4_cmp(wut_Vec4 in1, wut_Vec4 in2)
{
	if(in1[0] != in2[0] || in1[1] != in2[1] || in1[2] != in2[2] ||
			in1[3] != in2[3])
		return 0;

	return 1;
}


WUT_API void wut_vec4_add(wut_Vec4 in1, wut_Vec4 in2, wut_Vec4 out)
{
	out[0] = in1[0] + in2[0];
	out[1] = in1[1] + in2[1];
	out[2] = in1[2] + in2[2];
	out[3] = in1[3] + in2[3];
}


WUT_API void wut_vec4_sub(wut_Vec4 in1, wut_Vec4 in2, wut_Vec4 out)
{
	out[0] = in1[0] - in2[0];
	out[1] = in1[1] - in2[1];
	out[2] = in1[2] - in2[2];
	out[3] = in1[3] - in2[3];
}


WUT_API void wut_vec4_scl(wut_Vec4 in, f32 f, wut_Vec4 out)
{
	out[0] = in[0] * f;
	out[1] = in[1] * f;
	out[2] = in[2] * f;
	out[3] = in[3] * f;
}


WUT_API f32 wut_vec4_len(wut_Vec4 in)
{
	f64 l;
	l = (in[0] * in[0]) + (in[1] * in[1]) + (in[2] * in[2]) + (in[3] * in[3]);
	return (f32)sqrt(l);
}


WUT_API void wut_vec4_nrm(wut_Vec4 in, wut_Vec4 out)
{
	f32 len = wut_vec4_len(in);
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


WUT_API f32 wut_vec4_dot(wut_Vec4 in1, wut_Vec4 in2)
{
	return (in1[0] * in2[0]) + (in1[1] * in2[1]) + (in1[2] * in2[2]) +
		(in1[3] * in2[3]);
}


WUT_API void wut_vec4_trans(wut_Vec4 in, wut_Mat4 mat, wut_Vec4 out)
{
	wut_Vec4 tmp;
	wut_vec4_cpy(tmp, in);

	out[0] = tmp[0] * mat[0x0] + tmp[1] * mat[0x4] + tmp[2] * mat[0x8] + tmp[3] * mat[0xc];
	out[1] = tmp[0] * mat[0x1] + tmp[1] * mat[0x5] + tmp[2] * mat[0x9] + tmp[3] * mat[0xd];
	out[2] = tmp[0] * mat[0x2] + tmp[1] * mat[0x6] + tmp[2] * mat[0xa] + tmp[3] * mat[0xe];
	out[3] = tmp[0] * mat[0x3] + tmp[1] * mat[0x7] + tmp[2] * mat[0xb] + tmp[3] * mat[0xf];
}


WUT_API void wut_vec4_print(wut_Vec4 in)
{
	printf("%.2f/%.2f/%.2f/%.2f", in[0], in[1], in[2], in[3]);
}


/*
 * -=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-
 *
 * INTEGER VECTORS
 */

/*
 *
 * 2D-Vector
 *
 */

WUT_API void wut_ivec2_set(wut_iVec2 out, s32 x, s32 y)
{
	out[0] = x;
	out[1] = y;
}


WUT_API void wut_ivec2_clr(wut_iVec2 in)
{
	memset(in, 0, WUT_IVEC2_SIZE);
}


WUT_API void wut_ivec2_cpy(wut_iVec2 out, wut_iVec2 in)
{
	memcpy(out, in, WUT_IVEC2_SIZE);
}


WUT_API s32 wut_ivec2_cmp(wut_iVec2 in1, wut_iVec2 in2)
{
	if(in1[0] != in2[0] || in1[1] != in2[1])
		return 0;

	return 1;
}


WUT_API void wut_ivec2_add(wut_iVec2 in1, wut_iVec2 in2, wut_iVec2 out)
{
	out[0] = in1[0] + in2[0];
	out[1] = in1[1] + in2[1];
}


WUT_API void wut_ivec2_sub(wut_iVec2 in1, wut_iVec2 in2, wut_iVec2 out)
{
	out[0] = in1[0] - in2[0];
	out[1] = in1[1] - in2[1];
}


WUT_API void wut_ivec2_scl(wut_iVec2 in, f32 f, wut_iVec2 out)
{
	out[0] = in[0] * f;
	out[1] = in[1] * f;
}


WUT_API void wut_ivec2_inv_scl(wut_iVec2 in, f32 f, wut_iVec2 out)
{
	out[0] = in[0] / f;
	out[1] = in[1] / f;
}


WUT_API void wut_ivec2_mult(wut_iVec2 in1, wut_iVec2 in2, wut_iVec2 out)
{
	out[0] = in1[0] * in2[0];
	out[1] = in1[1] * in2[1];
}


WUT_API void wut_ivec2_div(wut_iVec2 in1, wut_iVec2 in2, wut_iVec2 out)
{
	out[0] = in1[0] / in2[0];
	out[1] = in1[1] / in2[1];
}


WUT_API f32 wut_ivec2_len(wut_iVec2 in)
{
	f64 l = (in[0] * in[0]) + (in[1] * in[1]);
	return (f32)sqrt(l);
}


WUT_API f32 wut_ivec2_sqrlen(wut_iVec2 in)
{
        f32 a = (f32)in[0];
        f32 b = (f32)in[1];

	return (a * a) + (b * b);
}


WUT_API void wut_ivec2_print(wut_iVec2 v)
{
	printf("%d/%d", v[0], v[1]);
}
