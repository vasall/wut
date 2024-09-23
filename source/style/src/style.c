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

WUT_XMOD s8 wut_stl_init(struct wut_Style *style, struct wut_Style *ref)
{
	if(!style) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	/* First of all: Wipe everything */
	memset(style, 0, sizeof(struct wut_Stylesheet));

	/* Then reset the stylesheet */
	wut_ResetStyle(style);	

	/* And set the reference */
	style->ref = ref;

	return 0;

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to initialize style structure");
	return -1;
}


WUT_XMOD s8 wut_stl_link(struct wut_Style *style, struct wut_Style *ref)
{
	style->ref = ref;

	return 0;
}


WUT_XMOD s8 wut_stl_get(struct wut_Style *style, enum wut_eSheetAttribId id,
		struct wut_SheetEntry *ret)
{
	struct wut_Style *run;

	if(!style) {
		return -1;
	}

	run = style;
	while(run) {
		if(run->sheet.mask & (1<<id)) {
                	return wut_sht_get(&run->sheet, id, ret);
		}

                if(run->classes.mask & (1<<id)) {
                        return wut_cls_find(&run->classes, id, ret);
                }

		run = run->ref;
	}

        return -1;
}


WUT_XMOD s8 wut_stl_process(struct wut_Style *style,
                struct wut_StylePass *pass)
{
	u32 uref;

	struct wut_SheetEntry ret;
	u16 refv[2];

	u16 ref_height;
	u16 ref_width;
	u16 ref_text;	

	u32 height;
	u32 width;

	u32 spacing[4];
	u32 padding[4];

	struct wut_Style *ref;
	struct wut_Style *out;
                        
	ref = style->ref;
	out = style;

	/*
	 * CALCULATE REFERENCE SIZE
	 */

	if(ref) {
		ref_width =
			ref->shape_bounding_box[2] + ref->shape_content_delta[2];
		ref_height =
			ref->shape_bounding_box[3] + ref->shape_content_delta[3];
		ref_text =
			ref->text_size;
	}
	else {
		ref_width = (u16)(*pass->document_shape)[2];
		ref_height = (u16)(*pass->document_shape)[3];
		ref_text = 12;
	}

	refv[1] = ref_text;

	/* 
	 * CALCULATE ELEMENT SIZE
	 */

	refv[0] = ref_width;
	wut_stl_get(style, WUT_SHEET_WIDTH, &ret);

	width = wut_flx_process(ret.value.flex.pointer, refv);


	refv[0] = ref_height;
	wut_stl_get(style, WUT_SHEET_HEIGHT, &ret);
	height = wut_flx_process(ret.value.flex.pointer, refv);

	/*
	 * *********************************************************************
	 *
	 * 		SPACING
	 *
	 * *********************************************************************
	 */


	refv[0] = ref_height;
	wut_stl_get(style, WUT_SHEET_SPACING_TOP, &ret);
	spacing[0] = wut_flx_process(ret.value.flex.pointer, refv);
	wut_stl_get(style, WUT_SHEET_SPACING_BOTTOM, &ret);
	spacing[2] = wut_flx_process(ret.value.flex.pointer, refv);

	refv[0] = ref_width;
	wut_stl_get(style, WUT_SHEET_SPACING_RIGHT, &ret);
	spacing[1] = wut_flx_process(ret.value.flex.pointer, refv);
	wut_stl_get(style, WUT_SHEET_SPACING_LEFT, &ret);
	spacing[3] = wut_flx_process(ret.value.flex.pointer, refv);


	/*
	 * *********************************************************************
	 *
	 * 		PADDING
	 *
	 * *********************************************************************
	 */

	refv[0] = ref_height;
	wut_stl_get(style, WUT_SHEET_PADDING_TOP, &ret);
	padding[0] = wut_flx_process(ret.value.flex.pointer, refv);
	wut_stl_get(style, WUT_SHEET_PADDING_BOTTOM, &ret);
	padding[2] = wut_flx_process(ret.value.flex.pointer, refv);

	refv[0] = ref_width;
	wut_stl_get(style, WUT_SHEET_PADDING_RIGHT, &ret);
	padding[1] = wut_flx_process(ret.value.flex.pointer, refv);
	wut_stl_get(style, WUT_SHEET_PADDING_LEFT, &ret);
	padding[3] = wut_flx_process(ret.value.flex.pointer, refv);


	/*
	 * *********************************************************************
	 *
	 * 		BORDER
	 *
	 * *********************************************************************
	 */

	wut_stl_get(style, WUT_SHEET_BORDER_MODE, &ret);
	out->border_mode = ret.value.keyword.code;

