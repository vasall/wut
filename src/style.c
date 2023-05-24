#include "style.h"

#include "alarm.h"
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

	struct fh_stylesheet *in;
	struct fh_style *ref;
	struct fh_style *out;

	in = &style->sheet;
	ref = style->reference;
	out = style;


	if(!in || !out) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

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
	 * OUTER SIZE
	 */
	
	/* height */
	uref = ref ? ref->inner_size.height : rect->h;

	out->outer_size.height = fh_flex_comp_lim(&in->vsize, &in->vsize_min,
			&in->vsize_max, uref);


	/* width */
	uref = ref ? ref->inner_size.width : rect->w;

	out->outer_size.width = fh_flex_comp_lim(&in->hsize, &in->hsize_min,
			&in->hsize_max, uref);


	/*
	 * PADDING
	 */
	
	/* vertical */
	uref = ref ? ref->inner_size.height : rect->h;

	out->padding.top = fh_flex_comp(&in->padding_top, uref);
	out->padding.bottom = fh_flex_comp(&in->padding_bottom, uref);

	/* horizontal */
	uref = ref ? ref->inner_size.width : rect->w;

	out->padding.left = fh_flex_comp(&in->padding_left, uref);
	out->padding.right = fh_flex_comp(&in->padding_right, uref);


	/*
	 * INNER SIZE
	 */

	/* height */
	diff = out->padding.top + out->padding.bottom; 
	out->inner_size.height = out->outer_size.height - diff;

	/* width */
	diff = out->padding.left + out->padding.right;
	out->inner_size.width = out->outer_size.width - diff;


	/*
	 * OUTER POSITION
	 */

	/* Y-position */
	uref = ref ? ref->inner_size.height : rect->h;

	uval = fh_flex_comp(&in->vposition, uref);
	uval = (in->vorientation == FH_ORIENTATION_BOTTOM) ? uref - uval : uval;

	uval = ref ? ref->inner_position.y + uval : uval;

	out->outer_position.y = uval;

	/* X-position */
	uref = ref ? ref->inner_size.width : rect->w;

	uval = fh_flex_comp(&in->hposition, uref);
	uval = (in->horientation == FH_ORIENTATION_RIGHT) ? uref - uval : uval;

	uval = ref ? ref->inner_position.x + uval : uval;

	out->outer_position.x = uval;

	/*
	 * INNER POSITION
	 */

	/* Y-position */
	uval = out->padding.top;

	out->inner_position.y = out->outer_position.y + uval;

	/* X-position */
	uval = out->padding.left;

	out->inner_position.x = out->outer_position.x + uval;

	/*
	 * COLOR
	 */
	out->infill.color = out->sheet.infill_color;

	return 0;

err_no_ref:
	ALARM(ALARM_ERR, "Inherit, but not reference");
	return-1;

err_return:
	ALARM(ALARM_ERR, "Failed to process stylesheet");
	return -1;
}
