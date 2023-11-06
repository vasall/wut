#ifndef _FH_GRAPHIC_RESOURCES_FONT_H
#define _FH_GRAPHIC_RESOURCES_FONT_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "document/inc/context.h"

#include "utility/inc/utils.h"

#define FH_FONT_NAME_LIM	128


struct fh_font {
	char name[FH_FONT_NAME_LIM];	

	TTF_Font *font;

	struct fh_context *context;
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
 * Load a TTF-font from a file.
 *
 * @ctx: Pointer to the context
 * @name: The name of the font
 * @pth: The path to the file
 * @size: The font size in pixels
 *
 * Returns: Either a pointer to the newly created font or NULL if an error
 * 	    occurred
 */
FH_API struct fh_font *fh_LoadFont(struct fh_context *ctx,
		char *name, char *pth, u8 size);


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


#endif /* _FH_GRAPHIC_RESOURCES_FONT_H */
