#include "source/graphic/inc/flat.h"

#include "source/utility/inc/alarm.h"

#include "source/system/inc/system.h"

WUT_INTERN void flat_mod(struct wut_Flat *f, s32 x, s32 y,
		struct wut_Color col)
{
	struct wut_Color px;
	struct wut_Color swap;
	u32 off;


	if(y > f->height) {
		return;
	}

	off = ((f->height - 1 - y) * f->width) + x;

	px = f->pixels[off];
	swap = wut_BlendColor(px, col);
	f->pixels[off] = wut_InvColor(swap);

}


WUT_INTERN void flat_set(struct wut_Flat *f, s32 x, s32 y,
		struct wut_Color col)
{
	struct wut_Color *px;
	u32 off;

	if(y > f->height) {
		return;
	}

	off = ((f->height - 1 - y) * f->width) + x;

	px = &f->pixels[off];
	*px = wut_InvColor(col);

}

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


WUT_API struct wut_Flat *wut_CreateFlat(struct wut_Context *ctx, char *name, s16 w, s16 h)
{
	u32 i;

	struct wut_Flat *f;
	u32 size;

	struct wut_Color *px;
		
	if(!ctx || !name || w < 1 || h < 1) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(!(f = wut_malloc(sizeof(struct wut_Flat)))) {
		WUT_ALARM(WUT_ERROR, "Failed to allocate memory for flat struct");
		goto err_return;
	}

	/* Set the attributes of the flat struct */
	strcpy(f->name, name);
	f->width = w;
	f->height = h;
	f->context = ctx;

	/* Allocate memory for pixels */
	size = w * h * sizeof(struct wut_Color);
	if(!(f->pixels = wut_malloc(size))) {
		WUT_ALARM(WUT_ERROR, "Failed to allocate pixel data buffer");
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
	if(!(f->texture = wut_CreateTexture(ctx, name, w, h, GL_RGBA, (u8 *)f->pixels))) {
		WUT_ALARM(WUT_ERROR, "Failed to create tetxure for flat");
		goto err_free_pixels;
	}

	return f;


err_free_pixels:
	wut_free(f->pixels);

err_free_f:
	wut_free(f);

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to create flat structure");
	return NULL;
}


WUT_API void wut_DestroyFlat(struct wut_Flat *f)
{
	if(!f)
		return;

	wut_RemoveTexture(f->texture);
	wut_free(f->pixels);
	wut_free(f);
}


WUT_API s8 wut_ResizeFlat(struct wut_Flat *f, s16 w, s16 h)
{
	void *p;
	u32 size;
	u32 i;

	if(!f) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	/*
	 * Reallocate memory for new pixel data.
	 */
	size = w * h * sizeof(struct wut_Color); 
	if(!(p = wut_realloc(f->pixels, size))) {
		WUT_ALARM(WUT_ERROR, "Failed to reallocate memory");
		goto err_return;
	}

	f->pixels = p;

	size = w * h;
	for(i = 0; i < size; i++) {
		struct wut_Color *px = &f->pixels[i];

		px->r = 0xff;
		px->g = 0xff;
		px->b = 0xff;
		px->a = 0xff;

	}

	/*
	 * Update the OpenGL texture.
	 */
	if(wut_ResizeTexture(f->texture, w, h, (u8 *)f->pixels) < 0) 
		goto err_return;

	/*
	 * Update attributes.
	 */
	f->width = w;
	f->height = h;

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to resize flat struct");
	return -1;
}


WUT_API void wut_UpdateFlat(struct wut_Flat *f, wut_iRect r)
{
	struct wut_Color *swap;	
	u32 size;
	s16 x;
	s16 y;
	u32 i;

	s32 lim_x;
	s32 lim_y;

	s32 off_x;
	s32 off_y;

	if(!f || !r) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

	
	lim_x = (r[0] + r[2] > f->width) ? f->width - r[0] : r[2];
	lim_y = (r[1] + r[3] > f->height) ? f->height - r[1] : r[3];

	size = lim_x * lim_y * WUT_COLOR_SIZE; 
	if(!(swap = wut_malloc(size))) {
		WUT_ALARM(WUT_ERROR, "Failed to allocate memory for swap buffer");
		return;
	}

	i = 0;
	
	off_x = r[0] + lim_x;
	off_y = r[1] + lim_y;

	for(y = r[1]; y < off_y; y++) {
		for(x = r[0]; x < off_x; x++) {
			swap[i] = f->pixels[(y * f->width)+x];
			i++;
		}
	}

	wut_SetTexture(f->texture, r[0], r[1], lim_x, lim_y, (u8 *)swap);

	wut_free(swap);
}


WUT_API void wut_FlatRect(struct wut_Flat *f, wut_iRect r, struct wut_Color c)
{
	s32 x;
	s32 y;

	if(!f) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

	for(x = 0; x < r[2] && x < f->width; x++) {
		for(y = 0; y < r[3] && y < f->height; y++) {
			flat_mod(f, x + r[0], y + r[1], c);
		}
	}
}


WUT_API void wut_FlatRectSet(struct wut_Flat *f, wut_iRect r,
		struct wut_Color c)
{
	s32 x;
	s32 y;

	if(!f) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

	for(x = 0; x < r[2] && x < f->width; x++) {
		for(y = 0; y < r[3] && y < f->height; y++) {
			flat_set(f, x + r[0], y + r[1], c);
		}
	}

}
