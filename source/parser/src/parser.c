#include "parser/inc/parser.h"

#include "utils/inc/utils.h"

#include "style/inc/stylesheet_tables.h"



FH_INTERN s8 parser_check_hex(char c)
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

FH_INTERN u8 parser_hex_to_int(char c)
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


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-FUNCTIONS
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

FH_XMOD s8 fh_parser_decimal(char *in, u8 ctg, u8 *out)
{
	s32 i;
	char c;
	s32 len = 0;
	char *start = NULL;
	u8 off;
	s32 del;

	*out = 0;

	fh_Ignore(ctg);

	/* Find the beginning of the integer */
	while((c = *in)) {
		if(c >= 0x30 && c <= 0x39) {
			start = in;
			break;
		}
		in++;
	}
	if(!start)
		return -1;

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

	return 1;
}


FH_XMOD s8 fh_parser_percent(char *in, u8 ctg, u8 *out)
{
	s32 i;
	char c;
	s32 len = 0;
	char *start = NULL;
	f32 off;
	f32 del;
	f32 *out_ptr = (f32 *)out;

	fh_Ignore(ctg);

	*out_ptr = 0;

	/* Find the beginning of the float */
	while((c = *in)) {
		if(c >= 0x30 && c <= 0x39) {
			start = in;
			break;
		}
		in++;
	}
	if(!start)
		return -1;

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
		return 4;
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

	return 4;
}


FH_XMOD s8 fh_parser_hexcode(char *in, u8 ctg, u8 *out)
{
	s32 i;
	char c;
	char *start = NULL;
	s32 off = 3;

	fh_Ignore(ctg);

	*((u32 *)out) = 0x000000FF;

	/* Find the beginning of the hex */
	while((c = *in)) {
		if(parser_check_hex(c)) {
			start = in;
			break;
		}
		in++;
	}
	if(!start)
		return -1;

	for(i = 0; i < 7; i += 2) {
		if(!parser_check_hex(start[i]))
			return 0;

		if(!parser_check_hex(start[i+1]))
			return -1;

		out[off] = parser_hex_to_int(start[i]) * 16;
		out[off] += parser_hex_to_int(start[i+1]);

		off -= 1;
	}

	return 4;
	
}


FH_XMOD s8 fh_parser_keyword(char *in, u8 ctg, u8 *out)
{
	s8 i;

	for(i = 0; i < fh_c_stylesheet_kv[ctg].number;  i++) {
		if(!strcmp(fh_c_stylesheet_kv[ctg].entries[i].string, in)) {
			*out = fh_c_stylesheet_kv[ctg].entries[i].value;
			return 1;
		}
	}
	

	return -1;
}


FH_XMOD s8 fh_parser_value(char *in, u8 type, u8 ctg, u8 *out)
{
	switch(type) {
		case FH_STYLE_INPUT_DECIMAL:
			return fh_parser_decimal(in, ctg, out);

		case FH_STYLE_INPUT_PERCENT:
			return fh_parser_percent(in, ctg, out);

		case FH_STYLE_INPUT_HEXCODE:
			return fh_parser_hexcode(in, ctg, out);

		case FH_STYLE_INPUT_KEYWORD:
			return fh_parser_keyword(in, ctg, out);
	}

	return -1;
}


FH_XMOD char *fh_parser_attribute(char *in, char *attr, char *val)
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