	refv[0] = ref_width;
	wut_stl_get(style, WUT_SHEET_BORDER_WIDTH, &ret);
	out->border_width = wut_flx_process(ret.value.flex.pointer, refv);

	wut_stl_get(style, WUT_SHEET_BORDER_COLOR, &ret);
	out->border_color = wut_SetColorHex(ret.value.hexcode.code);

	/*
	 * *********************************************************************
	 *
	 * 		BOUNDING BOX
	 *
	 * *********************************************************************
	 */

	uref = spacing[1] + spacing[3] + padding[1] + padding[3];
	out->shape_bounding_box[2] = width + uref + (2 * out->border_width);

	uref = spacing[0] + spacing[2] + padding[0] + padding[2];
	out->shape_bounding_box[3] = height + uref + (2 * out->border_width);
	
	out->shape_bounding_box[0] = 0;
	out->shape_bounding_box[1] = 0;

	/*
	 * *********************************************************************
	 *
	 * 		ELEMENT DELTA
	 *
	 * *********************************************************************
	 */
	
	out->shape_element_delta[2] = -(spacing[1] + spacing[3]);
	out->shape_element_delta[3] = -(spacing[0] + spacing[2]);

	out->shape_element_delta[0] = spacing[3];
	out->shape_element_delta[1] = spacing[0];

	/*
	 * *********************************************************************
	 *
	 * 		INNER DELTA
	 *
	 * *********************************************************************
	 */

	out->shape_inner_delta[2] = out->shape_element_delta[2] - (2 * out->border_width);
	out->shape_inner_delta[3] = out->shape_element_delta[3] - (2 * out->border_width);

	out->shape_inner_delta[0] = out->shape_element_delta[0] + out->border_width;
	out->shape_inner_delta[1] = out->shape_element_delta[1] + out->border_width;

	/*
	 * *********************************************************************
	 *
	 * 		CONTENT DELTA
	 *
	 * *********************************************************************
	 */

	out->shape_content_delta[2] =
		out->shape_inner_delta[2] - padding[1] - padding[3];
	out->shape_content_delta[3] =
		out->shape_inner_delta[3] - padding[0] - padding[2];

	out->shape_content_delta[0] = out->shape_inner_delta[0] + padding[3];
	out->shape_content_delta[1] = out->shape_inner_delta[1] + padding[0];

	/*
	 * *********************************************************************
	 *
	 * 		DISPLAY
	 *
	 * *********************************************************************
	 */

	wut_stl_get(style, WUT_SHEET_DISPLAY_MODE, &ret);
	out->display_mode = ret.value.keyword.code;

	/*
	 * *********************************************************************
	 *
	 * 		REFERENCE
	 *
	 * *********************************************************************
	 */

	wut_stl_get(style, WUT_SHEET_REFERENCE_MODE, &ret);
	out->reference_mode = ret.value.keyword.code;

	/*
	 * *********************************************************************
	 *
	 * 		RADIUS
	 *
	 * *********************************************************************
	 */

	refv[0] = ref_width;
	wut_stl_get(style, WUT_SHEET_RADIUS_TOP_LEFT, &ret);
	out->radius_corner[0] = wut_flx_process(ret.value.flex.pointer, refv);
	wut_stl_get(style, WUT_SHEET_RADIUS_TOP_RIGHT, &ret);
	out->radius_corner[1] = wut_flx_process(ret.value.flex.pointer, refv);
	wut_stl_get(style, WUT_SHEET_RADIUS_BOTTOM_RIGHT, &ret);
	out->radius_corner[2] = wut_flx_process(ret.value.flex.pointer, refv);
	wut_stl_get(style, WUT_SHEET_RADIUS_BOTTOM_LEFT, &ret);
	out->radius_corner[3] = wut_flx_process(ret.value.flex.pointer, refv);


	/*
	 * *********************************************************************
	 *
	 * 		INFILL
	 *
	 * *********************************************************************
	 */

	wut_stl_get(style, WUT_SHEET_INFILL_MODE, &ret);
	out->infill_mode = ret.value.keyword.code;

	wut_stl_get(style, WUT_SHEET_INFILL_COLOR, &ret);
	out->infill_color = wut_SetColorHex(ret.value.hexcode.code);


	/*
	 * *********************************************************************
	 *
	 * 		LAYOUT
	 *
	 * *********************************************************************
	 */

	wut_stl_get(style, WUT_SHEET_LAYOUT_MODE, &ret);
	out->layout_mode = ret.value.keyword.code;

