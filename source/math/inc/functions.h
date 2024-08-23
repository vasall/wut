#ifndef _WUT_MATH_FUNCTIONS_H
#define _WUT_MATH_FUNCTIONS_H

#include "core/inc/define.h"

#include "math/inc/vector.h"

#define WUT_PI 3.14159265358979323846

#define WUT_DEG_TO_RAD(x) (WUT_PI/180.0*x)
#define WUT_RAD_TO_DEG(x) (180.0/WUT_PI*x)

#define WUT_ABS(x) ((x < 0) ? (-x) : (x))
#define WUT_MIN(x, y) (((x) < (y)) ? (x) : (y))
#define WUT_MAX(x, y) (((x) > (y)) ? (x) : (y))
#define WUT_ROUND(x) ((x) >= 0 ? (s64)((x) + 0.5) : (s64)((x) - 0.5))
#define WUT_SIGN(x) ((x/ABS(x)))


#define WUT_INT2_SIZE (S32_S*2)
typedef s32 wut_Int2[2];

#define WUT_INT3_SIZE (S32_S*3)
typedef s32 wut_Int3[3];

#define WUT_INT4_SIZE (S32_S*4)
typedef s32 wut_Int4[4];

#define WUT_SHORT2_SIZE (S16_S*2)
typedef s16 wut_Short2[2];


WUT_API f32 wut_Clamp(f32 v);
WUT_API f32 wut_CalcDistance(f32 x, f32 y, f32 xp, f32 yp);
WUT_API s32 wut_LineCross(wut_Vec2 out, wut_Vec2 p0, wut_Vec2 v0,
                wut_Vec2 p1, wut_Vec2 v1);


#endif /* _WUT_MATH_FUNCTIONS_H */

