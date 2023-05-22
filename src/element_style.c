#include "element_style.h"

#include "alarm.h"
#include "system.h"

#include <stdlib.h>


FH_INTERN void fh_style_reset_sheet(struct fh_stylesheet *sheet)
{
	/* 
	 * DISPLAY
	 */
	sheet->display_mode = FH_DISPLAY_BLOCK;	

	/*
	 * SIZE
	 */
	sheet->vsize = fh_flex_set(FH_FLEX_RELATIVE, 10000);
	sheet->vsize_min = fh_flex_set(FH_FLEX_RELATIVE, 0);
	sheet->vsize_max = fh_flex_set(FH_FLEX_RELATIVE, 10000);

	sheet->hsize = fh_flex_set(FH_FLEX_RELATIVE, 10000);
	sheet->hsize_min = fh_flex_set(FH_FLEX_RELATIVE, 0);
	sheet->hsize_max = fh_flex_set(FH_FLEX_RELATIVE, 10000);

	/*
	 * POSITION
	 */
	sheet->vorientation = FH_ORIENTATION_TOP;
	sheet->vposition = fh_flex_set(FH_FLEX_AUTO, 0);

	sheet->horientation = FH_ORIENTATION_LEFT;
	sheet->hposition = fh_flex_set(FH_FLEX_AUTO, 0);

	/*
	 * PADDING 
	 */
	sheet->padding_top = fh_flex_set(FH_FLEX_ABSOLUTE, 0);
	sheet->padding_right = fh_flex_set(FH_FLEX_ABSOLUTE, 0);
	sheet->padding_bottom = fh_flex_set(FH_FLEX_ABSOLUTE, 0);
	sheet->padding_left = fh_flex_set(FH_FLEX_ABSOLUTE, 0);

	/*
	 * INFILL
	 */
	sheet->infill_mode = FH_INFILL_COLOR;
	sheet->infill_color =  fh_col_set(0xFF, 0x69, 0xB4, 0xff);

	/*
	 * BORDER
	 */
	sheet->border_mode = FH_BORDER_NONE;
}


FH_API s8 fh_style_init(struct fh_style *style, struct fh_style *ref)
{
	if(!style) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	/* First of all: Wipe everything */
	memset(style, 0, sizeof(struct fh_stylesheet));
		
	/* Then reset the stylesheet */
	fh_style_reset_sheet(&style->sheet);	

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
		if(!ref) {
			ALARM(ALARM_ERR, "Inherit but not reference given");
			goto err_return;
		}

		out->display.mode = ref->display.mode;
	}
	else {
		out->display.mode = in->display_mode;
	}


	/*
	 * SIZE
	 */
	
	/* height */
	if(ref) {
		uref = ref->size.height;
		uref -= (ref->padding.top + ref->padding.bottom);
	}
	else {
		uref = rect->h;
	}

	out->size.height = fh_flex_comp_lim(&in->vsize, &in->vsize_min,
			&in->vsize_max, uref);

	printf("Height: %d\n", out->size.height);

	/* width */
	if(ref) {
		uref = ref->size.width;
		uref -= (ref->padding.left + ref->padding.right);
	}
	else {
		uref = rect->w;
	}

	out->size.width = fh_flex_comp_lim(&in->hsize, &in->hsize_min,
			&in->hsize_max, uref);

	printf("Width: %d\n", out->size.width);
		
	/*
	 * POSITION
	 */

	/* y-position */
	if(ref) {
		uref = ref->size.height;
		uref -= (ref->padding.top + ref->padding.bottom);
	}
	else {
		uref = rect->h;
	}

	uval = fh_flex_comp(&in->vposition, uref);

	if(in->vorientation == FH_ORIENTATION_BOTTOM) {
		out->position.y = uref - uval;
	}
	else {
		out->position.y = uval;
	}

	printf("Y-Position: %d\n", out->position.y);

	/* x-position */
	if(ref) {
		uref = ref->size.width;
		uref -= (ref->padding.left + ref->padding.right);
	}
	else {
		uref = rect->w;
	}

	uval = fh_flex_comp(&in->hposition, uref);

	if(in->horientation == FH_ORIENTATION_RIGHT) {
		out->position.x = uref - uval;
	}
	else {
		out->position.x = uval;
	}

	printf("X-Position: %d\n", out->position.x);

	/*
	 * PADDING
	 */
	
	/* vertical */
	if(ref) {
		uref = ref->size.height;
		uref -= (ref->padding.top + ref->padding.bottom);
	}
	else {
		uref = rect->h;
	}

	out->padding.top = fh_flex_comp(&in->padding_top, uref);
	out->padding.bottom = fh_flex_comp(&in->padding_bottom, uref);

	/* horizontal */
	if(ref) {
		uref = ref->size.width;
		uref -= (ref->padding.left + ref->padding.right);
	}
	else {
		uref = rect->w;
	}

	out->padding.left = fh_flex_comp(&in->padding_left, uref);
	out->padding.right = fh_flex_comp(&in->padding_right, uref);

	/*
	 * COLOR
	 */
	out->infill.color = out->sheet.infill_color;

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to process stylesheet");
	return -1;
}
