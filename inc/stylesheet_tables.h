#ifndef _FH_STYLESHEET_TABLES_H
#define _FH_STYLESHEET_TABLES_H

#include "stdinc.h"
#include "stylesheet.h"


struct fh_stylesheet_hme {
	u16 hash;
	u16 offset;
	u8 size;
	u8 input;
	u8 category;
};


struct fh_stylesheet_hm {
	struct fh_stylesheet_hme *entries;
	u8 number;
};

static struct fh_stylesheet_hme fh_c_stylesheet_hm_row0[] = {
	{0xf8,  2, 4, FH_STYLE_INPUT_PCT, 0},
	{0x68, 24, 4, FH_STYLE_INPUT_PCT, 0},
	{0xd8, 44, 1, FH_STYLE_INPUT_OPT, 4}

};
static struct fh_stylesheet_hme fh_c_stylesheet_hm_row1[] = {
	{0x89,  0, 1, FH_STYLE_INPUT_OPT, 0},
	{0x41, 32, 4, FH_STYLE_INPUT_PCT, 0},
	{0xf9, 55, 4, FH_STYLE_INPUT_HEX, 0}

};
static struct fh_stylesheet_hme fh_c_stylesheet_hm_row2[] = {
	{0x02, 60, 1, FH_STYLE_INPUT_OPT, 7}
};
static struct fh_stylesheet_hme fh_c_stylesheet_hm_row3[] = {
	{0xfb, 12, 4, FH_STYLE_INPUT_PCT, 0},
	{0x43, 16, 4, FH_STYLE_INPUT_PCT, 0},
	{0x5b, 36, 4, FH_STYLE_INPUT_PCT, 0}
};
static struct fh_stylesheet_hme fh_c_stylesheet_hm_row4[] = {
	{0xb4, 20, 4, FH_STYLE_INPUT_PCT, 0},
	{0x2c, 28, 4, FH_STYLE_INPUT_PCT, 0},
	{0x54, 54, 1, FH_STYLE_INPUT_OPT, 5},
	{0x34, 61, 4, FH_STYLE_INPUT_OPT, 0}
};
static struct fh_stylesheet_hme fh_c_stylesheet_hm_row5[] = {
	{0x95,  6, 4, FH_STYLE_INPUT_PCT, 0},
	{0x25, 50, 1, FH_STYLE_INPUT_DEC, 0}
};
static struct fh_stylesheet_hme fh_c_stylesheet_hm_row6[] = {
	{0x9e, 40, 4, FH_STYLE_INPUT_PCT, 0},
	{0x56, 45, 1, FH_STYLE_INPUT_DEC, 0},
	{0xa6, 51, 1, FH_STYLE_INPUT_DEC, 0},
	{0x1e, 53, 1, FH_STYLE_INPUT_DEC, 0},
	{0x2e, 65, 4, FH_STYLE_INPUT_OPT, 0}
};
static struct fh_stylesheet_hme fh_c_stylesheet_hm_row7[] = {
	{0x77,  1, 1, FH_STYLE_INPUT_OPT, 1},
	{0x8f, 10, 1, FH_STYLE_INPUT_OPT, 2},
	{0xbf, 11, 1, FH_STYLE_INPUT_OPT, 3},
	{0x0f, 46, 4, FH_STYLE_INPUT_HEX, 0},
	{0x5f, 52, 1, FH_STYLE_INPUT_DEC, 0},
	{0x3f, 59, 1, FH_STYLE_INPUT_OPT, 6}
};

static struct fh_stylesheet_hm fh_c_stylesheet_hm[] = {
	{fh_c_stylesheet_hm_row0, 3},
	{fh_c_stylesheet_hm_row1, 3},
	{fh_c_stylesheet_hm_row2, 1},
	{fh_c_stylesheet_hm_row3, 3},
	{fh_c_stylesheet_hm_row4, 4},
	{fh_c_stylesheet_hm_row5, 2},
	{fh_c_stylesheet_hm_row6, 5},
	{fh_c_stylesheet_hm_row7, 6}
};


