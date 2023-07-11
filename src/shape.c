#include "shape.h"

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


FH_API void fh_rect_dump(struct fh_rect *r)
{
	printf("%d, %d, %d, %d", r->x, r->y, r->w, r->h);
}
