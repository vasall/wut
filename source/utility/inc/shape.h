#ifndef _WT_UTILITY_SHAPE_H
#define _WT_UTILITY_SHAPE_H

#include "core/inc/define.h"
#include "core/inc/import.h"


struct wt_sin2 {
	s32 x;
	s32 y;
};



#define WT_RECT_SIZE	(4*sizeof(s32))

struct wt_rect {
	s32 x;
	s32 y;
	s32 w;
	s32 h;
};

struct wt_rect_lim {
	s32 top;
	s32 right;
	s32 bottom;
	s32 left;
};


WT_API void wt_rect_rst(struct wt_rect *r);
WT_API void wt_rect_set(struct wt_rect *r, s32 x, s32 y, s32 w, s32 h);
WT_API void wt_rect_cpy(struct wt_rect *out, struct wt_rect *in);
WT_API void wt_rect_add(struct wt_rect *out, struct wt_rect *in1,
		struct wt_rect *in2);



WT_API void wt_rect_tolim(struct wt_rect_lim *out, struct wt_rect *rect);
WT_API void wt_rect_fromlim(struct wt_rect *out, struct wt_rect_lim *lim);


/*
 * Check if two rectangles intersect.
 *
 * @in1: The first rectangle
 * @in2: The second rectangle
 *
 * Returns: 1 if they intersect and 0 if not
 */
WT_API s8 wt_rect_intersecting(struct wt_rect *in1, struct wt_rect *in2);

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
WT_API s8 wt_rect_intersect(struct wt_rect *out, struct wt_rect *in1,
		struct wt_rect *in2);


WT_API void wt_rect_mov(struct wt_rect *out, struct wt_rect *in,
		struct wt_sin2 *del);




WT_API void wt_rect_dump(struct wt_rect *r);
WT_API void wt_rect_lim_dump(struct wt_rect_lim *lim);

#endif /* _WT_UTILITY_SHAPE_H */
