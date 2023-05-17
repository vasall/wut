#ifndef _FH_FLAT_H
#define _FH_FLAT_H

#include "define.h"
#include "datatype.h"
#include "import.h"
#include "window.h"
#include "texture.h"
#include "model.h"
#include "shader.h"

#define FH_FLAT_NAME_LIM	128


/*
 * A pixel is defined by 4 attributes representing the RGBA-values.
 * Each value can be set in range of 0 t0 255.
 */
struct fh_flat_pixel {
	u8 r;
	u8 g;
	u8 b;
	u8 a;
};



struct fh_flat {
	char name[FH_FLAT_NAME_LIM];

	/* The position of the flat in the window */
	u32 x;
	u32 y;

	/* The size of the flat */
	u32 width;
	u32 height;

	struct fh_flat_pixel *pixels;


	/* A texture used to render the flat struct using OpenGL */
	struct fh_texture *texture;

	struct fh_context *context;
};


/*
 * Create a new flat surface to enable 2d-rendering.
 *
 * @ctx: Pointer to the context
 * @name: The name of the flat
 * @w: The initial width
 * @h: The initial height
 *
 * Returns: Either a pointer to the newly created flat surface or NULL if an
 * 	    error occurred
 */
FH_API struct fh_flat *fh_CreateFlat(struct fh_context *ctx, char *name,
		u32 w, u32 h);


/*
 * Destroy a flat surface.
 *
 * @f: Pointer to the flat surface
 */
FH_API void fh_DestroyFlat(struct fh_flat *f);





#endif /* _FH_FLAT_H */
