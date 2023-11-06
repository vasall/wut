#include "graphic/inc/font.h"

#include "system/inc/system.h"

#include "utils/inc/table.h"

#include <stdlib.h>



FH_INTERN void font_destroy(struct fh_font *font)
{
	TTF_CloseFont(font->font);
	fh_free(font);
}


FH_INTERN void font_rmv_fnc(u32 size, void *ptr)
{
	struct fh_font *font;

	fh_Ignore(size);

	if(!ptr)
		return;

	font = (struct fh_font *)ptr;

	font_destroy(font);
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

FH_API s8 fh_InitFontTable(struct fh_context *ctx)
{
	struct fh_table *tbl;

	if(!ctx) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}
	
	if(!(tbl = fh_tbl_create(&font_rmv_fnc))) {
		ALARM(ALARM_ERR, "Failed to create the fh_table");
		goto err_return;
	}

	/* Attach the table to the context */
	ctx->fonts = tbl;	

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to initializie the font table");
	return -1;
}


FH_API void fh_CloseFontTable(struct fh_context *ctx)
{
	fh_tbl_destroy(ctx->fonts);
	ctx->fonts = NULL;
}


FH_API struct fh_font *fh_LoadFont(struct fh_context *ctx, char *name,
		char *pth, u8 s)
{
	struct fh_font *font;
	u32 size;
	void **p;

	if(!ctx || !name || !pth) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(!(font = fh_malloc(sizeof(struct fh_font)))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for font");
		goto err_return;
	}

	/* Set the attributes of the font */
	strcpy(font->name, name);
	font->context = ctx;

	/* Actually load the font from the file */
	if(!(font->font = TTF_OpenFont(pth, s))) {
		ALARM(ALARM_ERR, "Failed to load font from file");
		goto err_free_font;
	}

	/* Insert font into table */
	size = sizeof(struct fh_font);
	p = (void **)&font;
	if(fh_ContextAdd(ctx, FH_CONTEXT_FONTS, name, size, p) < 0)
		goto err_destroy_font;

	return font;

err_destroy_font:
	TTF_CloseFont(font->font);

err_free_font:
	fh_free(font);

err_return:
	ALARM(ALARM_ERR, "Failed to load font");
	return NULL;
}


FH_API void fh_RemoveFont(struct fh_font *font)
{
	if(!font)
		return;

	fh_ContextRemove(font->context, FH_CONTEXT_FONTS, font->name);
}


FH_API struct fh_font *fh_GetFont(struct fh_context *ctx, char *name)
{
	struct fh_font *font;

	if(!ctx || !name) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(fh_tbl_get(ctx->fonts, name, NULL, (void **)&font) != 1)
		return NULL;

	return font;

err_return:
	ALARM(ALARM_ERR, "Failed to get font from font table");
	return NULL;

}
