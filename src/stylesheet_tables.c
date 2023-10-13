#include "stylesheet_tables.h"

#include <stdlib.h>

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

const struct fh_stylesheet_attribute fh_stylesheet_attributes[27] = {
	{ 0, 1, FH_STYLE_INPUT_KEYWORD, 0},	/*  0 display_mode 	    */
	{ 1, 1, FH_STYLE_INPUT_KEYWORD, 1},	/*  1 reference_mode 	    */
	{ 2, 4, FH_STYLE_INPUT_PERCENT, 0},	/*  2 vsize 		    */
	{ 6, 4, FH_STYLE_INPUT_PERCENT, 0},	/*  3 hsize 		    */
	{10, 1, FH_STYLE_INPUT_KEYWORD, 2},	/*  4 valignment 	    */
	{11, 1, FH_STYLE_INPUT_KEYWORD, 3},	/*  5 halignment 	    */
	{12, 4, FH_STYLE_INPUT_PERCENT, 0},	/*  6 spacing_top 	    */
	{16, 4, FH_STYLE_INPUT_PERCENT, 0},	/*  7 spacing_right 	    */
	{20, 4, FH_STYLE_INPUT_PERCENT, 0},	/*  8 spacing_bottom 	    */
	{24, 4, FH_STYLE_INPUT_PERCENT, 0},	/*  9 spacing_left 	    */
	{28, 4, FH_STYLE_INPUT_PERCENT, 0},	/* 10 padding_top 	    */
	{32, 4, FH_STYLE_INPUT_PERCENT, 0},	/* 11 padding_right 	    */
	{36, 4, FH_STYLE_INPUT_PERCENT, 0},	/* 12 padding_bottom 	    */
	{40, 4, FH_STYLE_INPUT_PERCENT, 0},	/* 13 padding_left 	    */
	{44, 1, FH_STYLE_INPUT_KEYWORD, 4},	/* 14 border_mode 	    */
	{45, 1, FH_STYLE_INPUT_DECIMAL, 0},	/* 15 border_width 	    */
	{46, 4, FH_STYLE_INPUT_HEXCODE, 0},	/* 16 border_color 	    */
	{50, 1, FH_STYLE_INPUT_DECIMAL, 0},	/* 17 radius_top_left 	    */
	{51, 1, FH_STYLE_INPUT_DECIMAL, 0},	/* 18 radius_top_right 	    */
	{52, 1, FH_STYLE_INPUT_DECIMAL, 0},	/* 19 radius_bottom_right   */
	{53, 1, FH_STYLE_INPUT_DECIMAL, 0},	/* 20 radius_bottom_left    */
	{54, 1, FH_STYLE_INPUT_KEYWORD, 5},	/* 21 infill_mode 	    */
	{55, 4, FH_STYLE_INPUT_HEXCODE, 0},	/* 22 infill_color 	    */
	{59, 1, FH_STYLE_INPUT_KEYWORD, 6},	/* 23 layout_mode 	    */
	{60, 1, FH_STYLE_INPUT_KEYWORD, 7},	/* 24 scrollbar_mode 	    */
	{61, 4, FH_STYLE_INPUT_HEXCODE, 0},	/* 25 scrollbar_track_color */
	{65, 4, FH_STYLE_INPUT_HEXCODE, 0}	/* 26 scrollbar_thumb_color */
};


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

struct fh_stylesheet_hme fh_c_stylesheet_hm_row0[] = {
	{0xf8,  2},	/* vsize */
	{0x68,  9},	/* spacing_left */
	{0xd8, 14}	/* border_mode */

};
struct fh_stylesheet_hme fh_c_stylesheet_hm_row1[] = {
	{0x89,  0},	/* display_mode */
	{0x41, 11},	/* padding_right */
	{0xf9, 22}	/* infill_color */

};
struct fh_stylesheet_hme fh_c_stylesheet_hm_row2[] = {
	{0x02, 24}	/* scrollbar_mode */
};
struct fh_stylesheet_hme fh_c_stylesheet_hm_row3[] = {
	{0xfb,  6},	/* spacing_top */
	{0x43,  7},	/* spacing_right */
	{0x5b, 12}	/* padding_bottom */
};
struct fh_stylesheet_hme fh_c_stylesheet_hm_row4[] = {
	{0xb4,  8},	/* spacing_bottom */
	{0x2c, 10},	/* padding_top */
	{0x54, 21},	/* infill_mode */
	{0x34, 25}	/* scrollbar_track_color */
};
struct fh_stylesheet_hme fh_c_stylesheet_hm_row5[] = {
	{0x95,  3},	/* hsize */
	{0x25, 17}	/* radius_top_left */
};
struct fh_stylesheet_hme fh_c_stylesheet_hm_row6[] = {
	{0x9e, 13},	/* padding_left */
	{0x56, 15},	/* border_width */
	{0xa6, 18},	/* radius_top_right */
	{0x1e, 20},	/* radius_bottom_left */
	{0x2e, 25}	/* scrollbar_thumb_color */
};
struct fh_stylesheet_hme fh_c_stylesheet_hm_row7[] = {
	{0x77,  1},	/* reference_mode */
	{0x8f,  4},	/* valignment */
	{0xbf,  5},	/* halignment */
	{0x0f, 16},	/* border_color */
	{0x5f, 19},	/* radius_bottom_right */
	{0x3f, 23}	/* layout_mode */
};

