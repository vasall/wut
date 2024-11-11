#ifndef _WUT_GRAPHIC_FLAT_H
#define _WUT_GRAPHIC_FLAT_H

#include "source/core/inc/define.h"
#include "source/core/inc/import.h"

#include "source/window/inc/window.h"

#include "source/graphic/inc/texture.h"
#include "source/graphic/inc/shader.h"

#include "source/utility/inc/color.h"

#include "source/math/inc/polygon.h"

#define WUT_FLAT_NAME_LIM	128




struct wut_Flat {
	char name[WUT_FLAT_NAME_LIM];

	/* The size of the flat */
	s16 width;
	s16 height;

	struct wut_Color *pixels;

	/* A texture used to render the flat struct using OpenGL */
	struct wut_Texture *texture;

	struct wut_Context *context;
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
WUT_API struct wut_Flat *wut_CreateFlat(struct wut_Context *ctx, char *name,
		s16 w, s16 h);


/*
 * Destroy a flat struct.
 *
 * @f: Pointer to the flat struct
 */
WUT_API void wut_DestroyFlat(struct wut_Flat *f);


/*
 * Resize the flat. Note that this function will not scale the original image.
 *
 * @f: Pointer to the flat struct
 * @w: The new width
 * @h: The new height
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WUT_API s8 wut_ResizeFlat(struct wut_Flat *f, s16 w, s16 h);


/*
 * Update the flat and write it to the texture.
 *
 * @f: Pointer to the flat
 * @r: The rect to update
 */
WUT_API void wut_UpdateFlat(struct wut_Flat *f, wut_iRect r);


/*
 * Fill a rectangle with the given color. 
 *
 * @f: Pointer to the flat
 * @r: The rectangle
 * @c: The color
 */
WUT_API void wut_FlatRect(struct wut_Flat *f, wut_iRect r,
		struct wut_Color c);


/*
 * This will overwrite the pixels in the given rectangle with the color.
 *
 * @f: Pointer to the flat
 * @r: The rectangle
 * @c: The color
 */
WUT_API void wut_FlatRectSet(struct wut_Flat *f, wut_iRect r,
		struct wut_Color c);

#endif /* _WUT_GRAPHIC_FLAT_H */
