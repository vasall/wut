#ifndef _WT_UTILITY_MATRIX_H
#define _WT_UTILITY_MATRIX_H

#include "core/inc/define.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define DEG_TO_RAD(x) (M_PI/180.0*x)
#define RAD_TO_DEG(x) (180.0/M_PI*x)

#define ABS(x) ((x < 0) ? (-x) : (x))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define ROUND(x) ((x) >= 0 ? (s64)((x) + 0.5) : (s64)((x) - 0.5))
#define SIGN(x) ((x/ABS(x)))
#define POW2(x) (x * x)


#define WT_VEC2_SIZE (F32_S*2)
typedef f32 wt_vec2_t[2];

#define WT_VEC3_SIZE (F32_S*3)
typedef f32 wt_vec3_t[3];

#define WT_VEC4_SIZE (F32_S*4)
typedef f32 wt_vec4_t[4];

#define WT_INT2_SIZE (S32_S*2)
typedef s32 wt_int2_t[2];

#define WT_INT3_SIZE (S32_S*3)
typedef s32 wt_int3_t[3];

#define WT_INT4_SIZE (S32_S*4)
typedef s32 wt_int4_t[4];

#define WT_SHORT2_SIZE (S16_S*2)
typedef s16 wt_short2_t[2];


#define NULLMAT ((f32 *)NULL)

#define MAT3_SIZE (sizeof(f32)*9)
typedef f32 wt_mat3_t[9];

#define MAT4_SIZE (sizeof(f32)*16)
typedef f32 wt_mat4_t[16];



WT_API f32 clamp(f32 v);
WT_API f32 dist(f32 x, f32 y, f32 xp, f32 yp);
WT_API s32 line_cross(wt_vec2_t p0, wt_vec2_t v0, wt_vec2_t p1, wt_vec2_t v1, wt_vec2_t out);



WT_API void   wt_vec2_set(wt_vec2_t out, f32 x, f32 y);
WT_API void   wt_vec2_clr(wt_vec2_t in);
WT_API void   wt_vec2_cpy(wt_vec2_t out, wt_vec2_t in);
WT_API int    wt_vec2_cmp(wt_vec2_t in1, wt_vec2_t in2);
WT_API void   wt_vec2_add(wt_vec2_t in1, wt_vec2_t in2, wt_vec2_t out);
WT_API void   wt_vec2_sub(wt_vec2_t in1, wt_vec2_t in2, wt_vec2_t out);
WT_API void   wt_vec2_scl(wt_vec2_t in, f32 f, wt_vec2_t out);
WT_API void   wt_vec2_inv_scl(wt_vec2_t in, f32 f, wt_vec2_t out);
WT_API void   wt_vec2_mult(wt_vec2_t in1, wt_vec2_t in2, wt_vec2_t out);
WT_API void   wt_vec2_div(wt_vec2_t in1, wt_vec2_t in2, wt_vec2_t out);
WT_API f32  wt_vec2_len(wt_vec2_t in);
WT_API float  wt_vec2_sqrlen(wt_vec2_t in);
WT_API void   wt_vec2_nrm(wt_vec2_t in, wt_vec2_t out);
WT_API f32  wt_vec2_dot(wt_vec2_t in1, wt_vec2_t in2);
WT_API void   wt_vec2_print(wt_vec2_t in);



