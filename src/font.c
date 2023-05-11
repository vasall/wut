#include "font.h"

#include "alarm.h"
#include "system.h"
#include "table.h"

#include <stdlib.h>


FH_API s8 fh_font_init(struct fh_window *win)
{
	struct fh_table *tbl;

	if(!win) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}
	
	if(!(tbl = fh_tbl_create(&fh_font_rmv_fnc))) {
		ALARM(ALARM_ERR, "Failed to create the fh_table");
		goto err_return;
	}

	/* Attach the table to the window */
	win->fonts = tbl;	

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to initializie the font table");
	return -1;
}


FH_API void fh_font_close(struct fh_window *win)
{
	fh_tbl_destroy(win->fonts);
	win->fonts = NULL;
}


FH_API struct fh_font *fh_font_load(char *name, char *pth, u8 size)
{
	struct fh_font *font;

	if(!name || !pth) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(!(font = fh_malloc(sizeof(struct fh_font)))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for font");
		goto err_return;
	}

	/* Set the attributes of the font */
	strcpy(font->name, name);

	/* Actually load the font from the file */
	if(!(font->font = TTF_OpenFont(pth, size))) {
		ALARM(ALARM_ERR, "Failed to load font from file");
		goto err_free_font;
	}

	return font;

err_free_font:
	fh_free(font);

err_return:
	ALARM(ALARM_ERR, "Failed to load font");
	return NULL;
}


FH_API void fh_font_destroy(struct fh_font *font)
{
	if(!font)
		return;

	TTF_CloseFont(font->font);
	fh_free(font);
}


FH_API s8 fh_font_insert(struct fh_window *win, struct fh_font *font)
{
	u32 size;
	void **p;

	if(!win || !font) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	size = sizeof(struct fh_font);
	p = (void **)&font;

	if(fh_tbl_add(win->fonts, font->name, size, p) < 0) {
		ALARM(ALARM_ERR, "Failed to add font to table");
		goto err_return;
	}

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to insert font into font table");
	return -1;
}


FH_API void fh_font_remove(struct fh_window *win, struct fh_font *font)
{
	if(!win || !font) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	fh_tbl_rmv(win->fonts, font->name);
}


FH_API struct fh_font *fh_font_get(struct fh_window *win, char *name)
{
	struct fh_font  *font;

	if(!win || !name) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(fh_tbl_get(win->fonts, name, NULL, (void **)&font) != 0) {
		ALARM(ALARM_ERR, "Font could not be found in the table");
		goto err_return;
	}

	return font;

err_return:
	ALARM(ALARM_ERR, "Failed to get font from font table");
	return NULL;
}


FH_API void fh_font_rmv_fnc(u32 size, void *ptr)
{
	struct fh_font *font;

	/* SILENCIO */
	if(size) {}

	if(!ptr)
		return;

	font = (struct fh_font *)ptr;

	fh_font_destroy(font);
}
