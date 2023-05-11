#ifndef _FH_FONT_H
#define _FH_FONT_H

#include "define.h"
#include "import.h"
#include "datatype.h"
#include "window.h"

#define FH_FONT_NAME_LIM	128


struct fh_font {
	char name[FH_FONT_NAME_LIM];	

	TTF_Font *font;
};


/*
 * Initialize the font table for a window.
 *
 * @win: Pointer to the window
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_font_init(struct fh_window *win);


/*
 * Close the font table for a window.
 *
 * @win: Pointer to the window
 */
FH_API void fh_font_close(struct fh_window *win);


/*
 * Load a TTF-font from a file.
 *
 * @name: The name of the font
 * @pth: The path to the file
 * @size: The font size in pixels
 *
 * Returns: Either a pointer to the newly created font or NULL if an error
 * 	    occurred
 */
FH_API struct fh_font *fh_font_load(char *name, char *pth, u8 size);


/*
 * Destroy a font and free the allocated memory.
 *
 * @font: Pointer to the font
 */
FH_API void fh_font_destroy(struct fh_font *font);


/*
 * Insert a font into the font table of a window.
 *
 * @win: Pointer to the window
 * @font: Pointer to the font
 *
 * Returns: 0 on success or -1 if an error occurred Insert a font into the font
 * table of a window.
 *
 * @win: Pointer to the window
 * @font: Pointer to the font
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_font_insert(struct fh_window *win, struct fh_font *font);


/*
 * Remove a font from the font table.
 * This will also destroy the font.
 *
 * @win: Pointer to the window
 * @font: Pointer to the font
 */
FH_API void fh_font_remove(struct fh_window *win, struct fh_font *font);


/*
 * Retrieve a font from the font table of a window.
 *
 * @win: Pointer to the window
 * @name: The name of the font
 *
 * Returns: Eithr a pointer to the font or NULL if either an error occurred or
 * 	    the font could not be found
 */
FH_API struct fh_font *fh_font_get(struct fh_window *win, char *name);


/*
 * The callback funcion that will be passed to the table, which will be called
 * in case any entry will be removed from the table.
 */
FH_API void fh_font_rmv_fnc(u32 size, void *ptr);

#endif /* _FH_FONT_H */
