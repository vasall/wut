#include "utility/inc/shape.h"

#include <stdlib.h>
#include <string.h>


WT_API void wt_rect_rst(struct wt_rect *r)
{
	r->x = 0;
	r->y = 0;
	r->w = 0;
	r->h = 0;
}

WT_API void wt_rect_set(struct wt_rect *r, s32 x, s32 y, s32 w, s32 h)
{
	r->x = x;
	r->y = y;
	r->w = w;
	r->h = h;
}


WT_API void wt_rect_cpy(struct wt_rect *out, struct wt_rect *in)
{
	memcpy(out, in, WT_RECT_SIZE);
}


WT_API void wt_rect_add(struct wt_rect *out, struct wt_rect *in1,
		struct wt_rect *in2)
{
	out->x = in1->x + in2->x;
	out->y = in1->y + in2->y;
	out->w = in1->w + in2->w;
	out->h = in1->h + in2->h;
}


WT_API void wt_rect_tolim(struct wt_rect_lim *out, struct wt_rect *rect)
{
	out->top = rect->y;
	out->right = rect->x + rect->w;
	out->bottom = rect->y + rect->h;
	out->left = rect->x;
}


WT_API void wt_rect_fromlim(struct wt_rect *out, struct wt_rect_lim *lim)
{
	out->x = lim->left;
	out->y = lim->top;
	out->w = lim->right - lim->left;
	out->h = lim->bottom - lim->top;
}


WT_API s8 wt_rect_intersecting(struct wt_rect *in1, struct wt_rect *in2)
{
	struct wt_rect_lim r1;
	struct wt_rect_lim r2;

	wt_rect_tolim(&r1, in1);
	wt_rect_tolim(&r2, in2);

	/*
	 * Verify that the two rectangles intersect.
	 */
	if(r1.left > r2.right || r1.right < r2.left)
		return 0;

	if(r1.top > r2.bottom || r1.bottom < r2.top)
		return 0;

	return 1;
}


WT_API s8 wt_rect_intersect(struct wt_rect *out, struct wt_rect *in1,
		struct wt_rect *in2)
{
	struct wt_rect_lim r1;
	struct wt_rect_lim r2;
	struct wt_rect_lim inter;

	wt_rect_tolim(&r1, in1);
	wt_rect_tolim(&r2, in2);

	/*
	 * Verify that the two rectangles intersect.
	 */
	if(r1.left > r2.right || r1.right < r2.left)
		return 0;

	if(r1.top > r2.bottom || r1.bottom < r2.top)
		return 0;

	/*
	 * Get the limits for all sides of the intersection area.
	 */
	inter.top =  r1.top > r2.top ? r1.top : r2.top;
	inter.right = r1.right < r2.right ? r1.right : r2.right;
	inter.bottom = r1.bottom < r2.bottom ? r1.bottom : r2.bottom;
	inter.left = r1.left > r2.left ? r1.left : r2.left;

	/*
	 * Convert limits to rectangle.
	 */
	wt_rect_fromlim(out, &inter);

	return 1;
}


WT_API void wt_rect_mov(struct wt_rect *out, struct wt_rect *in,
		struct wt_sin2 *del)
{
	out->x = in->x + del->x;
	out->y = in->y + del->y;
	out->w = in->w;
	out->h = in->h;
}


WT_API void wt_rect_dump(struct wt_rect *r)
{
	printf("%4d/%4d/%4d/%4d", r->x, r->y, r->w, r->h);
}


WT_API void wt_rect_lim_dump(struct wt_rect_lim *lim)
{
	printf("%4d/%4d/%4d/%4d",
			lim->top,
			lim->right,
			lim->bottom,
			lim->left);
}