/*
 * THE OPTION-STINGS USED FOR OPTION-BASED INPUTS.
 */

struct fh_stylesheet_kve {
	char *string;
	u8 value;
};

struct fh_stylesheet_kv {
	struct fh_stylesheet_kve *entries;
	u8 number;
};

static struct fh_stylesheet_kve fh_c_stylesheet_kv_row0[] = {
	{"FH_DISPLAY_VISIBLE", 		FH_DISPLAY_VISIBLE},
	{"FH_DISPLAY_INVISIBLE", 	FH_DISPLAY_INVISIBLE}
};
static struct fh_stylesheet_kve fh_c_stylesheet_kv_row1[] = {
	{"FH_REFERENCE_AUTO", 		FH_REFERENCE_AUTO},
	{"FH_REFERENCE_ABSOLUTE", 	FH_REFERENCE_ABSOLUTE}
};
static struct fh_stylesheet_kve fh_c_stylesheet_kv_row2[] = {
	{"FH_ALIGNMENT_INHERIT",	FH_ALIGNMENT_INHERIT},
	{"FH_ALIGNMENT_TOP", 		FH_ALIGNMENT_TOP},
	{"FH_ALIGNMENT_BOTTOM", 	FH_ALIGNMENT_BOTTOM}
};
static struct fh_stylesheet_kve fh_c_stylesheet_kv_row3[] = {
	{"FH_ALIGNMENT_INHERIT", 	FH_ALIGNMENT_INHERIT},
	{"FH_ALIGNMENT_LEFT", 		FH_ALIGNMENT_LEFT},
	{"FH_ALIGNMENT_RIGHT", 		FH_ALIGNMENT_RIGHT}
};
static struct fh_stylesheet_kve fh_c_stylesheet_kv_row4[] = {
	{"FH_BORDER_INHERIT", 		FH_BORDER_INHERIT},
	{"FH_BORDER_NONE", 		FH_BORDER_NONE},
	{"FH_BORDER_SOLID", 		FH_BORDER_SOLID}
};
static struct fh_stylesheet_kve fh_c_stylesheet_kv_row5[] = {
	{"FH_INFILL_INHERIT", 		FH_INFILL_INHERIT},
	{"FH_INFILL_NONE", 		FH_INFILL_NONE},
	{"FH_INFILL_COLOR", 		FH_INFILL_COLOR}
};
static struct fh_stylesheet_kve fh_c_stylesheet_kv_row6[] = {
	{"FH_LAYOUT_INHERIT", 		FH_LAYOUT_INHERIT},
	{"FH_LAYOUT_BLOCKS", 		FH_LAYOUT_BLOCKS},
	{"FH_LAYOUT_ROWS",		FH_LAYOUT_ROWS},
	{"FH_LAYOUT_COLUMNS", 		FH_LAYOUT_COLUMNS}
};
static struct fh_stylesheet_kve fh_c_stylesheet_kv_row7[] = {
	{"FH_SCROLLBAR_AUTO", 		FH_SCROLLBAR_AUTO},
	{"FH_SCROLLBAR_NONE", 		FH_SCROLLBAR_NONE},
	{"FH_SCROLLBAR_VERTICAL", 	FH_SCROLLBAR_VERTICAL},
	{"FH_SCROLLBAR_HORIZONTAL", 	FH_SCROLLBAR_HORIZONTAL}
};
static struct fh_stylesheet_kv fh_c_stylesheet_kv[] = {
	{fh_c_stylesheet_kv_row0, 2},
	{fh_c_stylesheet_kv_row1, 2},
	{fh_c_stylesheet_kv_row2, 3},
	{fh_c_stylesheet_kv_row3, 3},
	{fh_c_stylesheet_kv_row4, 3},
	{fh_c_stylesheet_kv_row5, 3},
	{fh_c_stylesheet_kv_row6, 4},
	{fh_c_stylesheet_kv_row7, 4}
};


#endif /* _FH_STYLESHEET_TABLES_H */
