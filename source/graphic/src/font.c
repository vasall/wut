#include "source/graphic/inc/font.h"

#include "source/utility/inc/alarm.h"

#include "source/component/inc/table.h"

#include "source/system/inc/system.h"

#include "source/document/inc/document.h"

#include <stdlib.h>

WUT_INTERN void font_batch_cfnc_push(struct wut_Batch *ren, void *data)
{
	s32 frame[2];
	wut_iRect *ref = (wut_iRect *)data;

	frame[0] = (*ref)[2];
	frame[1] = (*ref)[3];
	wut_bat_push_uniform(ren, 0, frame);

}

WUT_INTERN s8 font_create_batch(struct wut_Font *font, struct wut_Texture *tex)
{
	struct wut_Shader *shd;
	struct wut_Batch *ren;

	struct wut_VertexAttrib v_attributes[] = {
		{3, GL_FLOAT},		/* position */
		{2, GL_FLOAT}		/* uv-coords */
	};

	struct wut_UniformTemp uniforms[] = {
		{"u_frame", WUT_UNI_2IV, 1, WUT_UNI_F_DEFAULT}	 /* 0 */
	};

	shd = wut_GetShader(font->context, "__def_text_shader");

	ren = wut_bat_create(
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

	if((font->batch_id = wut_ContextAddBatch(font->context, &ren)) < 0) {
		wut_bat_destroy(ren);
		return -1;
	}

	return 0;
}


WUT_INTERN struct wut_Texture *font_load_tex(struct wut_Context *ctx, char *pth)
{
	char *funnyname = "420xd";
	return wut_LoadTexture(ctx, funnyname, pth);
}


WUT_INTERN s8 font_import_meta(struct wut_Font_data *data, char *pth)
{
	FILE *fd;
	s32 i;

	struct wut_FontGlyph glyph;

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

		wut_PushList(data->glyphs, &glyph);
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

WUT_API s8 wut_InitFontTable(struct wut_Context *ctx)
{
	struct wut_StatList *lst;

	if(!ctx) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(!(lst = wut_CreateStatList(sizeof(struct wut_Font), 16))) {
		WUT_ALARM(WUT_ERROR, "Failed to create font list");
		goto err_return;
	}

	/* Attach the table to the context */
	ctx->fonts = lst;

	return 0;

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to initializie the font table");
	return -1;
}


WUT_API void wut_CloseFontTable(struct wut_Context *ctx)
{
	wut_DestroyStatList(ctx->fonts);
	ctx->fonts = NULL;
}


WUT_API struct wut_Font *wut_LoadFont(struct wut_Context *ctx,
		char *name, char *img_pth, char *meta_pth)
{
	struct wut_Texture *font_tex;
	struct wut_Font *font;
	u32 size;

	if(!ctx || !name || !img_pth || !meta_pth) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
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
	if(!(font = wut_malloc(sizeof(struct wut_Font)))) {
		WUT_ALARM(WUT_ERROR, "Failed to allocate memory for font");
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
	size = sizeof(struct wut_FontGlyph);
	if(!(font->data.glyphs = wut_CreateList(size, 255))) {
		WUT_ALARM(WUT_ERROR, "Create to create glyph list");
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
	if(wut_AddStatList(ctx->fonts, &font) < 0)
		goto err_return;

	return font;

err_free_font:
	wut_free(font);

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to load font");
	return NULL;
}


WUT_API void wut_RemoveFont(struct wut_Font *font)
{
	if(!font)
		return;
}

struct wut_FontFilter {
	char name[128];

	s8 found;
	struct wut_Font *font;
};

WUT_INTERN s8 font_cfnc_find(void *ptr, s16 idx, void *data)
{
	struct wut_Font *font = (struct wut_Font *)(*(long *)ptr);
	struct wut_FontFilter *pass = (struct wut_FontFilter *)data;

	WUT_IGNORE(idx);

	if(pass->found)
		return 1;
	
	if(strcmp(font->name, pass->name) == 0) {
		pass->found = 1;
		pass->font = font;
		return 1;
	}

	return 0;
}

WUT_API struct wut_Font *wut_GetFont(struct wut_Context *ctx, char *name)
{
	struct wut_FontFilter flt;

	if(!ctx || !name) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	flt.found = 0;
	strcpy(flt.name, name);

	wut_ApplyStatList(ctx->fonts, &font_cfnc_find, &flt);

	if(flt.found) {
		printf("Found font %s\n", name);
		return flt.font;
	}

	printf("Failed to find font\n");
	return NULL;

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to get font from font table");
	return NULL;
}

struct wut_GlyphFilter {
	s16 codepoint;

	s8 found;

	struct wut_FontGlyph *glyph;
	s16 index;
};


WUT_INTERN s8 font_cfnc_find_glyph(void *ptr, s16 idx, void *data)
{
	struct wut_FontGlyph *glyph = (struct wut_FontGlyph *)ptr;
	struct wut_GlyphFilter *flt = (struct wut_GlyphFilter *)data;

	WUT_IGNORE(idx);

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


WUT_API struct wut_FontGlyph *wut_GetGlyph(struct wut_Font *font, s16 cpnt)
{
	struct wut_GlyphFilter flt;

	flt.codepoint = cpnt;
	flt.found = 0;

	wut_ApplyList(font->data.glyphs, &font_cfnc_find_glyph, &flt);

	if(flt.found) {
		return flt.glyph;
	}

	return NULL;
}


WUT_API s16 wut_GetGlyphIndex(struct wut_Font *font, s16 cpnt)
{
	struct wut_GlyphFilter flt;

	flt.codepoint = cpnt;
	flt.found = 0;

	wut_ApplyList(font->data.glyphs, &font_cfnc_find_glyph, &flt);

	if(flt.found) {
		return flt.index;
	}

	return -1;
}


WUT_API struct wut_FontGlyph *wut_GetGlyphByIndex(struct wut_Font *font,
		s16 idx)
{
	struct wut_FontGlyph *glyph;

	if(wut_GetList(font->data.glyphs, idx, (void **)&glyph) < 0) {
		return NULL;
	}

	return glyph;
}
