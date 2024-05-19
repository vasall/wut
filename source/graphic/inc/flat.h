#ifndef _WT_GRAPHIC_FLAT_H
#define _WT_GRAPHIC_FLAT_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "window/inc/window.h"

#include "graphic/inc/texture.h"
#include "graphic/inc/shader.h"
#include "utility/inc/color.h"
#include "utility/inc/shape.h"

#define WT_FLAT_NAME_LIM	128




struct wt_flat {
	char name[WT_FLAT_NAME_LIM];

	/* The size of the flat */
	s16 width;
	s16 height;

	struct wt_color *pixels;

	/* A texture used to render the flat struct using OpenGL */
	struct wt_texture *texture;

	struct wt_context *context;
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
WT_API struct wt_flat *wt_CreateFlat(struct wt_context *ctx, char *name,
		s16 w, s16 h);


/*
 * Destroy a flat struct.
 *
 * @f: Pointer to the flat struct
 */
WT_API void wt_DestroyFlat(struct wt_flat *f);


/*
 * Resize the flat. Note that this function will not scale the original image.
 *
 * @f: Pointer to the flat struct
 * @w: The new width
 * @h: The new height
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WT_API s8 wt_ResizeFlat(struct wt_flat *f, s16 w, s16 h);


/*
 * Update the flat and write it to the texture.
 *
 * @f: Pointer to the flat
 * @r: The rect to update
 */
WT_API void wt_UpdateFlat(struct wt_flat *f, struct wt_rect *r);


/*
 * Fill a rectangle with the given color. 
 *
 * @f: Pointer to the flat
 * @r: The rectangle
 * @c: The color
 */
WT_API void wt_FlatRect(struct wt_flat *f, struct wt_rect *r,
		struct wt_color c);


/*
 * This will overwrite the pixels in the given rectangle with the color.
 *
 * @f: Pointer to the flat
 * @r: The rectangle
 * @c: The color
 */
WT_API void wt_FlatRectSet(struct wt_flat *f, struct wt_rect *r,
		struct wt_color c);

#endif /* _WT_GRAPHIC_FLAT_H */
