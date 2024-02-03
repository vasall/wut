#ifndef _FH_STYLE_STYLESHEET_H
#define _FH_STYLE_STYLESHEET_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "utility/inc/color.h"
#include "utility/inc/flex.h"

#define FH_KW_UNDEFINED			 0

#define FH_KW_DISPLAY_VISIBLE	 	 1
#define FH_KW_DISPLAY_INVISIBLE	 	 2

#define FH_KW_REFERENCE_RELATIVE	 1
#define FH_KW_REFERENCE_ABSOLUTE	 2

#define FH_KW_BORDER_NONE		 1
#define FH_KW_BORDER_SOLID		 2

#define FH_KW_INFILL_COLOR		 1
#define FH_KW_INFILL_NONE		 2

#define FH_KW_LAYOUT_BLOCK	 	 1
#define FH_KW_LAYOUT_ROW		 2
#define FH_KW_LAYOUT_COLUMN	 	 4

#define FH_KW_ALIGN_TOP      	 	 1
#define FH_KW_ALIGN_RIGHT		 2
#define FH_KW_ALIGN_BOTTOM  	 	 4
#define FH_KW_ALIGN_LEFT		 8
#define FH_KW_ALIGN_MIDDLE		16

#define FH_KW_SCROLLBAR_AUTO	 	 1
#define FH_KW_SCROLLBAR_NONE	 	 2
#define FH_KW_SCROLLBAR_VERTICAL	 3
#define FH_KW_SCROLLBAR_HORIZONTAL	 4

#define FH_KW_TEXT_UNDERLINE	 	 1

#define FH_KW_TEXT_WORDWRAP	 	 1
#define FH_KW_TEXT_NOWRAP	 	 2
#define FH_KW_TEXT_LETTERWRAP	 	 4


enum fh_sheet_type {
	FH_SHEET_FLEX =  	0,
	FH_SHEET_HEXCODE =	1,
	FH_SHEET_KEYWORD =	2
};


enum fh_sheet_id {
	FH_SHEET_UNDEFINED = 		 0,

	/*  	 DISPLAY						      */
	FH_SHEET_DISPLAY_MODE = 	 1,	/* 0x89   1     4   KEYWORD-0 */

	/*  	 SIZE							      */
	FH_SHEET_WIDTH =		 2,	/* 0x3B   3     5   FLEX      */
	FH_SHEET_HEIGHT =	 	 3,	/* 0x98   0    13   FLEX      */

	/*  	 REFERENCE						      */
	FH_SHEET_REFERENCE_MODE =	 4,	/* 0x77   7    21   KEYWORD-1 */

	/*  	 SPACING						      */
	FH_SHEET_SPACING_TOP =		 5,	/* 0xFB   3    22   FLEX      */
	FH_SHEET_SPACING_RIGHT =	 6,	/* 0x43   3    30   FLEX      */
	FH_SHEET_SPACING_BOTTOM =	 7,	/* 0xB4   4    38   FLEX      */
	FH_SHEET_SPACING_LEFT =		 8,	/* 0x68   0    46   FLEX      */

	/*  	 PADDING						      */
	FH_SHEET_PADDING_TOP = 		 9,	/* 0x2C   4    54   FLEX      */
	FH_SHEET_PADDING_RIGHT = 	10,	/* 0x41   1    62   FLEX      */
	FH_SHEET_PADDING_BOTTOM = 	11,	/* 0x5B   3    70   FLEX      */
	FH_SHEET_PADDING_LEFT = 	12,	/* 0x9E   6    78   FLEX      */

	/*  	 BORDER							      */
	FH_SHEET_BORDER_MODE =		13,	/* 0xD8   0    86   KEYWORD-2 */
	FH_SHEET_BORDER_WIDTH =		14,	/* 0x56   6    87   FLEX      */
	FH_SHEET_BORDER_COLOR =		15,	/* 0x0F   7    95   HEX-CODE  */

	/*  	 RADIUS							      */
	FH_SHEET_RADIUS_TOP_LEFT =	16,	/* 0x25   6    99   FLEX      */
	FH_SHEET_RADIUS_TOP_RIGHT =	17,	/* 0xA6   6   107   FLEX      */
	FH_SHEET_RADIUS_BOTTOM_RIGHT =	18,	/* 0x5F   7   115   FLEX      */
	FH_SHEET_RADIUS_BOTTOM_LEFT =	19,	/* 0x1E   6   123   FLEX      */

	/*  	 INFILL							      */
	FH_SHEET_INFILL_MODE =		20,	/* 0x54   4   131   KEYWORD-3 */
	FH_SHEET_INFILL_COLOR =		21,	/* 0xF9   1   132   HEX-CODE  */

	/*  	 LAYOUT							      */
	FH_SHEET_LAYOUT_MODE =		22,	/* 0x3F   7   136   KEYWORD-4 */

