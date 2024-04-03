#include "document/inc/element_rendering.h"

#include "document/inc/document.h"

#include "utility/inc/alarm.h"

#include "widget/inc/widget.h"

#include "utility/inc/color.h"


FH_API void fh_element_render(struct fh_batch *ren, struct fh_element *ele)
{
	s32 indices[4];
	s32 v_index[3];
	f32 color[4];

	struct tempStruct_vtx {
		f32 x;
		f32 y;
		f32 z;
		s32 index[3];
		s32 type;
	} vdata;

	s32 p0x;
	s32 p0y;
	s32 p1x;
	s32 p1y;


	/*
	 * Return if the element is not visible.
	 */
	if(!(ele->info_flags & FH_ELEMENT_F_VISIBLE))
		return;

	p0x = ele->element_rect.x;
	p0y = ele->element_rect.y;
	p1x = ele->element_rect.x + ele->element_rect.w;
	p1y = ele->element_rect.y + ele->element_rect.h;

	/* Unioform: u_rect */
	v_index[0] = fh_batch_push_uniform(ren, 1, &ele->element_rect);

	/* Uniform: u_color */
	fh_color_get_fv(ele->style.infill_color, color);
	v_index[2] = fh_batch_push_uniform(ren, 2, color);

	/* Uniform: u_radius */
	fh_batch_push_uniform(ren, 3, ele->style.radius_corner);

	/* Uniform: u_bwidth */
	fh_batch_push_uniform(ren, 4, &ele->style.border_width);

	/* Uniform: u_bcolor */
	fh_color_get_fv(ele->style.border_color, color);
	fh_batch_push_uniform(ren, 5, color);

	/* Uniform: u_limit */
	if(ele->parent) {
		v_index[1] = fh_batch_push_uniform(ren, 7, &ele->parent->inner_rect);
	}
	else {
		v_index[1] = -1;
	}

	vdata.z = (f32)ele->layer / 100.0;
	vdata.index[0] = v_index[0];	/* The element rectangle */
	vdata.index[1] = v_index[1];	/* The rendering zone */
	vdata.index[2] = v_index[2];	/* The color to use */
	vdata.type = FH_RENTYPE_DEFAULT;

	vdata.x = (f32)p0x;
	vdata.y = (f32)p0y;
	indices[0] = fh_batch_push_vertex(ren, (void *)&vdata);


	vdata.x = (f32)p1x;
	vdata.y = (f32)p0y;
	indices[1] = fh_batch_push_vertex(ren, (void *)&vdata);

	vdata.x = (f32)p1x;
	vdata.y = (f32)p1y;
	indices[2] = fh_batch_push_vertex(ren, (void *)&vdata);


	vdata.x = (f32)p0x;
	vdata.y = (f32)p1y;
	indices[3] = fh_batch_push_vertex(ren, (void *)&vdata);

	fh_batch_push_index(ren, indices[0]);
	fh_batch_push_index(ren, indices[2]);
	fh_batch_push_index(ren, indices[3]);

	fh_batch_push_index(ren, indices[0]);
	fh_batch_push_index(ren, indices[1]);
	fh_batch_push_index(ren, indices[2]);

#if 0
	printf("Render \"%s\": [%d, %d, %d, %d]\n", 
			ele->name,
			p0x, p0y,
			p1x, p1y);
#endif

	/* If the element has a widget, render that aswell */
	if(ele->widget) {
		fh_RenderWidget(ele->widget);
	}
}


