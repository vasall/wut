#ifndef _WUT_MATH_MATRIX_H
#define _WUT_MATH_MATRIX_H

#include "core/inc/define.h"


#define NULLMAT ((f32 *)NULL)

#define MAT3_SIZE (sizeof(f32)*9)
typedef f32 wut_Mat3[9];

#define MAT4_SIZE (sizeof(f32)*16)
typedef f32 wut_Mat4[16];


#include "math/inc/vector.h"


WUT_API void   wut_mat3_zero(wut_Mat3 m);
WUT_API void   wut_mat3_idt(wut_Mat3 m);
WUT_API void   wut_mat3_cpy(wut_Mat3 dst, wut_Mat3 src);
WUT_API void   wut_mat3_mult(wut_Mat3 m1, wut_Mat3 m2, wut_Mat3 res);
WUT_API void   wut_mat3_print(wut_Mat3 m);



WUT_API void   wut_mat4_zero(wut_Mat4 m);
WUT_API void   wut_mat4_idt(wut_Mat4 m);
WUT_API void   wut_mat4_cpy(wut_Mat4 out, wut_Mat4 in);
WUT_API void   wut_mat4_mult(wut_Mat4 m1, wut_Mat4 m2, wut_Mat4 out);
WUT_API void   wut_mat4_inv(wut_Mat4 out, wut_Mat4 in);
WUT_API void   wut_mat4ransp(wut_Mat4 out, wut_Mat4 in);
WUT_API void   wut_mat4_print(wut_Mat4 m);

WUT_API void   wut_mat4_std(wut_Mat4 m);

WUT_API void   wut_mat4_pfpos(wut_Mat4 m, wut_Vec3 v);
WUT_API void   wut_mat4_rfqat(wut_Mat4 m, wut_Vec4 v);
WUT_API void   wut_mat4_rfvec(wut_Mat4 m, wut_Vec3 v);
WUT_API void   wut_mat4_rfagl(wut_Mat4 m, wut_Vec3 v);

WUT_API void   wut_mat4_pfpos_s(wut_Mat4 m, f32 x, f32 y, f32 z);
WUT_API void   wut_mat4_rfqat_s(wut_Mat4 m, f32 w, f32 x, f32 y, f32 z);
WUT_API void   wut_mat4_rfagl_s(wut_Mat4 m, f32 x, f32 y, f32 z);

#endif /* _WUT_MATH_MATRIX_H */
