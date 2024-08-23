#include "graphic/inc/texture.h"

#include "utility/inc/alarm.h"
#include "utility/inc/table.h"

#include "core/inc/core.h"

#include "system/inc/system.h"
#include "system/inc/file.h"

#include <stdlib.h>


WUT_INTERN struct wut_Texture *tex_create(char *name, u16 w, u16 h,
		GLenum format, u8 *px)
{
	struct wut_Texture *tex;

	if(!px) {
		return NULL;
	}

	if(!(tex = wut_malloc(sizeof(struct wut_Texture)))) {
		WUT_ALARM(WUT_ERROR, "Failed to allocate memory for texture");
		goto err_return;
	}

	strcpy(tex->name, name);
	tex->format = format;
	tex->width = w;
	tex->height = h;

	glGenTextures(1, &tex->texture);
	glBindTexture(GL_TEXTURE_2D, tex->texture);

	/* TODO: Take format from PNG */
#if 0
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, px);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#else
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, w, h, 0, GL_RED,
			GL_UNSIGNED_BYTE, px);

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT  );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT  );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR  );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR  );


#endif


	glBindTexture(GL_TEXTURE_2D, 0);

	return tex;

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to create new texture");
	return NULL;
}


WUT_INTERN struct wut_Texture *tex_load(char *name, char *pth)
{
	struct wut_fs_r_image img;
	struct wut_Texture *tex;

	/* First load the raw pixel data from a PNG */
	if((wut_fs_image(pth, &img)) < 0)
		goto err_return;

	/* Then create an OpenGL texture and past them */
	if(!(tex = tex_create(name, img.w, img.h, img.format, img.data)))
		goto err_cleanup_img;

	/* Finally clean up the loading buffer */
	wut_fs_image_cleanup(&img);

	return tex;

err_cleanup_img:
	wut_fs_image_cleanup(&img);

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to load texture");
	return NULL;
}


WUT_INTERN void tex_destroy(struct wut_Texture *tex)
{
	glDeleteTextures(1, &tex->texture);
	wut_free(tex);
}


WUT_INTERN void tex_batch_cfnc_push(struct wut_Batch *ren, void *data)
{
	s32 frame[2];
	wut_iRect *ref = (wut_iRect *)data;

	frame[0] = (*ref)[2];
	frame[1] = (*ref)[3];
	wut_bat_push_uniform(ren, 0, frame);

}

WUT_INTERN s8 tex_create_batch(struct wut_Texture *tex)
{
	struct wut_Shader *shd;
	struct wut_Batch *ren;

	struct wut_VertexAttrib v_attributes[] = {
		{3, GL_FLOAT},		/* position */
		{2, GL_FLOAT},		/* uv-coords */
		{3, GL_INT}		/* 0: shape, 1: limits */
	};

	struct wut_UniformTemp uniforms[] = {
		{"u_frame", WUT_UNI_2IV, 1, WUT_UNI_F_DEFAULT},	 /* 0 */
		{"u_rect", WUT_UNI_4IV, 200, WUT_UNI_F_DEFAULT},	 /* 1 */
		{"u_radius", WUT_UNI_4IV, 200, WUT_UNI_F_DEFAULT}, /* 2 */
		{"u_limit", WUT_UNI_4IV, 200, WUT_UNI_F_DEFAULT}	 /* 3 */
	};

	shd = wut_GetShader(tex->context, "__def_texture_shader");

	ren = wut_bat_create(
			shd,		/* Pointer to the shader to use */
			tex,		/* Pointer to the texture to use */
			3,		/* Number of vertex attributes */
			v_attributes,	/* List of all vertex attributes */
			6000,		/* Vertex capacity */
			6000,		/* Index capacity */
			4,		/* Number of uniform buffers */
			uniforms,	/* List of all uniforms */
			&tex_batch_cfnc_push,
			tex->context->window->document->shape_ref
			);

	if(!ren)
		return -1;

	if((tex->batch_id = wut_ContextAddBatch(tex->context, &ren)) < 0) {
		wut_bat_destroy(ren);
		return -1;
	}

	return 0;
}


