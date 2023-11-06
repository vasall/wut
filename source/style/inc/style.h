#ifndef _FH_ELEMENT_STYLE_H
#define _FH_ELEMENT_STYLE_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "style/inc/stylesheet.h"

#include "graphic/inc/color.h"
#include "graphic/inc/shape.h"

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
	u8 				mode;
};

struct fh_restyle_reference {
	u8 				mode;
};

struct fh_restyle_infill {
	u8				mode;
	struct fh_color			color;

};

struct fh_restyle_layout {
	u8				mode;
};


#define FH_RESTYLE_SCROLL_V	(1<<1)
#define FH_RESTYLE_SCROLL_H	(1<<2)

struct fh_restyle_scrollbar {
	u8				flags;
};


struct fh_style {
	struct fh_style			*ref;
	struct fh_stylesheet		sheet;

	/*
	 * DISPLAY
	 */
	struct fh_restyle_display	display;

	/*
	 * REFERENCE
	 */
	struct fh_restyle_reference	reference;

	/*
	 * SHAPE
	 */
	struct fh_rect			bounding_box;	
	struct fh_rect			element_delta;	/* -Spacing */
	struct fh_rect			content_delta;	/* -Spacing, -Padding */

	/*
	 * INFILL
	 */
	struct fh_restyle_infill	infill;

	/*
	 * LAYOUT
	 */
	struct fh_restyle_layout	layout;

	/*
	 * SCROLLBAR
	 */
	struct fh_restyle_scrollbar	scrollbar;
};


struct fh_style_pass {
	struct fh_rect *document_shape;
};


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

/*
 * Initialize a style structure.
 *
 * @style: The structure to intialize
 * @ref: The reference to use
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_XMOD s8 fh_style_init(struct fh_style *style, struct fh_style *ref);


/*
 * Set a new reference.
 * Note that after linking, the style will have to be processed.
 *
 * @style: Pointer to the style struct to link
 * @ref: Pointer to the style struct to link to
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_XMOD s8 fh_style_link(struct fh_style *style, struct fh_style *ref);


/*
 * Take in a stylesheet and a reference, apply the specified configurations and
 * write the resutling style to the output.
 *
 * @style: The style struct to process
 * @pass: A buffer to pass through for calculation
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_XMOD s8 fh_style_process(struct fh_style *style, struct fh_style_pass *pass);


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

/*
 * Reset the stylesheet.
 *
 * @style: Pointer to the style struct
 */
FH_API void fh_ResetStyle(struct fh_style *style);

/*
 * Modify the stylesheet of a style struct.
 *
 * @style: Pointer to the style struct
 * @in: A string containing the requested modifications
 */
FH_API void fh_ModifyStyle(struct fh_style *style, char *in);


/*
 * Display the attached stylesheet in the console.
 *
 * @style: Pointer to the style struct
 */
FH_API void fh_DumpStylesheet(struct fh_style *style);


/*
 * Display the active style settings in the console.
 *
 * @style: Pointer to the style struct
 */
FH_API void fh_DumpStyle(struct fh_style *style);

#endif /* _FH_ELEMENT_STYLE_H */
