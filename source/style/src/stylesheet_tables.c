#include "style/inc/stylesheet_tables.h"

#include <stdlib.h>

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

const struct wut_SheetAttrib wut_c_sheet_attribs[WUT_SHEET_ATTRIBS] = {
        {0, 		       0,	  0},	/*  0 UNDEFINED		    */

        {WUT_SHEET_KEYWORD,    0,	  8},	/*  1 display_mode 	    */

        {WUT_SHEET_FLEX,       0,	  9},	/*  2 width 		    */
        {WUT_SHEET_FLEX,       0,	 17},	/*  3 height 		    */

        {WUT_SHEET_KEYWORD,    1,	 25},	/*  4 reference_mode 	    */

        {WUT_SHEET_FLEX,       0,	 26},	/*  5 spacing_top 	    */
        {WUT_SHEET_FLEX,       0,	 34},	/*  6 spacing_right 	    */
        {WUT_SHEET_FLEX,       0,	 42},	/*  7 spacing_bottom 	    */
        {WUT_SHEET_FLEX,       0,	 50},	/*  8 spacing_left 	    */

        {WUT_SHEET_FLEX,       0,	 58},	/*  9 padding_top 	    */
        {WUT_SHEET_FLEX,       0,	 66},	/* 10 padding_right 	    */
        {WUT_SHEET_FLEX,       0,	 74},	/* 11 padding_bottom 	    */
        {WUT_SHEET_FLEX,       0,	 82},	/* 12 padding_left 	    */

        {WUT_SHEET_KEYWORD,    2,	 90},	/* 13 border_mode 	    */
        {WUT_SHEET_FLEX,       0,	 91},	/* 14 border_width 	    */
        {WUT_SHEET_HEXCODE,    0,	 99},	/* 15 border_color 	    */

        {WUT_SHEET_FLEX,       0,	103},	/* 16 radius_top_left 	    */
        {WUT_SHEET_FLEX,       0,	111},	/* 17 radius_top_right 	    */
        {WUT_SHEET_FLEX,       0,	119},	/* 18 radius_bottom_right   */
        {WUT_SHEET_FLEX,       0,	127},	/* 19 radius_bottom_left    */

        {WUT_SHEET_KEYWORD,    3,	135},	/* 20 infill_mode 	    */
        {WUT_SHEET_HEXCODE,    0,	136},	/* 21 infill_color 	    */

        {WUT_SHEET_KEYWORD,    4,	140},	/* 22 layout_mode 	    */

        {WUT_SHEET_KEYWORD,    5,	141},	/* 23 align_v    	    */
        {WUT_SHEET_KEYWORD,    6,	142},	/* 24 align_h   	    */

        {WUT_SHEET_KEYWORD,    7,	143},	/* 25 scrollbar_mode 	    */

        {WUT_SHEET_FLEX,       0,	144},	/* 26 text_size 	    */
        {WUT_SHEET_HEXCODE,    0,	152},	/* 27 text_color 	    */
        {WUT_SHEET_FLEX,       0,	156},	/* 28 text_mass 	    */
        {WUT_SHEET_KEYWORD,    8,	164},	/* 29 text_options 	    */
        {WUT_SHEET_KEYWORD,    9,	165},	/* 30 text_wrap_mode 	    */
        {WUT_SHEET_FLEX,       0,	166},	/* 31 text_spacing          */
        {WUT_SHEET_FLEX,       0,	174}	/* 32 line_height 	    */
};


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

struct wut_SheetAttribIdEntry wut_c_sheet_attr_row0[] = {
        {0x98, WUT_SHEET_HEIGHT},
        {0x68, WUT_SHEET_SPACING_LEFT},
        {0xD8, WUT_SHEET_BORDER_MODE},
        {0xB0, WUT_SHEET_TEXT_SIZE},
        {0xE8, WUT_SHEET_TEXT_OPTIONS}
};
struct wut_SheetAttribIdEntry wut_c_sheet_attr_row1[] = {
        {0x89, WUT_SHEET_DISPLAY_MODE},
        {0x41, WUT_SHEET_PADDING_RIGHT},
        {0xF9, WUT_SHEET_INFILL_COLOR},
        {0x19, WUT_SHEET_TEXT_MASS},
        {0x01, WUT_SHEET_TEXT_SPACING}
};
struct wut_SheetAttribIdEntry wut_c_sheet_attr_row2[] = {
        {0x02, WUT_SHEET_SCROLLBAR_MODE},
        {0xD2, WUT_SHEET_LINE_HEIGHT}
};
struct wut_SheetAttribIdEntry wut_c_sheet_attr_row3[] = {
        {0x3B, WUT_SHEET_WIDTH},
        {0xFB, WUT_SHEET_SPACING_TOP},
        {0x43, WUT_SHEET_SPACING_RIGHT},
        {0x5B, WUT_SHEET_PADDING_BOTTOM},
        {0x93, WUT_SHEET_TEXT_COLOR},
        {0xB3, WUT_SHEET_TEXT_WRAP_MODE}
};
struct wut_SheetAttribIdEntry wut_c_sheet_attr_row4[] = {
        {0xB4, WUT_SHEET_SPACING_BOTTOM},
        {0x2C, WUT_SHEET_PADDING_TOP},
        {0x54, WUT_SHEET_INFILL_MODE}
};
struct wut_SheetAttribIdEntry wut_c_sheet_attr_row5[] = {
        {0x0D, WUT_SHEET_ALIGN_V},
        {0x25, WUT_SHEET_RADIUS_TOP_LEFT},
};
struct wut_SheetAttribIdEntry wut_c_sheet_attr_row6[] = {
        {0x9E, WUT_SHEET_PADDING_LEFT},
        {0x56, WUT_SHEET_BORDER_WIDTH},
        {0xA6, WUT_SHEET_RADIUS_TOP_RIGHT},
        {0x1E, WUT_SHEET_RADIUS_BOTTOM_RIGHT},
        {0xFE, WUT_SHEET_ALIGN_H}
};
struct wut_SheetAttribIdEntry wut_c_sheet_attr_row7[] = {
        {0x77, WUT_SHEET_REFERENCE_MODE},
        {0x0F, WUT_SHEET_BORDER_COLOR},
        {0x5F, WUT_SHEET_RADIUS_BOTTOM_LEFT},
        {0x3F, WUT_SHEET_LAYOUT_MODE}
};

