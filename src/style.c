#include "style.h"

#include "system.h"
#include "stylesheet_tables.h"

#include <stdlib.h>


#define STYLE_DEBUG 1


FH_INTERN char *style_parse(char *in, char *attr, char *val)
{
	char *runner;
	char c;

	char *ptr;

	if(!in)
		return NULL;

	runner = in;
	ptr = attr;

	while((c = *(runner++)) != ':') {
		/* ERROR */
		if(c == 0)
			return NULL;
	
		/* SKIP */
		if(c == ' ' || c == '\n')
			continue;
			
		*(ptr++) = c;
	}
	*(ptr) = 0;

	ptr = val;

	while((c = *(runner++)) != ';') {
		/* ERROR */
		if(c == 0) {
			return NULL;
		}

		/* SKIP */
		if(c == ' ' || c == '\n')
			continue;

		*(ptr++) = c; 
	}
	*(ptr) = 0;

	return runner;	
}


FH_INTERN u8 style_hash(char *in)
{
	u64 hash = 5381;
	s32 c;

	while ((c = *in++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash % 0xff;	
}


FH_INTERN s8 style_find(char *attrib, u8 *off, u8 *size, u8 *input, u8 *cat)
{
	u8 hash = style_hash(attrib);
	u16 row = hash % 8;
	u8 i;

	for(i = 0; i < fh_c_stylesheet_hm[row].number; i++) {
		if(fh_c_stylesheet_hm[row].entries[i].hash == hash) {
			*off = fh_c_stylesheet_hm[row].entries[i].offset;
			*size = fh_c_stylesheet_hm[row].entries[i].size;
			*input = fh_c_stylesheet_hm[row].entries[i].input;
			*cat = fh_c_stylesheet_hm[row].entries[i].category;
			return 0;
		}
	}

	return -1;
}


FH_INTERN s8 style_parse_dec(char *in, u8 cat, u8 *out)
{
	s32 i;
	char c;
	s32 len = 0;
	char *start = NULL;
	u8 off;
	s32 del;

	*out = 0;

	fh_Ignore(cat);

	/* Find the beginning of the integer */
	while((c = *in)) {
		if(c >= 0x30 && c <= 0x39) {
			start = in;
			break;
		}
		in++;
	}
	if(!start) return -1;

	while((c = *(in++))) {
		if(!(c >= 0x30 && c <= 0x39)) {
			break;
		}
		len++;
	}

	off = 1;
	for(i = 1; i <= len; i++) {
		del = (start[len-i]-0x30)*off;

		*out += del;
		off *= 10;
	}

	return 0;
}


FH_INTERN s8 style_parse_pct(char *in, u8 cat, u8 *out)
{
	s32 i;
	char c;
	s32 len = 0;
	char *start = NULL;
	f32 off;
	f32 del;
	f32 *out_ptr = (f32 *)out;

	fh_Ignore(cat);

	*out_ptr = 0;

	/* Find the beginning of the float */
	while((c = *in)) {
		if(c >= 0x30 && c <= 0x39) {
			start = in;
			break;
		}
		in++;
	}
	if(!start) return -1;

	while((c = *(in++))) {
		if(!(c >= 0x30 && c <= 0x39)) {
			break;
		}
		len++;
	}

	/* Process the font */
	off = 1;
	for(i = 1; i <= len; i++) {
		del = (start[len-i]-0x30)*off;

		*out_ptr += del;
		off *= 10;
	}

	if(c != '.') {
		*out_ptr /= 100.0;
		return 0;
	}

	len = 0;
	start = in;

	while((c = *(in++))) {
		if(!(c >= 0x30 && c <= 0x39)) {
			break;
		}
		len++;
	}

	/* Process the exponent */
	off = 10;
	for(i = 0; i < len; i++) {
		del = ((f32)(start[i]-0x30))/off;

		*out_ptr += del;
		off *= 10;
	}

	*out_ptr /= 100.0;

	return 0;
	
}


FH_INTERN s8 style_check_hex(char c)
{
	/* NUMBERS */
	if(c >= 0x30 && c <= 0x39)
		return 1;

	/* BIG CHARACTERS */
	if(c >= 0x41 && c <= 0x46)
		return 1;

	/* SMALL CHARACTERS */
	if(c >= 0x61 && c <= 0x66)
		return 1;

	return 0;
}

FH_INTERN u8 style_hex_to_int(char c)
{
	/* SMALL CHARACTERS */
	if(c >= 0x61)
		return (c - 0x61)+10;

	/* BIG CHARACTERS */
	if(c >= 0x41)
		return (c - 0x41)+10;

	/* NUMBERS */
	return c - 0x30;
	
}

FH_INTERN s8 style_parse_hex(char *in, u8 cat, u8 *out)
{
	s32 i;
	char c;
	char *start = NULL;
	s32 off = 3;

	fh_Ignore(cat);

	*((u32 *)out) = 0x000000FF;

	/* Find the beginning of the hex */
	while((c = *in)) {
		if(style_check_hex(c)) {
			start = in;
			break;
		}
		in++;
	}
	if(!start) return -1;

	for(i = 0; i < 7; i += 2) {
		if(!style_check_hex(start[i]))
			return 0;

		if(!style_check_hex(start[i+1]))
			return -1;

		out[off] = style_hex_to_int(start[i]) * 16;
		out[off] += style_hex_to_int(start[i+1]);

		off -= 1;
	}

	return 0;
	
}


FH_INTERN s8 style_parse_opt(char *in, u8 cat, u8 *out)
{
	s8 i;

	for(i = 0; i < fh_c_stylesheet_kv[cat].number;  i++) {
		if(!strcmp(fh_c_stylesheet_kv[cat].entries[i].string, in)) {
			*out = fh_c_stylesheet_kv[cat].entries[i].value;
			return 0;
		}
	}
	

	return -1;
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-FUNCTIONS
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

FH_API s8 fh_style_init(struct fh_style *style, struct fh_style *ref)
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


FH_API s8 fh_style_process(struct fh_style *style, struct fh_style_pass *pass)
{
	u32 uref;

	u32 height;
	u32 width;

	u32 spacing[4];
	u32 padding[4];

	struct fh_style *ref;
	struct fh_style *out;

	ref = style->ref;
	out = style;

#if STYLE_DEBUG
	(ref) ? printf("Ref given\n") : printf("No ref\n");
#endif

	/*
	 * DISPLAY
	 */
	out->display.mode = style->sheet.display_mode;

	/*
	 * REFERENCE
	 */
	out->reference.mode = style->sheet.reference_mode;

	/* 
	 * CALCULATE SIZE
	 */

	/* height */
	uref = ref ? ref->inner_shape.h : (u16)pass->document_shape->h;
	height = uref * style->sheet.vsize;


	/* width */
	uref = ref ? ref->inner_shape.w : (u16)pass->document_shape->w;
	width = uref * style->sheet.hsize;

#if STYLE_DEBUG
	printf("[SIZE] w=%d, h=%d\n", width, height);
#endif

	/*
	 * SPACING
	 */

	/* vertical */
	uref = ref ? ref->inner_shape.h : (u16)pass->document_shape->h;

	spacing[0] = uref * style->sheet.spacing_top;
	spacing[2] = uref * style->sheet.spacing_bottom;

	/* horizontal */
	uref = ref ? ref->inner_shape.w : (u16)pass->document_shape->w;

	spacing[3] = uref * style->sheet.spacing_left;
	spacing[1] = uref * style->sheet.spacing_right;

#if STYLE_DEBUG
	printf("[SPACING] top=%d, right=%d, bottom=%d, left=%d\n",
			spacing[0], spacing[1], spacing[2], spacing[3]);
#endif

	/*
	 * BOUNDING SHAPE
	 */

	/* size */
	out->bounding_shape.h = height + spacing[0] + spacing[2];
	out->bounding_shape.w = width + spacing[1] + spacing[3];

	/* position */
	out->bounding_shape.y = 0;
	out->bounding_shape.x = 0;

#if STYLE_DEBUG
	printf("[BOUNDING_SHAPE] x=%d, y=%d, w=%d, h=%d\n",
			out->bounding_shape.x, out->bounding_shape.y, 
			out->bounding_shape.w, out->bounding_shape.h);
#endif

	/*
	 * SHAPE
	 */

	/* size */
	out->shape.h = -(spacing[0] + spacing[2]);
	out->shape.w = -(spacing[1] + spacing[3]);

	/* position */
	out->shape.y = spacing[0];
	out->shape.x = spacing[3];

#if STYLE_DEBUG
	printf("[SHAPE] x=%d, y=%d, w=%d, h=%d\n",
			out->shape.x, out->shape.y, out->shape.w, out->shape.h);
#endif

	/*
	 * PADDING
	 */

	/* vertical */
	uref = out->shape.h;

	padding[0] = uref * style->sheet.padding_top;
	padding[2] = uref * style->sheet.padding_bottom;

	/* horizontal */
	uref = out->shape.w;

	padding[3] = uref * style->sheet.padding_left;
	padding[1] = uref * style->sheet.padding_right;

#if STYLE_DEBUG
	printf("[PADDING] top=%d, right=%d, bottom=%d, left=%d\n",
			padding[0], padding[1], padding[2], padding[3]);
#endif

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

#if STYLE_DEBUG
	printf("[INNER_SHAPE] x=%d, y=%d, w=%d, h=%d\n",
			out->inner_shape.x, out->inner_shape.y, 
			out->inner_shape.w, out->inner_shape.h);
#endif

	/*
	 * INFILL
	 */

	out->infill.mode = style->sheet.infill_mode;
	out->infill.color = fh_col_conv_itos(style->sheet.infill_color);

	/*
	 * LAYOUT
	 */

	out->layout.mode = style->sheet.layout_mode;


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
	style->sheet.infill_color =  fh_col_set_u32(0xFF, 0x69, 0xB4, 0xFF);

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

	u8 off;
	u8 size;
	u8 inp;
	u8 ctg;

	u8 out[4];

	if(!style || !in) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		return;
	}

	swap = in;

	/*
	 * Go through all stylesheet inputs.
	 */
	while((swap = style_parse(swap, attr, value))) {
		/*
		 * Validate that the requested attribute exists.
		 *
		 */
		if(style_find(attr, &off, &size, &inp, &ctg) < 0) {
			printf("Attribute \"%s\" not found\n", attr);
			continue;
		}

		/*
		 * Parse the given value to the contraints of the requested
		 * attribute.
		 */
		switch(inp) {
			case FH_STYLE_INPUT_DEC: 
				if(style_parse_dec(value, ctg, out) < 0) {
					ALARM(ALARM_ERR, "value not dec");
					break;
				}
				break;
			case FH_STYLE_INPUT_PCT: 
				if(style_parse_pct(value, ctg, out) < 0) {
					ALARM(ALARM_ERR, "value not flt");
					break;
				}
				break;
			case FH_STYLE_INPUT_HEX:
				if(style_parse_hex(value, ctg, out) < 0) {
					ALARM(ALARM_ERR, "value not hex");
					break;
				}
				break;
			case FH_STYLE_INPUT_OPT:
				if(style_parse_opt(value, ctg, out) < 0) {
					ALARM(ALARM_ERR, "value not opt");
					break;
				}
				break;
		}

		/* Finally write the new data to the stylesheet */
		memcpy(((u8 *)&style->sheet) + off, out, size);
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

	printf("%22s:\tx=%d, y=%d, w=%d, h=%d\n", "bounding_shape",
			style->bounding_shape.x, style->bounding_shape.y,
			style->bounding_shape.w, style->bounding_shape.h);

	printf("%22s:\tx=%d, y=%d, w=%d, h=%d\n", "shape",
			style->shape.x, style->shape.y,
			style->shape.w, style->shape.h);

	printf("%22s:\tx=%d, y=%d, w=%d, h=%d\n", "inner_shape",
			style->inner_shape.x, style->inner_shape.y,
			style->inner_shape.w, style->inner_shape.h);

	printf("%22s:\tmode=%d, color=%08X\n", "infill", style->infill.mode,
			fh_color_get(style->infill.color));

	printf("%22s:\tmode=%d\n", "layout", style->layout.mode);
}
