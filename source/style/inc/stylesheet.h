#ifndef _WUT_STYLE_STYLESHEET_H
#define _WUT_STYLE_STYLESHEET_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "utility/inc/color.h"
#include "utility/inc/flex.h"

#define WUT_KW_UNDEFINED			 0

#define WUT_KW_DISPLAY_VISIBLE	 	 1
#define WUT_KW_DISPLAY_INVISIBLE	 	 2

#define WUT_KW_REFERENCE_RELATIVE	 1
#define WUT_KW_REFERENCE_ABSOLUTE	 2

#define WUT_KW_BORDER_NONE		 1
#define WUT_KW_BORDER_SOLID		 2

#define WUT_KW_INFILL_COLOR		 1
#define WUT_KW_INFILL_NONE		 2

#define WUT_KW_LAYOUT_BLOCK	 	 1
#define WUT_KW_LAYOUT_ROW		 2
#define WUT_KW_LAYOUT_COLUMN	 	 4

#define WUT_KW_ALIGN_TOP      	 	 1
#define WUT_KW_ALIGN_RIGHT		 2
#define WUT_KW_ALIGN_BOTTOM  	 	 4
#define WUT_KW_ALIGN_LEFT		 8
#define WUT_KW_ALIGN_MIDDLE		16

#define WUT_KW_SCROLLBAR_AUTO	 	 1
#define WUT_KW_SCROLLBAR_NONE	 	 2
#define WUT_KW_SCROLLBAR_VERTICAL	 3
#define WUT_KW_SCROLLBAR_HORIZONTAL	 4

#define WUT_KW_TEXT_UNDERLINE	 	 1

#define WUT_KW_TEXT_WORDWRAP	 	 1
#define WUT_KW_TEXT_NOWRAP	 	 2
#define WUT_KW_TEXT_LETTERWRAP	 	 4


enum wut_eSheetType {
        WUT_SHEET_FLEX =  	0,
        WUT_SHEET_HEXCODE =	1,
        WUT_SHEET_KEYWORD =	2
};


enum wut_eSheetAttribId {
        WUT_SHEET_UNDEFINED = 		 0,

        /*  	 DISPLAY						      */
        WUT_SHEET_DISPLAY_MODE = 	 1,	/* 0x89   1     8   KEYWORD-0 */

        /*  	 SIZE							      */
        WUT_SHEET_WIDTH =		 2,	/* 0x3B   3     9   FLEX      */
        WUT_SHEET_HEIGHT =	 	 3,	/* 0x98   0    17   FLEX      */

        /*  	 REFERENCE						      */
        WUT_SHEET_REFERENCE_MODE =	 4,	/* 0x77   7    25   KEYWORD-1 */

        /*  	 SPACING						      */
        WUT_SHEET_SPACING_TOP =		 5,	/* 0xFB   3    26   FLEX      */
        WUT_SHEET_SPACING_RIGHT =	 6,	/* 0x43   3    34   FLEX      */
        WUT_SHEET_SPACING_BOTTOM =	 7,	/* 0xB4   4    42   FLEX      */
        WUT_SHEET_SPACING_LEFT =		 8,	/* 0x68   0    50   FLEX      */

        /*  	 PADDING						      */
        WUT_SHEET_PADDING_TOP = 		 9,	/* 0x2C   4    58   FLEX      */
        WUT_SHEET_PADDING_RIGHT = 	10,	/* 0x41   1    66   FLEX      */
        WUT_SHEET_PADDING_BOTTOM = 	11,	/* 0x5B   3    74   FLEX      */
        WUT_SHEET_PADDING_LEFT = 	12,	/* 0x9E   6    82   FLEX      */

        /*  	 BORDER							      */
        WUT_SHEET_BORDER_MODE =		13,	/* 0xD8   0    90   KEYWORD-2 */
        WUT_SHEET_BORDER_WIDTH =		14,	/* 0x56   6    91   FLEX      */
        WUT_SHEET_BORDER_COLOR =		15,	/* 0x0F   7    99   HEX-CODE  */

        /*  	 RADIUS							      */
        WUT_SHEET_RADIUS_TOP_LEFT =	16,	/* 0x25   6   103   FLEX      */
        WUT_SHEET_RADIUS_TOP_RIGHT =	17,	/* 0xA6   6   111   FLEX      */
        WUT_SHEET_RADIUS_BOTTOM_RIGHT =	18,	/* 0x5F   7   119   FLEX      */
        WUT_SHEET_RADIUS_BOTTOM_LEFT =	19,	/* 0x1E   6   127   FLEX      */

        /*  	 INFILL							      */
        WUT_SHEET_INFILL_MODE =		20,	/* 0x54   4   135   KEYWORD-3 */
        WUT_SHEET_INFILL_COLOR =		21,	/* 0xF9   1   136   HEX-CODE  */