WT_API void   wt_vec3_set(wt_vec3_t out, f32 x, f32 y, f32 z);
WT_API void   wt_vec3_clr(wt_vec3_t in);
WT_API void   wt_vec3_cpy(wt_vec3_t out, wt_vec3_t in);
WT_API void   wt_vec3_cpy_v2(wt_vec3_t out, wt_vec2_t in);
WT_API int    wt_vec3_cmp(wt_vec3_t in1, wt_vec3_t in2);
WT_API void   wt_vec3_add(wt_vec3_t in1, wt_vec3_t in2, wt_vec3_t out);
WT_API void   wt_vec3_sub(wt_vec3_t in1, wt_vec3_t in2, wt_vec3_t out);
WT_API void   wt_vec3_scl(wt_vec3_t in, f32 f, wt_vec3_t out);
WT_API void   wt_vec3_inv_scl(wt_vec3_t in, f32 f, wt_vec3_t out);
WT_API void   wt_vec3_mult(wt_vec3_t in1, wt_vec3_t in2, wt_vec3_t out);
WT_API void   wt_vec3_div(wt_vec3_t in1, wt_vec3_t in2, wt_vec3_t out);
WT_API f32  wt_vec3_len(wt_vec3_t in);
WT_API f32  wt_vec3_sqrlen(wt_vec3_t in);
WT_API void   wt_vec3_nrm(wt_vec3_t in, wt_vec3_t out);
WT_API void   wt_vec3_setlen(wt_vec3_t in, f32 l, wt_vec3_t out);
WT_API f32  wt_vec3_dot(wt_vec3_t in1, wt_vec3_t in2);
WT_API void   wt_vec3_cross(wt_vec3_t in1, wt_vec3_t in2, wt_vec3_t out);
WT_API void   wt_vec3_rot_x(wt_vec3_t in, f32 angle, wt_vec3_t out);
WT_API void   wt_vec3_rot_y(wt_vec3_t in, f32 angle, wt_vec3_t out);
WT_API void   wt_vec3_rot_z(wt_vec3_t in, f32 angle, wt_vec3_t out);
WT_API void   wt_vec3_rot_axes(wt_vec3_t in, f32 angle, wt_vec3_t axis, wt_vec3_t out);
WT_API void   wt_vec3_trans(wt_vec3_t in, wt_mat3_t wt_mat, wt_vec3_t out);
WT_API void   wt_vec3_calc_nrm(wt_vec3_t p0, wt_vec3_t p1, wt_vec3_t p2, wt_vec3_t nrm);
WT_API f32  wt_vec3_barry_centric(wt_vec3_t p0, wt_vec3_t p1, wt_vec3_t p2, wt_vec2_t pos);
WT_API void   wt_vec3_flip(wt_vec3_t in, wt_vec3_t out);
WT_API f32  wt_vec3_angle(wt_vec3_t v1, wt_vec3_t v2);
WT_API void   wt_vec3_angle_v2(wt_vec3_t, wt_vec2_t out);
WT_API void   wt_vec3_interp(wt_vec3_t v1, wt_vec3_t v2, f32 p, wt_vec3_t out);
WT_API void   wt_vec3_print(wt_vec3_t in);



WT_API void   wt_vec4_set(wt_vec4_t out, f32 x, f32 y, f32 z, f32 w);
WT_API void   wt_vec4_clr(wt_vec4_t in);
WT_API void   wt_vec4_cpy(wt_vec4_t out, wt_vec4_t in);
WT_API s32    wt_vec4_cmp(wt_vec4_t in1, wt_vec4_t in2);
WT_API void   wt_vec4_add(wt_vec4_t in1, wt_vec4_t in2, wt_vec4_t out);
WT_API void   wt_vec4_sub(wt_vec4_t in1, wt_vec4_t in2, wt_vec4_t out);
WT_API void   wt_vec4_scl(wt_vec4_t in1, f32 f, wt_vec4_t out);
WT_API void   wt_vec4_mul(wt_vec4_t q1, wt_vec4_t q2, wt_vec4_t out);
WT_API f32  wt_vec4_len(wt_vec4_t in);
WT_API void   wt_vec4_nrm(wt_vec4_t in, wt_vec4_t out);
WT_API f32  wt_vec4_dot(wt_vec4_t in1, wt_vec4_t in2);
WT_API void   wt_vec4_trans(wt_vec4_t in, wt_mat4_t wt_mat, wt_vec4_t out);
WT_API void   wt_vec4_print(wt_vec4_t in);

WT_API void wt_mat3_zero(wt_mat3_t wt_mat);
WT_API void wt_mat3_idt(wt_mat3_t wt_mat);
WT_API void wt_mat3_cpy(wt_mat3_t dst, wt_mat3_t src);
WT_API void wt_mat3_mult(wt_mat3_t m1, wt_mat3_t m2, wt_mat3_t res);
WT_API void wt_mat3_print(wt_mat3_t wt_mat);


WT_API void wt_mat4_zero(wt_mat4_t m);
WT_API void wt_mat4_idt(wt_mat4_t m);
WT_API void wt_mat4_cpy(wt_mat4_t out, wt_mat4_t in);
WT_API void wt_mat4_mult(wt_mat4_t m1, wt_mat4_t m2, wt_mat4_t out);
WT_API void wt_mat4_inv(wt_mat4_t out, wt_mat4_t in);
WT_API void wt_mat4_transp(wt_mat4_t out, wt_mat4_t in);
WT_API void wt_mat4_print(wt_mat4_t m);

WT_API void wt_mat4_std(wt_mat4_t m);

WT_API void wt_mat4_pfpos(wt_mat4_t m, wt_vec3_t v);
WT_API void wt_mat4_rfqat(wt_mat4_t m, wt_vec4_t v);
WT_API void wt_mat4_rfvec(wt_mat4_t m, wt_vec3_t v);
WT_API void wt_mat4_rfagl(wt_mat4_t m, wt_vec3_t v);

WT_API void wt_mat4_pfpos_s(wt_mat4_t m, f32 x, f32 y, f32 z);
WT_API void wt_mat4_rfqat_s(wt_mat4_t m, f32 w, f32 x, f32 y, f32 z);
WT_API void wt_mat4_rfagl_s(wt_mat4_t m, f32 x, f32 y, f32 z);

#endif /* _WT_UTILITY_MATRIX_H */

