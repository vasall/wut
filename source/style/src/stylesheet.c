#include "style/inc/stylesheet.h"

#include "style/inc/stylesheet_tables.h"

#include "system/inc/system.h"

#include "utility/alarm/inc/alarm.h"

#include <stdlib.h>


FH_INTERN void sheet_sanitize(char *s_in, char *s_out)
{
	u8 i;

	for(i = 0; i < strlen(s_in); i++) {
		s_out[i] = s_in[i];

		if(s_in[i] >= 0x41 && s_in[i] <= 0x5A) {
			s_out[i] += 0x20;
		}
	}
	s_out[i] = 0;
}

FH_INTERN s8 sheet_isspace(char c)
{
	return c == 0x20;
}


FH_INTERN s8 sheet_isletter(char c)
{
	if(c >= 0x61 && c <= 0x7A)
		return 1;

	return 0;
}


FH_INTERN s8 sheet_ishex(char c)
{
	/* Numbers */
	if(c >= 0x30 && c <= 0x39)
		return 1;

	/* Letters */
	return sheet_isletter(c);
}


FH_INTERN s16 sheet_offset(enum fh_sheet_id id)
{
	return fh_c_sheet_attribs[id].offset;
}


FH_INTERN s8 sheet_sizeof(enum fh_sheet_id id)
{
	switch(fh_c_sheet_attribs[id].type) {
		case FH_SHEET_FLEX:	return sizeof(fh_flex_t);
		case FH_SHEET_HEXCODE:	return U32_S;
		case FH_SHEET_KEYWORD:	return U8_S;
	}
	return 0;
}


FH_INTERN fh_keyword_t sheet_typeof(enum fh_sheet_id id)
{
	return fh_c_sheet_attribs[id].type;
}


FH_INTERN void sheet_read(struct fh_stylesheet *sheet, enum fh_sheet_id id,
		void *ptr)
{
	memcpy(ptr, ((u8 *)sheet) + sheet_offset(id), sheet_sizeof(id));
}


FH_INTERN void sheet_write(struct fh_stylesheet *sheet, enum fh_sheet_id id,
		void *ptr)
{		
	memcpy(((u8 *)sheet) + sheet_offset(id), ptr, sheet_sizeof(id));
}


FH_INTERN u8 sheet_category(enum fh_sheet_id id)
{
	return fh_c_sheet_attribs[id].category;
}


/*
 * This function will get the next key-value-pair from the input and extract
 * both of them. After that it will set the pointer to the end of the read part,
 * so it can be called again to extract the next key-value-pair.
 *
 * @s: The input string
 * @attr: A pointer to write the attribute string to
 * @val: A pointer to write the value string to
 *
 * Returns: The updated pointer or NULL if an error occurred
 */
FH_INTERN char *sheet_next(char *s, char *attr, char *val)
{
	char *run;
	char c;
	char *ptr;

	run = s;

	/* Extract the attribute string */
	ptr = attr;
	while((c = *(run++)) != ':') {
		if(c == 0)
			return NULL;

		/* Skip */
		if(c == '\n' || sheet_isspace(c))
			continue;

		*(ptr++) = c;
	}
	*(ptr) = 0;

	/* Extract the value string */
	ptr = val;
	while((c = *(run++)) != ';') {
		if(c == 0)
			return NULL;

		/* Skip */
		if(c == '\n')
			continue;

		*(ptr++) = c;
	}
	*(ptr) = 0;

	return run;
}

FH_INTERN u8 sheet_hash(char *in)
{
        u64 hash = 5381;
        s32 c;

        while ((c = *in++))
                hash = ((hash << 5) + hash) + c; /* hash *+ c */

        return hash % 0xff;
}


FH_INTERN enum fh_sheet_id sheet_get_id(char *s)
{
	u8 hash;
	u8 row;
	u8 i;

	hash = sheet_hash(s);
	row = hash %  FH_SHEET_ROWS;

	for(i = 0; i < fh_c_sheet_ids[row].number; i++) {
		if(fh_c_sheet_ids[row].entries[i].hash == hash) {
			return fh_c_sheet_ids[row].entries[i].id;
		}
	}

	return FH_SHEET_UNDEFINED;
}


FH_INTERN void sheet_parse_flex(struct fh_stylesheet *sheet, enum fh_sheet_id id,
		char *val)
{
	fh_flex_t flx = FH_FLEX_UNDEF;

	sheet_read(sheet, id, &flx);

	flx = fh_flex_parse(flx, val);

	sheet_write(sheet, id, &flx);
}


