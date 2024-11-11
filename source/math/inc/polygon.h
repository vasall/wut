#ifndef _WUT_MATH_POLYGON_H
#define _WUT_MATH_POLYGON_H

#include "source/core/inc/define.h"

#include <math.h>

#define WUT_IPNT_SIZE   (2*sizeof(s32))
typedef s32 wut_iPnt[2];

#define WUT_IRECT_SIZE  (4*sizeof(s32))
typedef s32 wut_iRect[4];

#include "source/math/inc/vector.h"

/*
 * The default use for edges is:
 *
 *  - 0: Top
 *  - 1: Right
 *  - 2: Bottom
 *  - 3: Left
 */
#define WUT_IEDGES_SIZE (4*sizeof(s32))
typedef s32 wut_iEdges[4];

WUT_API void wut_irect_clr(wut_iRect r);
WUT_API void wut_irect_set(wut_iRect r, s32 x, s32 y, s32 w, s32 h);
WUT_API void wut_irect_cpy(wut_iRect out, wut_iRect in);
WUT_API void wut_irect_add(wut_iRect out, wut_iRect in1, wut_iRect in2);



WUT_API void wut_irect_get_edges(wut_iEdges out, wut_iRect rect);
WUT_API void wut_irect_from_edges(wut_iRect out, wut_iEdges lim);


/*
 * Check if two rectangles intersect.
 *
 * @in1: The first rectangle
 * @in2: The second rectangle
 *
 * Returns: 1 if they intersect and 0 if not
 */
WUT_API s8 wut_irect_intersecting(wut_iRect in1, wut_iRect in2);

/*
 * Check if the two given rectangles intersect and then return the intersection
 * rectangle.
 *
 * @out: The rectangle to write the intersecting area to
 * @in1: The first rectangle
 * @in2: The second rectangle
 *
 * Returns: 1 if they intersect and 0 if not
 */
WUT_API s8 wut_irect_intersect(wut_iRect out, wut_iRect in1, wut_iRect in2);


WUT_API void wut_irect_mov(wut_iRect out, wut_iRect in, wut_iVec2 del);


WUT_API void wut_irect_dump(wut_iRect r);
WUT_API void wut_iedges_dump(wut_iEdges lim);

#endif /* _WUT_MATH_POLYGON_H */
