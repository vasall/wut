#include "style/inc/stylesheet_tables.h"

#include <stdlib.h>

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

const struct fh_sheet_attr fh_c_sheet_attribs[FH_SHEET_ATTRIBS] = {
	{0, 		      0,	  0},	/*  0 UNDEFINED		    */

	{FH_SHEET_KEYWORD,    0,	  4},	/*  1 display_mode 	    */
	
	{FH_SHEET_FLEX,       0,	  5},	/*  2 width 		    */
	{FH_SHEET_FLEX,       0,	 13},	/*  3 height 		    */
	
	{FH_SHEET_KEYWORD,    1,	 21},	/*  4 reference_mode 	    */
	
	{FH_SHEET_FLEX,       0,	 22},	/*  5 spacing_top 	    */
	{FH_SHEET_FLEX,       0,	 30},	/*  6 spacing_right 	    */
	{FH_SHEET_FLEX,       0,	 38},	/*  7 spacing_bottom 	    */
	{FH_SHEET_FLEX,       0,	 46},	/*  8 spacing_left 	    */
	
	{FH_SHEET_FLEX,       0,	 54},	/*  9 padding_top 	    */
	{FH_SHEET_FLEX,       0,	 62},	/* 10 padding_right 	    */
	{FH_SHEET_FLEX,       0,	 70},	/* 11 padding_bottom 	    */
	{FH_SHEET_FLEX,       0,	 78},	/* 12 padding_left 	    */
	
	{FH_SHEET_KEYWORD,    2,	 86},	/* 13 border_mode 	    */
	{FH_SHEET_FLEX,       0,	 87},	/* 14 border_width 	    */
	{FH_SHEET_HEXCODE,    0,	 95},	/* 15 border_color 	    */
	
	{FH_SHEET_FLEX,       0,	 99},	/* 16 radius_top_left 	    */
	{FH_SHEET_FLEX,       0,	107},	/* 17 radius_top_right 	    */
	{FH_SHEET_FLEX,       0,	115},	/* 18 radius_bottom_right   */
	{FH_SHEET_FLEX,       0,	123},	/* 19 radius_bottom_left    */
	
	{FH_SHEET_KEYWORD,    3,	131},	/* 20 infill_mode 	    */
	{FH_SHEET_HEXCODE,    0,	132},	/* 21 infill_color 	    */

	{FH_SHEET_KEYWORD,    4,	136},	/* 22 layout_mode 	    */
	
	{FH_SHEET_KEYWORD,    5,	137},	/* 23 align_v    	    */
	{FH_SHEET_KEYWORD,    6,	138},	/* 24 align_h   	    */
	
	{FH_SHEET_KEYWORD,    7,	149},	/* 25 scrollbar_mode 	    */
	
	{FH_SHEET_FLEX,       0,	140},	/* 26 text_size 	    */
	{FH_SHEET_HEXCODE,    0,	148},	/* 27 text_color 	    */
	{FH_SHEET_FLEX,       0,	152},	/* 28 text_mass 	    */
	{FH_SHEET_KEYWORD,    1,	160},	/* 29 text_options 	    */
	{FH_SHEET_KEYWORD,    8,	161},	/* 30 text_wrap_mode 	    */
};


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

struct fh_sheet_id_entry fh_c_sheet_attr_row0[] = {
	{0x98, FH_SHEET_HEIGHT},
	{0x68, FH_SHEET_SPACING_LEFT},
	{0xD8, FH_SHEET_BORDER_MODE},
	{0xB0, FH_SHEET_TEXT_SIZE},
	{0xE8, FH_SHEET_TEXT_OPTIONS}
};
struct fh_sheet_id_entry fh_c_sheet_attr_row1[] = {
	{0x89, FH_SHEET_DISPLAY_MODE},
	{0x41, FH_SHEET_PADDING_RIGHT},
	{0xF9, FH_SHEET_INFILL_COLOR},
	{0x19, FH_SHEET_TEXT_MASS}
};
struct fh_sheet_id_entry fh_c_sheet_attr_row2[] = {
	{0x02, FH_SHEET_SCROLLBAR_MODE}
};
struct fh_sheet_id_entry fh_c_sheet_attr_row3[] = {
	{0x3B, FH_SHEET_WIDTH},
	{0xFB, FH_SHEET_SPACING_TOP},
	{0x43, FH_SHEET_SPACING_RIGHT},
	{0x5B, FH_SHEET_PADDING_BOTTOM},
	{0x93, FH_SHEET_TEXT_COLOR},
	{0xB3, FH_SHEET_TEXT_WRAP_MODE}
};
struct fh_sheet_id_entry fh_c_sheet_attr_row4[] = {
	{0xB4, FH_SHEET_SPACING_BOTTOM},
	{0x2C, FH_SHEET_PADDING_TOP},
	{0x54, FH_SHEET_INFILL_MODE}
};
struct fh_sheet_id_entry fh_c_sheet_attr_row5[] = {
	{0x0D, FH_SHEET_ALIGN_V},
	{0x25, FH_SHEET_RADIUS_TOP_LEFT},
};
struct fh_sheet_id_entry fh_c_sheet_attr_row6[] = {
	{0x9E, FH_SHEET_PADDING_LEFT},
	{0x56, FH_SHEET_BORDER_WIDTH},
	{0xA6, FH_SHEET_RADIUS_TOP_RIGHT},
	{0x1E, FH_SHEET_RADIUS_BOTTOM_LEFT},
	{0xFE, FH_SHEET_ALIGN_H}
};
struct fh_sheet_id_entry fh_c_sheet_attr_row7[] = {
	{0x77, FH_SHEET_REFERENCE_MODE},
	{0x0F, FH_SHEET_BORDER_COLOR},
	{0x5F, FH_SHEET_RADIUS_BOTTOM_RIGHT},
	{0x3F, FH_SHEET_LAYOUT_MODE}
};

