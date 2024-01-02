#include "graphic/inc/flat.h"

#include "utility/alarm/inc/alarm.h"

#include "system/inc/system.h"

FH_INTERN void flat_mod(struct fh_flat *f, s32 x, s32 y,
		struct fh_color col)
{
	struct fh_color px;
	struct fh_color swap;
	u32 off;


	if(y > f->height) {
		return;
	}

	off = ((f->height - 1 - y) * f->width) + x;

	px = f->pixels[off];
	swap = fh_BlendColor(px, col);
	f->pixels[off] = fh_col_invform(swap);

}


FH_INTERN void flat_set(struct fh_flat *f, s32 x, s32 y,
		struct fh_color col)
{
	struct fh_color *px;
	u32 off;

	if(y > f->height) {
		return;
	}

	off = ((f->height - 1 - y) * f->width) + x;

	px = &f->pixels[off];
	*px = fh_col_invform(col);

}

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


FH_API struct fh_flat *fh_CreateFlat(struct fh_context *ctx, char *name, s16 w, s16 h)
{
	u32 i;

	struct fh_flat *f;
	u32 size;

	struct fh_color *px;
		
	if(!ctx || !name || w < 1 || h < 1) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(!(f = fh_malloc(sizeof(struct fh_flat)))) {
		FH_ALARM(FH_ERROR, "Failed to allocate memory for flat struct");
		goto err_return;
	}

	/* Set the attributes of the flat struct */
	strcpy(f->name, name);
	f->width = w;
	f->height = h;
	f->context = ctx;

	/* Allocate memory for pixels */
	size = w * h * sizeof(struct fh_color);
	if(!(f->pixels = fh_malloc(size))) {
		FH_ALARM(FH_ERROR, "Failed to allocate pixel data buffer");
		goto err_free_f;
	}

	size = w * h;
	for(i = 0; i < size; i++) {
		px = &f->pixels[i];

		px->r = 0xff;
		px->g = 0xff;
		px->b = 0xff;
		px->a = 0xff;

	}

	/* Create a texture to put the flat struct onto */
	if(!(f->texture = fh_CreateTexture(ctx, name, w, h, GL_RGBA, (u8 *)f->pixels))) {
		FH_ALARM(FH_ERROR, "Failed to create tetxure for flat");
		goto err_free_pixels;
	}

	return f;


err_free_pixels:
	fh_free(f->pixels);

err_free_f:
	fh_free(f);

err_return:
	FH_ALARM(FH_ERROR, "Failed to create flat structure");
	return NULL;
}


FH_API void fh_DestroyFlat(struct fh_flat *f)
{
	if(!f)
		return;

	fh_RemoveTexture(f->texture);
	fh_free(f->pixels);
	fh_free(f);
}


FH_API s8 fh_ResizeFlat(struct fh_flat *f, s16 w, s16 h)
{
	void *p;
	u32 size;
	u32 i;

	if(!f) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		goto err_return;
	}

	/*
	 * Reallocate memory for new pixel data.
	 */
	size = w * h * sizeof(struct fh_color); 
	if(!(p = fh_realloc(f->pixels, size))) {
		FH_ALARM(FH_ERROR, "Failed to reallocate memory");
		goto err_return;
	}

	f->pixels = p;

	size = w * h;
	for(i = 0; i < size; i++) {
		struct fh_color *px = &f->pixels[i];

		px->r = 0xff;
		px->g = 0xff;
		px->b = 0xff;
		px->a = 0xff;

	}

	/*
	 * Update the OpenGL texture.
	 */
	if(fh_ResizeTexture(f->texture, w, h, (u8 *)f->pixels) < 0) 
		goto err_return;

	/*
	 * Update attributes.
	 */
	f->width = w;
	f->height = h;

err_return:
	FH_ALARM(FH_ERROR, "Failed to resize flat struct");
	return -1;
}


FH_API void fh_UpdateFlat(struct fh_flat *f, struct fh_rect *r)
{
	struct fh_color *swap;	
	u32 size;
	s16 x;
	s16 y;
	u32 i;

	s32 lim_x;
	s32 lim_y;

	s32 off_x;
	s32 off_y;

	if(!f || !r) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	
	lim_x = (r->x + r->w > f->width) ? f->width - r->x : r->w;
	lim_y = (r->y + r->h > f->height) ? f->height - r->y : r->h;

	size = lim_x * lim_y * FH_COLOR_SIZE; 
	if(!(swap = fh_malloc(size))) {
		FH_ALARM(FH_ERROR, "Failed to allocate memory for swap buffer");
		return;
	}

	i = 0;
	
	off_x = r->x + lim_x;
	off_y = r->y + lim_y;

	for(y = r->y; y < off_y; y++) {
		for(x = r->x; x < off_x; x++) {
			swap[i] = f->pixels[(y * f->width)+x];
			i++;
		}
	}

	fh_SetTexture(f->texture, r->x, r->y, lim_x, lim_y, (u8 *)swap);

	fh_free(swap);
}


FH_API void fh_FlatRect(struct fh_flat *f, struct fh_rect *r, struct fh_color c)
{
	s32 x;
	s32 y;

	if(!f) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	for(x = 0; x < r->w && x < f->width; x++) {
		for(y = 0; y < r->h && y < f->height; y++) {
			flat_mod(f, x + r->x, y + r->y, c);
		}
	}
}


FH_API void fh_FlatRectSet(struct fh_flat *f, struct fh_rect *r,
		struct fh_color c)
{
	s32 x;
	s32 y;

	if(!f) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	for(x = 0; x < r->w && x < f->width; x++) {
		for(y = 0; y < r->h && y < f->height; y++) {
			flat_set(f, x + r->x, y + r->y, c);
		}
	}

}
