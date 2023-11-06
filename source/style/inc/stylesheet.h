#ifndef _FH_STYLE_STYLESHEET_H
#define _FH_STYLE_STYLESHEET_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "graphic/inc/flex.h"
#include "utility/inc/color.h"


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


struct fh_stylesheet {

/*  	
 *   	<type>	<attribute_name>	   <hash> <row> <off>	<input>
 */

	/* 	DISPLAY 			            		      */
	u8	display_mode;		/* 0x89     1 	 0    	KEYWORD-0     */

	/* 	REFERENCE				    		      */
	u8	reference_mode;		/* 0x77     7    1    	KEYWORD-1     */

	/* 	SIZE					    		      */
	f32	vsize;			/* 0xf8     0    2 	PERCENT       */
	f32	hsize;			/* 0x95     5    6 	PERCENT       */

	/* 	ALIGNMENT				    		      */
	u8	valignment;		/* 0x8f     7   10 	KEYWORD-2     */
	u8	halignment;		/* 0xbf     7   11 	KEYWORD-3     */

	/* 	SPACING 				    		      */
	f32	spacing_top;		/* 0xfb     3   12 	PERCENT       */
	f32	spacing_right;		/* 0x43     3   16 	PERCENT       */
	f32	spacing_bottom;		/* 0xb4     4   20 	PERCENT       */
	f32	spacing_left;		/* 0x68     0   24 	PERCENT       */

	/* 	PADDING					    		      */
	f32	padding_top;		/* 0x2c     4   28 	PERCENT       */
	f32	padding_right;		/* 0x41     1   32 	PERCENT       */
	f32	padding_bottom;		/* 0x5b     3   36 	PERCENT       */
	f32	padding_left;		/* 0x9e     6   40 	PERCENT       */

	/* 	BORDER					    		      */
	u8	border_mode;		/* 0xd8     0   44 	KEYWORD-4     */
	u8 	border_width;		/* 0x56     6   45 	DECIMAL	      */
	u32	border_color;		/* 0x0f     7	46 	HEX-CODE      */

	/* 	RADIUS					    		      */
	u8	radius_top_left;	/* 0x25     5   50 	DECIMAL	      */
	u8	radius_top_right;	/* 0xa6     6   51 	DECIMAL	      */
	u8	radius_bottom_right;	/* 0x5f     7   52 	DECIMAL	      */
	u8	radius_bottom_left;	/* 0x1e     6   53 	DECIMAL	      */

	/* 	INFILL 					    		      */
	u8	infill_mode;		/* 0x54     4   54 	KEYWORD-5     */
	u32	infill_color;		/* 0xf9     1   55 	HEX-CODE      */

	/* 	LAYOUT					    		      */
	u8	layout_mode;		/* 0x3f     7   59 	KEYWORD-6     */

	/* 	SCROLLBAR				    		      */
	u8	scrollbar_mode;		/* 0x02     2   60 	KEYWORD-7     */
	u32	scrollbar_track_color;	/* 0x34	    4	61	HEX-CODE      */
	u32	scrollbar_thumb_color;	/* 0x2e	    6   65	HEX-CODE      */
} __attribute__((__packed__));


struct fh_stylesheet_attribute {
	u16 	offset;
	u8 	size;
	u8 	input;
	u8 	category;
};

#endif /* _FH_STYLE_STYLESHEET_H */
