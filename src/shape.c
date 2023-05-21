#include "shape.h"

#include <stdlib.h>
#include <string.h>


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
