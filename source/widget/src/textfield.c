#include "widget/inc/textfield.h"

#include "system/inc/system.h"

#include "document/inc/document.h"
#include "document/inc/element.h"

#include <stdlib.h>


FH_API struct fh_textfield *fh_txtfield_create(struct fh_element *ele,
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


FH_API void fh_txtfield_destroy(struct fh_textfield *txt)
{
	fh_free(txt);
}


FH_API void fh_txtfield_render(struct fh_textfield *txt)
{
	struct fh_font *font = txt->font;
	struct fh_font_glyph *glyph;
	struct fh_font_glyph *last_glyph;
	struct fh_batch *ren;

	s32 indices[4];

	struct vertex {
		f32 vertex_x;
		f32 vertex_y;
		f32 vertex_z;
		f32 texture_u;
		f32 texture_v;
	} vtx[4];

	f32 font_size = 0.1;
	f32 origin[2] = {0, 0};

	s32 i;

	/* expand the rectangle to include 40% of the maximum spread */
	f32 spread_texture = font->data.spread_in_tex * 0.4f;
	f32 spread_vertex = font->data.spread_in_font * 0.4f;

	for(i = 0; i < 10; i++) {
		glyph = fh_GetFontGlyph(font, i+65);
		ren = fh_ContextGetBatch(txt->element->document->context, font->batch_id);

		if(i > 0) {
			origin[0] += last_glyph->hori_advance * font_size * 1.1;
		}
		last_glyph = glyph;



		/* bottom left */
		vtx[0].vertex_x = origin[0] - spread_vertex * font_size;
		vtx[0].vertex_y = origin[1] - spread_vertex * font_size;
		vtx[0].vertex_z = 1.0f;
		vtx[0].texture_u = glyph->tex_coord_x - spread_texture;
		vtx[0].texture_v = glyph->tex_coord_y + glyph->tex_height + spread_texture;
		indices[0] = fh_batch_push_vertex(ren, (void *)&vtx[0]);

		printf("%f\t%f\t%f\t%f\t%f\n",
				vtx[0].vertex_x,
				vtx[0].vertex_y,
				vtx[0].vertex_z,
				vtx[0].texture_u,
				vtx[0].texture_v);

		/* bottom right */
		vtx[1].vertex_x = origin[0] + ( glyph->width + spread_vertex ) * font_size;
		vtx[1].vertex_y = origin[1] - spread_vertex * font_size;
		vtx[1].vertex_z = 1.0f;
		vtx[1].texture_u = glyph->tex_coord_x + glyph->tex_width + spread_texture;
		vtx[1].texture_v = glyph->tex_coord_y + glyph->tex_height + spread_texture;
		indices[1] = fh_batch_push_vertex(ren, (void *)&vtx[1]);

		printf("%f\t%f\t%f\t%f\t%f\n",
				vtx[1].vertex_x,
				vtx[1].vertex_y,
				vtx[1].vertex_z,
				vtx[1].texture_u,
				vtx[1].texture_v);

		/* top right */
		vtx[2].vertex_x = origin[0] + ( glyph->width + spread_vertex ) * font_size;
		vtx[2].vertex_y = origin[1] + ( glyph->height + spread_vertex ) * font_size;
		vtx[2].vertex_z = 1.0f;
		vtx[2].texture_u = glyph->tex_coord_x + glyph->tex_width + spread_texture;
		vtx[2].texture_v = glyph->tex_coord_y - spread_texture;

		printf("%f\t%f\t%f\t%f\t%f\n",
				vtx[2].vertex_x,
				vtx[2].vertex_y,
				vtx[2].vertex_z,
				vtx[2].texture_u,
				vtx[2].texture_v);

		/* top left */
		vtx[3].vertex_x = origin[0] - spread_vertex * font_size;
		vtx[3].vertex_y = origin[1] + ( glyph->height + spread_vertex ) * font_size;
		vtx[3].vertex_z = 1.0f;
		vtx[3].texture_u = glyph->tex_coord_x - spread_texture;
		vtx[3].texture_v = glyph->tex_coord_y - spread_texture;
		indices[3] = fh_batch_push_vertex(ren, (void *)&vtx[3]);

		printf("%f\t%f\t%f\t%f\t%f\n",
				vtx[3].vertex_x,
				vtx[3].vertex_y,
				vtx[3].vertex_z,
				vtx[3].texture_u,
				vtx[3].texture_v);


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
	}
}
