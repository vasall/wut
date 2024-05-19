#ifndef _WT_STYLE_STYLESHEET_H
#define _WT_STYLE_STYLESHEET_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "utility/inc/color.h"
#include "utility/inc/flex.h"

#define WT_KW_UNDEFINED			 0

#define WT_KW_DISPLAY_VISIBLE	 	 1
#define WT_KW_DISPLAY_INVISIBLE	 	 2

#define WT_KW_REFERENCE_RELATIVE	 1
#define WT_KW_REFERENCE_ABSOLUTE	 2

#define WT_KW_BORDER_NONE		 1
#define WT_KW_BORDER_SOLID		 2

#define WT_KW_INFILL_COLOR		 1
#define WT_KW_INFILL_NONE		 2

#define WT_KW_LAYOUT_BLOCK	 	 1
#define WT_KW_LAYOUT_ROW		 2
#define WT_KW_LAYOUT_COLUMN	 	 4

#define WT_KW_ALIGN_TOP      	 	 1
#define WT_KW_ALIGN_RIGHT		 2
#define WT_KW_ALIGN_BOTTOM  	 	 4
#define WT_KW_ALIGN_LEFT		 8
#define WT_KW_ALIGN_MIDDLE		16

#define WT_KW_SCROLLBAR_AUTO	 	 1
#define WT_KW_SCROLLBAR_NONE	 	 2
#define WT_KW_SCROLLBAR_VERTICAL	 3
#define WT_KW_SCROLLBAR_HORIZONTAL	 4

#define WT_KW_TEXT_UNDERLINE	 	 1

#define WT_KW_TEXT_WORDWRAP	 	 1
#define WT_KW_TEXT_NOWRAP	 	 2
#define WT_KW_TEXT_LETTERWRAP	 	 4


enum wt_sheet_type {
	WT_SHEET_FLEX =  	0,
	WT_SHEET_HEXCODE =	1,
	WT_SHEET_KEYWORD =	2
};


enum wt_sheet_id {
	WT_SHEET_UNDEFINED = 		 0,

	/*  	 DISPLAY						      */
	WT_SHEET_DISPLAY_MODE = 	 1,	/* 0x89   1     8   KEYWORD-0 */

	/*  	 SIZE							      */
	WT_SHEET_WIDTH =		 2,	/* 0x3B   3     9   FLEX      */
	WT_SHEET_HEIGHT =	 	 3,	/* 0x98   0    17   FLEX      */

	/*  	 REFERENCE						      */
	WT_SHEET_REFERENCE_MODE =	 4,	/* 0x77   7    25   KEYWORD-1 */

	/*  	 SPACING						      */
	WT_SHEET_SPACING_TOP =		 5,	/* 0xFB   3    26   FLEX      */
	WT_SHEET_SPACING_RIGHT =	 6,	/* 0x43   3    34   FLEX      */
	WT_SHEET_SPACING_BOTTOM =	 7,	/* 0xB4   4    42   FLEX      */
	WT_SHEET_SPACING_LEFT =		 8,	/* 0x68   0    50   FLEX      */

	/*  	 PADDING						      */
	WT_SHEET_PADDING_TOP = 		 9,	/* 0x2C   4    58   FLEX      */
	WT_SHEET_PADDING_RIGHT = 	10,	/* 0x41   1    66   FLEX      */
	WT_SHEET_PADDING_BOTTOM = 	11,	/* 0x5B   3    74   FLEX      */
	WT_SHEET_PADDING_LEFT = 	12,	/* 0x9E   6    82   FLEX      */

	/*  	 BORDER							      */
	WT_SHEET_BORDER_MODE =		13,	/* 0xD8   0    90   KEYWORD-2 */
	WT_SHEET_BORDER_WIDTH =		14,	/* 0x56   6    91   FLEX      */
	WT_SHEET_BORDER_COLOR =		15,	/* 0x0F   7    99   HEX-CODE  */

	/*  	 RADIUS							      */
	WT_SHEET_RADIUS_TOP_LEFT =	16,	/* 0x25   6   103   FLEX      */
	WT_SHEET_RADIUS_TOP_RIGHT =	17,	/* 0xA6   6   111   FLEX      */
	WT_SHEET_RADIUS_BOTTOM_RIGHT =	18,	/* 0x5F   7   119   FLEX      */
	WT_SHEET_RADIUS_BOTTOM_LEFT =	19,	/* 0x1E   6   127   FLEX      */

	/*  	 INFILL							      */
	WT_SHEET_INFILL_MODE =		20,	/* 0x54   4   135   KEYWORD-3 */
	WT_SHEET_INFILL_COLOR =		21,	/* 0xF9   1   136   HEX-CODE  */

	/*  	 LAYOUT							      */
	WT_SHEET_LAYOUT_MODE =		22,	/* 0x3F   7   140   KEYWORD-4 */

