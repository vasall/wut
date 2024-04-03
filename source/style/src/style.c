#include "style/inc/style.h"

#include <stdlib.h>

#include "utility/inc/alarm.h"

#include "system/inc/system.h"



#define STYLE_DEBUG 0


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-FUNCTIONS
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

FH_XMOD s8 fh_style_init(struct fh_style *style, struct fh_style *ref)
{
	if(!style) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		goto err_return;
	}

	/* First of all: Wipe everything */
	memset(style, 0, sizeof(struct fh_stylesheet));
		
	/* Then reset the stylesheet */
	fh_ResetStyle(style);	

	/* And set the reference */
	style->ref = ref;

	return 0;

err_return:
	FH_ALARM(FH_ERROR, "Failed to initialize style structure");
	return -1;
}


FH_XMOD s8 fh_style_link(struct fh_style *style, struct fh_style *ref)
{
	style->ref = ref;

	return 0;
}


FH_XMOD s8 fh_style_get(struct fh_style *style, enum fh_sheet_id id,
		struct fh_sheet_ret *ret)
{
	struct fh_style *run;

	if(!style) {
		return -1;
	}

	run = style;
	while(run) {
		if(run->sheet.mask & (1<<id)) {
			break;
		}

		run = run->ref;
	}

	return fh_sheet_get(&run->sheet, id, ret);
}


