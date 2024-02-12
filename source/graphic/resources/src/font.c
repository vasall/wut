#include "graphic/resources/inc/font.h"

#include "utility/alarm/inc/alarm.h"
#include "utility/inc/table.h"

#include "system/inc/system.h"

#include "document/inc/document.h"

#include <stdlib.h>

FH_INTERN void font_batch_cfnc_push(struct fh_batch *ren, void *data)
{
	s32 frame[2];
	struct fh_rect *ref = (struct fh_rect *)data;

	frame[0] = ref->w;
	frame[1] = ref->h;
	fh_batch_push_uniform(ren, 0, frame);

}

FH_INTERN s8 font_create_batch(struct fh_font *font, struct fh_texture *tex)
{
	struct fh_shader *shd;
	struct fh_batch *ren;

	struct fh_vertex_attrib v_attributes[] = {
		{3, GL_FLOAT},		/* position */
		{2, GL_FLOAT}		/* uv-coords */
	};

	struct fh_uniform_temp uniforms[] = {
		{"u_frame", FH_UNIFORM_2IV, 1, FH_UNIFORM_F_DEFAULT}	 /* 0 */
	};

	shd = fh_GetShader(font->context, "__def_font_shader");

	ren = fh_batch_create(
			shd,		/* Pointer to the shader to use */
			tex,		/* Pointer to the texture to use */
			2,		/* Number of vertex attributes */
			v_attributes,	/* List of all vertex attributes */
			6000,		/* Vertex capacity */
			6000,		/* Index capacity */
			1,		/* Number of uniform buffers */
			uniforms,	/* List of all uniforms */
			&font_batch_cfnc_push,
			font->context->window->document->shape_ref
			);

	if(!ren)
		return -1;

	if((font->batch_id = fh_ContextAddBatch(font->context, &ren)) < 0) {
		fh_batch_destroy(ren);
		return -1;
	}

	return 0;
}


FH_INTERN struct fh_texture *font_load_tex(struct fh_context *ctx, char *pth)
{
	char *funnyname = "420xd";
	return fh_LoadTexture(ctx, funnyname, pth);
}


FH_INTERN s8 font_import_meta(struct fh_font_data *data, char *pth)
{
	FILE *fd;
	s32 i;

	struct fh_font_glyph glyph;

	/* Open the file */
	if(!(fd = fopen(pth, "r")))
		return -1;

	fscanf(fd, "%f", &data->spread_in_tex);
	fscanf(fd, "%f", &data->spread_in_font);

	for(i = 0; i < 194; i++) {
		fscanf(fd, "%hd %f %f %f %f %f %f %f %f %f %f %f %f",
				&glyph.codepoint,

				&glyph.width,
				&glyph.height,

				&glyph.hori_bearing_x,
				&glyph.hori_bearing_y,
				&glyph.hori_advance,

				&glyph.verti_bearing_x,
				&glyph.verti_bearing_y,
				&glyph.verti_advance,

				&glyph.tex_coord_x,
				&glyph.tex_coord_y,

				&glyph.tex_width,
				&glyph.tex_height
		      );

		fh_list_push(data->glyphs, &glyph);
	}

	fclose(fd);
	return 0;
}

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

FH_API s8 fh_InitFontTable(struct fh_context *ctx)
{
	struct fh_statlist *lst;

	if(!ctx) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(!(lst = fh_statlist_create(sizeof(struct fh_font), 16))) {
		FH_ALARM(FH_ERROR, "Failed to create font list");
		goto err_return;
	}

	/* Attach the table to the context */
	ctx->fonts = lst;

	return 0;

err_return:
	FH_ALARM(FH_ERROR, "Failed to initializie the font table");
	return -1;
}


FH_API void fh_CloseFontTable(struct fh_context *ctx)
{
	fh_statlist_destroy(ctx->fonts);
	ctx->fonts = NULL;
}


