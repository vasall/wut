#include "source/math/inc/polygon.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


WUT_API void wut_irect_clr(wut_iRect r)
{
	r[0] = 0;
	r[1] = 0;
	r[2] = 0;
	r[3] = 0;
}

WUT_API void wut_irect_set(wut_iRect r, s32 x, s32 y, s32 w, s32 h)
{
	r[0] = x;
	r[1] = y;
	r[2] = w;
	r[3] = h;
}


WUT_API void wut_irect_cpy(wut_iRect out, wut_iRect in)
{
	memcpy(out, in, WUT_IRECT_SIZE);
}


WUT_API void wut_irect_add(wut_iRect out, wut_iRect in1, wut_iRect in2)
{
	out[0] = in1[0] + in2[0];
	out[1] = in1[1] + in2[1];
	out[2] = in1[2] + in2[2];
	out[3] = in1[3] + in2[3];
}


WUT_API void wut_irect_get_edges(wut_iEdges out, wut_iRect rect)
{
	out[0] = rect[1];
	out[1] = rect[0] + rect[2];
	out[2] = rect[1] + rect[3];
	out[3] = rect[0];
}


WUT_API void wut_irect_from_edges(wut_iRect out, wut_iEdges lim)
{
	out[0] = lim[3];
	out[1] = lim[0];
	out[2] = lim[1] - lim[3];
	out[3] = lim[2] - lim[0];
}


WUT_API s8 wut_irect_intersecting(wut_iRect in1, wut_iRect in2)
{
	wut_iEdges r1;
	wut_iEdges r2;

	wut_irect_get_edges(r1, in1);
	wut_irect_get_edges(r2, in2);

	/*
	 * Verify that the two rectangles intersect.
	 */
	if(r1[3] > r2[1] || r1[1] < r2[3])
		return 0;

	if(r1[0] > r2[2] || r1[2] < r2[0])
		return 0;

	return 1;
}


WUT_API s8 wut_irect_intersect(wut_iRect out, wut_iRect in1, wut_iRect in2)
{
	wut_iEdges r1;
	wut_iEdges r2;
	wut_iEdges inter;

	wut_irect_get_edges(r1, in1);
	wut_irect_get_edges(r2, in2);

	/*
	 * Verify that the two rectangles intersect.
	 */
	if(r1[3] > r2[1] || r1[1] < r2[3])
		return 0;

	if(r1[0] > r2[2] || r1[2] < r2[0])
		return 0;

	/*
	 * Get the limits for all sides of the intersection area.
	 */
	inter[0] =  r1[0] > r2[0] ? r1[0] : r2[0];
	inter[1] = r1[1] < r2[1] ? r1[1] : r2[1];
	inter[2] = r1[2] < r2[2] ? r1[2] : r2[2];
	inter[3] = r1[3] > r2[3] ? r1[3] : r2[3];

	/*
	 * Convert limits to rectangle.
	 */
	wut_irect_from_edges(out, inter);

	return 1;
}


WUT_API void wut_irect_mov(wut_iRect out, wut_iRect in, wut_iVec2 del)
{
	out[0] = in[0] + del[0];
	out[1] = in[1] + del[1];
	out[2] = in[2];
	out[3] = in[3];
}


WUT_API void wut_irect_dump(wut_iRect r)
{
	printf("%4d/%4d/%4d/%4d", r[0], r[1], r[2], r[3]);
}


WUT_API void wut_iedges_dump(wut_iEdges lim)
{
	printf("%4d/%4d/%4d/%4d",
			lim[0],
			lim[1],
			lim[2],
			lim[3]);
}
