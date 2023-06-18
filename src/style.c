#include "style.h"

#include "system.h"

#include <stdlib.h>




FH_API s8 fh_style_init(struct fh_style *style, struct fh_style *ref)
{
	if(!style) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	/* First of all: Wipe everything */
	memset(style, 0, sizeof(struct fh_stylesheet));
		
	/* Then reset the stylesheet */
	fh_ResetStylesheet(&style->sheet);	

	/* And set the reference */
	style->reference = ref;

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to initialize style structure");
	return -1;
}


FH_API s8 fh_style_process(struct fh_style *style, struct fh_rect *rect)
{
	u32 uref;
	u32 uval;
	u32 diff;

	u32 height;
	u32 width;

	struct fh_stylesheet *in;
	struct fh_style *ref;
	struct fh_style *out;

	in = &style->sheet;
	ref = style->reference;
	out = style;

	/*
	 * DISPLAY
	 */
	if(in->display_mode == FH_DISPLAY_INHERIT) {
		if(!ref) goto err_no_ref;

		out->display.mode = ref->display.mode;
	}
	else {
		out->display.mode = in->display_mode;
	}
	
	/*
	 * SPACING
	 */

	/* vertical */
	uref = ref ? ref->inner_size.height : (u16)rect->h;

	in->spacing.top = fh_flex_comp(&in->spacing_top, uref);
	in->spacing.bottom = fh_flex_comp(&in->spacing_bottom, uref);

	/* horizontal */
	uref = ref ? ref->inner_size.width : (u16)rect->w;

	in->spacing.left = fh_flex_comp(&in->spacing_left, uref);
	in->spacing.right = fh_flex_comp(&in->spacing_right, uref);

	/*
	 * PADDING
	 */

	/* vertical */
	uref = out->size.height;

	in->padding.top = fh_flex_comp(&in->spacing_top, uref);
	in->spacing.bottom = fh_flex_comp(&in->spacing_bottom, uref);

	/* horizontal */
	uref = out->size.height;

	in->spacing.left = fh_flex_comp(&in->spacing_left, uref);
	in->spacing.right = fh_flex_comp(&in->spacing_right, uref);

	/* 
	 * CALCULATE SIZE
	 */

	/* height */
	uref = ref ? ref->inner_shape.h : (u16)pass->window->shape.h;

	height = fh_flex_comp_lim(&in->vsize, &in->vsize_min,
			&in->vsize_max, uref);


	/* width */
	uref = ref ? ref->inner_shape.w : (u16)pass->window->shape.w;

	width = fh_flex_comp_lim(&in->hsize, &in->hsize_min,
			&in->hsize_max, uref);


	/*
	 * BOUNDING SHAPE
	 */

	/* size */
	out->bounding_shape.h = height;
	out->bounding_shape.w = width;

	/* position */
	out->bounding_shape.y = 0;
	out->bounding_shape.x = 0;


	/*
	 * SHAPE
	 */

	/* size */
	out->shape.h = -(in->spacing_top + in->spacing_bottom);
	out->shape.w = -(in->spacing_left + in->spacing_right);

	/* position */
	out->shape.y = in->spacing_top;
	out->shape.x = in->spacing_left;


	/*
	 * INNER SHAPE
	 */
	
	/* size */
	uref = in->padding_top + in->padding_bottom;
	out->inner_shape.h = out->shape.h - uref; 

	uref = in->padding_left + in->padding_right;
	out->inner_shape.w = out->shape.w - uref;

	/* position */
	out->inner_shape.y = out->shape.y + in->padding_top;
	out->inner_shape.x = out->shape.x + in->padding_left;

	/*
	 * INFILL
	 */

	out->infill.mode = in->infill_mode;
	out->infill.color = in->infill_color;

	/*
	 * LAYOUT
	 */

	out->layout.mode = in->layout_mode;


	return 0;

err_no_ref:
	ALARM(ALARM_ERR, "Inherit, but not reference");
	return-1;

err_return:
	ALARM(ALARM_ERR, "Failed to process stylesheet");
	return -1;
}
