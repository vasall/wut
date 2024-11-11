#include "source/math/inc/functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


WUT_INTERN f32 mth_slope(wut_Vec2 a)
{ 
	if(a[0] == 0)
		return 0;

	return a[1] / a[0]; 
}

WUT_API f32 wut_Clamp(f32 v)
{
	if(v < 0.0) return 0.0;
	if(v > 1.0) return 1.0;
	return v;
}


WUT_API f32 wut_CalcDistance(f32 x, f32 y, f32 xp, f32 yp)
{
	return (sqrt((x - xp) * (x - xp) + (y - yp) * (y - yp)));
}


WUT_API s32 wut_LineCross(wut_Vec2 p0, wut_Vec2 v0, wut_Vec2 p1, wut_Vec2 v1, wut_Vec2 out)
{
	f32 slope_a = mth_slope(v0);
	f32 slope_b = mth_slope(v1);
	f32 del = slope_a - slope_b;

	if(slope_a == slope_b)
		return 0;

	out[0] = (slope_a * p0[0] - slope_b * p1[0] + p1[1] - p0[1]) / del;
	out[1] = slope_b * (out[0] - p1[0]) + p1[1];
 
	return 1;
}
