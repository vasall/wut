#include "style.h"

#include "system.h"

#include <stdlib.h>


#define STYLE_DEBUG 0


FH_INTERN char *style_parse(char *in, char *attr, char *val)
{
	char *runner;
	char c;

	if(!in)
		return NULL;

	runner = in;

	while((c = *(runner++)) != ':') {
		/* ERROR */
		if(c == 0)
			return NULL;
	
		/* SKIP */
		if(c == ' ' || c == '\n')
			continue;
			
		*(attr++) = c;
	}
	*(attr) = 0;

	while((c = *(runner++)) != ';') {
		/* ERROR */
		if(c == 0)
			return NULL;

		/* SKIP */
		if(c == ' ' || c == '\n')
			continue;

		*(val++) = c; 
	}
	*(val) = 0;

	return runner;	
}


FH_INTERN u16 style_hash(char *in)
{
	u64 hash = 5381;
	s32 c;

	while ((c = *in++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash % 0xffff;	
}


FH_INTERN s8 style_get_offset(char *attrib, u16 *off)
{
	u16 hash = style_hash(attrib);
	u16 row = hash % 16;
	u8 i;

	for(i = 0; i < fh_c_stylesheet_hm[row].number; i++) {
		if(fh_c_stylesheet_hm[row].entries[i].hash == hash) {
			*off = fh_c_stylesheet_hm[row].entries[i].offset;
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

#if STYLE_DEBUG
	(ref) ? printf("Ref given\n") : printf("No ref\n");
#endif

	/*
	 * DISPLAY
	 */
	out->display.mode = in->display_mode;
	

	/* 
	 * CALCULATE SIZE
	 */

	/* height */
	uref = ref ? ref->inner_shape.h : (u16)pass->document_shape->h;
	height = fh_flex_comp(&in->vsize, uref);


	/* width */
	uref = ref ? ref->inner_shape.w : (u16)pass->document_shape->w;
	width = fh_flex_comp(&in->hsize, uref);

#if STYLE_DEBUG
	printf("[SIZE] w=%d, h=%d\n", width, height);
#endif

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

#if STYLE_DEBUG
	printf("[SPACING] top=%d, right=%d, bottom=%d, left=%d\n",
			spacing[0], spacing[1], spacing[2], spacing[3]);
#endif

	/*
	 * BOUNDING SHAPE
	 */

	/* size */
	out->bounding_shape.h = height;
	out->bounding_shape.w = width;

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

	padding[0] = fh_flex_comp(&in->padding_top, uref);
	padding[2] = fh_flex_comp(&in->padding_bottom, uref);

	/* horizontal */
	uref = out->shape.w;

	padding[3] = fh_flex_comp(&in->padding_left, uref);
	padding[1] = fh_flex_comp(&in->padding_right, uref);

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
	style->sheet.reference_mode = FH_REFERENCE_RELATIVE;

	/*
	 * SIZE
	 */
	style->sheet.vsize = fh_flex_set(FH_FLEX_RELATIVE, 10000);
	style->sheet.hsize = fh_flex_set(FH_FLEX_RELATIVE, 10000);

	/*
	 * SPACING
	 */
	style->sheet.spacing_top = fh_flex_set(FH_FLEX_ABSOLUTE, 0);
	style->sheet.spacing_right = fh_flex_set(FH_FLEX_ABSOLUTE, 0);
	style->sheet.spacing_bottom = fh_flex_set(FH_FLEX_ABSOLUTE, 0);
	style->sheet.spacing_left = fh_flex_set(FH_FLEX_ABSOLUTE, 0);

	/*
	 * PADDING 
	 */
	style->sheet.padding_top = fh_flex_set(FH_FLEX_ABSOLUTE, 0);
	style->sheet.padding_right = fh_flex_set(FH_FLEX_ABSOLUTE, 0);
	style->sheet.padding_bottom = fh_flex_set(FH_FLEX_ABSOLUTE, 0);
	style->sheet.padding_left = fh_flex_set(FH_FLEX_ABSOLUTE, 0);

	/*
	 * INFILL
	 */
	style->sheet.infill_mode = FH_INFILL_COLOR;
	style->sheet.infill_color =  fh_col_set(0xFF, 0x69, 0xB4, 0xFF);

	/*
	 * LAYOUT
	 */
	style->sheet.layout_mode = FH_LAYOUT_BLOCKS;

	/*
	 * SCROLLBAR
	 */
	style->sheet.scrollbar_mode = FH_SCROLLBAR_VERTICAL;
}


FH_API void fh_ModifyStyle(struct fh_style *style, char *in)
{
	char *swap;
	char attr[32];
	char val[32];
	u16 hash;

	if(!style || !in) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		return;
	}

	swap = in;

	while(!(swap = style_parse(swap, attr, attr))) {
		hash = style_hash(attr);	
	}

}