FH_XMOD s8 fh_style_process(struct fh_style *style, struct fh_style_pass *pass)
{
	u32 uref;

	struct fh_sheet_ret ret;
	u16 refv[2];

	u16 ref_height;
	u16 ref_width;
	u16 ref_text;	

	u32 height;
	u32 width;

	u32 spacing[4];
	u32 padding[4];

	struct fh_style *ref;
	struct fh_style *out;
	struct fh_stylesheet *sheet;

	ref = style->ref;
	out = style;
	sheet = &style->sheet;

		
	/*
	 * CALCULATE REFERENCE SIZE
	 */

	if(ref) {
		ref_width =
			ref->shape_bounding_box.w + ref->shape_content_delta.w;
		ref_height =
			ref->shape_bounding_box.h + ref->shape_content_delta.h;
		ref_text =
			ref->text_size;
	}
	else {
		ref_width = (u16)pass->document_shape->w;
		ref_height = (u16)pass->document_shape->h;
		ref_text = 12;
	}

	refv[1] = ref_text;

	/* 
	 * CALCULATE ELEMENT SIZE
	 */

	refv[0] = ref_width;
	fh_style_get(style, FH_SHEET_WIDTH, &ret);
	width = fh_flex_process(ret.flex, refv);


	refv[0] = ref_height;
	fh_style_get(style, FH_SHEET_HEIGHT, &ret);
	height = fh_flex_process(ret.flex, refv);

	/*
	 * *********************************************************************
	 *
	 * 		SPACING
	 *
	 * *********************************************************************
	 */


	refv[0] = ref_height;
	fh_style_get(style, FH_SHEET_SPACING_TOP, &ret);
	spacing[0] = fh_flex_process(ret.flex, refv);
	fh_style_get(style, FH_SHEET_SPACING_BOTTOM, &ret);
	spacing[2] = fh_flex_process(ret.flex, refv);

	refv[0] = ref_width;
	fh_style_get(style, FH_SHEET_SPACING_RIGHT, &ret);
	spacing[1] = fh_flex_process(ret.flex, refv);
	fh_style_get(style, FH_SHEET_SPACING_LEFT, &ret);
	spacing[3] = fh_flex_process(ret.flex, refv);


	/*
	 * *********************************************************************
	 *
	 * 		PADDING
	 *
	 * *********************************************************************
	 */

	refv[0] = ref_height;
	fh_style_get(style, FH_SHEET_PADDING_TOP, &ret);
	padding[0] = fh_flex_process(ret.flex, refv);
	fh_style_get(style, FH_SHEET_PADDING_BOTTOM, &ret);
	padding[2] = fh_flex_process(ret.flex, refv);

	refv[0] = ref_width;
	fh_style_get(style, FH_SHEET_PADDING_RIGHT, &ret);
	padding[1] = fh_flex_process(ret.flex, refv);
	fh_style_get(style, FH_SHEET_PADDING_LEFT, &ret);
	padding[3] = fh_flex_process(ret.flex, refv);


	/*
	 * *********************************************************************
	 *
	 * 		BORDER
	 *
	 * *********************************************************************
	 */

	fh_style_get(style, FH_SHEET_BORDER_MODE, &ret);
	out->border_mode = ret.keyword;

	refv[0] = ref_width;
	fh_style_get(style, FH_SHEET_BORDER_WIDTH, &ret);
	out->border_width = fh_flex_process(ret.flex, refv);

	fh_style_get(style, FH_SHEET_BORDER_COLOR, &ret);
	out->border_color = fh_col_conv_itos(ret.hexcode);

	/*
	 * *********************************************************************
	 *
	 * 		BOUNDING BOX
	 *
	 * *********************************************************************
	 */

	uref = spacing[1] + spacing[3] + padding[1] + padding[3];
	out->shape_bounding_box.w = width + uref + (2 * out->border_width);

	uref = spacing[0] + spacing[2] + padding[0] + padding[2];
	out->shape_bounding_box.h = height + uref + (2 * out->border_width);
	
	out->shape_bounding_box.x = 0;
	out->shape_bounding_box.y = 0;

	/*
	 * *********************************************************************
	 *
	 * 		ELEMENT DELTA
	 *
	 * *********************************************************************
	 */
	
	out->shape_element_delta.w = -(spacing[1] + spacing[3]);
	out->shape_element_delta.h = -(spacing[0] + spacing[2]);

	out->shape_element_delta.x = spacing[3];
	out->shape_element_delta.y = spacing[0];

	/*
	 * *********************************************************************
	 *
	 * 		INNER DELTA
	 *
	 * *********************************************************************
	 */

	out->shape_inner_delta.w = out->shape_element_delta.w - (2 * out->border_width);
	out->shape_inner_delta.h = out->shape_element_delta.h - (2 * out->border_width);

	out->shape_inner_delta.x = out->shape_element_delta.x + out->border_width;
	out->shape_inner_delta.y = out->shape_element_delta.y + out->border_width;

	/*
	 * *********************************************************************
	 *
	 * 		CONTENT DELTA
	 *
	 * *********************************************************************
	 */

	out->shape_content_delta.w =
		out->shape_inner_delta.w - padding[1] - padding[3];
	out->shape_content_delta.h =
		out->shape_inner_delta.h - padding[0] - padding[2];

	out->shape_content_delta.x = out->shape_inner_delta.x + padding[3];
	out->shape_content_delta.y = out->shape_inner_delta.y + padding[0];

	/*
	 * *********************************************************************
	 *
	 * 		DISPLAY
	 *
	 * *********************************************************************
	 */

	fh_style_get(style, FH_SHEET_DISPLAY_MODE, &ret);
	out->display_mode = ret.keyword;

	/*
	 * *********************************************************************
	 *
	 * 		REFERENCE
	 *
	 * *********************************************************************
	 */

	fh_style_get(style, FH_SHEET_REFERENCE_MODE, &ret);
	out->reference_mode = ret.keyword;

	/*
	 * *********************************************************************
	 *
	 * 		RADIUS
	 *
	 * *********************************************************************
	 */

	refv[0] = ref_width;
	fh_style_get(style, FH_SHEET_RADIUS_TOP_LEFT, &ret);
	out->radius_corner[0] = fh_flex_process(ret.flex, refv);
	fh_style_get(style, FH_SHEET_RADIUS_TOP_RIGHT, &ret);
	out->radius_corner[1] = fh_flex_process(ret.flex, refv);
	fh_style_get(style, FH_SHEET_RADIUS_BOTTOM_RIGHT, &ret);
	out->radius_corner[2] = fh_flex_process(ret.flex, refv);
	fh_style_get(style, FH_SHEET_RADIUS_BOTTOM_LEFT, &ret);
	out->radius_corner[3] = fh_flex_process(ret.flex, refv);


	/*
	 * *********************************************************************
	 *
	 * 		INFILL
	 *
	 * *********************************************************************
	 */

	fh_style_get(style, FH_SHEET_INFILL_MODE, &ret);
	out->infill_mode = ret.keyword;

	fh_style_get(style, FH_SHEET_INFILL_COLOR, &ret);
	out->infill_color = fh_col_conv_itos(ret.hexcode);


	/*
	 * *********************************************************************
	 *
	 * 		LAYOUT
	 *
	 * *********************************************************************
	 */

	fh_style_get(style, FH_SHEET_LAYOUT_MODE, &ret);
	out->layout_mode = ret.keyword;

	/*
	 * *********************************************************************
	 *
	 * 		SCROLLBAR
	 *
	 * *********************************************************************
	 */

	fh_style_get(style, FH_SHEET_SCROLLBAR_MODE, &ret);

	out->scrollbar_flags = 0;
	switch(ret.keyword) {
		case FH_KW_SCROLLBAR_AUTO:
			out->scrollbar_flags |= FH_RESTYLE_SCROLL_V;
			out->scrollbar_flags |= FH_RESTYLE_SCROLL_H;
			break;
		case FH_KW_SCROLLBAR_NONE:
			break;
		case FH_KW_SCROLLBAR_VERTICAL:
			out->scrollbar_flags |= FH_RESTYLE_SCROLL_V;
			break;
		case FH_KW_SCROLLBAR_HORIZONTAL:
			out->scrollbar_flags |= FH_RESTYLE_SCROLL_H;
			break;
	}

	/*
	 * *********************************************************************
	 *
	 * 		TEXT
	 *
	 * *********************************************************************
	 */

	fh_style_get(style, FH_SHEET_TEXT_SIZE, &ret);
	out->text_size = fh_flex_process(ret.flex, refv);
	
	fh_style_get(style, FH_SHEET_TEXT_COLOR, &ret);
	out->text_color = fh_col_conv_itos(ret.hexcode);

	fh_style_get(style, FH_SHEET_TEXT_MASS, &ret);
	out->text_mass = fh_flex_process(ret.flex, refv);

	fh_style_get(style, FH_SHEET_TEXT_OPTIONS, &ret);
	out->text_options = ret.keyword;

	fh_style_get(style, FH_SHEET_TEXT_WRAP_MODE, &ret);
	out->text_wrap_mode = ret.keyword;

	fh_style_get(style, FH_SHEET_TEXT_SPACING, &ret);
	out->text_spacing = 1;
	/* out->text_spacing = fh_flex_process(ret.flex, refv); */

	fh_style_get(style, FH_SHEET_LINE_HEIGHT, &ret);
	out->text_line_height = fh_flex_process(ret.flex, refv);

	return 0;
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

FH_API void fh_ResetStyle(struct fh_style *style)
{
	if(!style) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	style->ref = NULL;

	fh_sheet_reset(&style->sheet);
}


FH_API void fh_ModifyStyle(struct fh_style *style, char *in)
{
	if(!style || !in) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		return;
	}

	/*
	 * Parse the input expressions and modify the stylesheet accordingly.
	 */
	fh_sheet_parse(&style->sheet, in);
}


