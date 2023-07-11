#ifndef _FH_SHAPE_H
#define _FH_SHAPE_H

#include "stdinc.h"


#define FH_RECT_SIZE	(4*sizeof(s32))

struct fh_rect {
	s32 x;
	s32 y;
	s32 w;
	s32 h;
};



FH_API void fh_rect_rst(struct fh_rect *r);
FH_API void fh_rect_set(struct fh_rect *r, s32 x, s32 y, s32 w, s32 h);
FH_API void fh_rect_cpy(struct fh_rect *out, struct fh_rect *in);
FH_API void fh_rect_add(struct fh_rect *out, struct fh_rect *in1,
		struct fh_rect *in2);
FH_API void fh_rect_dump(struct fh_rect *r);

#endif /* _FH_SHAPE_H */
