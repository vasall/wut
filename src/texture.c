#include "texture.h"

#include "alarm.h"
#include "system.h"
#include "file.h"
#include "core.h"
#include "table.h"

#include <stdlib.h>


FH_INTERN struct fh_texture *tex_create(char *name, u16 w, u16 h,
		GLenum format, u8 *px)
{
	struct fh_texture *tex;

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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	return tex;

err_return:
	ALARM(ALARM_ERR, "Failed to create new texture");
	return NULL;
}


FH_INTERN struct fh_texture *tex_load(char *name, char *pth)
{
	struct fh_fs_r_image img;
	struct fh_texture *tex;

	/* First load the raw pixel data from a PNG */
	if((fh_fs_image(pth, &img)) < 0)
		goto err_return;

	/* Then create an OpenGL texture and past them */
	if(!(tex = tex_create(name, img.w, img.h, img.format, img.data)))
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


FH_INTERN void tex_destroy(struct fh_texture *tex)
{
	glDeleteTextures(1, &tex->texture);
	fh_free(tex);
}


FH_INTERN void tex_rmv_fnc(u32 size, void *ptr)
{
	struct fh_texture *tex;

	fh_Ignore(size);

	if(!ptr)
		return;

	tex = (struct fh_texture *)ptr;

	tex_destroy(tex);
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


FH_API s8 fh_InitTextureTable(struct fh_context *ctx)
{
	struct fh_table *tbl;

	if(!ctx) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(!(tbl = fh_tbl_create(&tex_rmv_fnc))) {
		ALARM(ALARM_ERR, "Failed to create fh_table");
		goto err_return;
	}

	/* Attach the table to the context */
	ctx->textures = tbl;

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to initialize the texture table");
	return -1;
}


FH_API void fh_CloseTextureTable(struct fh_context *ctx)
{
	if(!ctx) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	fh_tbl_destroy(ctx->textures);
	ctx->textures = NULL;
}


FH_API struct fh_texture *fh_CreateTexture(struct fh_context *ctx, char *name,
		u16 w, u16 h, GLenum format, u8 *px)
{
	struct fh_texture *tex;
	u32 size;
	void **p;

	if(!ctx || !name || w < 1 || h < 1 || !px) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		goto err_return;
	}

	if(!(tex = tex_create(name, w, h, format, px)))
		goto err_return;

	/* Set additional attributes */
	tex->context = ctx;

	/* Insert texture into table */
	size = sizeof(struct fh_texture);
	p = (void **)&tex;
	if(fh_ContextAdd(ctx, FH_CONTEXT_TEXTURES, name, size, p) < 0)
		goto err_destroy_tex;

	return tex;

err_destroy_tex:
	tex_destroy(tex);

err_return:
	ALARM(ALARM_ERR, "Failed to create new texture");
	return NULL;
}


FH_API struct fh_texture *fh_LoadTexture(struct fh_context *ctx, char *name, char *pth)
{
	struct fh_texture *tex;
	u32 size;
	void **p;

	if(!ctx || !name || !pth) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(!(tex = tex_load(name, pth)))
		goto err_return;

	/* Set additional attributes */
	tex->context = ctx;

	/* Insert texture into table */
	size = sizeof(struct fh_texture);
	p = (void **)&tex;
	if(fh_ContextAdd(ctx, FH_CONTEXT_TEXTURES, name, size, p) < 0)
		goto err_destroy_tex;

	return tex;

err_destroy_tex:
	tex_destroy(tex);

err_return:
	ALARM(ALARM_ERR, "Failed to load new texture");
	return NULL;
}


FH_API void fh_RemoveTexture(struct fh_texture *tex)
{
	if(!tex) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	fh_ContextRemove(tex->context, FH_CONTEXT_TEXTURES, tex->name);
}


FH_API s8 fh_ResizeTexture(struct fh_texture *tex, u16 w, u16 h, u8 *px)
{
	u32 newTex;
		
	if(!tex) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	printf("Resize!!!!!\n");

	/*
	 * Create the new texture.
	 */
	glGenTextures(1, &newTex);
	glBindTexture(GL_TEXTURE_2D, newTex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, px);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	/*
	 * Destroy the old texture.
	 */
	glDeleteTextures(1, &tex->texture);

	/*
	 * Update attributes.
	 */
	tex->width = w;
	tex->height = h;
	tex->texture = newTex;

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to resize texture");
	return -1;
}


FH_API s8 fh_SetTexture(struct fh_texture *tex, u16 x, u16 y, u16 w, u16 h,
		u8 *px)
{
	u32 i;

	printf("BEFORE\n");

	if(!tex || !px) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		return -1;
	}

	printf("Hello world\n");

	glBindTexture(GL_TEXTURE_2D, tex->texture);

#if 0
	for(i = 0; i < (w * h * 4); i++) {

		if(i % 4 == 0 && i != 0)
			printf("\n");
		printf("%02x ", px[i]);

	}

	printf("\n");
#endif

	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RGBA,
			GL_UNSIGNED_BYTE, px);

	glBindTexture(GL_TEXTURE_2D, 0);

	return 0;
}


FH_API struct fh_texture *fh_GetTexture(struct fh_context *ctx, char *name)
{
	struct fh_texture *tex;

	if(!ctx || !name) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(fh_tbl_get(ctx->textures, name, NULL, (void **)&tex) != 1) {
		ALARM(ALARM_ERR, "Texture could not be found in fh_table");
		goto err_return;
	}

	return tex;

err_return:
	ALARM(ALARM_ERR, "Failed to get texture from texture table");
	return NULL;
}


FH_API void fh_UseTexture(struct fh_texture *tex)
{
	if(!tex) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex->texture);
}


FH_API void fh_UnuseTexture(void)
{
	glBindTexture(GL_TEXTURE_2D, 0);
}


