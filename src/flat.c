#include "flat.h"

#include "alarm.h"
#include "system.h"



/*
 * Update the underlying texture and rewrite it with the current pixel data.
 *
 * @f: A pointer to the flat surface
 */
FH_INTERN void fh_flat_update_tex(struct fh_flat *f);



FH_API void fh_flat_render(struct fh_flat *f)
{
	if(!f || !f->model)
		return;

	fh_mdl_render(f->model);		
}



FH_INTERN void fh_flat_update_tex(struct fh_flat *f)
{	
	fh_tex_set(f->texture, 0, 0, f->width, f->height,
			(u8 *)f->pixels);
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


FH_API struct fh_flat *fh_create_flat(char *name, struct fh_window *win,
		u32 x, u32 y, u32 w, u32 h)
{
	u32 i;
	u32 j;

	struct fh_flat *f;
	u32 size;


	struct fh_model_c *c;

	unsigned int vtxnum = 4;
	unsigned int idxnum = 6;

	float vertices[] = {
		-1,  -1,   0,
		-1,   1,   0,
		 1,   1,   0,
		 1,  -1,   0
	};

	/* Texture coordinates (2 floats per vertex) */
	float texCoords[] = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	/* Indices (3 ints per triangle) */
	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};


	if(!name || !win || w < 1 || h < 1) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(!(f = fh_malloc(sizeof(struct fh_flat)))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for flat struct");
		goto err_return;
	}

	/* Set the attributes of the flat struct */
	strcpy(f->name, name);
	f->x = x;
	f->y = y;
	f->width = w;
	f->height = h;
	f->shader = fh_shd_get(win, "flat");

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
	if(!(f->texture = fh_tex_create(name, w, h, GL_RGBA, (u8 *)f->pixels))) {
		ALARM(ALARM_ERR, "Failed to create tetxure for flat");
		goto err_free_pixels;
	}

	/* Insert the texture into the texture table */	
	if(fh_tex_insert(win, f->texture) < 0)
		goto err_destroy_tex;

	/* Create a model to render the texture onto the screen */
	if(!(c = fh_mdlc_begin(name, vtxnum, idxnum, indices))) {
		printf("Failed to begin construction\n");
		goto err_remove_tex;
	}

	fh_mdlc_shader(c, f->shader->name);
	fh_mdlc_texture(c, name);

	fh_mdlc_attrib(c, "v_pos", 3, GL_FLOAT, vertices);
	fh_mdlc_attrib(c, "v_uv", 2, GL_FLOAT, texCoords);

	if(!(f->model = fh_mdl_create(win, c))) {
		printf("Failed to finalize construction\n");
		goto err_destroy_c;
	}

	fh_mdlc_destroy(c);

	return f;

err_destroy_c:
	fh_mdlc_destroy(c);

err_remove_tex:
	fh_tex_remove(win, f->texture);
	f->texture = NULL;

err_destroy_tex:
	fh_tex_destroy(f->texture);

err_free_pixels:
	fh_free(f->pixels);

err_free_f:
	fh_free(f);

err_return:
	ALARM(ALARM_ERR, "Failed to create flat structure");
	return NULL;
}


FH_API void fh_destroy_flat(struct fh_flat *f)
{
	if(!f)
		return;

	fh_free(f->pixels);
	fh_free(f);
}
