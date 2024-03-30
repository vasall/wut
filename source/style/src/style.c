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
	struct fh_style *run = style;

	if(!style || !ret)
		return -1;

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
	struct fh_restyle_shape *rshape;
	struct fh_style *out;
	struct fh_restyle_shape *oshape;

	ref = style->ref;
	rshape = &ref->shape;
	out = style;
	oshape = &out->shape;

		
	/*
	 * CALCULATE REFERENCE SIZE
	 */

	if(ref) {
		ref_width = rshape->bounding_box.w + rshape->content_delta.w;
		ref_height = rshape->bounding_box.h + rshape->content_delta.h;
		ref_text = ref->text.size;
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
	out->border.mode = ret.keyword;

	refv[0] = ref_width;
	fh_style_get(style, FH_SHEET_BORDER_WIDTH, &ret);
	out->border.width = fh_flex_process(ret.flex, refv);

	fh_style_get(style, FH_SHEET_BORDER_COLOR, &ret);
	out->border.color = fh_col_conv_itos(ret.hexcode);

	/*
	 * *********************************************************************
	 *
	 * 		BOUNDING BOX
	 *
	 * *********************************************************************
	 */

	uref = spacing[1] + spacing[3] + padding[1] + padding[3];
	oshape->bounding_box.w = width + uref + (2 * out->border.width);

	uref = spacing[0] + spacing[2] + padding[0] + padding[2];
	oshape->bounding_box.h = height + uref + (2 * out->border.width);
	
	oshape->bounding_box.x = 0;
	oshape->bounding_box.y = 0;

	/*
	 * *********************************************************************
	 *
	 * 		ELEMENT DELTA
	 *
	 * *********************************************************************
	 */
	
	oshape->element_delta.w = -(spacing[1] + spacing[3]);
	oshape->element_delta.h = -(spacing[0] + spacing[2]);

	oshape->element_delta.x = spacing[3];
	oshape->element_delta.y = spacing[0];

	/*
	 * *********************************************************************
	 *
	 * 		INNER DELTA
	 *
	 * *********************************************************************
	 */

	oshape->inner_delta.w = oshape->element_delta.w - (2 * out->border.width);
	oshape->inner_delta.h = oshape->element_delta.h - (2 * out->border.width);

	oshape->inner_delta.x = oshape->element_delta.x + out->border.width;
	oshape->inner_delta.y = oshape->element_delta.y + out->border.width;

	/*
	 * *********************************************************************
	 *
	 * 		CONTENT DELTA
	 *
	 * *********************************************************************
	 */

	oshape->content_delta.w =
		oshape->inner_delta.w - padding[1] - padding[3];
	oshape->content_delta.h =
		oshape->inner_delta.h - padding[0] - padding[2];

	oshape->content_delta.x = oshape->inner_delta.x + padding[3];
	oshape->content_delta.y = oshape->inner_delta.y + padding[0];

	/*
	 * *********************************************************************
	 *
	 * 		DISPLAY
	 *
	 * *********************************************************************
	 */

	fh_style_get(style, FH_SHEET_DISPLAY_MODE, &ret);
	out->display.mode = ret.keyword;

	/*
	 * *********************************************************************
	 *
	 * 		REFERENCE
	 *
	 * *********************************************************************
	 */

	fh_style_get(style, FH_SHEET_REFERENCE_MODE, &ret);
	out->reference.mode = ret.keyword;

	/*
	 * *********************************************************************
	 *
	 * 		RADIUS
	 *
	 * *********************************************************************
	 */


	refv[0] = ref_width;
	fh_style_get(style, FH_SHEET_RADIUS_TOP_LEFT, &ret);
	out->radius.corner[0] = fh_flex_process(ret.flex, refv);
	fh_style_get(style, FH_SHEET_RADIUS_TOP_RIGHT, &ret);
	out->radius.corner[1] = fh_flex_process(ret.flex, refv);
	fh_style_get(style, FH_SHEET_RADIUS_BOTTOM_RIGHT, &ret);
	out->radius.corner[2] = fh_flex_process(ret.flex, refv);
	fh_style_get(style, FH_SHEET_RADIUS_BOTTOM_LEFT, &ret);
	out->radius.corner[3] = fh_flex_process(ret.flex, refv);


	/*
	 * *********************************************************************
	 *
	 * 		INFILL
	 *
	 * *********************************************************************
	 */


	fh_style_get(style, FH_SHEET_INFILL_MODE, &ret);
	out->infill.mode = ret.keyword;

	fh_style_get(style, FH_SHEET_INFILL_COLOR, &ret);
	out->infill.color = fh_col_conv_itos(ret.hexcode);


	/*
	 * *********************************************************************
	 *
	 * 		LAYOUT
	 *
	 * *********************************************************************
	 */

	fh_style_get(style, FH_SHEET_LAYOUT_MODE, &ret);
	out->layout.mode = ret.keyword;

	/*
	 * *********************************************************************
	 *
	 * 		SCROLLBAR
	 *
	 * *********************************************************************
	 */

	fh_style_get(style, FH_SHEET_SCROLLBAR_MODE, &ret);

	out->scrollbar.flags = 0;
	switch(ret.keyword) {
		case FH_KW_SCROLLBAR_AUTO:
			out->scrollbar.flags |= FH_RESTYLE_SCROLL_V;
			out->scrollbar.flags |= FH_RESTYLE_SCROLL_H;
			break;
		case FH_KW_SCROLLBAR_NONE:
			break;
		case FH_KW_SCROLLBAR_VERTICAL:
			out->scrollbar.flags |= FH_RESTYLE_SCROLL_V;
			break;
		case FH_KW_SCROLLBAR_HORIZONTAL:
			out->scrollbar.flags |= FH_RESTYLE_SCROLL_H;
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
	out->text.size = fh_flex_process(ret.flex, refv);
	
	fh_style_get(style, FH_SHEET_TEXT_COLOR, &ret);
	out->text.color = fh_col_conv_itos(ret.hexcode);

	fh_style_get(style, FH_SHEET_TEXT_MASS, &ret);
	out->text.mass = fh_flex_process(ret.flex, refv);

	fh_style_get(style, FH_SHEET_TEXT_OPTIONS, &ret);
	out->text.options = ret.keyword;
	
	fh_style_get(style, FH_SHEET_TEXT_WRAP_MODE, &ret);
	out->text.wrap_mode = ret.keyword;

	fh_style_get(style, FH_SHEET_TEXT_SPACING, &ret);
	out->text.spacing = 1;
	/* out->text.spacing = fh_flex_process(ret.flex, refv); */

	fh_style_get(style, FH_SHEET_LINE_HEIGHT, &ret);
	out->text.line_height = fh_flex_process(ret.flex, refv);

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


FH_API void *fh_GetReStyle(struct fh_style *style, enum fh_restyle_type type)
{
	if(!style) {
		return NULL;
	}

	switch(type) {
		case(FH_STYLE_DISPLAY):		return &style->display; 
		case(FH_STYLE_REFERENCE):	return &style->reference;
		case(FH_STYLE_SHAPE):		return &style->shape;
		case(FH_STYLE_RADIUS):		return &style->radius;
		case(FH_STYLE_BORDER):		return &style->border;
		case(FH_STYLE_INFILL):		return &style->infill;
		case(FH_STYLE_LAYOUT):		return &style->layout;
		case(FH_STYLE_TEXT):		return &style->text;
	}

	return NULL;
}


FH_API void fh_DumpStyle(struct fh_style *style)
{
	/*
	 * DISPLAY
	 */
	printf("%22s:\tmode=%d\n", "display",
			style->display.mode);

	/*
	 * REFERENCE
	 */
	printf("%22s:\tmode=%d\n", "reference",
			style->reference.mode);

	/*
	 * SHAPE
	 */
	printf("%22s:\tx=%d, y=%d, w=%d, h=%d\n", "bounding_box",
			style->shape.bounding_box.x,
			style->shape.bounding_box.y,
			style->shape.bounding_box.w,
			style->shape.bounding_box.h);

	printf("%22s:\tx=%d, y=%d, w=%d, h=%d\n", "element_delta",
			style->shape.element_delta.x,
			style->shape.element_delta.y,
			style->shape.element_delta.w,
			style->shape.element_delta.h);

	printf("%22s:\tx=%d, y=%d, w=%d, h=%d\n", "inner_delta",
			style->shape.inner_delta.x,
			style->shape.inner_delta.y,
			style->shape.inner_delta.w,
			style->shape.inner_delta.h);

	printf("%22s:\tx=%d, y=%d, w=%d, h=%d\n", "content_delta",
			style->shape.content_delta.x,
			style->shape.content_delta.y,
			style->shape.content_delta.w,
			style->shape.content_delta.h);


	/*
	 * BORDER
	 */
	printf("%22s:\tmode=%d, width=%d, color=%08X\n", "border", 
			style->border.mode,
			style->border.width,
			fh_color_get(style->border.color));

	/*
	 * RADIUS
	 */	
	printf("%22s:\ttl=%d, tr=%d, br=%d, bl=%d\n", "radius",
			style->radius.corner[0],
			style->radius.corner[1],
			style->radius.corner[2],
			style->radius.corner[3]);

	/*
	 * INFILL
	 */
	printf("%22s:\tmode=%d, color=%08X\n", "infill",
			style->infill.mode,
			fh_color_get(style->infill.color));

	/*
	 * LAYOUT
	 */
	printf("%22s:\tmode=%d\n", "layout",
			style->layout.mode);

	/*
	 * TEXT
	 */
	printf("%22s:\tsize=%d, color=%08x, mass=%d, options=%02x\n", "text1",
			style->text.size,
			fh_color_get(style->text.color),
			style->text.mass,
			style->text.options);

	printf("%22s:\twrap_mode=%02x, spacing=%d, line_height=%d\n", "text2",
			style->text.wrap_mode,
			style->text.spacing,
			style->text.line_height);
}
