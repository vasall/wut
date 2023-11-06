#ifndef _FH_UTILITY_SHAPE_H
#define _FH_UTILITY_SHAPE_H

#include "core/inc/define.h"
#include "core/inc/import.h"


struct fh_sin2 {
	s32 x;
	s32 y;
};



#define FH_RECT_SIZE	(4*sizeof(s32))

struct fh_rect {
	s32 x;
	s32 y;
	s32 w;
	s32 h;
};

struct fh_rect_lim {
	s32 top;
	s32 right;
	s32 bottom;
	s32 left;
};


FH_API void fh_rect_rst(struct fh_rect *r);
FH_API void fh_rect_set(struct fh_rect *r, s32 x, s32 y, s32 w, s32 h);
FH_API void fh_rect_cpy(struct fh_rect *out, struct fh_rect *in);
FH_API void fh_rect_add(struct fh_rect *out, struct fh_rect *in1,
		struct fh_rect *in2);



FH_API void fh_rect_tolim(struct fh_rect_lim *out, struct fh_rect *rect);
FH_API void fh_rect_fromlim(struct fh_rect *out, struct fh_rect_lim *lim);


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
FH_API s8 fh_rect_intersect(struct fh_rect *out, struct fh_rect *in1,
		struct fh_rect *in2);


FH_API void fh_rect_mov(struct fh_rect *out, struct fh_rect *in,
		struct fh_sin2 *del);




FH_API void fh_rect_dump(struct fh_rect *r);
FH_API void fh_rect_lim_dump(struct fh_rect_lim *lim);

#endif /* _FH_UTILITY_SHAPE_H */