	/*  	 ALIGNMENT						      */
	WT_SHEET_ALIGN_V =		23,	/* 0x0D   5   141   KEYWORD-5 */
	WT_SHEET_ALIGN_H = 		24,	/* 0xFE   6   142   KEYWORD-6 */

	/*  	 SCROLLBAR						      */
	WT_SHEET_SCROLLBAR_MODE =	25,	/* 0x02   2   143   KEYWORD-7 */

	/*	 TEXT							      */
	WT_SHEET_TEXT_SIZE =		26,	/* 0xB0   0   144   FLEX      */
	WT_SHEET_TEXT_COLOR =		27,	/* 0x93   3   152   HEX-CODE  */
	WT_SHEET_TEXT_MASS =		28,	/* 0x19   1   156   FLEX      */
	WT_SHEET_TEXT_OPTIONS =		29,	/* 0xE8   0   164   KEYWORD-8 */
	WT_SHEET_TEXT_WRAP_MODE =	30,	/* 0xB3   3   165   KEYWORD-9 */
	WT_SHEET_TEXT_SPACING =		31,	/* 0x01   1   166   FLEX      */
	WT_SHEET_LINE_HEIGHT =		32	/* 0xD2   2   174   FLEX      */
};


struct wt_sheet_attr {
	/*
	 * The type of content used by this attribute(ie flex, color, etc.).
	 */
	enum wt_sheet_type type;
	
	/*
	 * The category index used by some content types like keywords.
	 */
	u8 category;

	/* 
	 * The offset of the attribute in the stylesheet.
	 */
	s16 offset;
};


struct wt_stylesheet {
	/* The attribute mask */
	u64 		mask;

	/*  	 DISPLAY						   */
	u8		display_mode;

	/*  	 SIZE							   */
	wt_flex_t	width;
	wt_flex_t	height;

	/*  	 REFERENCE						   */
	u8		reference_mode;

	/*  	 SPACING						   */
	wt_flex_t	spacing_top;
	wt_flex_t	spacing_right;
	wt_flex_t	spacing_bottom;
	wt_flex_t	spacing_left;

	/*  	 PADDING						   */
	wt_flex_t	padding_top;
	wt_flex_t	padding_right;
	wt_flex_t	padding_bottom;
	wt_flex_t	padding_left;

	/*  	 BORDER							   */
	u8		border_mode;
	wt_flex_t	border_width;
	wt_color_t	border_color;

	/*  	 RADIUS							   */
	wt_flex_t 	radius_top_left;
	wt_flex_t 	radius_top_right;
	wt_flex_t 	radius_bottom_right;
	wt_flex_t 	radius_bottom_left;

	/*  	 INFILL							   */
	u8		infill_mode;
	wt_color_t	infill_color;

	/*  	 LAYOUT							   */
	u8		layout_mode;

	/*  	 ALIGNMENT						   */
	u8		align_v;
	u8		align_h;

	/*  	 SCROLLBAR						   */
	u8		scrollbar_mode;

	/*	 TEXT							   */
	wt_flex_t	text_size;
	wt_color_t	text_color;
	wt_flex_t	text_mass;
	u8		text_options;
	u8		text_wrap_mode;
	wt_flex_t	text_spacing;
	wt_flex_t	line_height;
} __attribute__((__packed__));


struct wt_sheet_ret {
	enum wt_sheet_id 	id;
	
	enum wt_sheet_type 	type;
	wt_flex_t 		flex;
	u32 			hexcode;
	u8 			keyword;
};


/*
 * Reset a stylesheet and free all potentially allocated memory.
 *
 * @sheet: Pointer to the stylesheet
 */
WT_XMOD void wt_sheet_reset(struct wt_stylesheet *sheet);


/*
 * Parse an input string and update the stylesheet accordingly.
 *
 * @sheet: Pointer to the stylesheet
 * @s: The input string
 */
WT_XMOD void wt_sheet_parse(struct wt_stylesheet *sheet, char *s);


/*
 * Retrieve the value of an attribute from the stylesheet.
 *
 * @sheet: Pointer to the stylesheet
 * @id: The id of the attribute to get
 * @ret: The return struct
 *
 * Returns: 1 if the attribute value has been returned, 0 if the requested
 * 	    attribute is not available and -1 if an error occurred
 */
WT_XMOD s8 wt_sheet_get(struct wt_stylesheet *sheet, enum wt_sheet_id id,
		struct wt_sheet_ret *ret);



/*
 * Print the stylesheet in the console.
 *
 * @sheet: Pointer to the stylesheet
 */
WT_XMOD void wt_sheet_print(struct wt_stylesheet *sheet);


#endif /* _WT_STYLE_STYLESHEET_H */
