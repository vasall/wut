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


enum fh_alignment_mode {
	FH_ALIGNMENT_INHERIT	= 0,
	FH_ALIGNMENT_TOP      	= 1,
	FH_ALIGNMENT_RIGHT	= 2,
	FH_ALIGNMENT_BOTTOM  	= 3,
	FH_ALIGNMENT_LEFT	= 4
};


enum fh_infill_mode {
	FH_INFILL_INHERIT	= 0,
	FH_INFILL_NONE		= 1,
	FH_INFILL_COLOR		= 2
};


enum fh_layout_mode {
	FH_LAYOUT_INHERIT,
	FH_LAYOUT_BLOCKS,
	FH_LAYOUT_ROWS,
	FH_LAYOUT_COLUMNS
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
	 *
	 * This attribute will indicate the display mode of the element.
	 */
	enum fh_display_mode		display_mode;

	/*
	 * REFERENCE
	 */
	enum fh_reference_mode		reference_mode;

	/*
	 * SIZE
	 * 
	 * The size of the element relative to the parent element.
	 */
	struct fh_flex			vsize;
	struct fh_flex			hsize;

	/*
	 * ALIGNMENT
	 *
	 * The orientation used for positioning.
	 */
	enum fh_alignment_mode		valignment;
	enum fh_alignment_mode		halignment;	

	/*
	 * SPACING
	 *
	 * The spacing around the element. By setting this, the margin between
	 * elements can be adjusted.
	 */
	struct fh_flex			spacing_top;
	struct fh_flex			spacing_right;
	struct fh_flex			spacing_bottom;
	struct fh_flex			spacing_left;

	/*
	 * PADDING
	 *
	 * The padding inside of the element. Using this, the space used by the
	 * children element can be set.
	 */
	struct fh_flex			padding_top;
	struct fh_flex			padding_right;
	struct fh_flex			padding_bottom;
	struct fh_flex			padding_left;

	/*
	 * INFILL
	 *
	 * The color to use for rendering the inner space of the element.
	 */
	enum fh_infill_mode		infill_mode;
	struct fh_color			infill_color;

	/*
	 * LAYOUT
	 *
	 * The way in which the children elements will be placed.
	 */
	enum fh_layout_mode		layout_mode;

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
