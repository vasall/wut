#include "style/inc/style.h"

#include "style/inc/style_utils.h"

#include "system/inc/system.h"

#include "parser/inc/parser.h"



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
		ALARM(ALARM_ERR, "Input parameters invalid");
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
	ALARM(ALARM_ERR, "Failed to initialize style structure");
	return -1;
}


FH_XMOD s8 fh_style_link(struct fh_style *style, struct fh_style *ref)
{
	style->ref = ref;

	return 0;
}


FH_XMOD s8 fh_style_process(struct fh_style *style, struct fh_style_pass *pass)
{
	u32 uref;

	u32 ref_height;
	u32 ref_width;

	u32 height;
	u32 width;

	u32 spacing[4];
	u32 padding[4];

	struct fh_style *ref;
	struct fh_style *out;

	ref = style->ref;
	out = style;

	/*
	 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	 *
	 * DISPLAY
	 */
	
	out->display.mode = style->sheet.display_mode;

	/*
	 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	 *
	 * REFERENCE
	 */

	out->reference.mode = style->sheet.reference_mode;

	
	/*
	 * CALCULATE REFERENCE SIZE
	 */

	/* height */

	if(ref)
		ref_height = ref->bounding_box.h + ref->content_delta.h;
	else
		ref_height = (u16)pass->document_shape->h;

	/* width */

	if(ref)
		ref_width = ref->bounding_box.w + ref->content_delta.w;
	else
		ref_width = (u16)pass->document_shape->w;


	/* 
	 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	 *
	 * SIZE
	 */

	/* height */
	height = ref_height * style->sheet.vsize;

	/* width */
	width = ref_width * style->sheet.hsize;

	/*
	 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	 *
	 * SPACING
	 */

	/* vertical */
	uref = ref_height;

	spacing[0] = uref * style->sheet.spacing_top;
	spacing[2] = uref * style->sheet.spacing_bottom;

	/* horizontal */
	uref = ref_width;

	spacing[3] = uref * style->sheet.spacing_left;
	spacing[1] = uref * style->sheet.spacing_right;

	/*
	 * BOUNDING SHAPE
	 */

	/* size */
	out->bounding_box.h = height + spacing[0] + spacing[2];
	out->bounding_box.w = width + spacing[1] + spacing[3];

	/* position */
	out->bounding_box.y = 0;
	out->bounding_box.x = 0;

	/*
	 * SHAPE
	 */

	/* size */
	out->element_delta.h = -(spacing[0] + spacing[2]);
	out->element_delta.w = -(spacing[1] + spacing[3]);

	/* position */
	out->element_delta.y = spacing[0];
	out->element_delta.x = spacing[3];

	/*
	 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	 *
	 * PADDING
	 */

	/* vertical */
	uref = height;

	padding[0] = uref * style->sheet.padding_top;
	padding[2] = uref * style->sheet.padding_bottom;

	/* horizontal */
	uref = width;

	padding[3] = uref * style->sheet.padding_left;
	padding[1] = uref * style->sheet.padding_right;

	/*
	 * INNER SHAPE
	 */
	
	/* size */
	uref = padding[0] + padding[2];
	out->content_delta.h = out->element_delta.h - uref; 

	uref = padding[1] + padding[3];
	out->content_delta.w = out->element_delta.w - uref;

	/* position */
	out->content_delta.y = out->element_delta.y + padding[0];
	out->content_delta.x = out->element_delta.x + padding[3];
	
	/*
	 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	 *
	 * BORDER
	 */
	
	out->border.mode = style->sheet.border_mode;
	out->border.width = style->sheet.border_width;
	out->border.color = fh_col_conv_itos(style->sheet.border_color);

	/*
	 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	 *
	 * RADIUS
	 */

	out->radius.corner[0] = style->sheet.radius_top_left;
	out->radius.corner[1] = style->sheet.radius_top_right;
	out->radius.corner[2] = style->sheet.radius_bottom_right;
	out->radius.corner[3] = style->sheet.radius_bottom_left;

	/*
	 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	 *
	 * INFILL
	 */

	out->infill.mode = style->sheet.infill_mode;
	out->infill.color = fh_col_conv_itos(style->sheet.infill_color);

	/*
	 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	 *
	 * LAYOUT
	 */

	out->layout.mode = style->sheet.layout_mode;

	/*
	 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	 *
	 * SCROLLBAR
	 */
	out->scrollbar.flags = 0;

	switch(style->sheet.scrollbar_mode) {
		case FH_SCROLLBAR_AUTO:
			out->scrollbar.flags |= FH_RESTYLE_SCROLL_V;
			out->scrollbar.flags |= FH_RESTYLE_SCROLL_H;
			break;
		case FH_SCROLLBAR_NONE:
			break;
		case FH_SCROLLBAR_VERTICAL:
			out->scrollbar.flags |= FH_RESTYLE_SCROLL_V;
			break;
		case FH_SCROLLBAR_HORIZONTAL:
			out->scrollbar.flags |= FH_RESTYLE_SCROLL_H;
			break;
	}

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
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	/* 
	 * DISPLAY
	 */
	style->sheet.display_mode = FH_DISPLAY_VISIBLE;	

	/*
	 * REFERENCE
	 */
	style->sheet.reference_mode = FH_REFERENCE_AUTO;

	/*
	 * SIZE
	 */
	style->sheet.vsize = 1;
	style->sheet.hsize = 1;

	/*
	 * SPACING
	 */
	style->sheet.spacing_top = 0;
	style->sheet.spacing_right = 0;
	style->sheet.spacing_bottom = 0;
	style->sheet.spacing_left = 0;

	/*
	 * PADDING 
	 */
	style->sheet.padding_top = 0;
	style->sheet.padding_right = 0;
	style->sheet.padding_bottom = 0;
	style->sheet.padding_left = 0;

	/*
	 * INFILL
	 */
	style->sheet.infill_mode = FH_INFILL_COLOR;
	style->sheet.infill_color =  fh_col_set_u32(0xB0, 0x0B, 0x1E, 0xFF);

	/*
	 * LAYOUT
	 */
	style->sheet.layout_mode = FH_LAYOUT_BLOCKS;

	/*
	 * SCROLLBAR
	 */
	style->sheet.scrollbar_mode = FH_SCROLLBAR_AUTO;
}


