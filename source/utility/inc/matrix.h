#ifndef _FH_UTILITY_MATRIX_H
#define _FH_UTILITY_MATRIX_H

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


#define FH_VEC2_SIZE (F32_S*2)
typedef f32 fh_vec2_t[2];

#define FH_VEC3_SIZE (F32_S*3)
typedef f32 fh_vec3_t[3];

#define FH_VEC4_SIZE (F32_S*4)
typedef f32 fh_vec4_t[4];

#define FH_INT2_SIZE (S32_S*2)
typedef s32 fh_int2_t[2];

#define FH_INT3_SIZE (S32_S*3)
typedef s32 fh_int3_t[3];

#define FH_INT4_SIZE (S32_S*4)
typedef s32 fh_int4_t[4];

#define FH_SHORT2_SIZE (S16_S*2)
typedef s16 fh_short2_t[2];


#define NULLMAT ((f32 *)NULL)

#define MAT3_SIZE (sizeof(f32)*9)
typedef f32 fh_mat3_t[9];

#define MAT4_SIZE (sizeof(f32)*16)
typedef f32 fh_mat4_t[16];



FH_API f32 clamp(f32 v);
FH_API f32 dist(f32 x, f32 y, f32 xp, f32 yp);
FH_API s32 line_cross(fh_vec2_t p0, fh_vec2_t v0, fh_vec2_t p1, fh_vec2_t v1, fh_vec2_t out);



FH_API void   fh_vec2_set(fh_vec2_t out, f32 x, f32 y);
FH_API void   fh_vec2_clr(fh_vec2_t in);
FH_API void   fh_vec2_cpy(fh_vec2_t out, fh_vec2_t in);
FH_API int    fh_vec2_cmp(fh_vec2_t in1, fh_vec2_t in2);
FH_API void   fh_vec2_add(fh_vec2_t in1, fh_vec2_t in2, fh_vec2_t out);
FH_API void   fh_vec2_sub(fh_vec2_t in1, fh_vec2_t in2, fh_vec2_t out);
FH_API void   fh_vec2_scl(fh_vec2_t in, f32 f, fh_vec2_t out);
FH_API void   fh_vec2_inv_scl(fh_vec2_t in, f32 f, fh_vec2_t out);
FH_API void   fh_vec2_mult(fh_vec2_t in1, fh_vec2_t in2, fh_vec2_t out);
FH_API void   fh_vec2_div(fh_vec2_t in1, fh_vec2_t in2, fh_vec2_t out);
FH_API f32  fh_vec2_len(fh_vec2_t in);
FH_API float  fh_vec2_sqrlen(fh_vec2_t in);
FH_API void   fh_vec2_nrm(fh_vec2_t in, fh_vec2_t out);
FH_API f32  fh_vec2_dot(fh_vec2_t in1, fh_vec2_t in2);
FH_API void   fh_vec2_print(fh_vec2_t in);



FH_API void   fh_vec3_set(fh_vec3_t out, f32 x, f32 y, f32 z);
FH_API void   fh_vec3_clr(fh_vec3_t in);
FH_API void   fh_vec3_cpy(fh_vec3_t out, fh_vec3_t in);
FH_API void   fh_vec3_cpy_v2(fh_vec3_t out, fh_vec2_t in);
FH_API int    fh_vec3_cmp(fh_vec3_t in1, fh_vec3_t in2);
FH_API void   fh_vec3_add(fh_vec3_t in1, fh_vec3_t in2, fh_vec3_t out);
FH_API void   fh_vec3_sub(fh_vec3_t in1, fh_vec3_t in2, fh_vec3_t out);
FH_API void   fh_vec3_scl(fh_vec3_t in, f32 f, fh_vec3_t out);
FH_API void   fh_vec3_inv_scl(fh_vec3_t in, f32 f, fh_vec3_t out);
FH_API void   fh_vec3_mult(fh_vec3_t in1, fh_vec3_t in2, fh_vec3_t out);
FH_API void   fh_vec3_div(fh_vec3_t in1, fh_vec3_t in2, fh_vec3_t out);
FH_API f32  fh_vec3_len(fh_vec3_t in);
FH_API f32  fh_vec3_sqrlen(fh_vec3_t in);
FH_API void   fh_vec3_nrm(fh_vec3_t in, fh_vec3_t out);
FH_API void   fh_vec3_setlen(fh_vec3_t in, f32 l, fh_vec3_t out);
FH_API f32  fh_vec3_dot(fh_vec3_t in1, fh_vec3_t in2);
FH_API void   fh_vec3_cross(fh_vec3_t in1, fh_vec3_t in2, fh_vec3_t out);
FH_API void   fh_vec3_rot_x(fh_vec3_t in, f32 angle, fh_vec3_t out);
FH_API void   fh_vec3_rot_y(fh_vec3_t in, f32 angle, fh_vec3_t out);
FH_API void   fh_vec3_rot_z(fh_vec3_t in, f32 angle, fh_vec3_t out);
FH_API void   fh_vec3_rot_axes(fh_vec3_t in, f32 angle, fh_vec3_t axis, fh_vec3_t out);
FH_API void   fh_vec3_trans(fh_vec3_t in, fh_mat3_t fh_mat, fh_vec3_t out);
FH_API void   fh_vec3_calc_nrm(fh_vec3_t p0, fh_vec3_t p1, fh_vec3_t p2, fh_vec3_t nrm);
FH_API f32  fh_vec3_barry_centric(fh_vec3_t p0, fh_vec3_t p1, fh_vec3_t p2, fh_vec2_t pos);
FH_API void   fh_vec3_flip(fh_vec3_t in, fh_vec3_t out);
FH_API f32  fh_vec3_angle(fh_vec3_t v1, fh_vec3_t v2);
FH_API void   fh_vec3_angle_v2(fh_vec3_t, fh_vec2_t out);
FH_API void   fh_vec3_interp(fh_vec3_t v1, fh_vec3_t v2, f32 p, fh_vec3_t out);
FH_API void   fh_vec3_print(fh_vec3_t in);



