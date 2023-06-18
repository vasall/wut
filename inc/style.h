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
	FH_STYLE_BORDER
};

struct fh_restyle_display {	
	enum fh_display_mode		mode;
};

struct fh_restyle_shape {
	u16 x;
	u16 y;
	u16 width;
	u16 height;
};

struct fh_restyle_infill {
	enum fh_infill_mode		mode;
	struct fh_color			color;

};

struct fh_restyle_layout {
	enum fh_layout_mode		mode;
};

struct fh_style {
	struct fh_style			*reference;
	struct fh_stylesheet		sheet;

	/*
	 * DISPLAY
	 */
	struct fh_restyle_display	display;

	/*
	 * SHAPE
	 */
	struct fh_rect			bounding_shape;	
	struct fh_rect			shape;		/* -Spacing */
	struct fh_rect			inner_shape;	/* -Spacing, -Padding */

	/*
	 * INFILL
	 */
	struct fh_restyle_infill	infill;

	/*
	 * LAYOUT
	 */
	struct fh_restyle_layout	layout;
};


struct fh_style_pass {
	/* The size of the window */
	struct fh_rect window;
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
