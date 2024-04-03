#include "style/inc/layout.h"

#include "document/inc/element_manager.h"

#include <stdlib.h>

#define FH_LAYOUT_DEBUG		0


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


FH_XMOD void fh_layout_block(struct fh_element *ele)
{
	struct fh_element *run;
	s32 off_x = 0;
	s32 off_y = 0;

	s32 lim_y = 0;

	s32 w;
	s32 h;

	s32 content_width = 0;
	s32 content_height = 0;

	struct fh_style *style;

	struct fh_rect inner_rect;

	
	style = &ele->style;
	fh_rect_add(&inner_rect,
			&style->shape_bounding_box,
			&style->shape_content_delta);


	/* Go through all children */
	run = ele->firstborn;
	while(run) {
		style = &run->style;

		/* Get the size of the current child */
		w = style->shape_bounding_box.w;
		h = style->shape_bounding_box.h;


		/* If the position is absolute, do nothing */
		if(style->reference_mode == FH_KW_REFERENCE_ABSOLUTE) {
			run->layout_offset.x = 0;
			run->layout_offset.y = 0;

			fh_element_adjust_shape(run);
		}
		/* Otherwise... */
		else {
			if(off_x + w > inner_rect.w) {
				if(off_x != 0) {
					off_y += lim_y;
				}

				off_x = 0;
				lim_y = 0;
			}

			run->layout_offset.x = off_x;
			run->layout_offset.y = off_y;

			if(h > lim_y) {
				lim_y = h;
			}

			/* Adjust the offset */
			off_x += w;

			if(content_width < off_x)
				content_width = off_x;

			if(content_height < off_y)
				content_height = off_y;

			fh_element_adjust_shape(run);
		}

		run = run->younger_sibling;
	}

	content_height += lim_y;

#if FH_LAYOUT_DEBUG
	printf("BLOCKS for \"%s\": w=%d, h=%d\n",
			ele->name,
			content_width,
			content_height);
#endif

	ele->content_size.x = content_width;
	ele->content_size.y = content_height;

	fh_element_hdl_scrollbar(ele);
}


FH_XMOD void fh_layout_row(struct fh_element *ele)
{
	struct fh_element *run;
	s32 off_x = 0;
	s32 off_y = 0;

	s32 w;
	s32 h;

	s32 content_width = 0;
	s32 content_height = 0;

	struct fh_style *style;

	run = ele->firstborn;
	while(run) {
		style = &run->style;

		w = style->shape_bounding_box.w;
		h = style->shape_bounding_box.h;

		if(style->reference_mode == FH_KW_REFERENCE_ABSOLUTE) {
			run->layout_offset.x = 0;
			run->layout_offset.y = 0;

			fh_element_adjust_shape(run);
		}
		else {
			run->layout_offset.x = off_x;
			run->layout_offset.y = off_y;

			off_y += h;


			content_height += h;
			if(w > content_width) {
				content_width = w;
			}

			fh_element_adjust_shape(run);
		}

		run = run->younger_sibling;
	}

#if FH_LAYOUT_DEBUG
	printf("ROWS for \"%s\": w=%d, h=%d\n",
			ele->name,
			content_width,
			content_height);
#endif

	ele->content_size.x = content_width;
	ele->content_size.y = content_height;

	fh_element_hdl_scrollbar(ele);
}


FH_XMOD void fh_layout_column(struct fh_element *ele)
{
	struct fh_element *run;
	s32 off_x = 0;
	s32 off_y = 0;

	s32 w;
	s32 h;

	s32 content_width = 0;
	s32 content_height = 0;

	struct fh_style *style;

	run = ele->firstborn;
	while(run) {
		style = &run->style;

		w = style->shape_bounding_box.w;
		h = style->shape_bounding_box.h;

		if(style->reference_mode == FH_KW_REFERENCE_ABSOLUTE) {
			run->layout_offset.x = 0;
			run->layout_offset.y = 0;

			fh_element_adjust_shape(run);
		}
		else {
			run->layout_offset.x = off_x;
			run->layout_offset.y = off_y;

			off_x += w;

			content_width += w;
			if(h > content_height) {
				content_height = h;
			}

			fh_element_adjust_shape(run);
		}

		run = run->younger_sibling;
	}

#if FH_LAYOUT_DEBUG
	printf("COLUMNS for \"%s\": w=%d, h=%d\n",
			ele->name,
			content_width,
			content_height);
#endif

	ele->content_size.x = content_width;
	ele->content_size.y = content_height;

	fh_element_hdl_scrollbar(ele);
}
