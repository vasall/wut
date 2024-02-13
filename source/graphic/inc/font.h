#ifndef _FH_GRAPHIC_RESOURCES_FONT_H
#define _FH_GRAPHIC_RESOURCES_FONT_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "utility/inc/utility.h"
#include "utility/inc/static_list.h"
#include "utility/inc/list.h"

#include "graphic/inc/context.h"

#define FH_FONT_NAME_LIM	128

struct fh_font_glyph {
	s16 codepoint;

	f32 width;
	f32 height;

	f32 hori_bearing_x;
	f32 hori_bearing_y;
	f32 hori_advance;

	f32 verti_bearing_x;
	f32 verti_bearing_y;
	f32 verti_advance;

	f32 tex_coord_x;
	f32 tex_coord_y;

	f32 tex_width;
	f32 tex_height;
};

struct fh_font_data {
	f32 spread_in_tex;	/* Spread in texture */
	f32 spread_in_font;	/* Spread in font metrics */

	/*
	 * A list with all loaded glyphs.
	 */
	struct fh_list *glyphs;

};

struct fh_font {
	/* The name of the font */
	char name[FH_FONT_NAME_LIM];	

	/* Pointer to the context this font is attached to */
	struct fh_context *context;

	/* The attached batch renderer */
	s16 batch_id;	

	/*
	 * Font-Data
	 */
	struct fh_font_data data;
};


/*
 * Create and initialize the font table for a context.
 *
 * @ctx: Pointer to the context
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_InitFontTable(struct fh_context *ctx);


/*
 * Close the font table for a context.
 *
 * @ctx: Pointer to the context
 */
FH_API void fh_CloseFontTable(struct fh_context *ctx);


/*
 * Load a SDF-Font with both the image and metadata.
 *
 * @ctx: Pointer to the context
 * @name: The name of the font
 * @img_pth: The path to the image of the font
 * @meta_pth: The path to the text file containing the meta info
 *
 * Returns: Either a pointer to the newly created font or NULL if an error
 * 	    occurred
 */
FH_API struct fh_font *fh_LoadFont(struct fh_context *ctx,
		char *name, char *img_pth, char *meta_pth);


/*
 * Destroy a font and free the allocated memory.
 *
 * @font: Pointer to the font
 */
FH_API void fh_RemoveFont(struct fh_font *font);


/*
 * Retrieve a font from the font table of a context.
 *
 * @ctx: Pointer to the context
 * @name: The name of the font
 *
 * Returns: Eithr a pointer to the font or NULL if either an error occurred or
 * 	    the font could not be found
 */
FH_API struct fh_font *fh_GetFont(struct fh_context *ctx, char *name);


/*
 * Get a pointer to the glyph data for the font.
 *
 * @font: Pointer to the font
 * @idx: The codepoint for the glyph
 *
 * Returns: Either a pointer to the glyph or NULL if its missing or an error
 * 	    occurred
 */
FH_API struct fh_font_glyph *fh_GetFontGlyph(struct fh_font *font, s16 idx);




#endif /* _FH_GRAPHIC_RESOURCES_FONT_H */
