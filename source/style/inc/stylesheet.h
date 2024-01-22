#ifndef _FH_STYLE_STYLESHEET_H
#define _FH_STYLE_STYLESHEET_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "graphic/inc/flex.h"

#include "utility/inc/color.h"
#include "utility/inc/flex.h"

#define FH_DISPLAY_VISIBLE	0
#define FH_DISPLAY_INVISIBLE	1

#define FH_REFERENCE_AUTO	0
#define FH_REFERENCE_ABSOLUTE	1

#define FH_ALIGNMENT_INHERIT	0
#define FH_ALIGNMENT_TOP      	1
#define FH_ALIGNMENT_RIGHT	2
#define FH_ALIGNMENT_BOTTOM  	3
#define FH_ALIGNMENT_LEFT	4

#define FH_BORDER_INHERIT	0
#define FH_BORDER_NONE		1
#define FH_BORDER_SOLID		2

#define FH_INFILL_INHERIT	0
#define FH_INFILL_NONE		1
#define FH_INFILL_COLOR		2

#define FH_LAYOUT_INHERIT	0
#define FH_LAYOUT_BLOCKS	1
#define FH_LAYOUT_ROWS		2
#define FH_LAYOUT_COLUMNS	3

#define FH_SCROLLBAR_AUTO	0
#define FH_SCROLLBAR_NONE	1
#define FH_SCROLLBAR_VERTICAL	2
#define FH_SCROLLBAR_HORIZONTAL	3

#define FH_STYLE_INPUT_DECIMAL	0
#define FH_STYLE_INPUT_PERCENT	1
#define FH_STYLE_INPUT_HEXCODE	2
#define FH_STYLE_INPUT_KEYWORD	3

typedef u8 fh_keyword_t;


struct fh_stylesheet {

/*  	
 *   	<type>	         <attribute_name>	  <hash> <row> <off> <input>
 */

	/* 		  DISPLAY 			                      */
	fh_keyword_t	  display_mode;		/* 0x89   1    0    KEYWORD-0 */

	/* 		  REFERENCE				    	      */
	fh_keyword_t	  reference_mode;	/* 0x77   7    1    KEYWORD-1 */

	/* 		  SIZE					    	      */
	fh_flex_t	  width;		/* 0x3b   3    2    FLEX      */
	fh_flex_t	  height;		/* 0x98   0   10    FLEX      */

	/* 		  ALIGNMENT				    	      */
	fh_keyword_t	  valignment;		/* 0x8f   7   18    KEYWORD-2 */
	fh_keyword_t	  halignment;		/* 0xbf   7   19    KEYWORD-3 */

	/* 		  SPACING 				    	      */
	fh_flex_t	  spacing_top;		/* 0xfb   3   20    FLEX      */
	fh_flex_t	  spacing_right;	/* 0x43   3   28    FLEX      */
	fh_flex_t	  spacing_bottom;	/* 0xb4   4   36    FLEX      */
	fh_flex_t	  spacing_left;		/* 0x68   0   44    FLEX      */

	/* 		  PADDING					      */
	fh_flex_t	  padding_top;		/* 0x2c   4   52    FLEX      */
	fh_flex_t	  padding_right;	/* 0x41   1   60    FLEX      */
	fh_flex_t	  padding_bottom;	/* 0x5b   3   68    FLEX      */
	fh_flex_t	  padding_left;		/* 0x9e   6   76    FLEX      */

	/* 		  BORDER					      */
	fh_keyword_t	  border_mode;		/* 0xd8   0   84    KEYWORD-4 */
	u8 		  border_width;		/* 0x56   6   85    DECIMAL   */
	u32		  border_color;		/* 0x0f   7   86    HEX-CODE  */

	/* 		  RADIUS					      */
	fh_flex_t	  radius_top_left;	/* 0x25   5   90    FLEX      */
	fh_flex_t	  radius_top_right;	/* 0xa6   6   98    FLEX      */
	fh_flex_t	  radius_bottom_right;	/* 0x5f   7  106    FLEX      */
	fh_flex_t	  radius_bottom_left;	/* 0x1e   6  114    FLEX      */

	/* 		  INFILL 					      */
	fh_keyword_t	  infill_mode;		/* 0x54   4  122    KEYWORD-5 */
	u32		  infill_color;		/* 0xf9   1  123    HEX-CODE  */

	/* 		  LAYOUT					      */
	fh_keyword_t	  layout_mode;		/* 0x3f   7  127    KEYWORD-6 */

	/* 		  SCROLLBAR				    	      */
	fh_keyword_t	  scrollbar_mode;	/* 0x02   2  128    KEYWORD-7 */

	/* 		  TEXT						      */
	fh_flex_t	  text_size;		/* 0xb0   0  129    FLEX      */
	u32		  text_color;		/* 0x93   3  137    HEX-CODE  */
	u16		  text_weight;		/* 0x0f   7  141    DECIMAL   */
} __attribute__((__packed__));


struct fh_stylesheet_attribute {
	u16 	offset;
	u8 	size;
	u8 	input;
	u8 	category;
};

#endif /* _FH_STYLE_STYLESHEET_H */
