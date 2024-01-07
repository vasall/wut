#include "document/inc/element_rendering.h"

#include "document/inc/document.h"

#include "utility/alarm/inc/alarm.h"

#include "widget/inc/widget.h"

#include "utility/inc/color.h"


FH_API void fh_element_render(struct fh_batch *ren, struct fh_element *ele)
{
	s32 indices[4];
	s32 v_index[2];
	f32 color[4];

	struct tempStruct_vtx {
		f32 x;
		f32 y;
		f32 z;
		s32 index[2];
		s32 type;
	} vdata;

	s32 p0x = ele->output_rect.x;
	s32 p0y = ele->output_rect.y;
	s32 p1x = ele->output_rect.x + ele->output_rect.w;
	s32 p1y = ele->output_rect.y + ele->output_rect.h;


	/*
	 * Return if the element is not visible.
	 */
	if(!(ele->info_flags & FH_ELEMENT_F_VISIBLE))
		return;

	/* Unioform: u_rect */
	v_index[0] = fh_batch_push_uniform(ren, 1, &ele->output_rect);

	/* Uniform: u_color */
	fh_color_get_fv(ele->style.infill.color, color);
	v_index[1] = fh_batch_push_uniform(ren, 2, color);

	/* Uniform: u_radius */
	fh_batch_push_uniform(ren, 3, ele->style.radius.corner);

	/* Uniform: u_bwidth */
	fh_batch_push_uniform(ren, 4, &ele->style.border.width);

	/* Uniform: u_bcolor */
	fh_color_get_fv(ele->style.border.color, color);
	fh_batch_push_uniform(ren, 5, color);

	vdata.z = (f32)ele->layer / 100.0;
	vdata.index[0] = v_index[0];
	vdata.index[1] = v_index[1];
	vdata.type = 1;


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

	/* If the element has a context, render that aswell */
	if(ele->widget) {
		fh_RenderWidget(ele->widget);
	}
#endif
}


FH_XMOD void fh_element_ren_scrollbar(struct fh_batch *ren, struct fh_element *ele)
{
	struct fh_rect rect;
	s32 indices[4];
	s32 s_index[2];

	s32 scroll[2] = {0, 100};

	struct tempStruct_vtx {
		f32 x;
		f32 y;
		f32 z;
		s32 index[2];
		s32 type;
	} vdata;

	/* vertical */
	if(ele->scrollbar_flags & FH_RESTYLE_SCROLL_V || 1) {
		s32 p0x = ele->output_rect.x + ele->output_rect.w - ele->style.border.width - 10;
		s32 p0y = ele->output_rect.y + ele->style.border.width;
		s32 p1x = ele->output_rect.x + ele->output_rect.w - ele->style.border.width;
		s32 p1y = ele->output_rect.y + ele->output_rect.h - ele->style.border.width;
		
		fh_rect_set(&rect,
				p0x,
				p0y,
				10,
				ele->output_rect.h - (ele->style.border.width * 2));


		/* Unioform: u_rect */
		s_index[0] = fh_batch_push_uniform(ren, 1, &rect);

		/* Uniform: u_scroll */
		s_index[1] = fh_batch_push_uniform(ren, 6, scroll);

		vdata.z = (f32)ele->layer / 100.0;
		vdata.index[0] = s_index[0];
		vdata.index[1] = s_index[1];
		vdata.type = 2;


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
	if(ele->scrollbar_flags & FH_RESTYLE_SCROLL_H || 1) {
		s32 p0x = ele->output_rect.x + ele->style.border.width;
		s32 p0y = ele->output_rect.y + ele->output_rect.h - ele->style.border.width - 10;
		s32 p1x = ele->output_rect.x + ele->output_rect.w - ele->style.border.width;
		s32 p1y = ele->output_rect.y + ele->output_rect.h - ele->style.border.width;
		
		fh_rect_set(&rect,
				p0x,
				p0y, 
				ele->output_rect.w - (ele->style.border.width * 2),
				10);


		/* Unioform: u_rect */
		s_index[0] = fh_batch_push_uniform(ren, 1, &rect);

		/* Uniform: u_scroll */
		s_index[1] = fh_batch_push_uniform(ren, 6, scroll);

		vdata.z = (f32)ele->layer / 100.0;
		vdata.index[0] = s_index[0];
		vdata.index[1] = s_index[1];
		vdata.type = 3;


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
		prop = (f32)elebox.h / (f32)ele->content_size.y;
		size = prop * elebox.h;

		out.x = (elebox.x + elebox.w) - (width + 5);
		out.y = elebox.y;
		out.w = width;
		out.h = size;

		printf("Render scrollbar at: ");
		fh_rect_dump(&out);
		printf("\n");

		/*fh_FlatRectSet(ele->document->flat, &out, col);*/
#endif
	}
}
