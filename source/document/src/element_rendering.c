#include "document/inc/element_rendering.h"

#include "document/inc/document.h"

#include "utility/alarm/inc/alarm.h"

#include "widget/inc/widget.h"

#include "utility/inc/color.h"


FH_API void fh_element_render(struct fh_batch *ren, struct fh_element *ele)
{
	s32 indices[4];
	s32 rect_index;
	f32 color[4];

	struct tempStruct {
		f32 x;
		f32 y;
		f32 z;
		s32 index;
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

	printf("%s: X- %d, Y- %d, W- %d, H- %d\n", ele->name, 
			ele->output_rect.x, ele->output_rect.y,
			ele->output_rect.w, ele->output_rect.h);

	/* Unioform: u_rect */
	rect_index = fh_batch_push_uniform(ren, 1, &ele->output_rect);

	/* Uniform: u_color */
	fh_color_get_fv(ele->style.infill.color, color);
	fh_batch_push_uniform(ren, 2, color);

	/* Uniform: u_radius */
	fh_batch_push_uniform(ren, 3, ele->style.radius.corner);

	/* Uniform: u_bwidth */
	fh_batch_push_uniform(ren, 4, &ele->style.border.width);

	/* Uniform: u_bcolor */
	fh_color_get_fv(ele->style.border.color, color);
	fh_batch_push_uniform(ren, 5, color);

	vdata.index = rect_index;
	vdata.z = (f32)ele->layer / 100.0;

	printf("Layer: %f\n", vdata.z);

	vdata.x = (f32)p0x;
	vdata.y = (f32)p0y;
	indices[0] = fh_batch_push_vertex(ren, (void *)&vdata);
	printf(">A: (%f, %f)\n", vdata.x, vdata.y);


	vdata.x = (f32)p1x;
	vdata.y = (f32)p0y;
	indices[1] = fh_batch_push_vertex(ren, (void *)&vdata);
	printf(">B: (%f, %f)\n", vdata.x, vdata.y);

	vdata.x = (f32)p1x;
	vdata.y = (f32)p1y;
	indices[2] = fh_batch_push_vertex(ren, (void *)&vdata);
	printf(">C: (%f, %f)\n", vdata.x, vdata.y);
	
	
	vdata.x = (f32)p0x;
	vdata.y = (f32)p1y;
	indices[3] = fh_batch_push_vertex(ren, (void *)&vdata);
	printf(">D: (%f, %f)\n", vdata.x, vdata.y);

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


FH_XMOD void fh_element_ren_scrollbar(struct fh_element *ele)
{
	s32 size;
	s32 width = 5;

	f32 prop;

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

		/*fh_FlatRectSet(ele->document->flat, &out, col);*/
	}



	/* horizontal */
}
