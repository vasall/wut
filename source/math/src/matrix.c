#include "math/inc/matrix.h"

#include "math/inc/functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 * 
 * 3x3-MATRIX
 *
 */

WUT_API void wut_mat3_zero(wut_Mat3 m) 
{
	memset(m, 0, MAT3_SIZE);
}


WUT_API void wut_mat3_idt(wut_Mat3 m) 
{
	wut_mat3_zero(m);
	m[0x0] = 1.0;
	m[0x4] = 1.0;
	m[0x8] = 1.0;
}


WUT_API void wut_mat3_cpy(wut_Mat3 out, wut_Mat3 in) 
{
	memcpy(out, in, MAT3_SIZE);
}


WUT_API void wut_mat3_mult(wut_Mat3 m1, wut_Mat3 m2, wut_Mat3 res)
{
	s32 i, j, k;
	wut_Mat3 ret;
	wut_mat3_zero(ret);
	wut_mat3_zero(res);

	for(i = 0; i < 3; i++) {
		for(j = 0; j < 3; j++) {
			ret[j * 4 + i] = 0;

			for(k = 0; k < 3; k++)
				ret[j * 3 + i] += m1[k * 3 + i] * 
					m2[j * 3 + k];
		}
	}

	wut_mat3_cpy(res, ret);
}


WUT_API void wut_mat3_print(wut_Mat3 m)
{
	s32 r, c;

	for(r = 0; r < 3; r++) {
		for(c = 0; c < 3; c++)
			printf("%6.2f ", m[c * 3 + r]);

		printf("\n");
	}
}


/*
 * 
 * 4x4-MATRIX
 *
 */


WUT_API void wut_mat4_zero(wut_Mat4 m)
{
	s32 i;

	for(i = 0; i < 16; i++)
		m[i] = 0.0;
}


WUT_API void wut_mat4_idt(wut_Mat4 m)
{
	wut_mat4_zero(m);
	m[0x00] = 1.0;
	m[0x05] = 1.0;
	m[0x0a] = 1.0;
	m[0x0f] = 1.0;
}


WUT_API void wut_mat4_cpy(wut_Mat4 out, wut_Mat4 in)
{
	if(in) {
		memcpy(out, in, MAT4_SIZE);
        }
}


WUT_API void wut_mat4_mult(wut_Mat4 m1, wut_Mat4 m2, wut_Mat4 out)
{
	s32 i, j, k;
	f32 a, b;
	wut_Mat4 conv;
	wut_mat4_zero(conv);

	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			for(k = 0; k < 4; k++) {
				a = m1[k * 4 + i];
				b = m2[j * 4 + k];

				conv[j * 4 + i] += a * b;
			}
		}
	}

	wut_mat4_cpy(out, conv);
}


WUT_API void wut_mat4_inv(wut_Mat4 out, wut_Mat4 in)
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


WUT_API void wut_mat4ransp(wut_Mat4 out, wut_Mat4 in)
{
	wut_Mat4 tmp;

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

	wut_mat4_cpy(out, tmp);
}


WUT_API void wut_mat4_print(wut_Mat4 m)
{
	s32 r, c;

	for(r = 0; r < 4; r++) {
		for(c = 0; c < 4; c++)
			printf("%.6f ", m[r + c * 4]);

		printf("\n");
	}
}


WUT_API void wut_mat4_std(wut_Mat4 m)
{
	s32 i;

	for(i = 0; i < 16; i++) {
		if(m[i] == 0.0) {
			m[i] = 0.0;
		}
	}
}


WUT_API void wut_mat4_pfpos(wut_Mat4 m, wut_Vec3 v)
{	
	m[0xc] = v[0];
	m[0xd] = v[1];
	m[0xe] = v[2];

	m[0xf] = 1.0;
}


WUT_API void wut_mat4_rfqat(wut_Mat4 m, wut_Vec4 v)
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


WUT_API void wut_mat4_rfvec(wut_Mat4 m, wut_Vec3 v)
{
	wut_Vec3 up = {0, 0, 1};
	wut_Vec3 xaxis;
	wut_Vec3 yaxis;

	wut_vec3_cross(up, v, xaxis);
	wut_vec3_nrm(xaxis, xaxis);

	wut_vec3_cross(v, xaxis, yaxis);
	wut_vec3_nrm(yaxis, yaxis);

	wut_mat4_idt(m);

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
WUT_API void wut_mat4_rfagl(wut_Mat4 m, wut_Vec3 v)
{
	f32 x = WUT_DEG_TO_RAD(v[0]);
	f32 y = WUT_DEG_TO_RAD(v[1]);
	f32 z = WUT_DEG_TO_RAD(v[2]);

	/* Roll */
	f32 cosY = cos(y);
	f32 sinY = sin(y);

	/* Pitch */
	f32 cosP = cos(x);
	f32 sinP = sin(x);

	/* Yaw */
	f32 cosR = cos(z);
	f32 sinR = sin(z);

	wut_mat4_idt(m);
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


WUT_API void wut_mat4_pfpos_s(wut_Mat4 m, f32 x, f32 y, f32 z)
{	
	m[0xc] = x;
	m[0xd] = y;
	m[0xe] = z;

	m[0xf] = 1.0;
}


WUT_API void wut_mat4_rfqat_s(wut_Mat4 m, f32 w, f32 x, f32 y, f32 z)
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


/* see wut_mat4_rfagl() for parameter info */
WUT_API void wut_mat4_rfagl_s(wut_Mat4 m, f32 x, f32 y, f32 z)
{
	wut_Vec3 v;
	wut_Mat4 c;

	wut_vec3_set(v, x, y, z);
	wut_mat4_rfagl(c, v);
	wut_mat4_cpy(m, c);
}


