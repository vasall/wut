#ifndef _WUT_MATH_VECTOR_H
#define _WUT_MATH_VECTOR_H

#include "source/core/inc/define.h"

#include <math.h>

#define WUT_VEC2_SIZE (F32_S*2)
typedef f32 wut_Vec2[2];

#define WUT_VEC3_SIZE (F32_S*3)
typedef f32 wut_Vec3[3];

#define WUT_VEC4_SIZE (F32_S*4)
typedef f32 wut_Vec4[4];


#define WUT_IVEC2_SIZE (S32_S*2)
typedef s32 wut_iVec2[2];

#define WUT_IVEC3_SIZE (S32_S*3)
typedef s32 wut_iVec3[3];

#define WUT_IVEC4_SIZE (S32_S*4)
typedef s32 wut_iVec4[4];

#include "source/math/inc/matrix.h"


/*
 * FLOAT VECTORS
 */

WUT_API void   wut_vec2_set(wut_Vec2 out, f32 x, f32 y);
WUT_API void   wut_vec2_clr(wut_Vec2 in);
WUT_API void   wut_vec2_cpy(wut_Vec2 out, wut_Vec2 in);
WUT_API int    wut_vec2_cmp(wut_Vec2 in1, wut_Vec2 in2);
WUT_API void   wut_vec2_add(wut_Vec2 in1, wut_Vec2 in2, wut_Vec2 out);
WUT_API void   wut_vec2_sub(wut_Vec2 in1, wut_Vec2 in2, wut_Vec2 out);
WUT_API void   wut_vec2_scl(wut_Vec2 in, f32 f, wut_Vec2 out);
WUT_API void   wut_vec2_inv_scl(wut_Vec2 in, f32 f, wut_Vec2 out);
WUT_API void   wut_vec2_mult(wut_Vec2 in1, wut_Vec2 in2, wut_Vec2 out);
WUT_API void   wut_vec2_div(wut_Vec2 in1, wut_Vec2 in2, wut_Vec2 out);
WUT_API f32    wut_vec2_len(wut_Vec2 in);
WUT_API float  wut_vec2_sqrlen(wut_Vec2 in);
WUT_API void   wut_vec2_nrm(wut_Vec2 in, wut_Vec2 out);
WUT_API f32    wut_vec2_dot(wut_Vec2 in1, wut_Vec2 in2);
WUT_API void   wut_vec2_print(wut_Vec2 in);



WUT_API void   wut_vec3_set(wut_Vec3 out, f32 x, f32 y, f32 z);
WUT_API void   wut_vec3_clr(wut_Vec3 in);
WUT_API void   wut_vec3_cpy(wut_Vec3 out, wut_Vec3 in);
WUT_API void   wut_vec3_cpy_v2(wut_Vec3 out, wut_Vec2 in);
WUT_API int    wut_vec3_cmp(wut_Vec3 in1, wut_Vec3 in2);
WUT_API void   wut_vec3_add(wut_Vec3 in1, wut_Vec3 in2, wut_Vec3 out);
WUT_API void   wut_vec3_sub(wut_Vec3 in1, wut_Vec3 in2, wut_Vec3 out);
WUT_API void   wut_vec3_scl(wut_Vec3 in, f32 f, wut_Vec3 out);
WUT_API void   wut_vec3_inv_scl(wut_Vec3 in, f32 f, wut_Vec3 out);
WUT_API void   wut_vec3_mult(wut_Vec3 in1, wut_Vec3 in2, wut_Vec3 out);
WUT_API void   wut_vec3_div(wut_Vec3 in1, wut_Vec3 in2, wut_Vec3 out);
WUT_API f32    wut_vec3_len(wut_Vec3 in);
WUT_API f32    wut_vec3_sqrlen(wut_Vec3 in);
WUT_API void   wut_vec3_nrm(wut_Vec3 in, wut_Vec3 out);
WUT_API void   wut_vec3_setlen(wut_Vec3 in, f32 l, wut_Vec3 out);
WUT_API f32    wut_vec3_dot(wut_Vec3 in1, wut_Vec3 in2);
WUT_API void   wut_vec3_cross(wut_Vec3 in1, wut_Vec3 in2, wut_Vec3 out);
WUT_API void   wut_vec3_rot_x(wut_Vec3 in, f32 angle, wut_Vec3 out);
WUT_API void   wut_vec3_rot_y(wut_Vec3 in, f32 angle, wut_Vec3 out);
WUT_API void   wut_vec3_rot_z(wut_Vec3 in, f32 angle, wut_Vec3 out);
WUT_API void   wut_vec3_rot_axes(wut_Vec3 in, f32 angle, wut_Vec3 axis, wut_Vec3 out);
WUT_API void   wut_vec3rans(wut_Vec3 in, wut_Mat3 wut_mat, wut_Vec3 out);
WUT_API void   wut_vec3_calc_nrm(wut_Vec3 p0, wut_Vec3 p1, wut_Vec3 p2, wut_Vec3 nrm);
WUT_API f32    wut_vec3_barry_centric(wut_Vec3 p0, wut_Vec3 p1, wut_Vec3 p2, wut_Vec2 pos);
WUT_API void   wut_vec3_flip(wut_Vec3 in, wut_Vec3 out);
WUT_API f32    wut_vec3_angle(wut_Vec3 v1, wut_Vec3 v2);
WUT_API void   wut_vec3_interp(wut_Vec3 v1, wut_Vec3 v2, f32 p, wut_Vec3 out);
WUT_API void   wut_vec3_print(wut_Vec3 in);



