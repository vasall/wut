#ifndef _FH_ELEMENT_STYLE_H
#define _FH_ELEMENT_STYLE_H

#include "stdinc.h"
#include "stylesheet.h"
#include "flex.h"
#include "color.h"


enum fh_style_attrib {
	FH_STYLE_DISPLAY_MODE,

	FH_STYLE_SIZE_V,
	FH_STYLE_SIZE_V_MIN,
	FH_STYLE_SIZE_V_MAX,
	FH_STYLE_SIZE_H,
	FH_STYLE_SIZE_H_MIN,
	FH_STYLE_SIZE_H_MAX,
	
	FH_POSITION_V_ORIENT,
	FH_POSITION_V,
	FH_POSITION_H_ORIENT,
	FH_POSITION_H,

	FH_STYLE_PADDING_TOP,
	FH_STYLE_PADDING_RIGHT,
	FH_STYLE_PADDING_BOTTOM,
	FH_STYLE_PADDING_LEFT,

	FH_STYLE_INFILL_MODE,
	FH_STYLE_INFILL_COLOR,

	FH_STYLE_BORDER_MODE,
	FH_STYLE_BORDER_WIDTH,
	FH_STYLE_BORDER_COLOR,

	FH_STYLE_TEXT_COLOR,
	FH_STYLE_TEXT_SIZE,
	FH_STYLE_TEXT_OPTIONS
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
	enum fh_display_mode		mode;
};

struct fh_restyle_size {
	u16 width;
	u16 height;
};

struct fh_restyle_position {
	u16 x;
	u16 y;
};

struct fh_restyle_padding {
	u16 top;
	u16 right;
	u16 bottom;
	u16 left;
};

struct fh_restyle_infill {
	enum fh_infill_mode		mode;
	struct fh_color				color;

};

struct fh_restyle_border {
	enum fh_border_mode		mode;
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

	struct fh_restyle_size		outer_size;
	struct fh_restyle_size		inner_size;

	struct fh_restyle_position	outer_position;
	struct fh_restyle_position	inner_position;

	struct fh_restyle_padding	padding;

	struct fh_restyle_infill	infill;

	struct fh_restyle_border	border;

	struct fh_restyle_text		text;
};


struct fh_style_pass {
	/* The size of the window */
	struct fh_rect window;

	/* The required size to fit the content */
	struct fh_rect content;
};


/*
 * Initialize a style structure.
 *
 * @style: The structure to intialize
 * @ref: The reference to use
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_style_init(struct fh_style *style, struct fh_style *ref);


FH_API s8 fh_style_process_size(struct fh_style *style,
		struct fh_style_pass *pass);


/*
 * Take in a stylesheet and a reference, apply the specified configurations and
 * write the resutling style to the output.
 *
 * @style: The style struct to process
 * @rect: The size of the window
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_style_process(struct fh_style *style, struct fh_rect *rect);


#endif /* _FH_ELEMENT_STYLE_H */