FH_INTERN u8 sheet_fromhex(char c)
{
	/* Small letters */
	if(c >= 0x61)
		return (c - 0x61) + 10;

	/* Big letters */
	if(c >= 0x41)
		return (c - 0x41) + 10;

	/* Numbers */
	return c - 0x30;	
}


FH_INTERN s8 sheet_convhex(char *val, u32 *out)
{
	s32 i;
	char *run;
	char c;
	char *head = NULL;

	u32 buf = 0x000000FF;

	run = val;

	/* Find the beginning of the hex */
	while((c = *run)) {
		if(sheet_ishex(c)) {
			head = run;
			break;
		}
		run++;
	}
	if(!head)
		return -1;

	for(i = 2; i >= 0; i--) {
		if(!sheet_ishex(head[i*2])) {
			return 0;
		}

		if(!sheet_ishex(head[i*2+1])) {
			return -1;
		}

		((u8 *)&buf)[3 - i] = sheet_fromhex(head[i*2]) * 16;
		((u8 *)&buf)[3 - i] += sheet_fromhex(head[i*2+1]);
	}

	*out = buf;

	return 0;
}


FH_INTERN void sheet_parse_hexcode(struct fh_stylesheet *sheet,
		enum fh_sheet_id id, char *val)
{
	u32 hex;

	sheet_convhex(val, &hex);

	sheet_write(sheet, id, &hex);
}


FH_INTERN char *sheet_next_keyword(char *s, char *buf)
{
	char *run = s;
	char *head = buf;

	/* If everything has been read, return NULL */
	if(*run == 0x00)
		return NULL;

	/* Remove leading spaces */
	while(sheet_isspace(*run))
		run++;

	/* Read the text */
	while(sheet_isletter(*run)) {
		*(head++) = *(run++);
	}
	*head = 0;

	return run;
}


FH_INTERN fh_keyword_t sheet_find_keyword(u8 ctg, char *val)
{
	s8 i;

	for(i = 0; i < fh_c_sheet_keywords[ctg].number; i++) {
		if(!strcmp(fh_c_sheet_keywords[ctg].entries[i].string, val)) {
			return fh_c_sheet_keywords[ctg].entries[i].value;
		}
	}

	return FH_KW_UNDEFINED;	
}


FH_INTERN void sheet_parse_keyword(struct fh_stylesheet *sheet,
		enum fh_sheet_id id, char *val)
{
	fh_keyword_t kw = FH_KW_UNDEFINED;
	u8 ctg;
	
	char *ptr;
	char buf[127];

	ctg = sheet_category(id);

	/* First process the input and set the according keyword flags */
	ptr = val;
	while((ptr = sheet_next_keyword(ptr, buf))) {
		kw |= sheet_find_keyword(ctg, buf);
	}

	sheet_write(sheet, id, &kw);
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-FUNCTIONS
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */



FH_XMOD void fh_sheet_reset(struct fh_stylesheet *sheet)
{
	if(!sheet) return;

	sheet->mask = 0;

	sheet->display_mode = 0;

	sheet->width = FH_FLEX_UNDEF;
	sheet->height = FH_FLEX_UNDEF;

	sheet->reference_mode = 0;

	sheet->spacing_top = FH_FLEX_UNDEF;
	sheet->spacing_right = FH_FLEX_UNDEF;
	sheet->spacing_bottom = FH_FLEX_UNDEF;
	sheet->spacing_left = FH_FLEX_UNDEF;

	sheet->padding_top = FH_FLEX_UNDEF;
	sheet->padding_right = FH_FLEX_UNDEF;
	sheet->padding_bottom = FH_FLEX_UNDEF;
	sheet->padding_left = FH_FLEX_UNDEF;

	sheet->border_mode = 0;
	sheet->border_width = FH_FLEX_UNDEF;
	sheet->border_color = fh_color_set(0, 0, 0, 1);

	sheet->radius_top_left = FH_FLEX_UNDEF;
	sheet->radius_top_right = FH_FLEX_UNDEF;
	sheet->radius_bottom_right = FH_FLEX_UNDEF;
	sheet->radius_bottom_left = FH_FLEX_UNDEF;

	sheet->infill_mode = 0;
	sheet->infill_color = fh_color_set(0, 0, 0, 1);

	sheet->layout_mode = 0;

	sheet->align_v = 0;
	sheet->align_h = 0;

	sheet->scrollbar_mode = 0;

	sheet->text_size = FH_FLEX_UNDEF;
	sheet->text_color = fh_color_set(0, 0, 0, 1);
	sheet->text_mass = FH_FLEX_UNDEF;
	sheet->text_options = 0;
	sheet->text_wrap_mode = FH_KW_TEXT_WORDWRAP;
}


FH_XMOD void fh_sheet_parse(struct fh_stylesheet *sheet, char *s)
{
	char *run;
	char inp_s[106];
	char attr_s[64];
	char val_s[64];

	enum fh_sheet_id id;

	if(!sheet || !s) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		return;
	}

