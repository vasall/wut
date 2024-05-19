#include "graphic/inc/flat.h"

#include "utility/inc/alarm.h"

#include "system/inc/system.h"

WT_INTERN void flat_mod(struct wt_flat *f, s32 x, s32 y,
		struct wt_color col)
{
	struct wt_color px;
	struct wt_color swap;
	u32 off;


	if(y > f->height) {
		return;
	}

	off = ((f->height - 1 - y) * f->width) + x;

	px = f->pixels[off];
	swap = wt_BlendColor(px, col);
	f->pixels[off] = wt_col_invform(swap);

}


WT_INTERN void flat_set(struct wt_flat *f, s32 x, s32 y,
		struct wt_color col)
{
	struct wt_color *px;
	u32 off;

	if(y > f->height) {
		return;
	}

	off = ((f->height - 1 - y) * f->width) + x;

	px = &f->pixels[off];
	*px = wt_col_invform(col);

}

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


WT_API struct wt_flat *wt_CreateFlat(struct wt_context *ctx, char *name, s16 w, s16 h)
{
	u32 i;

	struct wt_flat *f;
	u32 size;

	struct wt_color *px;
		
	if(!ctx || !name || w < 1 || h < 1) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(!(f = wt_malloc(sizeof(struct wt_flat)))) {
		WT_ALARM(WT_ERROR, "Failed to allocate memory for flat struct");
		goto err_return;
	}

	/* Set the attributes of the flat struct */
	strcpy(f->name, name);
	f->width = w;
	f->height = h;
	f->context = ctx;

	/* Allocate memory for pixels */
	size = w * h * sizeof(struct wt_color);
	if(!(f->pixels = wt_malloc(size))) {
		WT_ALARM(WT_ERROR, "Failed to allocate pixel data buffer");
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
	if(!(f->texture = wt_CreateTexture(ctx, name, w, h, GL_RGBA, (u8 *)f->pixels))) {
		WT_ALARM(WT_ERROR, "Failed to create tetxure for flat");
		goto err_free_pixels;
	}

	return f;


err_free_pixels:
	wt_free(f->pixels);

err_free_f:
	wt_free(f);

err_return:
	WT_ALARM(WT_ERROR, "Failed to create flat structure");
	return NULL;
}


WT_API void wt_DestroyFlat(struct wt_flat *f)
{
	if(!f)
		return;

	wt_RemoveTexture(f->texture);
	wt_free(f->pixels);
	wt_free(f);
}


WT_API s8 wt_ResizeFlat(struct wt_flat *f, s16 w, s16 h)
{
	void *p;
	u32 size;
	u32 i;

	if(!f) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	/*
	 * Reallocate memory for new pixel data.
	 */
	size = w * h * sizeof(struct wt_color); 
	if(!(p = wt_realloc(f->pixels, size))) {
		WT_ALARM(WT_ERROR, "Failed to reallocate memory");
		goto err_return;
	}

	f->pixels = p;

	size = w * h;
	for(i = 0; i < size; i++) {
		struct wt_color *px = &f->pixels[i];

		px->r = 0xff;
		px->g = 0xff;
		px->b = 0xff;
		px->a = 0xff;

	}

	/*
	 * Update the OpenGL texture.
	 */
	if(wt_ResizeTexture(f->texture, w, h, (u8 *)f->pixels) < 0) 
		goto err_return;

	/*
	 * Update attributes.
	 */
	f->width = w;
	f->height = h;

err_return:
	WT_ALARM(WT_ERROR, "Failed to resize flat struct");
	return -1;
}


WT_API void wt_UpdateFlat(struct wt_flat *f, struct wt_rect *r)
{
	struct wt_color *swap;	
	u32 size;
	s16 x;
	s16 y;
	u32 i;

	s32 lim_x;
	s32 lim_y;

	s32 off_x;
	s32 off_y;

	if(!f || !r) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	
	lim_x = (r->x + r->w > f->width) ? f->width - r->x : r->w;
	lim_y = (r->y + r->h > f->height) ? f->height - r->y : r->h;

	size = lim_x * lim_y * WT_COLOR_SIZE; 
	if(!(swap = wt_malloc(size))) {
		WT_ALARM(WT_ERROR, "Failed to allocate memory for swap buffer");
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

	wt_SetTexture(f->texture, r->x, r->y, lim_x, lim_y, (u8 *)swap);

	wt_free(swap);
}


WT_API void wt_FlatRect(struct wt_flat *f, struct wt_rect *r, struct wt_color c)
{
	s32 x;
	s32 y;

	if(!f) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	for(x = 0; x < r->w && x < f->width; x++) {
		for(y = 0; y < r->h && y < f->height; y++) {
			flat_mod(f, x + r->x, y + r->y, c);
		}
	}
}


WT_API void wt_FlatRectSet(struct wt_flat *f, struct wt_rect *r,
		struct wt_color c)
{
	s32 x;
	s32 y;

	if(!f) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	for(x = 0; x < r->w && x < f->width; x++) {
		for(y = 0; y < r->h && y < f->height; y++) {
			flat_set(f, x + r->x, y + r->y, c);
		}
	}

}
