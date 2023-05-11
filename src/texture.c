#include "texture.h"

#include "alarm.h"
#include "system.h"
#include "file.h"
#include "core.h"
#include "table.h"

#include <stdlib.h>



FH_API s8 fh_tex_init(struct fh_window *win)
{
	struct fh_table *tbl;

	if(!win) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(!(tbl = fh_tbl_create(&fh_tex_rmv_fnc))) {
		ALARM(ALARM_ERR, "Failed to create fh_table");
		goto err_return;
	}

	/* Attach the table to the window */
	win->textures = tbl;

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to initialize the texture table");
	return -1;
}


FH_API void fh_tex_close(struct fh_window *win)
{
	fh_tbl_destroy(win->textures);
	win->textures = NULL;
}


FH_API struct fh_texture *fh_tex_create(char *name, u16 w, u16 h,
		GLenum format, u8 *px)
{
	struct fh_texture *tex;

	if(!name || !px) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(!(tex = fh_malloc(sizeof(struct fh_texture)))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for texture");
		goto err_return;
	}

	strcpy(tex->name, name);
	tex->format = format;
	tex->width = w;
	tex->height = h;

	glGenTextures(1, &tex->texture);
	glBindTexture(GL_TEXTURE_2D, tex->texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, px);


	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	return tex;


err_return:
	ALARM(ALARM_ERR, "Failed to create new texture");
	return NULL;
}


FH_API struct fh_texture *fh_tex_load(char *name, char *pth)
{
	struct fh_fs_r_image img;
	struct fh_texture *tex;

	if(!name || !pth) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	/* First load the raw pixel data from a PNG */
	if((fh_fs_image(pth, &img)) < 0)
		goto err_return;

	/* Then create an OpenGL texture and past them */
	if(!(tex = fh_tex_create(name, img.w, img.h, img.format, img.data)))
		goto err_cleanup_img;

	/* Finally clean up the loading buffer */
	fh_fs_image_cleanup(&img);

	return tex;

err_cleanup_img:
	fh_fs_image_cleanup(&img);


err_return:
	ALARM(ALARM_ERR, "Failed to load texture");
	return NULL;
}


FH_API void fh_tex_destroy(struct fh_texture *tex)
{
	if(!tex)
		return;

	glDeleteTextures(1, &tex->texture);
	fh_free(tex);
}


FH_API s8 fh_tex_insert(struct fh_window *win, struct fh_texture *tex)
{
	u32 size;
	void **p;

	if(!win || !tex) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	size = sizeof(struct fh_texture);
	p = (void **)&tex;

	if(fh_tbl_add(win->textures, tex->name, size, p) < 0) {
		ALARM(ALARM_ERR, "Failed to add entry to fh_table");
		goto err_return;
	}

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to insert texture into texture table");
	return -1;
}


FH_API void fh_tex_remove(struct fh_window *win, struct fh_texture *texture)
{
	if(!win || !texture) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	fh_tbl_rmv(win->textures, texture->name);
}


FH_API s8 fh_tex_set(struct fh_texture *t, u16 x, u16 y, u16 w, u16 h, u8 *px)
{
	if(!t || !px) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		return -1;
	}

	glBindTexture(GL_TEXTURE_2D, t->texture);

	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RGBA,
			GL_UNSIGNED_BYTE, px);

	glBindTexture(GL_TEXTURE_2D, 0);

	return 0;
}


FH_API struct fh_texture *fh_tex_get(struct fh_window *win, char *name)
{
	struct fh_texture *tex;

	if(!win || !name) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(fh_tbl_get(win->textures, name, NULL, (void **)&tex) != 1) {
		ALARM(ALARM_ERR, "Texture could not be found in fh_table");
		goto err_return;
	}

	return tex;

err_return:
	ALARM(ALARM_ERR, "Failed to get texture from texture table");
	return NULL;

}


FH_API void fh_tex_use(struct fh_texture *tex)
{
	if(!tex) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex->texture);
}


FH_API void fh_tex_unuse(void)
{
	glBindTexture(GL_TEXTURE_2D, 0);
}


FH_API void fh_tex_rmv_fnc(u32 size, void *ptr)
{
	struct fh_texture *tex;

	/* SILENCIO */
	if(size) {}

	if(!ptr)
		return;

	tex = (struct fh_texture *)ptr;

	fh_tex_destroy(tex);
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

FH_API s8 fh_create_texture(struct fh_window *win, char *name, u16 w, u16 h,
		GLenum format, u8 *px)
{
	struct fh_texture *tex;

	if(!win || !name || w < 1 || h < 1 || !px) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		goto err_return;
	}

	if(!(tex = fh_tex_create(name, w, h, format, px)))
		goto err_return;

	if(fh_tex_insert(win, tex) < 0)
		goto err_destroy_tex;

	return 0;

err_destroy_tex:
	fh_tex_destroy(tex);

err_return:
	ALARM(ALARM_ERR, "Failed to create new texture");
	return -1;
}


FH_API s8 fh_load_texture(struct fh_window *win, char *name, char *pth)
{
	struct fh_texture *tex;

	if(!win || !name || !pth) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(!(tex = fh_tex_load(name, pth)))
		goto err_return;

	if(fh_tex_insert(win, tex))
		goto err_destroy_tex;

	return 0;

err_destroy_tex:
	fh_tex_destroy(tex);

err_return:
	ALARM(ALARM_ERR, "Failed to load new texture");
	return -1;
}


FH_API void fh_remove_texture(struct fh_window *win, char *name)
{
	struct fh_texture *tex;

	if(!win || !name) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	if(!(tex = fh_tex_get(win, name)))
		return;

	fh_tex_remove(win, tex);
}