        /*  	 LAYOUT							      */
        WUT_SHEET_LAYOUT_MODE =		22,	/* 0x3F   7   140   KEYWORD-4 */

        /*  	 ALIGNMENT						      */
        WUT_SHEET_ALIGN_V =		23,	/* 0x0D   5   141   KEYWORD-5 */
        WUT_SHEET_ALIGN_H = 		24,	/* 0xFE   6   142   KEYWORD-6 */

        /*  	 SCROLLBAR						      */
        WUT_SHEET_SCROLLBAR_MODE =	25,	/* 0x02   2   143   KEYWORD-7 */

        /*	 TEXT							      */
        WUT_SHEET_TEXT_SIZE =		26,	/* 0xB0   0   144   FLEX      */
        WUT_SHEET_TEXT_COLOR =		27,	/* 0x93   3   152   HEX-CODE  */
        WUT_SHEET_TEXT_MASS =		28,	/* 0x19   1   156   FLEX      */
        WUT_SHEET_TEXT_OPTIONS =		29,	/* 0xE8   0   164   KEYWORD-8 */
        WUT_SHEET_TEXT_WRAP_MODE =	30,	/* 0xB3   3   165   KEYWORD-9 */
        WUT_SHEET_TEXT_SPACING =		31,	/* 0x01   1   166   FLEX      */
        WUT_SHEET_LINE_HEIGHT =		32	/* 0xD2   2   174   FLEX      */
};


struct wut_SheetAttrib {
        /*
         * The type of content used by this attribute(ie flex, color, etc.).
         */
        enum wut_eSheetType type;

        /*
         * The category index used by some content types like keywords.
         */
        u8 category;

        /* 
         * The offset of the attribute in the stylesheet.
         */
        s16 offset;
};


struct wut_Stylesheet {
        /* The attribute mask */
        u64 		mask;

        /*  	 DISPLAY						   */
        u8		display_mode;

        /*  	 SIZE							   */
        struct wut_Flex	width;
        struct wut_Flex	height;

        /*  	 REFERENCE						   */
        u8		reference_mode;

        /*  	 SPACING						   */
        struct wut_Flex	spacing_top;
        struct wut_Flex	spacing_right;
        struct wut_Flex	spacing_bottom;
        struct wut_Flex	spacing_left;

        /*  	 PADDING						   */
        struct wut_Flex	padding_top;
        struct wut_Flex	padding_right;
        struct wut_Flex	padding_bottom;
        struct wut_Flex	padding_left;

        /*  	 BORDER							   */
        u8		border_mode;
        struct wut_Flex	border_width;
        struct wut_Color	border_color;

        /*  	 RADIUS							   */
        struct wut_Flex 	radius_top_left;
        struct wut_Flex 	radius_top_right;
        struct wut_Flex 	radius_bottom_right;
        struct wut_Flex 	radius_bottom_left;

        /*  	 INFILL							   */
        u8		infill_mode;
        struct wut_Color	infill_color;

        /*  	 LAYOUT							   */
        u8		layout_mode;

        /*  	 ALIGNMENT						   */
        u8		align_v;
        u8		align_h;

        /*  	 SCROLLBAR						   */
        u8		scrollbar_mode;

        /*	 TEXT							   */
        struct wut_Flex	text_size;
        struct wut_Color	text_color;
        struct wut_Flex	text_mass;
        u8		text_options;
        u8		text_wrap_mode;
        struct wut_Flex	text_spacing;
        struct wut_Flex	line_height;
} __attribute__((__packed__));


struct wut_SheetReturn {
        enum wut_eSheetAttribId 	id;

        enum wut_eSheetType 	type;
        struct wut_Flex 		flex;
        u32 			hexcode;
        u8 			keyword;
};


/*
 * Reset a stylesheet and free all potentially allocated memory.
 *
 * @sheet: Pointer to the stylesheet
 */
WUT_XMOD void wut_sht_reset(struct wut_Stylesheet *sheet);


/*
 * Parse an input string and update the stylesheet accordingly.
 *
 * @sheet: Pointer to the stylesheet
 * @s: The input string
 */
WUT_XMOD void wut_sht_parse(struct wut_Stylesheet *sheet, char *s);


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
WUT_XMOD s8 wut_sht_get(struct wut_Stylesheet *sheet, enum wut_eSheetAttribId id,
                struct wut_SheetReturn *ret);



/*
 * Print the stylesheet in the console.
 *
 * @sheet: Pointer to the stylesheet
 */
WUT_XMOD void wut_sht_print(struct wut_Stylesheet *sheet);


#endif /* _WUT_STYLE_STYLESHEET_H */
