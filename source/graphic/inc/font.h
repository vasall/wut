#ifndef _WT_GRAPHIC_RESOURCES_FONT_H
#define _WT_GRAPHIC_RESOURCES_FONT_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "utility/inc/utility.h"
#include "utility/inc/static_list.h"
#include "utility/inc/list.h"

#include "graphic/inc/context.h"

#define WT_FONT_NAME_LIM	128

struct wt_font_glyph {
	s16 codepoint;

	f32 width;
	f32 height;

	f32 hbearing_x;
	f32 hbearing_y;
	f32 hadvance;

	f32 vbearing_x;
	f32 vbearing_y;
	f32 vadvance;

	f32 tex_coord_x;
	f32 tex_coord_y;

	f32 tex_width;
	f32 tex_height;
};

struct wt_font_data {
	f32 spread_in_tex;	/* Spread in texture */
	f32 spread_in_font;	/* Spread in font metrics */

	/*
	 * A list with all loaded glyphs.
	 */
	struct wt_list *glyphs;

};

struct wt_font {
	/* The name of the font */
	char name[WT_FONT_NAME_LIM];	

	/* Pointer to the context this font is attached to */
	struct wt_context *context;

	/* The attached batch renderer */
	s16 batch_id;	

	/*
	 * Font-Data
	 */
	struct wt_font_data data;
};


/*
 * Create and initialize the font table for a context.
 *
 * @ctx: Pointer to the context
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WT_API s8 wt_InitFontTable(struct wt_context *ctx);


/*
 * Close the font table for a context.
 *
 * @ctx: Pointer to the context
 */
WT_API void wt_CloseFontTable(struct wt_context *ctx);


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
WT_API struct wt_font *wt_LoadFont(struct wt_context *ctx,
		char *name, char *img_pth, char *meta_pth);


/*
 * Destroy a font and free the allocated memory.
 *
 * @font: Pointer to the font
 */
WT_API void wt_RemoveFont(struct wt_font *font);


/*
 * Retrieve a font from the font table of a context.
 *
 * @ctx: Pointer to the context
 * @name: The name of the font
 *
 * Returns: Eithr a pointer to the font or NULL if either an error occurred or
 * 	    the font could not be found
 */
WT_API struct wt_font *wt_GetFont(struct wt_context *ctx, char *name);


/*
 * Get a pointer to the glyph data for the font.
 *
 * @font: Pointer to the font
 * @cpnt: The codepoint for the glyph
 *
 * Returns: Either a pointer to the glyph or NULL if its missing or an error
 * 	    occurred
 */
WT_API struct wt_font_glyph *wt_GetGlyph(struct wt_font *font, s16 cpnt);


/*
 * Get the index of the codepoint in the font atlas.
 *
 * @font: Pointer to the font
 * @cpnt: The codepoint for the glyph
 *
 * Returns: Either the index or -1 if an error occurred
 */
WT_API s16 wt_GetGlyphIndex(struct wt_font *font, s16 cpnt);


/*
 * Get a glyph directly from the glyph list through the index.
 *
 * @font: Pointer to the font
 * @idx: The index of the glyph in the list
 *
 * Returns: Either a pointer to the glyph or NULL if an error occurred
 */
WT_API struct wt_font_glyph *wt_GetGlyphByIndex(struct wt_font *font,
		s16 idx);

#endif /* _WT_GRAPHIC_RESOURCES_FONT_H */
