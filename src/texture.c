#include "texture.h"

#include "core.h"

#include <stdlib.h>


FH_API s8 fh_tex_init(void)
{
	struct dbs_table *tbl;

	if(!(tbl = dbs_tbl_create(&fh_tex_rmv_fnc))) {
		ALARM(ALARM_ERR, "Failed to create dbs_table");
		goto err_return;
	}

	/* Attach the texture table to the global core */
	g_fh_core.textures = tbl;

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to initialize the texture table");
	return -1;
}


FH_API void fh_tex_close(void)
{
	dbs_tbl_destroy(g_fh_core.textures);
	g_fh_core.textures = NULL;
}


FH_API s8 fh_tex_insert(struct fh_texture *tex)
{
	u32 size;
	void **p;

	if(!tex) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	size = sizeof(struct fh_texture);
	p = (void **)&tex;

	if(dbs_tbl_add(g_fh_core.textures, tex->name, size, p) < 0) {
		ALARM(ALARM_ERR, "Failed to add entry to dbs_table");
		goto err_return;
	}

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to insert texture into texture table");
	return -1;
}


FH_API void fh_tex_remove(char *name)
{
	if(!name) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	dbs_tbl_rmv(g_fh_core.textures, name);
}


FH_API struct fh_texture *fh_tex_create(char *name, u16 w, u16 h, u8 *px)
{
	struct fh_texture *tex;
	float ani;

	if(!name || !px) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(!(tex = smalloc(sizeof(struct fh_texture)))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for texture");
		goto err_return;
	}

	strcpy(tex->name, name);
	tex->width = w;
	tex->height = h;

	glGenTextures(1, &tex->texture);
	glBindTexture(GL_TEXTURE_2D, tex->texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, px);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);

	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &ani);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, ani);

	glBindTexture(GL_TEXTURE_2D, 0);

	return tex;


err_return:
	ALARM(ALARM_ERR, "Failed to create new texture");
	return NULL;
}


FH_API void fh_tex_destroy(struct fh_texture *tex)
{
	if(!tex)
		return;

	glDeleteTextures(1, &tex->texture);
	sfree(tex);
}


FH_API s8 fh_tex_set(struct fh_texture *tex, u16 w, u16 h, u8 *px)
{

	if(!tex || !px) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		return -1;
	}

	glBindTexture(GL_TEXTURE_2D, tex->texture);

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGBA,
			GL_UNSIGNED_BYTE, px);

	glBindTexture(GL_TEXTURE_2D, 0);

	return 0;
}


FH_API void fh_tex_use(struct fh_texture *tex)
{
	if(!tex) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	glBindTexture(GL_TEXTURE_2D, tex->texture);
}


FH_API void fh_tex_unuse(void)
{
	glBindTexture(GL_TEXTURE_2D, 0);
}


FH_API void fh_tex_rmv_fnc(u32 size, void *ptr)
{
	struct fh_texture *tex;

	if(!ptr)
		return;

	tex = (struct fh_texture *)ptr;

	fh_tex_destroy(tex);
}