FH_API void fh_DumpStyle(struct fh_style *style)
{
	/*
	 * DISPLAY
	 */
	printf("%22s:\tmode=%d\n", "display",
			style->display_mode);

	/*
	 * REFERENCE
	 */
	printf("%22s:\tmode=%d\n", "reference",
			style->reference_mode);

	/*
	 * SHAPE
	 */
	printf("%22s:\tx=%d, y=%d, w=%d, h=%d\n", "bounding_box",
			style->shape_bounding_box.x,
			style->shape_bounding_box.y,
			style->shape_bounding_box.w,
			style->shape_bounding_box.h);

	printf("%22s:\tx=%d, y=%d, w=%d, h=%d\n", "element_delta",
			style->shape_element_delta.x,
			style->shape_element_delta.y,
			style->shape_element_delta.w,
			style->shape_element_delta.h);

	printf("%22s:\tx=%d, y=%d, w=%d, h=%d\n", "inner_delta",
			style->shape_inner_delta.x,
			style->shape_inner_delta.y,
			style->shape_inner_delta.w,
			style->shape_inner_delta.h);

	printf("%22s:\tx=%d, y=%d, w=%d, h=%d\n", "content_delta",
			style->shape_content_delta.x,
			style->shape_content_delta.y,
			style->shape_content_delta.w,
			style->shape_content_delta.h);


	/*
	 * BORDER
	 */
	printf("%22s:\tmode=%d, width=%d, color=%08X\n", "border", 
			style->border_mode,
			style->border_width,
			fh_color_get(style->border_color));

	/*
	 * RADIUS
	 */	
	printf("%22s:\ttl=%d, tr=%d, br=%d, bl=%d\n", "radius",
			style->radius_corner[0],
			style->radius_corner[1],
			style->radius_corner[2],
			style->radius_corner[3]);

	/*
	 * INFILL
	 */
	printf("%22s:\tmode=%d, color=%08X\n", "infill",
			style->infill_mode,
			fh_color_get(style->infill_color));

	/*
	 * LAYOUT
	 */
	printf("%22s:\tmode=%d\n", "layout",
			style->layout_mode);

	/*
	 * TEXT
	 */
	printf("%22s:\tsize=%d, color=%08x, mass=%d, options=%02x\n", "text1",
			style->text_size,
			fh_color_get(style->text_color),
			style->text_mass,
			style->text_options);

	printf("%22s:\twrap_mode=%02x, spacing=%d, line_height=%d\n", "text2",
			style->text_wrap_mode,
			style->text_spacing,
			style->text_line_height);
}
