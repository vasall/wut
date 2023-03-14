#ifndef _FH_ELEMENT_STYLE_H
#define _FH_ELEMENT_STYLE_H

#include "define.h"
#include "import.h"
#include "flex.h"
#include "color.h"


enum fh_style_display_mode {
	FH_DISPLAY_INHERIT	= 0,
	FH_DISPLAY_NONE		= 1,
	FH_DISPLAY_BLOCK	= 2
};

enum fh_style_orientation_mode {
	FH_ORIENTATION_TOP      = 0,
	FH_ORIENTATION_RIGHT	= 1,
	FH_ORIENTATION_BOTTOM   = 1,
	FH_ORIENTATION_LEFT	= 0
};


enum fh_style_infill_mode {
	FH_INFILL_INHERIT	= 0,
	FH_INFILL_NONE		= 1,
	FH_INFILL_COLOR		= 2
};


enum fh_style_border_mode {
	FH_BORDER_INHERIT	= 0,
	FH_BORDER_NONE		= 1,
	FH_BORDER_SET		= 2
};


enum fh_style_text_opt {
	FH_TEXT_BOLD		= (1<<0),
	FH_TEXT_ITALIC		= (1<<1)
};

struct fh_stylesheet {
	/*
	 * DISPLAY
	 */
	enum fh_style_display_mode		display_mode;

	/*
	 * SIZE
	 */
	struct fh_flex			vsize;
	struct fh_flex			vsize_min;
	struct fh_flex			vsize_max;

	struct fh_flex			hsize;
	struct fh_flex			hsize_min;
	struct fh_flex			hsize_max;


	/*
	 * POSITION 
	 */
	enum fh_style_orientation_mode	vorientation;
	struct fh_flex			vposition;

	enum fh_style_orientation_mode	horientation;
	struct fh_flex			hposition;


	/*
	 * PADDING
	 */
	struct fh_flex			padding_top;
	struct fh_flex			padding_right;
	struct fh_flex			padding_bottom;
	struct fh_flex			padding_left;


	/*
	 * INFILL
	 */
	enum fh_style_infill_mode	infill_mode;
	struct fh_color			infill_color;


	/*
	 * BORDER
	 */
	enum fh_style_border_mode	border_mode;
	struct fh_flex 			border_width;
	struct fh_color			border_color;


	/*
	 * TEXT
	 */
	struct fh_color			text_color;
	u8				text_size;
	u8				text_options;
};


/*
 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *
 *
 * Compact return structures.
 * They are used when handling with active styling.
 */

enum fh_style_attribute {
	FH_STYLE_DISPLAY,
	FH_STYLE_SIZE,
	FH_STYLE_POSITION,
	FH_STYLE_PADDING,
	FH_STYLE_INFILL,
	FH_STYLE_BORDER,
	FH_STYLE_TEXT
};

struct fh_restyle_display {	
	enum fh_style_display_mode		mode;
};

struct fh_restyle_size {
	u32 width;
	u32 height;
};

struct fh_restyle_position {
	u32 x;
	u32 y;
};

struct fh_restyle_padding {
	u32 top;
	u32 right;
	u32 bottom;
	u32 left;
};

struct fh_restyle_infill {
	enum fh_style_infill_mode		mode;
	struct fh_color				color;
	
};

struct fh_restyle_border {
	enum fh_style_border_mode		mode;
	struct fh_flex 				width;
	struct fh_color				color;

};

struct fh_restyle_text {
	struct fh_color				color;
	u8					size;
	u8					options;
};


struct fh_style {
	struct fh_style			*reference;

	struct fh_stylesheet		sheet;

	struct fh_restyle_display	display;

	struct fh_restyle_size		size;

	struct fh_restyle_position	position;

	struct fh_restyle_padding	padding;

	struct fh_restyle_infill	infill;

	struct fh_restyle_border	border;

	struct fh_restyle_text		text;
};


/*
 * Reset a stylesheet.
 *
 * @sheet: Pointer to the stylesheet
 */
FH_API void fh_style_reset_sheet(struct fh_stylesheet *sheet);

/*
 * Initialize a style structure.
 *
 * @style: The structure to intialize
 * @ref: The reference to use
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_style_init(struct fh_style *style, struct fh_style *ref);


/*
 * Take in a stylesheet and a reference, apply the specified configurations and
 * write the resutling style to the output.
 *
 * @style: The style struct to process
 * @rect: The size of the window
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_style_process(struct fh_style *style, SDL_Rect *rect);


#endif /* _FH_ELEMENT_STYLE_H */