	/*  	 ALIGNMENT						      */
	FH_SHEET_ALIGN_V =		23,	/* 0x0D   5   137   KEYWORD-5 */
	FH_SHEET_ALIGN_H = 		24,	/* 0xFE   6   138   KEYWORD-6 */

	/*  	 SCROLLBAR						      */
	FH_SHEET_SCROLLBAR_MODE =	25,	/* 0x02   2   139   KEYWORD-7 */

	/*	 TEXT							      */
	FH_SHEET_TEXT_SIZE =		26,	/* 0xB0   0   140   FLEX      */
	FH_SHEET_TEXT_COLOR =		27,	/* 0x93   3   148   HEX-CODE  */
	FH_SHEET_TEXT_MASS =		28,	/* 0x19   1   152   FLEX      */
	FH_SHEET_TEXT_OPTIONS =		29,	/* 0xE8   0   160   KEYWORD-8 */
	FH_SHEET_TEXT_WRAP_MODE =	30	/* 0xB3   3   161   KEYWORD-9 */
};


struct fh_sheet_attr {
	/*
	 * The type of content used by this attribute(ie flex, color, etc.).
	 */
	enum fh_sheet_type type;
	
	/*
	 * The category index used by some content types like keywords.
	 */
	u8 category;

	/* 
	 * The offset of the attribute in the stylesheet.
	 */
	s16 offset;
};


typedef u8 fh_keyword_t;


struct fh_stylesheet {
	/* The attribute mask */
	u32 		mask;

	/*  	 DISPLAY						   */
	fh_keyword_t	display_mode;

	/*  	 SIZE							   */
	fh_flex_t	width;
	fh_flex_t	height;

	/*  	 REFERENCE						   */
	fh_keyword_t	reference_mode;

	/*  	 SPACING						   */
	fh_flex_t	spacing_top;
	fh_flex_t	spacing_right;
	fh_flex_t	spacing_bottom;
	fh_flex_t	spacing_left;

	/*  	 PADDING						   */
	fh_flex_t	padding_top;
	fh_flex_t	padding_right;
	fh_flex_t	padding_bottom;
	fh_flex_t	padding_left;

	/*  	 BORDER							   */
	fh_keyword_t	border_mode;
	fh_flex_t	border_width;
	fh_color_t	border_color;

	/*  	 RADIUS							   */
	fh_flex_t 	radius_top_left;
	fh_flex_t 	radius_top_right;
	fh_flex_t 	radius_bottom_right;
	fh_flex_t 	radius_bottom_left;

	/*  	 INFILL							   */
	fh_keyword_t	infill_mode;
	fh_color_t	infill_color;

	/*  	 LAYOUT							   */
	fh_keyword_t	layout_mode;

	/*  	 ALIGNMENT						   */
	fh_keyword_t	align_v;
	fh_keyword_t	align_h;

	/*  	 SCROLLBAR						   */
	fh_keyword_t	scrollbar_mode;

	/*	 TEXT							   */
	fh_flex_t	text_size;
	fh_color_t	text_color;
	fh_flex_t	text_mass;
	fh_keyword_t	text_options;
	fh_keyword_t	text_wrap_mode;
} __attribute__((__packed__));


struct fh_sheet_ret {
	enum fh_sheet_id 	id;
	
	enum fh_sheet_type 	type;
	fh_flex_t 		flex;
	u32 			hexcode;
	fh_keyword_t 		keyword;
};


/*
 * Reset a stylesheet and free all potentially allocated memory.
 *
 * @sheet: Pointer to the stylesheet
 */
FH_XMOD void fh_sheet_reset(struct fh_stylesheet *sheet);


/*
 * Parse an input string and update the stylesheet accordingly.
 *
 * @sheet: Pointer to the stylesheet
 * @s: The input string
 */
FH_XMOD void fh_sheet_parse(struct fh_stylesheet *sheet, char *s);


/*
 * Retrieve the value of an attribute from the stylesheet. Note that the
 * stylesheet is dynamic, so it doesn't have to contain the requested attribute.
 *
 * @sheet: Pointer to the stylesheet
 * @id: The id of the attribute to get
 * @ret: The return struct
 *
 * Returns: 1 if the attribute value has been returned, 0 if the requested
 * 	    attribute is not available and -1 if an error occurred
 */
FH_XMOD s8 fh_sheet_get(struct fh_stylesheet *sheet, enum fh_sheet_id id,
		struct fh_sheet_ret *ret);


/*
 * Print the stylesheet in the console.
 *
 * @sheet: Pointer to the stylesheet
 */
FH_XMOD void fh_sheet_print(struct fh_stylesheet *sheet);


#endif /* _FH_STYLE_STYLESHEET_H */