FH_XMOD void fh_element_ren_scrollbar(struct fh_batch *ren, struct fh_element *ele)
{
	struct fh_rect rect;
	s32 indices[4];
	s32 s_index[3];

	s32 scroll[2];
	s32 h_scroll[2] = {0, 100};

	struct tempStruct_vtx {
		f32 x;
		f32 y;
		f32 z;
		s32 index[3];
		s32 type;
	} vdata;

	/* vertical */
	if(ele->scrollbar_flags & FH_RESTYLE_SCROLL_V) {
		s32 p0x = ele->element_rect.x + ele->element_rect.w - ele->style.border_width - 10;
		s32 p0y = ele->element_rect.y + ele->style.border_width;
		s32 p1x = ele->element_rect.x + ele->element_rect.w - ele->style.border_width;
		s32 p1y = ele->element_rect.y + ele->element_rect.h - ele->style.border_width;

		fh_rect_set(&rect,
				p0x,
				p0y,
				10,
				ele->element_rect.h - (ele->style.border_width * 2));


		/* Unioform: u_rect */
		s_index[0] = fh_batch_push_uniform(ren, 1, &rect);

		/* Uniform: u_scroll */	
		scroll[1] = (p1y - p0y) * ((f32)ele->content_rect.h / (f32)ele->content_size.y);
		scroll[0] = (p1y - p0y - 2 - scroll[1]) * ((f32)ele->content_offset.y / (f32)(ele->content_size.y - ele->content_rect.h));
		s_index[2] = fh_batch_push_uniform(ren, 6, scroll);

		vdata.z = (f32)ele->layer / 100.0;
		vdata.index[0] = s_index[0];
		vdata.index[1] = -1;
		vdata.index[2] = s_index[2];
		vdata.type = FH_RENTYPE_SCROLL_V;


		vdata.x = (f32)p0x;
		vdata.y = (f32)p0y;
		indices[0] = fh_batch_push_vertex(ren, (void *)&vdata);


		vdata.x = (f32)p1x;
		vdata.y = (f32)p0y;
		indices[1] = fh_batch_push_vertex(ren, (void *)&vdata);

		vdata.x = (f32)p1x;
		vdata.y = (f32)p1y;
		indices[2] = fh_batch_push_vertex(ren, (void *)&vdata);


		vdata.x = (f32)p0x;
		vdata.y = (f32)p1y;
		indices[3] = fh_batch_push_vertex(ren, (void *)&vdata);

		fh_batch_push_index(ren, indices[0]);
		fh_batch_push_index(ren, indices[2]);
		fh_batch_push_index(ren, indices[3]);

		fh_batch_push_index(ren, indices[0]);
		fh_batch_push_index(ren, indices[1]);
		fh_batch_push_index(ren, indices[2]);	
	}

	/* horizontal */
	if(ele->scrollbar_flags & FH_RESTYLE_SCROLL_H) {
		s32 p0x = ele->element_rect.x + ele->style.border_width;
		s32 p0y = ele->element_rect.y + ele->element_rect.h - ele->style.border_width - 10;
		s32 p1x = ele->element_rect.x + ele->element_rect.w - ele->style.border_width;
		s32 p1y = ele->element_rect.y + ele->element_rect.h - ele->style.border_width;

		fh_rect_set(&rect,
				p0x,
				p0y, 
				ele->element_rect.w - (ele->style.border_width * 2),
				10);


		/* Unioform: u_rect */
		s_index[0] = fh_batch_push_uniform(ren, 1, &rect);

		/* Uniform: u_scroll */
		s_index[2] = fh_batch_push_uniform(ren, 6, h_scroll);

		vdata.z = (f32)ele->layer / 100.0;
		vdata.index[0] = s_index[0];
		vdata.index[1] = -1;
		vdata.index[2] = s_index[2];
		vdata.type = FH_RENTYPE_SCROLL_H;


		vdata.x = (f32)p0x;
		vdata.y = (f32)p0y;
		indices[0] = fh_batch_push_vertex(ren, (void *)&vdata);


		vdata.x = (f32)p1x;
		vdata.y = (f32)p0y;
		indices[1] = fh_batch_push_vertex(ren, (void *)&vdata);

		vdata.x = (f32)p1x;
		vdata.y = (f32)p1y;
		indices[2] = fh_batch_push_vertex(ren, (void *)&vdata);


		vdata.x = (f32)p0x;
		vdata.y = (f32)p1y;
		indices[3] = fh_batch_push_vertex(ren, (void *)&vdata);

		fh_batch_push_index(ren, indices[0]);
		fh_batch_push_index(ren, indices[2]);
		fh_batch_push_index(ren, indices[3]);

		fh_batch_push_index(ren, indices[0]);
		fh_batch_push_index(ren, indices[1]);
		fh_batch_push_index(ren, indices[2]);
	}
}