WUT_API void   wut_vec4_set(wut_Vec4 out, f32 x, f32 y, f32 z, f32 w);
WUT_API void   wut_vec4_clr(wut_Vec4 in);
WUT_API void   wut_vec4_cpy(wut_Vec4 out, wut_Vec4 in);
WUT_API s32    wut_vec4_cmp(wut_Vec4 in1, wut_Vec4 in2);
WUT_API void   wut_vec4_add(wut_Vec4 in1, wut_Vec4 in2, wut_Vec4 out);
WUT_API void   wut_vec4_sub(wut_Vec4 in1, wut_Vec4 in2, wut_Vec4 out);
WUT_API void   wut_vec4_scl(wut_Vec4 in1, f32 f, wut_Vec4 out);
WUT_API void   wut_vec4_mul(wut_Vec4 q1, wut_Vec4 q2, wut_Vec4 out);
WUT_API f32    wut_vec4_len(wut_Vec4 in);
WUT_API void   wut_vec4_nrm(wut_Vec4 in, wut_Vec4 out);
WUT_API f32    wut_vec4_dot(wut_Vec4 in1, wut_Vec4 in2);
WUT_API void   wut_vec4_trans(wut_Vec4 in, wut_Mat4 wut_mat, wut_Vec4 out);
WUT_API void   wut_vec4_print(wut_Vec4 in);


/*
 * INTEGER VECTORS
 */


WUT_API void   wut_ivec2_set(wut_iVec2 out, s32 x, s32 y);
WUT_API void   wut_ivec2_clr(wut_iVec2 in);
WUT_API void   wut_ivec2_cpy(wut_iVec2 out, wut_iVec2 in);
WUT_API int    wut_ivec2_cmp(wut_iVec2 in1, wut_iVec2 in2);
WUT_API void   wut_ivec2_add(wut_iVec2 in1, wut_iVec2 in2, wut_iVec2 out);
WUT_API void   wut_ivec2_sub(wut_iVec2 in1, wut_iVec2 in2, wut_iVec2 out);
WUT_API void   wut_ivec2_scl(wut_iVec2 in, f32 f, wut_iVec2 out);
WUT_API void   wut_ivec2_inv_scl(wut_iVec2 in, f32 f, wut_iVec2 out);
WUT_API void   wut_ivec2_mult(wut_iVec2 in1, wut_iVec2 in2, wut_iVec2 out);
WUT_API void   wut_ivec2_div(wut_iVec2 in1, wut_iVec2 in2, wut_iVec2 out);
WUT_API f32    wut_ivec2_len(wut_iVec2 in);
WUT_API f32    wut_ivec2_sqrlen(wut_iVec2 in);
WUT_API void   wut_ivec2_print(wut_iVec2 in);


#endif /* _WUT_MATH_VECTOR_H */
