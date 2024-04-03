#ifndef _FH_STYLE_H
#define _FH_STYLE_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "style/inc/stylesheet.h"

#include "utility/inc/color.h"
#include "utility/inc/shape.h"

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

enum fh_restyle_type {
	FH_STYLE_DISPLAY,
	FH_STYLE_REFERENCE,
	FH_STYLE_SHAPE,
	FH_STYLE_BORDER,
	FH_STYLE_RADIUS,
	FH_STYLE_INFILL,
	FH_STYLE_LAYOUT,
	FH_STYLE_TEXT
};



#define FH_RESTYLE_SCROLL_V	(1<<1)
#define FH_RESTYLE_SCROLL_H	(1<<2)



struct fh_style {
	struct fh_style			*ref;
	struct fh_stylesheet		sheet;

	/*
	 * DISPLAY
	 */
	u8 		display_mode;

	/*
	 * REFERENCE
	 */
	u8 		reference_mode;

	/*
	 * SHAPE
	 */
	struct fh_rect	shape_bounding_box;	
	struct fh_rect	shape_element_delta; /* -Spacing */
	struct fh_rect	shape_inner_delta;   /* -Spacing, -Border */
	struct fh_rect	shape_content_delta; /* -Spacing, -Border, -Padding */

	/*
	 * BORDER
	 */
	u8		border_mode;
	s32		border_width;
	struct fh_color	border_color;

	/*
	 * RADIUS
	 */
	s32		radius_corner[4];

	/*
	 * INFILL
	 */
	u8		infill_mode;
	struct fh_color	infill_color;

	/*
	 * LAYOUT
	 */
	u8		layout_mode;

	/* 
	 * SCROLLBAR
	 */
	u8		scrollbar_flags;

	/*
	 * TEXT
	 */
	u16 		text_size;
	struct fh_color	text_color;
	u16		text_mass;
	u8		text_options;
	u8		text_wrap_mode;
	u16		text_spacing;
	u16		text_line_height;
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
 * This function will aquire an attribute from the stylesheet. To do that, a
 * stylesheet has to be found which contains the wanted attribute. For that
 * purpose it will climb up the style-ladder until a stylesheet has been found
 * with the wanted attribute.
 *
 * @style: Pointer to the initial style struct
 * @id: The id of the attribute
 * @ret: The return struct
 *
 * Returns: 1 if the attribute has been found, 0 if not and -1 if an error
 * 	    occurred
 */
FH_XMOD s8 fh_style_get(struct fh_style *style, enum fh_sheet_id id,
		struct fh_sheet_ret *ret);


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
 * Reset the style struct and stylesheet.
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
 * Get a specific restyle block like text, shape, etc.
 *
 * @style: Pointer to the style struct
 * @type: The restyle block type
 *
 * Returns: Either a pointer to the restyle wrapper or NULL
 */
FH_API void *fh_GetReStyle(struct fh_style *style, enum fh_restyle_type type);


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

#endif /* _FH_STYLE_H */