const struct fh_sheet_id_table fh_c_sheet_ids[FH_SHEET_ROWS] = {
	{fh_c_sheet_attr_row0, 5},
	{fh_c_sheet_attr_row1, 4},
	{fh_c_sheet_attr_row2, 1},
	{fh_c_sheet_attr_row3, 6},
	{fh_c_sheet_attr_row4, 3},
	{fh_c_sheet_attr_row5, 2},
	{fh_c_sheet_attr_row6, 5},
	{fh_c_sheet_attr_row7, 4}
};


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

struct fh_sheet_kw fh_c_sheet_kw_row0[] = {
	{"visible", 		FH_KW_DISPLAY_VISIBLE},
	{"hidden", 		FH_KW_DISPLAY_INVISIBLE}
};
struct fh_sheet_kw fh_c_sheet_kw_row1[] = {
	{"relative", 		FH_KW_REFERENCE_RELATIVE},
	{"absolute", 		FH_KW_REFERENCE_ABSOLUTE}
};
struct fh_sheet_kw fh_c_sheet_kw_row2[] = {
	{"none", 		FH_KW_BORDER_NONE},
	{"solid", 		FH_KW_BORDER_SOLID}
};
struct fh_sheet_kw fh_c_sheet_kw_row3[] = {
	{"color", 		FH_KW_INFILL_COLOR},
	{"none", 		FH_KW_INFILL_NONE}
};
struct fh_sheet_kw fh_c_sheet_kw_row4[] = {
	{"block", 		FH_KW_LAYOUT_BLOCK},
	{"row",			FH_KW_LAYOUT_ROW},
	{"column", 		FH_KW_LAYOUT_COLUMN}
};
struct fh_sheet_kw fh_c_sheet_kw_row5[] = {
	{"top", 		FH_KW_ALIGN_TOP},
	{"middle",		FH_KW_ALIGN_MIDDLE},
	{"bottom", 		FH_KW_ALIGN_BOTTOM}
};
struct fh_sheet_kw fh_c_sheet_kw_row6[] = {
	{"left", 		FH_KW_ALIGN_LEFT},
	{"middle",		FH_KW_ALIGN_MIDDLE},
	{"right", 		FH_KW_ALIGN_RIGHT}
};
struct fh_sheet_kw fh_c_sheet_kw_row7[] = {
	{"auto", 		FH_KW_SCROLLBAR_AUTO},
	{"none", 		FH_KW_SCROLLBAR_NONE},
	{"vertical", 		FH_KW_SCROLLBAR_VERTICAL},
	{"horizontal", 		FH_KW_SCROLLBAR_HORIZONTAL}
};
struct fh_sheet_kw fh_c_sheet_kw_row8[] = {
	{"underline",		FH_KW_TEXT_UNDERLINE}
};
struct fh_sheet_kw fh_c_sheet_kw_row9[] = {
	{"wordwrap",		FH_KW_TEXT_WORDWRAP},
	{"nowrap",		FH_KW_TEXT_NOWRAP},
	{"letterwrap",		FH_KW_TEXT_LETTERWRAP}
};

const struct fh_sheet_kw_table fh_c_sheet_keywords[FH_SHEET_KW_ROWS] = {
	{fh_c_sheet_kw_row0, 2},
	{fh_c_sheet_kw_row1, 2},
	{fh_c_sheet_kw_row2, 3},
	{fh_c_sheet_kw_row3, 3},
	{fh_c_sheet_kw_row4, 4},
	{fh_c_sheet_kw_row5, 3},
	{fh_c_sheet_kw_row6, 3},
	{fh_c_sheet_kw_row7, 4},
	{fh_c_sheet_kw_row8, 1},
	{fh_c_sheet_kw_row9, 3}
};

