#ifndef _FH_STYLESHEET_H
#define _FH_STYLESHEET_H

#include "stdinc.h"
#include "flex.h"
#include "color.h"


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
#define FH_SCROLLBAR_BOTH	4


struct fh_stylesheet {

/*  	
 *   	<type>	<attribute_name>	   <hash> <row> <off>	<input>
 */

	/* 	DISPLAY 			            		      */
	u8	display_mode;		/* 0x89     1 	  0    	OPTION-STRING */

	/* 	REFERENCE				    		      */
	u8	reference_mode;		/* 0x77     7    1    	OPTION-STRING */

	/* 	SIZE					    		      */
	f32	vsize;			/* 0xf8     0    2 	PERCENTAGE    */
	f32	hsize;			/* 0x95     5    7 	PERCENTAGE    */

	/* 	ALIGNMENT				    		      */
	u8	valignment;		/* 0x8f     7   13 	OPTION-STRING */
	u8	halignment;		/* 0xbf     7   14 	OPTION-STRING */

	/* 	SPACING 				    		      */
	f32	spacing_top;		/* 0xfb     3   15 	PERCENTAGE    */
	f32	spacing_right;		/* 0x43     3   20 	PERCENTAGE    */
	f32	spacing_bottom;		/* 0xb4     4   25 	PERCENTAGE    */
	f32	spacing_left;		/* 0x68     0   30 	PERCENTAGE    */

	/* 	PADDING					    		      */
	f32	padding_top;		/* 0x2c     4   35 	PERCENTAGE    */
	f32	padding_right;		/* 0x41     1   40 	PERCENTAGE    */
	f32	padding_bottom;		/* 0x5b     3   45 	PERCENTAGE    */
	f32	padding_left;		/* 0x9e     6   50 	PERCENTAGE    */

	/* 	BORDER					    		      */
	u8	border_mode;		/* 0xd8     0   55 	OPTION-STRING */
	u8 	border_width;		/* 0x56     6   56 	DECIMAL	      */
	u32	border_color;		/* 0x0f     7	57 	HEX-CODE      */

	/* 	RADIUS					    		      */
	u8	radius_top_left;	/* 0x25     5   61 	DECIMAL	      */
	u8	radius_top_right;	/* 0xa6     6   62 	DECIMAL	      */
	u8	radius_bottom_left;	/* 0x5f     7   63 	DECIMAL	      */
	u8	radius_bottom_right;	/* 0x1e     6   64 	DECIMAL	      */

	/* 	INFILL 					    		      */
	u8	infill_mode;		/* 0x54     4   65 	OPTION-STRING */
	u32	infill_color;		/* 0xf9     1   66 	HEX-CODE      */

	/* 	LAYOUT					    		      */
	u8	layout_mode;		/* 0x3f     7   70 	OPTION-STRING */

	/* 	SCROLLBAR				    		      */
	u8	scrollbar_mode;		/* 0x02     2   71 	OPTION-STRING */
#if 1
};
#else
} __attribute__((__packed__));
#endif


struct fh_stylesheet_hme {
	unsigned short hash;
	unsigned short offset;
};


struct fh_stylesheet_hm {
	struct fh_stylesheet_hme *entries;
	int number;
};


struct  fh_stylesheet_hme fh_c_stylesheet_hm_row0[]  = {
	{0xa8e0,  0}
};
struct  fh_stylesheet_hme fh_c_stylesheet_hm_row2[]  = {
	{0xe2b2,  7}
};
struct  fh_stylesheet_hme fh_c_stylesheet_hm_row3[]  = {
	{0x7c43, 14}, {0x7883, 15}, {0x1033, 20}
};
struct  fh_stylesheet_hme fh_c_stylesheet_hm_row4[]  = {
	{0x96c4, 45}
};
struct  fh_stylesheet_hme fh_c_stylesheet_hm_row5[]  = {
	{0x89b5, 70}
};
struct  fh_stylesheet_hme fh_c_stylesheet_hm_row6[]  = {
	{0x4e66, 25}, {0x5006, 56}, {0x9366, 66}, {0xbb46, 71}
};
struct  fh_stylesheet_hme fh_c_stylesheet_hm_row7[]  = {
	{0xb477, 35}, {0x1a27, 40}, {0xed37, 61}
};
struct  fh_stylesheet_hme fh_c_stylesheet_hm_row9[]  = {
	{0xb4e9, 50}, {0xbf19, 55}
};
struct  fh_stylesheet_hme fh_c_stylesheet_hm_row10[] = {
	{0x5c4a, 62}};
struct  fh_stylesheet_hme fh_c_stylesheet_hm_row11[] = {
	{0xf39b, 13}, {0x441b, 63}
};
struct  fh_stylesheet_hme fh_c_stylesheet_hm_row12[] = {
	{0x67ec, 65}
};
struct  fh_stylesheet_hme fh_c_stylesheet_hm_row13[] = {
	{0xfa6d, 30}
};
struct  fh_stylesheet_hme fh_c_stylesheet_hm_row14[] = {
	{0x3abe,  2}, {0xd03e, 57}, {0x8f8e, 64}
};
struct  fh_stylesheet_hme fh_c_stylesheet_hm_row15[] = {
	{0x97df,  1}
};

const struct fh_stylesheet_hm fh_c_stylesheet_hm[] = {
	{fh_c_stylesheet_hm_row0,  1},
	{NULL,  0}, 
	{fh_c_stylesheet_hm_row2,  1}, 
	{fh_c_stylesheet_hm_row3,  3},
	{fh_c_stylesheet_hm_row4,  1}, 
	{fh_c_stylesheet_hm_row5,  1}, 
	{fh_c_stylesheet_hm_row6,  4}, 
	{fh_c_stylesheet_hm_row7,  3},
	{NULL,  0}, 
	{fh_c_stylesheet_hm_row9,  2}, 
	{fh_c_stylesheet_hm_row10, 1}, 
	{fh_c_stylesheet_hm_row11, 2},
	{fh_c_stylesheet_hm_row12, 1}, 
	{fh_c_stylesheet_hm_row13, 1}, 
	{fh_c_stylesheet_hm_row14, 3}, 
	{fh_c_stylesheet_hm_row15, 1},
};


#endif /* _FH_STYLESHEET_H */
