#include "document/inc/element_rendering.h"

#include "document/inc/document.h"

#include "widget/inc/widget.h"

#include "utility/inc/color.h"


FH_API void fh_element_render(struct fh_batch *ren, struct fh_element *ele)
{
	struct fh_rect out;
	s32 rect_index;
	s32 indices[4];

	struct tempStruct {
		s32 x;
		s32 y;
		f32 color[4];
		s32 rect_index;
	} vdata;

	f32 color[4];

	s32 radius[4] = {0, 0, 0, 0};
	s32 border_thickness = 0;
	f32 border_color[4] = {1, 0, 0, 1};

	s32 p0x = ele->output_rect.x;
	s32 p0y = ele->output_rect.y;
	s32 p1x = ele->output_rect.x + ele->output_rect.w;
	s32 p1y = ele->output_rect.x + ele->output_rect.h;

	if(!ele) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		return;
	}

	/*
	 * Return if the element is not visible.
	 */
	if(!(ele->info_flags & FH_ELEMENT_F_VISIBLE))
		return;

	color[0] = (f32)ele->style.infill.color.r / 255.0;
	color[1] = (f32)ele->style.infill.color.g / 255.0;
	color[2] = (f32)ele->style.infill.color.b / 255.0;
	color[3] = (f32)ele->style.infill.color.a / 255.0;

	rect_index = fh_batch_push_uniform(ren, 1, &ele->output_rect);
	fh_batch_push_uniform(ren, 2, radius);
	fh_batch_push_uniform(ren, 3, &border_thickness);
	fh_batch_push_uniform(ren, 4, border_color);

	vdata = (struct tempStruct){p0x, p0y, {color[0], color[1], color[2], color[3]}, rect_index};
	indices[0] = fh_batch_push_vertex(ren, (void *)&vdata);
	vdata = (struct tempStruct){p1x, p0y, {color[0], color[1], color[2], color[3]}, rect_index};
	indices[1] = fh_batch_push_vertex(ren, (void *)&vdata);
	vdata = (struct tempStruct){p1x, p1y, {color[0], color[1], color[2], color[3]}, rect_index};
	indices[2] = fh_batch_push_vertex(ren, (void *)&vdata);
	vdata = (struct tempStruct){p0x, p1y, {color[0], color[1], color[2], color[3]}, rect_index};
	indices[3] = fh_batch_push_vertex(ren, (void *)&vdata);

	fh_batch_push_index(ren, indices[0]);
	fh_batch_push_index(ren, indices[2]);
	fh_batch_push_index(ren, indices[3]);

	fh_batch_push_index(ren, indices[0]);
	fh_batch_push_index(ren, indices[1]);
	fh_batch_push_index(ren, indices[2]);

#if 0

	if(ele->type == FH_VIEW) {
		col = fh_col_set(0, 0, 0, 0);
		/*fh_FlatRectSet(ele->document->flat, &out, col);*/
	}
	else {
		/*fh_FlatRect(ele->document->flat, &out, col);*/
	}

	/* If the element has a context, render that aswell */
	if(ele->widget) {
		fh_RenderWidget(ele->widget);
	}
#endif
}


FH_XMOD void fh_element_ren_scrollbar(struct fh_element *ele)
{
	s32 size;
	s32 width = 5;

	f32 prop;

	struct fh_color col;
	struct fh_rect	elebox;
	struct fh_rect out;

	elebox = fh_GetElementBox(ele);

	/* vertical */
	if(ele->scrollbar_flags & FH_RESTYLE_SCROLL_V) {
		prop = (f32)elebox.h / (f32)ele->content_size.y;
		size = prop * elebox.h;

		out.x = (elebox.x + elebox.w) - (width + 5);
		out.y = elebox.y;
		out.w = width;
		out.h = size;

		printf("Render scrollbar at: ");
		fh_rect_dump(&out);
		printf("\n");

		col = fh_col_set(255, 0, 255, 255);
		/*fh_FlatRectSet(ele->document->flat, &out, col);*/
	}



	/* horizontal */
}