FH_API void fh_ModifyStyle(struct fh_style *style, char *in)
{
	char *swap;
	char attr[32];
	char value[32];

	u8 out[4];

	s8 idx;
	const struct fh_stylesheet_attribute *ent;

	if(!style || !in) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		return;
	}

	swap = in;

	/*
	 * Go through all stylesheet inputs.
	 */
	while((swap = fh_parser_attribute(swap, attr, value))) {	
		/*
		 * Get the index of the stylesheet attribute.
		 */
		if((idx = fh_style_utl_find(attr)) < 0) {
			printf("Attribute \"%s\" not found\n", attr);
			continue;
		}

		/*
		 * Get the according entry from the stylesheet table.
		 */
		if(!(ent = fh_style_utl_get(idx))) {
			printf("No entry in table for \"%s\"\n", attr);
			continue;
		}

		/*
		 * Parse the given value to the contraints of the requested
		 * attribute.
		 */
		if(fh_parser_value(value, ent->input, ent->category, out) < 0) {
			printf("Input type is invalid");
			continue;
		}

		/* Finally write the new data to the stylesheet */
		memcpy(((u8 *)&style->sheet) + ent->offset, out, ent->size);
	}
}


FH_API void fh_DumpStylesheet(struct fh_style *style)
{
	if(!style) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	printf("%22s\t%d\n", "display_mode", style->sheet.display_mode);
	
	printf("\n");
	printf("%22s:\t%d\n", "reference_mode", style->sheet.reference_mode);
	
	printf("%22s:\t%.2f\n", "vsize", style->sheet.vsize);
	printf("%22s:\t%.2f\n", "hsize", style->sheet.hsize);

	printf("\n");
	printf("%22s:\t%d\n", "valignment", style->sheet.valignment);
	printf("%22s:\t%d\n", "halignment", style->sheet.halignment);

	printf("\n");
	printf("%22s:\t%.2f\n", "spacing_top", style->sheet.spacing_top);
	printf("%22s:\t%.2f\n", "spacing_right", style->sheet.spacing_right);
	printf("%22s:\t%.2f\n", "spacing_bottom", style->sheet.spacing_bottom);
	printf("%22s:\t%.2f\n", "spacing_left", style->sheet.spacing_left);

	printf("\n");
	printf("%22s:\t%.2f\n", "padding_top", style->sheet.padding_top);
	printf("%22s:\t%.2f\n", "padding_right", style->sheet.padding_right);
	printf("%22s:\t%.2f\n", "padding_bottom", style->sheet.padding_bottom);
	printf("%22s:\t%.2f\n", "padding_left", style->sheet.padding_left);

	printf("\n");
	printf("%22s:\t%d\n", "border_mode", style->sheet.border_mode);
	printf("%22s:\t%d\n", "border_width", style->sheet.border_width);
	printf("%22s:\t%08X\n", "border_color", style->sheet.border_color);

	printf("\n");
	printf("%22s:\t%d\n", "radius_top_left", style->sheet.radius_top_left);
	printf("%22s:\t%d\n", "radius_top_right", style->sheet.radius_top_right);
	printf("%22s:\t%d\n", "radius_bottom_left", style->sheet.radius_bottom_left);
	printf("%22s:\t%d\n", "radius_bottom_right", style->sheet.radius_bottom_right);

	printf("\n");
	printf("%22s:\t%d\n", "infill_mode", style->sheet.infill_mode);
	printf("%22s:\t%08X\n", "infill_color", style->sheet.infill_color);

	printf("\n");
	printf("%22s:\t%d\n", "layout_mode", style->sheet.layout_mode);

	printf("\n");
	printf("%22s:\t%d\n", "scrollbar_mode", style->sheet.scrollbar_mode);
	printf("%22s:\t%08X\n", "scrollbar_track_color", style->sheet.scrollbar_track_color);
	printf("%22s:\t%08X\n", "scrollbar_thumb_color", style->sheet.scrollbar_thumb_color);
}


FH_API void fh_DumpStyle(struct fh_style *style)
{
	printf("%22s:\tmode=%d\n", "display", style->display.mode);

	printf("%22s:\tmode=%d\n", "reference", style->reference.mode);

	printf("%22s:\tx=%d, y=%d, w=%d, h=%d\n", "bounding_box",
			style->bounding_box.x, style->bounding_box.y,
			style->bounding_box.w, style->bounding_box.h);

	printf("%22s:\tx=%d, y=%d, w=%d, h=%d\n", "element_delta",
			style->element_delta.x, style->element_delta.y,
			style->element_delta.w, style->element_delta.h);

	printf("%22s:\tx=%d, y=%d, w=%d, h=%d\n", "content_delta",
			style->content_delta.x, style->content_delta.y,
			style->content_delta.w, style->content_delta.h);

	printf("%22s:\tmode=%d, color=%08X\n", "infill", style->infill.mode,
			fh_color_get(style->infill.color));

	printf("%22s:\tmode=%d\n", "layout", style->layout.mode);
}