	/*
	 * Sanitize the input string to make it conform with input requirements.
	 * This will for example convert letters from big(A..Z) to small(a..z).
	 */
	sheet_sanitize(s, inp_s);

	run = inp_s;
	while((run = sheet_next(run, attr_s, val_s))) {
		if((id = sheet_get_id(attr_s)) == FH_SHEET_UNDEFINED)
			continue;

		switch(sheet_typeof(id)) {
			case FH_SHEET_FLEX:
				sheet_parse_flex(sheet, id, val_s);
				break;
			case FH_SHEET_HEXCODE:
				sheet_parse_hexcode(sheet, id, val_s);
				break;
			case FH_SHEET_KEYWORD:
				sheet_parse_keyword(sheet, id, val_s);
				break;
		}

		/* Don't forget to add the flag to the mask */
		sheet->mask |= (1<<id);
	}
}



FH_XMOD s8 fh_sheet_get(struct fh_stylesheet *sheet, enum fh_sheet_id id,
		struct fh_sheet_ret *ret)
{
	if(!sheet || !ret)
		return -1;

	if(!(sheet->mask & (1<<id))) {
		return 0;
	}

	switch(sheet_typeof(id)) {
		case FH_SHEET_FLEX:
			sheet_read(sheet, id, &ret->flex);
			ret->type = FH_SHEET_FLEX;
			break;
		case FH_SHEET_HEXCODE:
			sheet_read(sheet, id, &ret->hexcode);
			ret->type = FH_SHEET_HEXCODE;
			break;
		case FH_SHEET_KEYWORD:
			sheet_read(sheet, id, &ret->keyword);
			ret->type = FH_SHEET_KEYWORD;
			break;
	}

	return 1;
}


FH_XMOD void fh_sheet_print(struct fh_stylesheet *sheet)
{
	if(!sheet)
		return;

	printf("Mask: %08x", sheet->mask);

	printf("\ndisplay_mode: %02x", sheet->display_mode);


	printf("\nwidth: "); fh_flex_print(sheet->width);
	printf("\nheight: "); fh_flex_print(sheet->height);

	printf("\nspacing_top: "); fh_flex_print(sheet->spacing_top);
	printf("\nspacing_right: "); fh_flex_print(sheet->spacing_right);
	printf("\nspacing_bottom: "); fh_flex_print(sheet->spacing_bottom);
	printf("\nspacing_left: "); fh_flex_print(sheet->spacing_left);

	printf("\npadding_top: "); fh_flex_print(sheet->padding_top);
	printf("\npadding_right: "); fh_flex_print(sheet->padding_right);
	printf("\npadding_bottom: "); fh_flex_print(sheet->padding_bottom);
	printf("\npadding_left: "); fh_flex_print(sheet->padding_left);

	printf("\nborder_mode: %02x", sheet->border_mode);
	printf("\nborder_width: "); fh_flex_print(sheet->border_width);
	printf("\nborder_color: %08x", fh_color_get(sheet->border_color));

	printf("\nradius_top_left: ");
	fh_flex_print(sheet->radius_top_left);
	printf("\nradius_top_right: ");
	fh_flex_print(sheet->radius_top_right);
	printf("\nradius_bottom_right: ");
	fh_flex_print(sheet->radius_bottom_right);
	printf("\nradius_bottom_left: ");
	fh_flex_print(sheet->radius_bottom_left);

	printf("\ninfill_mode: %02x", sheet->infill_mode);
	printf("\ninfill_color: %08x", fh_color_get(sheet->infill_color));

	printf("\nlayout_mode: %02x", sheet->layout_mode);

	printf("\nalign_v: %02x", sheet->align_v);
	printf("\nalign_h: %02x", sheet->align_h);

	printf("\nscrollbar_mode: %02x", sheet->scrollbar_mode);

	printf("\ntext_size: "); fh_flex_print(sheet->text_size);
	printf("\ntext_color: %08x", fh_color_get(sheet->text_color));
	printf("\ntext_mass: "); fh_flex_print(sheet->text_mass);
	printf("\ntext_options: %02x", sheet->text_options);
	printf("\ntext_wrap_mode: %02x", sheet->text_wrap_mode);
}