FH_API struct fh_font *fh_LoadFont(struct fh_context *ctx,
		char *name, char *img_pth, char *meta_pth)
{
	struct fh_texture *font_tex;
	struct fh_font *font;
	u32 size;

	if(!ctx || !name || !img_pth || !meta_pth) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		goto err_return;
	}

	/*
	 * First import the image texture.
	 */
	if(!(font_tex = font_load_tex(ctx, img_pth))) {
		goto err_return;
	}

	/*
	 * Allocate memory for the font-struct itself.
	 */
	if(!(font = fh_malloc(sizeof(struct fh_font)))) {
		FH_ALARM(FH_ERROR, "Failed to allocate memory for font");
		goto err_return;
	}

	/* Set the attributes of the font */
	strcpy(font->name, name);
	font->context = ctx;

	/*
	 * Create the batch renderer for the font.
	 */
	if(font_create_batch(font, font_tex) < 0) {
		goto err_free_font;
	}

	/*
	 * Initialite the codepoint-list.
	 */
	size = sizeof(struct fh_font_glyph);
	if(!(font->data.glyphs = fh_list_create(size, 255))) {
		FH_ALARM(FH_ERROR, "Create to create glyph list");
		goto err_free_font;
	}

	/*
	 * Import the meta data
	 */
	if(font_import_meta(&font->data, meta_pth) < 0)
		goto err_return;

	/*
	 * Add font to the font list.
	 */
	if(fh_statlist_add(ctx->fonts, &font) < 0)
		goto err_return;

	return font;

err_free_font:
	fh_free(font);

err_return:
	FH_ALARM(FH_ERROR, "Failed to load font");
	return NULL;
}


FH_API void fh_RemoveFont(struct fh_font *font)
{
	if(!font)
		return;
}

struct fh_font_filter {
	char name[128];

	s8 found;
	struct fh_font *font;
};

FH_INTERN s8 font_cfnc_find(void *ptr, s16 idx, void *data)
{
	struct fh_font *font = (struct fh_font *)(*(long *)ptr);
	struct fh_font_filter *pass = (struct fh_font_filter *)data;

	fh_Ignore(idx);

	if(pass->found)
		return 1;
	
	if(strcmp(font->name, pass->name) == 0) {
		pass->found = 1;
		pass->font = font;
		return 1;
	}

	return 0;
}

FH_API struct fh_font *fh_GetFont(struct fh_context *ctx, char *name)
{
	struct fh_font_filter flt;

	if(!ctx || !name) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		goto err_return;
	}

	flt.found = 0;
	strcpy(flt.name, name);

	fh_statlist_apply(ctx->fonts, &font_cfnc_find, &flt);

	if(flt.found) {
		printf("Found font %s\n", name);
		return flt.font;
	}

	printf("Failed to find font\n");
	return NULL;

err_return:
	FH_ALARM(FH_ERROR, "Failed to get font from font table");
	return NULL;
}

struct fh_glyph_filter {
	s16 codepoint;

	s8 found;

	struct fh_font_glyph *glyph;
};


FH_INTERN s8 font_cfnc_find_glyph(void *ptr, s16 idx, void *data)
{
	struct fh_font_glyph *glyph = (struct fh_font_glyph *)ptr;
	struct fh_glyph_filter *flt = (struct fh_glyph_filter *)data;

	fh_Ignore(idx);

	if(flt->found)
		return 1;

	if(glyph->codepoint == flt->codepoint) {
		flt->found = 1;
		flt->glyph = glyph;
		return 1;
	}

	return 0;
}


FH_API struct fh_font_glyph *fh_GetFontGlyph(struct fh_font *font, s16 idx)
{
	struct fh_glyph_filter flt;

	flt.codepoint = idx;
	flt.found = 0;

	fh_list_apply(font->data.glyphs, &font_cfnc_find_glyph, &flt);

	if(flt.found) {
		return flt.glyph;
	}

	return NULL;
}
