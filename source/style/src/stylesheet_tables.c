#include "style/inc/stylesheet_tables.h"

#include <stdlib.h>

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

const struct wt_sheet_attr wt_c_sheet_attribs[WT_SHEET_ATTRIBS] = {
	{0, 		      0,	  0},	/*  0 UNDEFINED		    */

	{WT_SHEET_KEYWORD,    0,	  8},	/*  1 display_mode 	    */
	
	{WT_SHEET_FLEX,       0,	  9},	/*  2 width 		    */
	{WT_SHEET_FLEX,       0,	 17},	/*  3 height 		    */
	
	{WT_SHEET_KEYWORD,    1,	 25},	/*  4 reference_mode 	    */
	
	{WT_SHEET_FLEX,       0,	 26},	/*  5 spacing_top 	    */
	{WT_SHEET_FLEX,       0,	 34},	/*  6 spacing_right 	    */
	{WT_SHEET_FLEX,       0,	 42},	/*  7 spacing_bottom 	    */
	{WT_SHEET_FLEX,       0,	 50},	/*  8 spacing_left 	    */
	
	{WT_SHEET_FLEX,       0,	 58},	/*  9 padding_top 	    */
	{WT_SHEET_FLEX,       0,	 66},	/* 10 padding_right 	    */
	{WT_SHEET_FLEX,       0,	 74},	/* 11 padding_bottom 	    */
	{WT_SHEET_FLEX,       0,	 82},	/* 12 padding_left 	    */
	
	{WT_SHEET_KEYWORD,    2,	 90},	/* 13 border_mode 	    */
	{WT_SHEET_FLEX,       0,	 91},	/* 14 border_width 	    */
	{WT_SHEET_HEXCODE,    0,	 99},	/* 15 border_color 	    */
	
	{WT_SHEET_FLEX,       0,	103},	/* 16 radius_top_left 	    */
	{WT_SHEET_FLEX,       0,	111},	/* 17 radius_top_right 	    */
	{WT_SHEET_FLEX,       0,	119},	/* 18 radius_bottom_right   */
	{WT_SHEET_FLEX,       0,	127},	/* 19 radius_bottom_left    */
	
	{WT_SHEET_KEYWORD,    3,	135},	/* 20 infill_mode 	    */
	{WT_SHEET_HEXCODE,    0,	136},	/* 21 infill_color 	    */

	{WT_SHEET_KEYWORD,    4,	140},	/* 22 layout_mode 	    */
	
	{WT_SHEET_KEYWORD,    5,	141},	/* 23 align_v    	    */
	{WT_SHEET_KEYWORD,    6,	142},	/* 24 align_h   	    */
	
	{WT_SHEET_KEYWORD,    7,	143},	/* 25 scrollbar_mode 	    */

	{WT_SHEET_FLEX,       0,	144},	/* 26 text_size 	    */
	{WT_SHEET_HEXCODE,    0,	152},	/* 27 text_color 	    */
	{WT_SHEET_FLEX,       0,	156},	/* 28 text_mass 	    */
	{WT_SHEET_KEYWORD,    8,	164},	/* 29 text_options 	    */
	{WT_SHEET_KEYWORD,    9,	165},	/* 30 text_wrap_mode 	    */
	{WT_SHEET_FLEX,	      0,	166},	/* 31 text_spacing          */
	{WT_SHEET_FLEX,	      0,	174}	/* 32 line_height 	    */
};


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

struct wt_sheet_id_entry wt_c_sheet_attr_row0[] = {
	{0x98, WT_SHEET_HEIGHT},
	{0x68, WT_SHEET_SPACING_LEFT},
	{0xD8, WT_SHEET_BORDER_MODE},
	{0xB0, WT_SHEET_TEXT_SIZE},
	{0xE8, WT_SHEET_TEXT_OPTIONS}
};
struct wt_sheet_id_entry wt_c_sheet_attr_row1[] = {
	{0x89, WT_SHEET_DISPLAY_MODE},
	{0x41, WT_SHEET_PADDING_RIGHT},
	{0xF9, WT_SHEET_INFILL_COLOR},
	{0x19, WT_SHEET_TEXT_MASS},
	{0x01, WT_SHEET_TEXT_SPACING}
};
struct wt_sheet_id_entry wt_c_sheet_attr_row2[] = {
	{0x02, WT_SHEET_SCROLLBAR_MODE},
	{0xD2, WT_SHEET_LINE_HEIGHT}
};
struct wt_sheet_id_entry wt_c_sheet_attr_row3[] = {
	{0x3B, WT_SHEET_WIDTH},
	{0xFB, WT_SHEET_SPACING_TOP},
	{0x43, WT_SHEET_SPACING_RIGHT},
	{0x5B, WT_SHEET_PADDING_BOTTOM},
	{0x93, WT_SHEET_TEXT_COLOR},
	{0xB3, WT_SHEET_TEXT_WRAP_MODE}
};
struct wt_sheet_id_entry wt_c_sheet_attr_row4[] = {
	{0xB4, WT_SHEET_SPACING_BOTTOM},
	{0x2C, WT_SHEET_PADDING_TOP},
	{0x54, WT_SHEET_INFILL_MODE}
};
struct wt_sheet_id_entry wt_c_sheet_attr_row5[] = {
	{0x0D, WT_SHEET_ALIGN_V},
	{0x25, WT_SHEET_RADIUS_TOP_LEFT},
};
struct wt_sheet_id_entry wt_c_sheet_attr_row6[] = {
	{0x9E, WT_SHEET_PADDING_LEFT},
	{0x56, WT_SHEET_BORDER_WIDTH},
	{0xA6, WT_SHEET_RADIUS_TOP_RIGHT},
	{0x1E, WT_SHEET_RADIUS_BOTTOM_LEFT},
	{0xFE, WT_SHEET_ALIGN_H}
};
struct wt_sheet_id_entry wt_c_sheet_attr_row7[] = {
	{0x77, WT_SHEET_REFERENCE_MODE},
	{0x0F, WT_SHEET_BORDER_COLOR},
	{0x5F, WT_SHEET_RADIUS_BOTTOM_RIGHT},
	{0x3F, WT_SHEET_LAYOUT_MODE}
};