const struct wut_SheetAttribIdTable wut_c_sheet_ids[WUT_SHEET_ROWS] = {
        {wut_c_sheet_attr_row0, 5},
        {wut_c_sheet_attr_row1, 5},
        {wut_c_sheet_attr_row2, 2},
        {wut_c_sheet_attr_row3, 6},
        {wut_c_sheet_attr_row4, 3},
        {wut_c_sheet_attr_row5, 2},
        {wut_c_sheet_attr_row6, 5},
        {wut_c_sheet_attr_row7, 4}
};

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

struct wut_sheet_kw wut_c_sheet_kw_row0[] = {
        {"visible", 		WUT_KW_DISPLAY_VISIBLE},
        {"hidden", 		WUT_KW_DISPLAY_INVISIBLE}
};
struct wut_sheet_kw wut_c_sheet_kw_row1[] = {
        {"relative", 		WUT_KW_REFERENCE_RELATIVE},
        {"absolute", 		WUT_KW_REFERENCE_ABSOLUTE}
};
struct wut_sheet_kw wut_c_sheet_kw_row2[] = {
        {"none", 		WUT_KW_BORDER_NONE},
        {"solid", 		WUT_KW_BORDER_SOLID}
};
struct wut_sheet_kw wut_c_sheet_kw_row3[] = {
        {"color", 		WUT_KW_INFILL_COLOR},
        {"none", 		WUT_KW_INFILL_NONE}
};
struct wut_sheet_kw wut_c_sheet_kw_row4[] = {
        {"block", 		WUT_KW_LAYOUT_BLOCK},
        {"row",			WUT_KW_LAYOUT_ROW},
        {"column", 		WUT_KW_LAYOUT_COLUMN}
};
struct wut_sheet_kw wut_c_sheet_kw_row5[] = {
        {"top", 		WUT_KW_ALIGN_TOP},
        {"middle",		WUT_KW_ALIGN_MIDDLE},
        {"bottom", 		WUT_KW_ALIGN_BOTTOM}
};
struct wut_sheet_kw wut_c_sheet_kw_row6[] = {
        {"left", 		WUT_KW_ALIGN_LEFT},
        {"middle",		WUT_KW_ALIGN_MIDDLE},
        {"right", 		WUT_KW_ALIGN_RIGHT}
};
struct wut_sheet_kw wut_c_sheet_kw_row7[] = {
        {"auto", 		WUT_KW_SCROLLBAR_AUTO},
        {"none", 		WUT_KW_SCROLLBAR_NONE},
        {"vertical", 		WUT_KW_SCROLLBAR_VERTICAL},
        {"horizontal", 		WUT_KW_SCROLLBAR_HORIZONTAL}
};
struct wut_sheet_kw wut_c_sheet_kw_row8[] = {
        {"underline",		WUT_KW_TEXT_UNDERLINE}
};
struct wut_sheet_kw wut_c_sheet_kw_row9[] = {
        {"wordwrap",		WUT_KW_TEXT_WORDWRAP},
        {"nowrap",		WUT_KW_TEXT_NOWRAP},
        {"letterwrap",		WUT_KW_TEXT_LETTERWRAP}
};

const struct wut_sheet_kw_table wut_c_sheet_keywords[WUT_SHEET_KW_ROWS] = {
        {wut_c_sheet_kw_row0, 2},
        {wut_c_sheet_kw_row1, 2},
        {wut_c_sheet_kw_row2, 3},
        {wut_c_sheet_kw_row3, 3},
        {wut_c_sheet_kw_row4, 4},
        {wut_c_sheet_kw_row5, 3},
        {wut_c_sheet_kw_row6, 3},
        {wut_c_sheet_kw_row7, 4},
        {wut_c_sheet_kw_row8, 1},
        {wut_c_sheet_kw_row9, 3}
};


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

const char *wut_sheet_attr_name[WUT_SHEET_ATTRIBS] = {
        "undefined",

        "display_mode",
        
        "width",
        "height",
        
        "reference_mode",

        "spacing_top",
        "spacing_right",
        "spacing_bottom",
        "spacing_left",

        "padding_top",
        "padding_right",
        "padding_bottom",
        "padding_left",

        "border_mode",
        "border_width",
        "border_color",

        "radius_top_left",
        "radius_top_right",
        "radius_bottom_right",
        "radius_bottom_left",

        "infill_mode",
        "infill_color",

        "layout_mode",

        "align_v",
        "align_h",

        "scrollbar_mode",

        "text_size",
        "text_color",
        "text_mass",
        "text_options",
        "text_wrap_mode",
        "text_spacing",
        "line_height"
};