	/*
	 * *********************************************************************
	 *
	 * 		SCROLLBAR
	 *
	 * *********************************************************************
	 */

	wut_stl_get(style, WUT_SHEET_SCROLLBAR_MODE, &ret);

	out->scrollbar_flags = 0;
	switch(ret.value.keyword.code) {
		case WUT_KW_SCROLLBAR_AUTO:
			out->scrollbar_flags |= WUT_RESTYLE_SCROLL_V;
			out->scrollbar_flags |= WUT_RESTYLE_SCROLL_H;
			break;
		case WUT_KW_SCROLLBAR_NONE:
			break;
		case WUT_KW_SCROLLBAR_VERTICAL:
			out->scrollbar_flags |= WUT_RESTYLE_SCROLL_V;
			break;
		case WUT_KW_SCROLLBAR_HORIZONTAL:
			out->scrollbar_flags |= WUT_RESTYLE_SCROLL_H;
			break;
	}

	/*
	 * *********************************************************************
	 *
	 * 		TEXT
	 *
	 * *********************************************************************
	 */

	wut_stl_get(style, WUT_SHEET_TEXT_SIZE, &ret);
	out->text_size = wut_flx_process(ret.value.flex.pointer, refv);
	
	wut_stl_get(style, WUT_SHEET_TEXT_COLOR, &ret);
	out->text_color = wut_SetColorHex(ret.value.hexcode.code);

	wut_stl_get(style, WUT_SHEET_TEXT_MASS, &ret);
	out->text_mass = wut_flx_process(ret.value.flex.pointer, refv);

	wut_stl_get(style, WUT_SHEET_TEXT_OPTIONS, &ret);
	out->text_options = ret.value.keyword.code;

	wut_stl_get(style, WUT_SHEET_TEXT_WRAP_MODE, &ret);
	out->text_wrap_mode = ret.value.keyword.code;

	wut_stl_get(style, WUT_SHEET_TEXT_SPACING, &ret);
	out->text_spacing = 1;
	/* out->text_spacing = wut_flx_process(ret.value.flex.pointer, refv); */

	wut_stl_get(style, WUT_SHEET_LINE_HEIGHT, &ret);
	out->text_line_height = wut_flx_process(ret.value.flex.pointer, refv);

	return 0;
}


WUT_XMOD void wut_stl_add_classes(struct wut_Style *style, char *classes)
{
        wut_cls_add_names(&style->classes, classes);
}


WUT_XMOD void wut_stl_link_classes(struct wut_Style *style,
                struct wut_ClassTable *tbl)
{
        wut_cls_link(tbl, &style->classes);
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

WUT_API void wut_ResetStyle(struct wut_Style *style)
{
	if(!style) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

	style->ref = NULL;

	wut_sht_reset(&style->sheet);

        wut_cls_reset_references(&style->classes);
}


WUT_API void wut_ModifyStyle(struct wut_Style *style, char *in)
{
	if(!style || !in) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		return;
	}

	/*
	 * Parse the input expressions and modify the stylesheet accordingly.
	 */
	wut_sht_parse(&style->sheet, in);
}


WUT_API void wut_DumpStyle(struct wut_Style *style)
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
			style->shape_bounding_box[0],
			style->shape_bounding_box[1],
			style->shape_bounding_box[2],
			style->shape_bounding_box[3]);

	printf("%22s:\tx=%d, y=%d, w=%d, h=%d\n", "element_delta",
			style->shape_element_delta[0],
			style->shape_element_delta[1],
			style->shape_element_delta[2],
			style->shape_element_delta[3]);

	printf("%22s:\tx=%d, y=%d, w=%d, h=%d\n", "inner_delta",
			style->shape_inner_delta[0],
			style->shape_inner_delta[1],
			style->shape_inner_delta[2],
			style->shape_inner_delta[3]);

	printf("%22s:\tx=%d, y=%d, w=%d, h=%d\n", "content_delta",
			style->shape_content_delta[0],
			style->shape_content_delta[1],
			style->shape_content_delta[2],
			style->shape_content_delta[3]);


	/*
	 * BORDER
	 */
	printf("%22s:\tmode=%d, width=%d, color=%08X\n", "border", 
			style->border_mode,
			style->border_width,
			wut_GetColorU32(style->border_color));

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
			wut_GetColorU32(style->infill_color));

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
			wut_GetColorU32(style->text_color),
			style->text_mass,
			style->text_options);

	printf("%22s:\twrap_mode=%02x, spacing=%d, line_height=%d\n", "text2",
			style->text_wrap_mode,
			style->text_spacing,
			style->text_line_height);
}
