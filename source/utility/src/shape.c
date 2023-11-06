#include "utility/inc/shape.h"

#include <stdlib.h>
#include <string.h>


FH_API void fh_rect_rst(struct fh_rect *r)
{
	r->x = 0;
	r->y = 0;
	r->w = 0;
	r->h = 0;
}

FH_API void fh_rect_set(struct fh_rect *r, s32 x, s32 y, s32 w, s32 h)
{
	r->x = x;
	r->y = y;
	r->w = w;
	r->h = h;
}


FH_API void fh_rect_cpy(struct fh_rect *out, struct fh_rect *in)
{
	memcpy(out, in, FH_RECT_SIZE);
}


FH_API void fh_rect_add(struct fh_rect *out, struct fh_rect *in1,
		struct fh_rect *in2)
{
	out->x = in1->x + in2->x;
	out->y = in1->y + in2->y;
	out->w = in1->w + in2->w;
	out->h = in1->h + in2->h;
}


FH_API void fh_rect_tolim(struct fh_rect_lim *out, struct fh_rect *rect)
{
	out->top = rect->y;
	out->right = rect->x + rect->w;
	out->bottom = rect->y + rect->h;
	out->left = rect->x;
}


FH_API void fh_rect_fromlim(struct fh_rect *out, struct fh_rect_lim *lim)
{
	out->x = lim->left;
	out->y = lim->top;
	out->w = lim->right - lim->left;
	out->h = lim->bottom - lim->top;
}


FH_API s8 fh_rect_intersect(struct fh_rect *out, struct fh_rect *in1,
		struct fh_rect *in2)
{
	struct fh_rect_lim r1;
	struct fh_rect_lim r2;
	struct fh_rect_lim inter;

	fh_rect_tolim(&r1, in1);
	fh_rect_tolim(&r2, in2);

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
	fh_rect_fromlim(out, &inter);

	return 1;
}


FH_API void fh_rect_mov(struct fh_rect *out, struct fh_rect *in,
		struct fh_sin2 *del)
{
	out->x = in->x + del->x;
	out->y = in->y + del->y;
	out->w = in->w;
	out->h = in->h;
}


FH_API void fh_rect_dump(struct fh_rect *r)
{
	printf("%4d/%4d/%4d/%4d", r->x, r->y, r->w, r->h);
}


FH_API void fh_rect_lim_dump(struct fh_rect_lim *lim)
{
	printf("%4d/%4d/%4d/%4d",
			lim->top,
			lim->right,
			lim->bottom,
			lim->left);
}
