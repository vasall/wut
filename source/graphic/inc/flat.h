#ifndef _FH_GRAPHIC_FLAT_H
#define _FH_GRAPHIC_FLAT_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "window/inc/window.h"

#include "graphic/inc/texture.h"
#include "graphic/inc/shader.h"
#include "utility/inc/color.h"
#include "utility/inc/shape.h"

#define FH_FLAT_NAME_LIM	128




struct fh_flat {
	char name[FH_FLAT_NAME_LIM];

	/* The size of the flat */
	s16 width;
	s16 height;

	struct fh_color *pixels;

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
		s16 w, s16 h);


/*
 * Destroy a flat struct.
 *
 * @f: Pointer to the flat struct
 */
FH_API void fh_DestroyFlat(struct fh_flat *f);


/*
 * Resize the flat. Note that this function will not scale the original image.
 *
 * @f: Pointer to the flat struct
 * @w: The new width
 * @h: The new height
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_ResizeFlat(struct fh_flat *f, s16 w, s16 h);


/*
 * Update the flat and write it to the texture.
 *
 * @f: Pointer to the flat
 * @r: The rect to update
 */
FH_API void fh_UpdateFlat(struct fh_flat *f, struct fh_rect *r);


/*
 * Fill a rectangle with the given color. 
 *
 * @f: Pointer to the flat
 * @r: The rectangle
 * @c: The color
 */
FH_API void fh_FlatRect(struct fh_flat *f, struct fh_rect *r,
		struct fh_color c);


/*
 * This will overwrite the pixels in the given rectangle with the color.
 *
 * @f: Pointer to the flat
 * @r: The rectangle
 * @c: The color
 */
FH_API void fh_FlatRectSet(struct fh_flat *f, struct fh_rect *r,
		struct fh_color c);

#endif /* _FH_GRAPHIC_FLAT_H */
