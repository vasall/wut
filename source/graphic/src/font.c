#include "graphic/inc/font.h"

#include "utility/inc/alarm.h"
#include "utility/inc/table.h"

#include "system/inc/system.h"

#include "document/inc/document.h"

#include <stdlib.h>

WT_INTERN void font_batch_cfnc_push(struct wt_batch *ren, void *data)
{
	s32 frame[2];
	struct wt_rect *ref = (struct wt_rect *)data;

	frame[0] = ref->w;
	frame[1] = ref->h;
	wt_batch_push_uniform(ren, 0, frame);

}

WT_INTERN s8 font_create_batch(struct wt_font *font, struct wt_texture *tex)
{
	struct wt_shader *shd;
	struct wt_batch *ren;

	struct wt_vertex_attrib v_attributes[] = {
		{3, GL_FLOAT},		/* position */
		{2, GL_FLOAT}		/* uv-coords */
	};

	struct wt_uniform_temp uniforms[] = {
		{"u_frame", WT_UNIFORM_2IV, 1, WT_UNIFORM_F_DEFAULT}	 /* 0 */
	};

	shd = wt_GetShader(font->context, "__def_text_shader");

	ren = wt_batch_create(
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

	if((font->batch_id = wt_ContextAddBatch(font->context, &ren)) < 0) {
		wt_batch_destroy(ren);
		return -1;
	}

	return 0;
}


WT_INTERN struct wt_texture *font_load_tex(struct wt_context *ctx, char *pth)
{
	char *funnyname = "420xd";
	return wt_LoadTexture(ctx, funnyname, pth);
}


WT_INTERN s8 font_import_meta(struct wt_font_data *data, char *pth)
{
	FILE *fd;
	s32 i;

	struct wt_font_glyph glyph;

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

				&glyph.hbearing_x,
				&glyph.hbearing_y,
				&glyph.hadvance,

				&glyph.vbearing_x,
				&glyph.vbearing_y,
				&glyph.vadvance,

				&glyph.tex_coord_x,
				&glyph.tex_coord_y,

				&glyph.tex_width,
				&glyph.tex_height
		      );

		wt_list_push(data->glyphs, &glyph);
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

WT_API s8 wt_InitFontTable(struct wt_context *ctx)
{
	struct wt_statlist *lst;

	if(!ctx) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(!(lst = wt_statlist_create(sizeof(struct wt_font), 16))) {
		WT_ALARM(WT_ERROR, "Failed to create font list");
		goto err_return;
	}

	/* Attach the table to the context */
	ctx->fonts = lst;

	return 0;

err_return:
	WT_ALARM(WT_ERROR, "Failed to initializie the font table");
	return -1;
}


WT_API void wt_CloseFontTable(struct wt_context *ctx)
{
	wt_statlist_destroy(ctx->fonts);
	ctx->fonts = NULL;
}


WT_API struct wt_font *wt_LoadFont(struct wt_context *ctx,
		char *name, char *img_pth, char *meta_pth)
{
	struct wt_texture *font_tex;
	struct wt_font *font;
	u32 size;

	if(!ctx || !name || !img_pth || !meta_pth) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
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
	if(!(font = wt_malloc(sizeof(struct wt_font)))) {
		WT_ALARM(WT_ERROR, "Failed to allocate memory for font");
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
	size = sizeof(struct wt_font_glyph);
	if(!(font->data.glyphs = wt_list_create(size, 255))) {
		WT_ALARM(WT_ERROR, "Create to create glyph list");
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
	if(wt_statlist_add(ctx->fonts, &font) < 0)
		goto err_return;

	return font;

err_free_font:
	wt_free(font);

err_return:
	WT_ALARM(WT_ERROR, "Failed to load font");
	return NULL;
}


WT_API void wt_RemoveFont(struct wt_font *font)
{
	if(!font)
		return;
}

struct wt_font_filter {
	char name[128];

	s8 found;
	struct wt_font *font;
};

WT_INTERN s8 font_cfnc_find(void *ptr, s16 idx, void *data)
{
	struct wt_font *font = (struct wt_font *)(*(long *)ptr);
	struct wt_font_filter *pass = (struct wt_font_filter *)data;

	wt_Ignore(idx);

	if(pass->found)
		return 1;
	
	if(strcmp(font->name, pass->name) == 0) {
		pass->found = 1;
		pass->font = font;
		return 1;
	}

	return 0;
}

WT_API struct wt_font *wt_GetFont(struct wt_context *ctx, char *name)
{
	struct wt_font_filter flt;

	if(!ctx || !name) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	flt.found = 0;
	strcpy(flt.name, name);

	wt_statlist_apply(ctx->fonts, &font_cfnc_find, &flt);

	if(flt.found) {
		printf("Found font %s\n", name);
		return flt.font;
	}

	printf("Failed to find font\n");
	return NULL;

err_return:
	WT_ALARM(WT_ERROR, "Failed to get font from font table");
	return NULL;
}

struct wt_glyph_filter {
	s16 codepoint;

	s8 found;

	struct wt_font_glyph *glyph;
	s16 index;
};


WT_INTERN s8 font_cfnc_find_glyph(void *ptr, s16 idx, void *data)
{
	struct wt_font_glyph *glyph = (struct wt_font_glyph *)ptr;
	struct wt_glyph_filter *flt = (struct wt_glyph_filter *)data;

	wt_Ignore(idx);

	if(flt->found)
		return 1;

	if(glyph->codepoint == flt->codepoint) {
		flt->found = 1;
		flt->glyph = glyph;
		flt->index = idx;
		return 1;
	}

	return 0;
}


WT_API struct wt_font_glyph *wt_GetGlyph(struct wt_font *font, s16 cpnt)
{
	struct wt_glyph_filter flt;

	flt.codepoint = cpnt;
	flt.found = 0;

	wt_list_apply(font->data.glyphs, &font_cfnc_find_glyph, &flt);

	if(flt.found) {
		return flt.glyph;
	}

	return NULL;
}


WT_API s16 wt_GetGlyphIndex(struct wt_font *font, s16 cpnt)
{
	struct wt_glyph_filter flt;

	flt.codepoint = cpnt;
	flt.found = 0;

	wt_list_apply(font->data.glyphs, &font_cfnc_find_glyph, &flt);

	if(flt.found) {
		return flt.index;
	}

	return -1;
}


WT_API struct wt_font_glyph *wt_GetGlyphByIndex(struct wt_font *font,
		s16 idx)
{
	struct wt_font_glyph *glyph;

	if(wt_list_get(font->data.glyphs, idx, &glyph) < 0) {
		return NULL;
	}

	return glyph;
}
