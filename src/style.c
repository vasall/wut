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


FH_API s8 fh_style_process(struct fh_style *style, struct fh_style_pass *pass)
{
	u32 uref;
	u32 uval;
	u32 diff;

	u32 height;
	u32 width;

	u32 spacing[4];
	u32 padding[4];

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
	 * CALCULATE SIZE
	 */

	/* height */
	uref = ref ? ref->inner_shape.h : (u16)pass->document_shape->h;
	height = fh_flex_comp(&in->vsize, uref);


	/* width */
	uref = ref ? ref->inner_shape.w : (u16)pass->document_shape->w;
	width = fh_flex_comp(&in->hsize, uref);

	/*
	 * SPACING
	 */

	/* vertical */
	uref = ref ? ref->inner_shape.h : (u16)pass->document_shape->h;

	spacing[0] = fh_flex_comp(&in->spacing_top, uref);
	spacing[2] = fh_flex_comp(&in->spacing_bottom, uref);

	/* horizontal */
	uref = ref ? ref->inner_shape.w : (u16)pass->document_shape->w;

	spacing[3] = fh_flex_comp(&in->spacing_left, uref);
	spacing[1] = fh_flex_comp(&in->spacing_right, uref);


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
	out->shape.h = -(spacing[0] + spacing[2]);
	out->shape.w = -(spacing[1] + spacing[3]);

	/* position */
	out->shape.y = spacing[0];
	out->shape.x = spacing[3];

	/*
	 * PADDING
	 */

	/* vertical */
	uref = out->shape.h;

	padding[0] = fh_flex_comp(&in->padding_top, uref);
	padding[2] = fh_flex_comp(&in->padding_bottom, uref);

	/* horizontal */
	uref = out->shape.w;

	padding[3] = fh_flex_comp(&in->padding_left, uref);
	padding[1] = fh_flex_comp(&in->padding_right, uref);

	/*
	 * INNER SHAPE
	 */
	
	/* size */
	uref = padding[0] + padding[2];
	out->inner_shape.h = out->shape.h - uref; 

	uref = padding[1] + padding[3];
	out->inner_shape.w = out->shape.w - uref;

	/* position */
	out->inner_shape.y = out->shape.y + padding[0];
	out->inner_shape.x = out->shape.x + padding[3];

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
