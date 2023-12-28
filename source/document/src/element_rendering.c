#include "document/inc/element_rendering.h"

#include "document/inc/document.h"

#include "widget/inc/widget.h"

#include "utility/inc/color.h"


FH_API void fh_element_render(struct fh_batch *ren, struct fh_element *ele)
{
	s32 indices[4];

	struct tempStruct {
		s32 x;
		s32 y;
		f32 color[4];
	} vdata;

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

	vdata.color[0] = (f32)ele->style.infill.color.r / 255.0;
	vdata.color[1] = (f32)ele->style.infill.color.g / 255.0;
	vdata.color[2] = (f32)ele->style.infill.color.b / 255.0;
	vdata.color[3] = (f32)ele->style.infill.color.a / 255.0;

	vdata.x = p0x;
	vdata.y = p0y;
	indices[0] = fh_batch_push_vertex(ren, (void *)&vdata);

	vdata.x = p1x;
	vdata.y = p0y;
	indices[1] = fh_batch_push_vertex(ren, (void *)&vdata);

	vdata.x = p1x;
	vdata.y = p1y;
	indices[2] = fh_batch_push_vertex(ren, (void *)&vdata);

	vdata.x = p0x;
	vdata.y = p1y;
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
