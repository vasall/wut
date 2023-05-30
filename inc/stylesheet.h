#ifndef _FH_ELEMENT_STYLESHEET_H
#define _FH_ELEMENT_STYLESHEET_H

#include "stdinc.h"
#include "flex.h"
#include "color.h"

enum fh_display_mode {
	FH_DISPLAY_INHERIT	= 0,
	FH_DISPLAY_NONE		= 1,
	FH_DISPLAY_BLOCK	= 2
};

enum fh_reference_mode {
	FH_REFERENCE_ABSOLUTE	= 0,
	FH_REFERENCE_RELATIVE	= 1
};

enum fh_orientation_mode {
	FH_ORIENTATION_TOP      = 0,
	FH_ORIENTATION_RIGHT	= 1,
	FH_ORIENTATION_BOTTOM   = 1,
	FH_ORIENTATION_LEFT	= 0
};


enum fh_infill_mode {
	FH_INFILL_INHERIT	= 0,
	FH_INFILL_NONE		= 1,
	FH_INFILL_COLOR		= 2
};


enum fh_border_mode {
	FH_BORDER_INHERIT	= 0,
	FH_BORDER_NONE		= 1,
	FH_BORDER_SET		= 2
};


enum fh_text_opt {
	FH_TEXT_BOLD		= (1<<0),
	FH_TEXT_ITALIC		= (1<<1)
};


enum fh_scrollbar_mode {
	FH_SCROLLBAR_NONE,
	FH_SCROLLBAR_AUTO,
	FH_SCROLLBAR_VERTICAL,
	FH_SCROLLBAR_HORIZONTAL,
	FH_SCROLLBAR_BOTH
};

struct fh_stylesheet {
	/*
	 * DISPLAY
	 */
	enum fh_display_mode		display_mode;

	/*
	 * REFERENCE
	 */
	enum fh_reference_mode		reference_mode;

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
	enum fh_orientation_mode	vorientation;
	struct fh_flex			vposition;

	enum fh_orientation_mode	horientation;
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
	enum fh_infill_mode		infill_mode;
	struct fh_color			infill_color;


	/*
	 * BORDER
	 */
	enum fh_border_mode		border_mode;
	struct fh_flex 			border_width;
	struct fh_color			border_color;


	/*
	 * TEXT
	 */
	struct fh_color			text_color;
	u8				text_size;
	u8				text_options;

	/*
	 * SCROLL WHEEL
	 */
	enum fh_scrollbar_mode		scrollbar_mode;

};



/*
 * Reset a stylesheet.
 *
 * @sheet: Pointer to the stylesheet
 */
FH_API void fh_ResetStylesheet(struct fh_stylesheet *sheet);


#endif /* _FH_ELEMENT_STYLESHEET_H */
