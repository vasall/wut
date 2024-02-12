#include "widget/inc/textfield.h"

#include "system/inc/system.h"

#include "document/inc/document.h"
#include "document/inc/element.h"

#include <stdlib.h>


FH_INTERN struct fh_rect *textfield_get_limit(struct fh_textfield *txt)
{
	if(txt->element->parent) {
		return &txt->element->parent->inner_rect;
	}
	
	return txt->element->document->shape_ref;
}


FH_API struct fh_textfield *fh_textfield_create(struct fh_element *ele,
		struct fh_font *font)
{
	struct fh_textfield *txt;

	if(!(txt = fh_malloc(sizeof(struct fh_textfield)))) {
		return NULL;
	}

	txt->element = ele;
	txt->font = font;
	*txt->content = 0;

	return txt;
}


FH_API void fh_textfield_destroy(struct fh_textfield *txt)
{
	fh_free(txt);
}


FH_API void fh_textfield_render(struct fh_textfield *txt)
{
	struct fh_font *font = txt->font;
	struct fh_font_glyph *glyph;
	struct fh_font_glyph *last_glyph;
	struct fh_batch *ren;

	struct fh_rect *lim;

	s32 indices[4];

	struct vertex {
		f32 vertex_x;
		f32 vertex_y;
		f32 vertex_z;
		f32 texture_u;
		f32 texture_v;
	} vtx[4];

	char *input_string = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut l";

	s16 font_size = 50;
	s16 baseline[2] = {0, 50};

	u32 i;
	u16 number = 0;
	struct {
		s16 x;
		s16 y;

		struct fh_font_glyph *g;
	} instance_origins[100];
	s16 bottom_left[100][2];

	/* expand the rectangle to include 40% of the maximum spread */
	f32 spread_texture = font->data.spread_in_tex * 0.4f;
	f32 spread_vertex = font->data.spread_in_font * 0.4f;

	f32 letter_spacing = 1.0;

	s16 origin[2];

	/*
	 * Get the limits of the rendering-space for this textfield.
	 */
	lim = textfield_get_limit(txt);	
	fh_Ignore(lim);

	/*
	 * First calculate the instance origin for all glyphs.
	 */
	for(i = 0; i < strlen(input_string); i++) {
		glyph = fh_GetFontGlyph(font, input_string[i]);

		instance_origins[i].g = glyph;

		if(i == 0) {
			instance_origins[i].y = baseline[1];
			instance_origins[i].x = baseline[0] - glyph->hori_bearing_x * font_size;
		}
		else {
			instance_origins[i].y = baseline[1];
			instance_origins[i].x = instance_origins[i-1].x
				+ last_glyph->hori_advance * font_size * letter_spacing;
		}

		number++;
		last_glyph = glyph;
	}

	/*
	 * Next, calculate the bottom-left corner for every glyph.
	 */
	for(i = 0; i < number; i++) {
		glyph = instance_origins[i].g;

		bottom_left[i][0] = instance_origins[i].x +
			glyph->hori_bearing_x * font_size;

		bottom_left[i][1] = instance_origins[i].y + 
			(glyph->height - glyph->hori_bearing_y) * font_size;
	}

	/*
	 * Finally push every glyph to the batch renderer.
	 */
	for(i = 0; i < number; i++) {
		ren = fh_ContextGetBatch(txt->element->document->context, font->batch_id);

		glyph = instance_origins[i].g;

		/* bottom left */
		vtx[0].vertex_x = bottom_left[i][0] - spread_vertex * font_size;
		vtx[0].vertex_y = bottom_left[i][1] - spread_vertex * font_size;
		vtx[0].vertex_z = 0.0f;
		vtx[0].texture_u = glyph->tex_coord_x - spread_texture;
		vtx[0].texture_v = glyph->tex_coord_y + glyph->tex_height + spread_texture;

		/* bottom right */
		vtx[1].vertex_x = bottom_left[i][0] + ( glyph->width + spread_vertex ) * font_size;
		vtx[1].vertex_y = bottom_left[i][1] - spread_vertex * font_size;
		vtx[1].vertex_z = 0.0f;
		vtx[1].texture_u = glyph->tex_coord_x + glyph->tex_width + spread_texture;
		vtx[1].texture_v = glyph->tex_coord_y + glyph->tex_height + spread_texture;

		/* top right */
		vtx[2].vertex_x = bottom_left[i][0] + ( glyph->width + spread_vertex ) * font_size;
		vtx[2].vertex_y = bottom_left[i][1] - ( glyph->height + spread_vertex ) * font_size;
		vtx[2].vertex_z = 0.0f;
		vtx[2].texture_u = glyph->tex_coord_x + glyph->tex_width + spread_texture;
		vtx[2].texture_v = glyph->tex_coord_y - spread_texture;

		/* top left */
		vtx[3].vertex_x = bottom_left[i][0] - spread_vertex * font_size;
		vtx[3].vertex_y = bottom_left[i][1] - ( glyph->height + spread_vertex ) * font_size;
		vtx[3].vertex_z = 0.0f;
		vtx[3].texture_u = glyph->tex_coord_x - spread_texture;
		vtx[3].texture_v = glyph->tex_coord_y - spread_texture;


		indices[0] = fh_batch_push_vertex(ren, (void *)&vtx[0]);
		indices[1] = fh_batch_push_vertex(ren, (void *)&vtx[1]);
		indices[2] = fh_batch_push_vertex(ren, (void *)&vtx[2]);
		indices[3] = fh_batch_push_vertex(ren, (void *)&vtx[3]);

		fh_batch_push_index(ren, indices[0]);
		fh_batch_push_index(ren, indices[2]);
		fh_batch_push_index(ren, indices[3]);

		fh_batch_push_index(ren, indices[0]);
		fh_batch_push_index(ren, indices[1]);
		fh_batch_push_index(ren, indices[2]);

		if(i > 0) {
			origin[0] += last_glyph->hori_advance * font_size * 1.1;
		}
	}
}