WUT_INTERN void tex_destroy_batch(struct wut_Texture *tex)
{
	wut_ContextRemoveBatch(tex->context, tex->batch_id);
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


WUT_API s8 wut_InitTextureTable(struct wut_Context *ctx)
{
	struct wut_StatList *lst;
	s16 size;		

	if(!ctx) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	size = sizeof(struct wut_Texture *);
	if(!(lst = wut_CreateStatList(size, WUT_TEXTURE_SLOTS))) {
		WUT_ALARM(WUT_ERROR, "Failed to create wut_table");
		goto err_return;	
	}

	/* Attach the texture list to the context */
	ctx->textures = lst;

	return 0;

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to initialize the texture table");
	return -1;
}


WUT_API void wut_CloseTextureTable(struct wut_Context *ctx)
{
	if(!ctx) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

	wut_DestroyStatList(ctx->textures);
	ctx->textures = NULL;
}


WUT_API struct wut_Texture *wut_CreateTexture(struct wut_Context *ctx, char *name,
		u16 w, u16 h, GLenum format, u8 *px)
{
	struct wut_Texture *tex;

	if(!ctx || !name || w < 1 || h < 1 || !px) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		goto err_return;
	}

	if(!(tex = tex_create(name, w, h, format, px)))
		goto err_return;

	/* Set the context */
	tex->context = ctx;

	/* Create the batch renderer */
	if(tex_create_batch(tex) < 0)
		goto err_destroy_tex;

	/* Add the texture to the list  */
	if((tex->texture_slot = wut_AddStatList(ctx->textures, &tex)) < 0) {
		goto err_destroy_batch;
	}

	return tex;

err_destroy_batch:
	tex_destroy_batch(tex);

err_destroy_tex:
	tex_destroy(tex);

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to create new texture");
	return NULL;
}


WUT_API struct wut_Texture *wut_LoadTexture(struct wut_Context *ctx, char *name, char *pth)
{
	struct wut_Texture *tex;

	if(!ctx || !name || !pth) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(!(tex = tex_load(name, pth)))
		goto err_return;

	/* Set additional attributes */
	tex->context = ctx;

	if(tex_create_batch(tex) < 0)
		goto err_destroy_tex;

	/* Add the texture to the list  */
	if((tex->texture_slot = wut_AddStatList(ctx->textures, &tex)) < 0) {
		goto err_destroy_batch;
	}

	return tex;

err_destroy_batch:
	tex_destroy_batch(tex);

err_destroy_tex:
	tex_destroy(tex);

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to load new texture");
	return NULL;
}


WUT_API void wut_RemoveTexture(struct wut_Texture *tex)
{
	if(!tex) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

	wut_ContextRemove(tex->context, WUT_CONTEXT_TEXTURES, tex->name);
}


WUT_API s8 wut_ResizeTexture(struct wut_Texture *tex, u16 w, u16 h, u8 *px)
{
	u32 newTex;

	if(!tex) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		goto err_return;
	}

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
	WUT_ALARM(WUT_ERROR, "Failed to resize texture");
	return -1;
}


WUT_API s8 wut_SetTexture(struct wut_Texture *tex, u16 x, u16 y, u16 w, u16 h,
		u8 *px)
{
	if(!tex || !px) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		return -1;
	}

	glBindTexture(GL_TEXTURE_2D, tex->texture);

	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RGBA,
			GL_UNSIGNED_BYTE, px);

	glBindTexture(GL_TEXTURE_2D, 0);

	return 0;
}

struct wut_TextureFilter {
	char name[128];

	s8 found;

	struct wut_Texture *tex;
};


WUT_INTERN s8 tex_cfnc_find(void *ptr, s16 idx, void *data)
{
	struct wut_Texture *tex = (struct wut_Texture *)(*(long *)ptr);
	struct wut_TextureFilter *pass = (struct wut_TextureFilter *)data;

	WUT_IGNORE(idx);

	if(pass->found)
		return 1;


	if(strcmp(tex->name, pass->name) == 0) {
		pass->found = 1;
		pass->tex = tex;
		return 1;
	}

	return 0;

}


WUT_API struct wut_Texture *wut_GetTexture(struct wut_Context *ctx, char *name)
{
	struct wut_TextureFilter flt;

	if(!ctx || !name) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	flt.found = 0;
	strcpy(flt.name, name);

	wut_ApplyStatList(ctx->textures, &tex_cfnc_find, &flt);

	if(flt.found) {
		return flt.tex;
	}

	return NULL;

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to get texture from texture table");
	return NULL;
}


WUT_API void wut_UseTexture(struct wut_Texture *tex)
{
	if(!tex) {
		return;
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex->texture);
}


WUT_API void wut_UnuseTexture(void)
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
