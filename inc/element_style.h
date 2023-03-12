#ifndef _FH_ELEMENT_STYLE_H
#define _FH_ELEMENT_STYLE_H

#include "define.h"
#include "import.h"
#include "color.h"

enum fh_element_mode_display {
	FH_DISPLAY_NONE		= 0,
	FH_DISPLAY_INHERIT	= 1,
	FH_DISPLAY_BLOCK	= 2
};


enum fh_element_mode_size {
	FH_SIZE_AUTO		= 0,
	FH_SIZE_RELATIVE	= 1,
	FH_SIZE_ABSOLUTE	= 2
};

struct fh_element_attr_size {
	u8 data[16];

	union {
		u32 width_abs;
		u32 height_abs;

		f32 width_rel;
		f32 height_rel;
	} shape;
};


enum fh_element_mode_position {
	FH_POSITION_AUTO	= 0,
	FH_POSITION_RELATIVE	= 1,
	FH_POSITION_ABSOLUTE	= 2
};

enum fh_element_mode_horient {
	FH_ORIENTATION_LEFT	= 0,
	FH_ORIENTATION_RIGHT	= 1
};


enum fh_element_mode_infill {
	FH_INFILL_NONE		= 0,
	FH_INFILL_INHERIT	= 1,
	FH_INFILL_COLOR		= 2
};


enum fh_element_mode_border {
	FH_BORDER_NONE		= 0,
	FH_BORDER_INHERIT	= 1,
	FH_BORDER_SET		= 2
};


enum fh_element_mode_margin {
	Fh_MARGIN_AUTO		= 0,
	FH_MARGIN_INHERIT	= 1,
	FH_MARGIN_SET		= 2
};


struct fh_element_attr_margin {
	u16 data[16];

	union {
		u16 top;
		u16 right;
		u16 bottom;
		u16 left;
	} dir;
};


enum fh_element_mode_padding {
	FH_PADDING_AUTO		= 0,
	FH_PADDING_INHERIT	= 1,
	FH_PADDING_SET		= 2
};

struct fh_element_attr_padding {
	u16 data[16];

	union {
		u16 top;
		u16 right;
		u16 bottom;
		u16 left;
	} dir;
};


enum fh_element_opt_text {
	FH_TEXT_BOLD,
	FH_TEXT_ITALIC
};


struct fh_element_attributes {
	/*
	 * DISPLAY
	 */
	enum fh_element_mode_display	display_mode;

	/*
	 * SIZE
	 */
	enum fh_element_mode_size	size_mode;
	struct fh_element_attr_size	size;


	/*
	 * POSITION 
	 */



	/*
	 * INFILL
	 */
	enum fh_element_mode_infill	infill_mode;
	struct fh_color			infill_color;


	/*
	 * BORDER
	 */
	enum fh_element_mode_border	border_mode;
	u8 				border_width;
	struct fh_color			border_color;

	
	/*
	 * MARGIN
	 */
	enum fh_element_mode_margin	margin_mode;
	struct fh_element_attr_margin	margin;


	/*
	 * PADDING
	 */
	enum fh_element_mode_padding	padding_mode;
	struct fh_element_attr_padding	padding;


	/*
	 * TEXT
	 */
	struct fh_color			text_color;
	u8				text_size;
	u8				text_options;	

	
	/* 
	 * HORIZONTAL ALIGN
	 */

	/*
	 * VERTICAL ALIGN
	 */

};



#endif /* _FH_ELEMENT_STYLE_H */
