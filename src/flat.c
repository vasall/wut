#include "flat.h"

#include "alarm.h"
#include "system.h"



/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


FH_API struct fh_flat *fh_CreateFlat(struct fh_context *ctx, char *name, u32 w, u32 h)
{
	u32 i;

	struct fh_flat *f;
	u32 size;

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
	size = w * h * sizeof(struct fh_flat_pixel);
	if(!(f->pixels = fh_zalloc(size))) {
		ALARM(ALARM_ERR, "Failed to allocate pixel data buffer");
		goto err_free_f;
	}

	size = w * h;
	for(i = 0; i < size; i++) {
		struct fh_flat_pixel *px = &f->pixels[i];

		px->r = 0;
		px->g = 0;
		px->b = 0;
		px->a = 100;

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
