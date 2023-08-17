#include "layout.h"


#include <stdlib.h>


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


FH_CROSS void fh_layout_blocks(struct fh_element *ele)
{
	struct fh_element *run;
	s32 off_x = 0;
	s32 off_y = 0;

	s32 lim_y = 0;

	s32 w;
	s32 h;

	s32 content_width = 0;
	s32 content_height = 0;

	struct fh_rect ref_r;


	run = ele->firstborn;
	while(run) {
		w = run->style.bounding_box.w;
		h = run->style.bounding_box.h;

		fh_rect_add(&ref_r, &ele->style.bounding_box,
				&ele->style.content_delta);

		if(run->style.reference.mode == FH_REFERENCE_ABSOLUTE) {
			run->layout_offset.x = 0;
			run->layout_offset.y = 0;

			fh_ele_adjust_shape(run);
		}
		else {

			if(off_x + w > ref_r.w) {
				off_y += lim_y;

				if(content_width < off_x)
					content_width = off_x;

				if(content_height < off_y)
					content_height = off_y;

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

			fh_ele_adjust_shape(run);
		}

		run = run->younger_sibling;
	}

	ele->content_size.x = content_width;
	ele->content_size.y = content_height;
}


FH_CROSS void fh_layout_rows(struct fh_element *ele)
{
	struct fh_element *run;
	s32 off_x = 0;
	s32 off_y = 0;

	s32 lim_y = 0;

	s32 w;
	s32 h;

	s32 content_width = 0;
	s32 content_height = 0;


	run = ele->firstborn;
	while(run) {
		w = run->style.bounding_box.w;
		h = run->style.bounding_box.h;

		if(run->style.reference.mode == FH_REFERENCE_ABSOLUTE) {
			run->layout_offset.x = 0;
			run->layout_offset.y = 0;

			fh_ele_adjust_shape(run);
		}
		else {
			run->layout_offset.x = off_x;
			run->layout_offset.y = off_y;

			off_y += h;


			content_height += h;
			if(w > content_width) {
				content_width = w;
			}

			fh_ele_adjust_shape(run);
		}

		run = run->younger_sibling;
	}


	ele->content_size.x = content_width;
	ele->content_size.y = content_height;
}


FH_CROSS void fh_layout_columns(struct fh_element *ele)
{
	struct fh_element *run;
	s32 off_x = 0;
	s32 off_y = 0;

	s32 lim_y = 0;

	s32 w;
	s32 h;

	s32 content_width = 0;
	s32 content_height = 0;

	run = ele->firstborn;
	while(run) {
		w = run->style.bounding_box.w;
		h = run->style.bounding_box.h;

		if(run->style.reference.mode == FH_REFERENCE_ABSOLUTE) {
			run->layout_offset.x = 0;
			run->layout_offset.y = 0;

			fh_ele_adjust_shape(run);
		}
		else {
			run->layout_offset.x = off_x;
			run->layout_offset.y = off_y;

			off_x += w;

			content_width += w;
			if(h > content_height) {
				content_height = h;
			}

			fh_ele_adjust_shape(run);
		}

		run = run->younger_sibling;
	}


	ele->content_size.x = content_width;
	ele->content_size.y = content_height;
}
