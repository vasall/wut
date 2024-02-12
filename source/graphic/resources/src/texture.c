#include "graphic/resources/inc/texture.h"

#include "utility/alarm/inc/alarm.h"
#include "utility/inc/table.h"

#include "core/inc/core.h"

#include "system/inc/system.h"
#include "system/inc/file.h"

#include <stdlib.h>


FH_INTERN struct fh_texture *tex_create(char *name, u16 w, u16 h,
		GLenum format, u8 *px)
{
	struct fh_texture *tex;

	if(!px) {
		return NULL;
	}

	if(!(tex = fh_malloc(sizeof(struct fh_texture)))) {
		FH_ALARM(FH_ERROR, "Failed to allocate memory for texture");
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
	FH_ALARM(FH_ERROR, "Failed to create new texture");
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
	FH_ALARM(FH_ERROR, "Failed to load texture");
	return NULL;
}


FH_INTERN void tex_destroy(struct fh_texture *tex)
{
	glDeleteTextures(1, &tex->texture);
	fh_free(tex);
}


FH_INTERN void tex_batch_cfnc_push(struct fh_batch *ren, void *data)
{
	s32 frame[2];
	struct fh_rect *ref = (struct fh_rect *)data;

	frame[0] = ref->w;
	frame[1] = ref->h;
	fh_batch_push_uniform(ren, 0, frame);

}

FH_INTERN s8 tex_create_batch(struct fh_texture *tex)
{
	struct fh_shader *shd;
	struct fh_batch *ren;

	struct fh_vertex_attrib v_attributes[] = {
		{3, GL_FLOAT},		/* position */
		{2, GL_FLOAT},		/* uv-coords */
		{3, GL_INT}		/* 0: shape, 1: limits */
	};

	struct fh_uniform_temp uniforms[] = {
		{"u_frame", FH_UNIFORM_2IV, 1, FH_UNIFORM_F_DEFAULT},	 /* 0 */
		{"u_rect", FH_UNIFORM_4IV, 200, FH_UNIFORM_F_DEFAULT},	 /* 1 */
		{"u_radius", FH_UNIFORM_4IV, 200, FH_UNIFORM_F_DEFAULT}, /* 2 */
		{"u_limit", FH_UNIFORM_4IV, 200, FH_UNIFORM_F_DEFAULT}	 /* 3 */
	};

	shd = fh_GetShader(tex->context, "__def_texture_shader");

	ren = fh_batch_create(
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

	if((tex->batch_id = fh_ContextAddBatch(tex->context, &ren)) < 0) {
		fh_batch_destroy(ren);
		return -1;
	}

	return 0;
}


FH_INTERN void tex_destroy_batch(struct fh_texture *tex)
{
	fh_ContextRmvBatch(tex->context, tex->batch_id);
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
	struct fh_statlist *lst;
	s16 size;		

	if(!ctx) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		goto err_return;
	}

	size = sizeof(struct fh_texture *);
	if(!(lst = fh_statlist_create(size, FH_TEXTURE_SLOTS))) {
		FH_ALARM(FH_ERROR, "Failed to create fh_table");
		goto err_return;	
	}

	/* Attach the texture list to the context */
	ctx->textures = lst;

	return 0;

err_return:
	FH_ALARM(FH_ERROR, "Failed to initialize the texture table");
	return -1;
}


FH_API void fh_CloseTextureTable(struct fh_context *ctx)
{
	if(!ctx) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	fh_statlist_destroy(ctx->textures);
	ctx->textures = NULL;
}


FH_API struct fh_texture *fh_CreateTexture(struct fh_context *ctx, char *name,
		u16 w, u16 h, GLenum format, u8 *px)
{
	struct fh_texture *tex;

	if(!ctx || !name || w < 1 || h < 1 || !px) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
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
	if((tex->texture_slot = fh_statlist_add(ctx->textures, &tex)) < 0) {
		goto err_destroy_batch;
	}

	return tex;

err_destroy_batch:
	tex_destroy_batch(tex);

err_destroy_tex:
	tex_destroy(tex);

err_return:
	FH_ALARM(FH_ERROR, "Failed to create new texture");
	return NULL;
}


FH_API struct fh_texture *fh_LoadTexture(struct fh_context *ctx, char *name, char *pth)
{
	struct fh_texture *tex;

	if(!ctx || !name || !pth) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(!(tex = tex_load(name, pth)))
		goto err_return;

	/* Set additional attributes */
	tex->context = ctx;

	if(tex_create_batch(tex) < 0)
		goto err_destroy_tex;

	/* Add the texture to the list  */
	if((tex->texture_slot = fh_statlist_add(ctx->textures, &tex)) < 0) {
		goto err_destroy_batch;
	}

	return tex;

err_destroy_batch:
	tex_destroy_batch(tex);

err_destroy_tex:
	tex_destroy(tex);

err_return:
	FH_ALARM(FH_ERROR, "Failed to load new texture");
	return NULL;
}


FH_API void fh_RemoveTexture(struct fh_texture *tex)
{
	if(!tex) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	fh_ContextRemove(tex->context, FH_CONTEXT_TEXTURES, tex->name);
}


FH_API s8 fh_ResizeTexture(struct fh_texture *tex, u16 w, u16 h, u8 *px)
{
	u32 newTex;

	if(!tex) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
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
	FH_ALARM(FH_ERROR, "Failed to resize texture");
	return -1;
}


FH_API s8 fh_SetTexture(struct fh_texture *tex, u16 x, u16 y, u16 w, u16 h,
		u8 *px)
{
	if(!tex || !px) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		return -1;
	}

	glBindTexture(GL_TEXTURE_2D, tex->texture);

	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RGBA,
			GL_UNSIGNED_BYTE, px);

	glBindTexture(GL_TEXTURE_2D, 0);

	return 0;
}

struct fh_tex_filter {
	char name[128];

	s8 found;

	struct fh_texture *tex;
};


FH_INTERN s8 tex_cfnc_find(void *ptr, s16 idx, void *data)
{
	struct fh_texture *tex = (struct fh_texture *)(*(long *)ptr);
	struct fh_tex_filter *pass = (struct fh_tex_filter *)data;

	fh_Ignore(idx);

	if(pass->found)
		return 1;


	if(strcmp(tex->name, pass->name) == 0) {
		pass->found = 1;
		pass->tex = tex;
		return 1;
	}

	return 0;

}


FH_API struct fh_texture *fh_GetTexture(struct fh_context *ctx, char *name)
{
	struct fh_tex_filter flt;

	if(!ctx || !name) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		goto err_return;
	}

	flt.found = 0;
	strcpy(flt.name, name);

	fh_statlist_apply(ctx->textures, &tex_cfnc_find, &flt);

	if(flt.found) {
		return flt.tex;
	}

	return NULL;

err_return:
	FH_ALARM(FH_ERROR, "Failed to get texture from texture table");
	return NULL;
}


FH_API void fh_UseTexture(struct fh_texture *tex)
{
	if(!tex) {
		return;
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex->texture);
}


FH_API void fh_UnuseTexture(void)
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
