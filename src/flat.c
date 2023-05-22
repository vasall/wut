#include "flat.h"

#include "alarm.h"
#include "system.h"

FH_INTERN void flat_mod(struct fh_flat *f, u16 x, u16 y,
		struct fh_color col)
{
	struct fh_color *px;

	px = &f->pixels[(y*f->width)+x];
	*px = fh_BlendColor(*px, col);

}


FH_INTERN void flat_set(struct fh_flat *f, u16 x, u16 y,
		struct fh_color col)
{
	struct fh_color *px;

	px = &f->pixels[(y*f->width)+x];
	*px = col;

}

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


FH_API struct fh_flat *fh_CreateFlat(struct fh_context *ctx, char *name, u16 w, u16 h)
{
	u32 i;

	struct fh_flat *f;
	u32 size;

	struct fh_color *px;
		
	if(!ctx || !name || w < 1 || h < 1) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(!(f = fh_malloc(sizeof(struct fh_flat)))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for flat struct");
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
		ALARM(ALARM_ERR, "Failed to allocate pixel data buffer");
		goto err_free_f;
	}

	size = w * h;
	for(i = 0; i < size; i++) {
		px = &f->pixels[i];

		px->r = 0;
		px->g = 0;
		px->b = 255;
		px->a = 255;

	}

	/* Create a texture to put the flat struct onto */
	if(!(f->texture = fh_CreateTexture(ctx, name, w, h, GL_RGBA, (u8 *)f->pixels))) {
		ALARM(ALARM_ERR, "Failed to create tetxure for flat");
		goto err_free_pixels;
	}

	return f;


err_free_pixels:
	fh_free(f->pixels);

err_free_f:
	fh_free(f);

err_return:
	ALARM(ALARM_ERR, "Failed to create flat structure");
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


FH_API s8 fh_ResizeFlat(struct fh_flat *f, u16 w, u16 h)
{
	void *p;
	u32 size;
	u32 i;

	if(!f) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	/*
	 * Reallocate memory for new pixel data.
	 */
	size = w * h * sizeof(struct fh_color); 
	if(!(p = fh_realloc(f->pixels, size))) {
		ALARM(ALARM_ERR, "Failed to reallocate memory");
		goto err_return;
	}

	f->pixels = p;

	size = w * h;
	for(i = 0; i < size; i++) {
		struct fh_color *px = &f->pixels[i];

		px->r = 0;
		px->g = 0;
		px->b = 0;
		px->a = 0;

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
	ALARM(ALARM_ERR, "Failed to resize flat struct");
	return -1;
}


FH_API void fh_UpdateFlat(struct fh_flat *f, struct fh_rect *r)
{
	struct fh_color *swap;	
	u32 size;
	s16 x;
	s16 y;
	u32 i;

	if(!f || !r) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	size = r->w * r->h * FH_COLOR_SIZE; 
	if(!(swap = fh_malloc(size))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for swap buffer");
		return;
	}

	i = 0;
	for(y = 0; y < r->h; y++) {
		for(x = 0; x < r->w; x++) {
			swap[i] = f->pixels[((y + r->y) * f->width)+(x+r->x)];
			i++;
		}
	}

	fh_SetTexture(f->texture, r->x, r->y, r->w, r->h, (u8 *)swap);

	fh_free(swap);
}


FH_API void fh_FlatRect(struct fh_flat *f, struct fh_rect *r, struct fh_color c)
{
	s32 x;
	s32 y;

	if(!f) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	printf("Draw FlatRect: %d, %d, %d, %d\n", r->x, r->y, r->w, r->h);

	for(x = 0; x < r->w; x++) {
		for(y = 0; y < r->h; y++) {
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
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	printf("Draw FlatRectSet: %d, %d, %d, %d\n", r->x, r->y, r->w, r->h);

	for(x = 0; x < r->w; x++) {
		for(y = 0; y < r->h; y++) {
			flat_set(f, x + r->x, y + r->y, c);
		}
	}

}
