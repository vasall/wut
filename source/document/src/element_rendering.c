#include "document/inc/element_rendering.h"

#include "document/inc/document.h"

#include "utility/alarm/inc/alarm.h"

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

	s32 p0x = ele->element_rect.x;
	s32 p0y = ele->element_rect.y;
	s32 p1x = ele->element_rect.x + ele->element_rect.w;
	s32 p1y = ele->element_rect.y + ele->element_rect.h;


	/*
	 * Return if the element is not visible.
	 */
	if(!(ele->info_flags & FH_ELEMENT_F_VISIBLE))
		return;

	/* Unioform: u_rect */
	v_index[0] = fh_batch_push_uniform(ren, 1, &ele->element_rect);

	/* Uniform: u_color */
	fh_color_get_fv(ele->style.infill.color, color);
	v_index[2] = fh_batch_push_uniform(ren, 2, color);

	/* Uniform: u_radius */
	fh_batch_push_uniform(ren, 3, ele->style.radius.corner);

	/* Uniform: u_bwidth */
	fh_batch_push_uniform(ren, 4, &ele->style.border.width);

	/* Uniform: u_bcolor */
	fh_color_get_fv(ele->style.border.color, color);
	fh_batch_push_uniform(ren, 5, color);

	/* Uniform: u_limit */
	if(ele->parent) {
		v_index[1] = fh_batch_push_uniform(ren, 7, &ele->parent->content_rect);
	}
	else {
		v_index[1] = -1;
	}
		
	vdata.z = (f32)ele->layer / 100.0;
	vdata.index[0] = v_index[0];
	vdata.index[1] = v_index[1];
	vdata.index[2] = v_index[2];
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

	/* If the element has a widget, render that aswell */
	if(ele->widget) {
		fh_RenderWidget(ele->widget);
	}
#endif
}


FH_XMOD void fh_element_ren_scrollbar(struct fh_batch *ren, struct fh_element *ele)
{
	struct fh_rect rect;
	s32 indices[4];
	s32 s_index[3];

	s32 v_scroll[2] = {100, 200};
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
		s32 p0x = ele->element_rect.x + ele->element_rect.w - ele->style.border.width - 10;
		s32 p0y = ele->element_rect.y + ele->style.border.width;
		s32 p1x = ele->element_rect.x + ele->element_rect.w - ele->style.border.width;
		s32 p1y = ele->element_rect.y + ele->element_rect.h - ele->style.border.width;
		
		fh_rect_set(&rect,
				p0x,
				p0y,
				10,
				ele->element_rect.h - (ele->style.border.width * 2));


		/* Unioform: u_rect */
		s_index[0] = fh_batch_push_uniform(ren, 1, &rect);

		/* Uniform: u_scroll */
		s_index[2] = fh_batch_push_uniform(ren, 6, v_scroll);

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
		s32 p0x = ele->element_rect.x + ele->style.border.width;
		s32 p0y = ele->element_rect.y + ele->element_rect.h - ele->style.border.width - 10;
		s32 p1x = ele->element_rect.x + ele->element_rect.w - ele->style.border.width;
		s32 p1y = ele->element_rect.y + ele->element_rect.h - ele->style.border.width;
		
		fh_rect_set(&rect,
				p0x,
				p0y, 
				ele->element_rect.w - (ele->style.border.width * 2),
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