const struct fh_stylesheet_hm fh_c_stylesheet_hm[FH_STYLESHEET_HASH_ROWS] = {
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
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

struct fh_stylesheet_kve fh_c_stylesheet_kv_row0[] = {
	{"FH_DISPLAY_VISIBLE", 		FH_DISPLAY_VISIBLE},
	{"FH_DISPLAY_INVISIBLE", 	FH_DISPLAY_INVISIBLE}
};
struct fh_stylesheet_kve fh_c_stylesheet_kv_row1[] = {
	{"FH_REFERENCE_AUTO", 		FH_REFERENCE_AUTO},
	{"FH_REFERENCE_ABSOLUTE", 	FH_REFERENCE_ABSOLUTE}
};
struct fh_stylesheet_kve fh_c_stylesheet_kv_row2[] = {
	{"FH_ALIGNMENT_INHERIT",	FH_ALIGNMENT_INHERIT},
	{"FH_ALIGNMENT_TOP", 		FH_ALIGNMENT_TOP},
	{"FH_ALIGNMENT_BOTTOM", 	FH_ALIGNMENT_BOTTOM}
};
struct fh_stylesheet_kve fh_c_stylesheet_kv_row3[] = {
	{"FH_ALIGNMENT_INHERIT", 	FH_ALIGNMENT_INHERIT},
	{"FH_ALIGNMENT_LEFT", 		FH_ALIGNMENT_LEFT},
	{"FH_ALIGNMENT_RIGHT", 		FH_ALIGNMENT_RIGHT}
};
struct fh_stylesheet_kve fh_c_stylesheet_kv_row4[] = {
	{"FH_BORDER_INHERIT", 		FH_BORDER_INHERIT},
	{"FH_BORDER_NONE", 		FH_BORDER_NONE},
	{"FH_BORDER_SOLID", 		FH_BORDER_SOLID}
};
struct fh_stylesheet_kve fh_c_stylesheet_kv_row5[] = {
	{"FH_INFILL_INHERIT", 		FH_INFILL_INHERIT},
	{"FH_INFILL_NONE", 		FH_INFILL_NONE},
	{"FH_INFILL_COLOR", 		FH_INFILL_COLOR}
};
struct fh_stylesheet_kve fh_c_stylesheet_kv_row6[] = {
	{"FH_LAYOUT_INHERIT", 		FH_LAYOUT_INHERIT},
	{"FH_LAYOUT_BLOCKS", 		FH_LAYOUT_BLOCKS},
	{"FH_LAYOUT_ROWS",		FH_LAYOUT_ROWS},
	{"FH_LAYOUT_COLUMNS", 		FH_LAYOUT_COLUMNS}
};
struct fh_stylesheet_kve fh_c_stylesheet_kv_row7[] = {
	{"FH_SCROLLBAR_AUTO", 		FH_SCROLLBAR_AUTO},
	{"FH_SCROLLBAR_NONE", 		FH_SCROLLBAR_NONE},
	{"FH_SCROLLBAR_VERTICAL", 	FH_SCROLLBAR_VERTICAL},
	{"FH_SCROLLBAR_HORIZONTAL", 	FH_SCROLLBAR_HORIZONTAL}
};


const struct fh_stylesheet_kv fh_c_stylesheet_kv[FH_STYLESHEET_OPT_ROWS] = {
	{fh_c_stylesheet_kv_row0, 2},
	{fh_c_stylesheet_kv_row1, 2},
	{fh_c_stylesheet_kv_row2, 3},
	{fh_c_stylesheet_kv_row3, 3},
	{fh_c_stylesheet_kv_row4, 3},
	{fh_c_stylesheet_kv_row5, 3},
	{fh_c_stylesheet_kv_row6, 4},
	{fh_c_stylesheet_kv_row7, 4}
};

