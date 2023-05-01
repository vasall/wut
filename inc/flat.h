#ifndef _FH_FLAT_H
#define _FH_FLAT_H

#include "define.h"
#include "datatype.h"
#include "import.h"


struct fh_flat_pixel {
	f32 r;
	f32 g;
	f32 b;
	f32 a;
};



struct fh_flat {
	u32 w;
	u32 h;

	struct fh_flat_pixel *pixels;
};



/*
 * Create a new flat surface.
 *
 * @w: The initial width
 * @h: The initial height
 *
 * Returns: Either a pointer to the newly created flat surface or NULL if an
 * 	    error occurred
 */
FH_API struct fh_flat *fh_flat_create(u32 w, u32 h);


/*
 * Destroy a flat surface.
 *
 * @f: Pointer to the flat surface
 */
FH_API void fh_flat_destroy(struct fh_flat *f);


/*
 * Resize a flat surface.
 *
 * @f: Pointer to the flat surface
 * @w: The new width
 * @h: The new height
 */
FH_API void fh_flat_resize(struct fh_flat *f, u32 w, u32 h);








#endif /* _FH_FLAT_H */