FH_API void   fh_vec4_set(fh_vec4_t out, f32 x, f32 y, f32 z, f32 w);
FH_API void   fh_vec4_clr(fh_vec4_t in);
FH_API void   fh_vec4_cpy(fh_vec4_t out, fh_vec4_t in);
FH_API s32    fh_vec4_cmp(fh_vec4_t in1, fh_vec4_t in2);
FH_API void   fh_vec4_add(fh_vec4_t in1, fh_vec4_t in2, fh_vec4_t out);
FH_API void   fh_vec4_sub(fh_vec4_t in1, fh_vec4_t in2, fh_vec4_t out);
FH_API void   fh_vec4_scl(fh_vec4_t in1, f32 f, fh_vec4_t out);
FH_API void   fh_vec4_mul(fh_vec4_t q1, fh_vec4_t q2, fh_vec4_t out);
FH_API f32  fh_vec4_len(fh_vec4_t in);
FH_API void   fh_vec4_nrm(fh_vec4_t in, fh_vec4_t out);
FH_API f32  fh_vec4_dot(fh_vec4_t in1, fh_vec4_t in2);
FH_API void   fh_vec4_trans(fh_vec4_t in, fh_mat4_t fh_mat, fh_vec4_t out);
FH_API void   fh_vec4_print(fh_vec4_t in);

FH_API void fh_mat3_zero(fh_mat3_t fh_mat);
FH_API void fh_mat3_idt(fh_mat3_t fh_mat);
FH_API void fh_mat3_cpy(fh_mat3_t dst, fh_mat3_t src);
FH_API void fh_mat3_mult(fh_mat3_t m1, fh_mat3_t m2, fh_mat3_t res);
FH_API void fh_mat3_print(fh_mat3_t fh_mat);


FH_API void fh_mat4_zero(fh_mat4_t m);
FH_API void fh_mat4_idt(fh_mat4_t m);
FH_API void fh_mat4_cpy(fh_mat4_t out, fh_mat4_t in);
FH_API void fh_mat4_mult(fh_mat4_t m1, fh_mat4_t m2, fh_mat4_t out);
FH_API void fh_mat4_inv(fh_mat4_t out, fh_mat4_t in);
FH_API void fh_mat4_transp(fh_mat4_t out, fh_mat4_t in);
FH_API void fh_mat4_print(fh_mat4_t m);

FH_API void fh_mat4_std(fh_mat4_t m);

FH_API void fh_mat4_pfpos(fh_mat4_t m, fh_vec3_t v);
FH_API void fh_mat4_rfqat(fh_mat4_t m, fh_vec4_t v);
FH_API void fh_mat4_rfvec(fh_mat4_t m, fh_vec3_t v);
FH_API void fh_mat4_rfagl(fh_mat4_t m, fh_vec3_t v);

FH_API void fh_mat4_pfpos_s(fh_mat4_t m, f32 x, f32 y, f32 z);
FH_API void fh_mat4_rfqat_s(fh_mat4_t m, f32 w, f32 x, f32 y, f32 z);
FH_API void fh_mat4_rfagl_s(fh_mat4_t m, f32 x, f32 y, f32 z);

#endif /* _FH_UTILITY_MATRIX_H */

