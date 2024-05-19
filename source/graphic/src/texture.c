#include "graphic/inc/texture.h"

#include "utility/inc/alarm.h"
#include "utility/inc/table.h"

#include "core/inc/core.h"

#include "system/inc/system.h"
#include "system/inc/file.h"

#include <stdlib.h>


WT_INTERN struct wt_texture *tex_create(char *name, u16 w, u16 h,
		GLenum format, u8 *px)
{
	struct wt_texture *tex;

	if(!px) {
		return NULL;
	}

	if(!(tex = wt_malloc(sizeof(struct wt_texture)))) {
		WT_ALARM(WT_ERROR, "Failed to allocate memory for texture");
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
	WT_ALARM(WT_ERROR, "Failed to create new texture");
	return NULL;
}


WT_INTERN struct wt_texture *tex_load(char *name, char *pth)
{
	struct wt_fs_r_image img;
	struct wt_texture *tex;

	/* First load the raw pixel data from a PNG */
	if((wt_fs_image(pth, &img)) < 0)
		goto err_return;

	/* Then create an OpenGL texture and past them */
	if(!(tex = tex_create(name, img.w, img.h, img.format, img.data)))
		goto err_cleanup_img;

	/* Finally clean up the loading buffer */
	wt_fs_image_cleanup(&img);

	return tex;

err_cleanup_img:
	wt_fs_image_cleanup(&img);

err_return:
	WT_ALARM(WT_ERROR, "Failed to load texture");
	return NULL;
}


WT_INTERN void tex_destroy(struct wt_texture *tex)
{
	glDeleteTextures(1, &tex->texture);
	wt_free(tex);
}


WT_INTERN void tex_batch_cfnc_push(struct wt_batch *ren, void *data)
{
	s32 frame[2];
	struct wt_rect *ref = (struct wt_rect *)data;

	frame[0] = ref->w;
	frame[1] = ref->h;
	wt_batch_push_uniform(ren, 0, frame);

}

WT_INTERN s8 tex_create_batch(struct wt_texture *tex)
{
	struct wt_shader *shd;
	struct wt_batch *ren;

	struct wt_vertex_attrib v_attributes[] = {
		{3, GL_FLOAT},		/* position */
		{2, GL_FLOAT},		/* uv-coords */
		{3, GL_INT}		/* 0: shape, 1: limits */
	};

	struct wt_uniform_temp uniforms[] = {
		{"u_frame", WT_UNIFORM_2IV, 1, WT_UNIFORM_F_DEFAULT},	 /* 0 */
		{"u_rect", WT_UNIFORM_4IV, 200, WT_UNIFORM_F_DEFAULT},	 /* 1 */
		{"u_radius", WT_UNIFORM_4IV, 200, WT_UNIFORM_F_DEFAULT}, /* 2 */
		{"u_limit", WT_UNIFORM_4IV, 200, WT_UNIFORM_F_DEFAULT}	 /* 3 */
	};

	shd = wt_GetShader(tex->context, "__def_texture_shader");

	ren = wt_batch_create(
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

	if((tex->batch_id = wt_ContextAddBatch(tex->context, &ren)) < 0) {
		wt_batch_destroy(ren);
		return -1;
	}

	return 0;
}


WT_INTERN void tex_destroy_batch(struct wt_texture *tex)
{
	wt_ContextRmvBatch(tex->context, tex->batch_id);
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


WT_API s8 wt_InitTextureTable(struct wt_context *ctx)
{
	struct wt_statlist *lst;
	s16 size;		

	if(!ctx) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	size = sizeof(struct wt_texture *);
	if(!(lst = wt_statlist_create(size, WT_TEXTURE_SLOTS))) {
		WT_ALARM(WT_ERROR, "Failed to create wt_table");
		goto err_return;	
	}

	/* Attach the texture list to the context */
	ctx->textures = lst;

	return 0;

err_return:
	WT_ALARM(WT_ERROR, "Failed to initialize the texture table");
	return -1;
}


WT_API void wt_CloseTextureTable(struct wt_context *ctx)
{
	if(!ctx) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	wt_statlist_destroy(ctx->textures);
	ctx->textures = NULL;
}


WT_API struct wt_texture *wt_CreateTexture(struct wt_context *ctx, char *name,
		u16 w, u16 h, GLenum format, u8 *px)
{
	struct wt_texture *tex;

	if(!ctx || !name || w < 1 || h < 1 || !px) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
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
	if((tex->texture_slot = wt_statlist_add(ctx->textures, &tex)) < 0) {
		goto err_destroy_batch;
	}

	return tex;

err_destroy_batch:
	tex_destroy_batch(tex);

err_destroy_tex:
	tex_destroy(tex);

err_return:
	WT_ALARM(WT_ERROR, "Failed to create new texture");
	return NULL;
}


WT_API struct wt_texture *wt_LoadTexture(struct wt_context *ctx, char *name, char *pth)
{
	struct wt_texture *tex;

	if(!ctx || !name || !pth) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(!(tex = tex_load(name, pth)))
		goto err_return;

	/* Set additional attributes */
	tex->context = ctx;

	if(tex_create_batch(tex) < 0)
		goto err_destroy_tex;

	/* Add the texture to the list  */
	if((tex->texture_slot = wt_statlist_add(ctx->textures, &tex)) < 0) {
		goto err_destroy_batch;
	}

	return tex;

err_destroy_batch:
	tex_destroy_batch(tex);

err_destroy_tex:
	tex_destroy(tex);

err_return:
	WT_ALARM(WT_ERROR, "Failed to load new texture");
	return NULL;
}


WT_API void wt_RemoveTexture(struct wt_texture *tex)
{
	if(!tex) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	wt_ContextRemove(tex->context, WT_CONTEXT_TEXTURES, tex->name);
}


WT_API s8 wt_ResizeTexture(struct wt_texture *tex, u16 w, u16 h, u8 *px)
{
	u32 newTex;

	if(!tex) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
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
	WT_ALARM(WT_ERROR, "Failed to resize texture");
	return -1;
}


WT_API s8 wt_SetTexture(struct wt_texture *tex, u16 x, u16 y, u16 w, u16 h,
		u8 *px)
{
	if(!tex || !px) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		return -1;
	}

	glBindTexture(GL_TEXTURE_2D, tex->texture);

	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RGBA,
			GL_UNSIGNED_BYTE, px);

	glBindTexture(GL_TEXTURE_2D, 0);

	return 0;
}

struct wt_tex_filter {
	char name[128];

	s8 found;

	struct wt_texture *tex;
};


WT_INTERN s8 tex_cfnc_find(void *ptr, s16 idx, void *data)
{
	struct wt_texture *tex = (struct wt_texture *)(*(long *)ptr);
	struct wt_tex_filter *pass = (struct wt_tex_filter *)data;

	wt_Ignore(idx);

	if(pass->found)
		return 1;


	if(strcmp(tex->name, pass->name) == 0) {
		pass->found = 1;
		pass->tex = tex;
		return 1;
	}

	return 0;

}


WT_API struct wt_texture *wt_GetTexture(struct wt_context *ctx, char *name)
{
	struct wt_tex_filter flt;

	if(!ctx || !name) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	flt.found = 0;
	strcpy(flt.name, name);

	wt_statlist_apply(ctx->textures, &tex_cfnc_find, &flt);

	if(flt.found) {
		return flt.tex;
	}

	return NULL;

err_return:
	WT_ALARM(WT_ERROR, "Failed to get texture from texture table");
	return NULL;
}


WT_API void wt_UseTexture(struct wt_texture *tex)
{
	if(!tex) {
		return;
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex->texture);
}


WT_API void wt_UnuseTexture(void)
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