const struct wt_sheet_id_table wt_c_sheet_ids[WT_SHEET_ROWS] = {
	{wt_c_sheet_attr_row0, 5},
	{wt_c_sheet_attr_row1, 5},
	{wt_c_sheet_attr_row2, 2},
	{wt_c_sheet_attr_row3, 6},
	{wt_c_sheet_attr_row4, 3},
	{wt_c_sheet_attr_row5, 2},
	{wt_c_sheet_attr_row6, 5},
	{wt_c_sheet_attr_row7, 4}
};


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

struct wt_sheet_kw wt_c_sheet_kw_row0[] = {
	{"visible", 		WT_KW_DISPLAY_VISIBLE},
	{"hidden", 		WT_KW_DISPLAY_INVISIBLE}
};
struct wt_sheet_kw wt_c_sheet_kw_row1[] = {
	{"relative", 		WT_KW_REFERENCE_RELATIVE},
	{"absolute", 		WT_KW_REFERENCE_ABSOLUTE}
};
struct wt_sheet_kw wt_c_sheet_kw_row2[] = {
	{"none", 		WT_KW_BORDER_NONE},
	{"solid", 		WT_KW_BORDER_SOLID}
};
struct wt_sheet_kw wt_c_sheet_kw_row3[] = {
	{"color", 		WT_KW_INFILL_COLOR},
	{"none", 		WT_KW_INFILL_NONE}
};
struct wt_sheet_kw wt_c_sheet_kw_row4[] = {
	{"block", 		WT_KW_LAYOUT_BLOCK},
	{"row",			WT_KW_LAYOUT_ROW},
	{"column", 		WT_KW_LAYOUT_COLUMN}
};
struct wt_sheet_kw wt_c_sheet_kw_row5[] = {
	{"top", 		WT_KW_ALIGN_TOP},
	{"middle",		WT_KW_ALIGN_MIDDLE},
	{"bottom", 		WT_KW_ALIGN_BOTTOM}
};
struct wt_sheet_kw wt_c_sheet_kw_row6[] = {
	{"left", 		WT_KW_ALIGN_LEFT},
	{"middle",		WT_KW_ALIGN_MIDDLE},
	{"right", 		WT_KW_ALIGN_RIGHT}
};
struct wt_sheet_kw wt_c_sheet_kw_row7[] = {
	{"auto", 		WT_KW_SCROLLBAR_AUTO},
	{"none", 		WT_KW_SCROLLBAR_NONE},
	{"vertical", 		WT_KW_SCROLLBAR_VERTICAL},
	{"horizontal", 		WT_KW_SCROLLBAR_HORIZONTAL}
};
struct wt_sheet_kw wt_c_sheet_kw_row8[] = {
	{"underline",		WT_KW_TEXT_UNDERLINE}
};
struct wt_sheet_kw wt_c_sheet_kw_row9[] = {
	{"wordwrap",		WT_KW_TEXT_WORDWRAP},
	{"nowrap",		WT_KW_TEXT_NOWRAP},
	{"letterwrap",		WT_KW_TEXT_LETTERWRAP}
};

const struct wt_sheet_kw_table wt_c_sheet_keywords[WT_SHEET_KW_ROWS] = {
	{wt_c_sheet_kw_row0, 2},
	{wt_c_sheet_kw_row1, 2},
	{wt_c_sheet_kw_row2, 3},
	{wt_c_sheet_kw_row3, 3},
	{wt_c_sheet_kw_row4, 4},
	{wt_c_sheet_kw_row5, 3},
	{wt_c_sheet_kw_row6, 3},
	{wt_c_sheet_kw_row7, 4},
	{wt_c_sheet_kw_row8, 1},
	{wt_c_sheet_kw_row9, 3}
};

