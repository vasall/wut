#ifndef _WT_STYLE_H
#define _WT_STYLE_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "style/inc/stylesheet.h"

#include "utility/inc/color.h"
#include "utility/inc/shape.h"

enum wt_style_attrib {
	WT_STYLE_DISPLAY_MODE,

	WT_STYLE_SIZE_V,
	WT_STYLE_SIZE_V_MIN,
	WT_STYLE_SIZE_V_MAX,
	WT_STYLE_SIZE_H,
	WT_STYLE_SIZE_H_MIN,
	WT_STYLE_SIZE_H_MAX,
	
	WT_POSITION_V_ORIENT,
	WT_POSITION_V,
	WT_POSITION_H_ORIENT,
	WT_POSITION_H,

	WT_STYLE_PADDING_TOP,
	WT_STYLE_PADDING_RIGHT,
	WT_STYLE_PADDING_BOTTOM,
	WT_STYLE_PADDING_LEFT,

	WT_STYLE_INFILL_MODE,
	WT_STYLE_INFILL_COLOR,

	WT_STYLE_BORDER_MODE,
	WT_STYLE_BORDER_WIDTH,
	WT_STYLE_BORDER_COLOR,

	WT_STYLE_TEXT_COLOR,
	WT_STYLE_TEXT_SIZE,
	WT_STYLE_TEXT_OPTIONS
};


/*
 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *
 *
 * Compact return structures.
 * They are used when handling with active styling.
 */

enum wt_restyle_type {
	WT_STYLE_DISPLAY,
	WT_STYLE_REFERENCE,
	WT_STYLE_SHAPE,
	WT_STYLE_BORDER,
	WT_STYLE_RADIUS,
	WT_STYLE_INFILL,
	WT_STYLE_LAYOUT,
	WT_STYLE_TEXT
};



#define WT_RESTYLE_SCROLL_V	(1<<1)
#define WT_RESTYLE_SCROLL_H	(1<<2)



struct wt_style {
	struct wt_style			*ref;
	struct wt_stylesheet		sheet;

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
	struct wt_rect	shape_bounding_box;	
	struct wt_rect	shape_element_delta; /* -Spacing */
	struct wt_rect	shape_inner_delta;   /* -Spacing, -Border */
	struct wt_rect	shape_content_delta; /* -Spacing, -Border, -Padding */

	/*
	 * BORDER
	 */
	u8		border_mode;
	s32		border_width;
	struct wt_color	border_color;

	/*
	 * RADIUS
	 */
	s32		radius_corner[4];

	/*
	 * INFILL
	 */
	u8		infill_mode;
	struct wt_color	infill_color;

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
	struct wt_color	text_color;
	u16		text_mass;
	u8		text_options;
	u8		text_wrap_mode;
	u16		text_spacing;
	u16		text_line_height;
};


struct wt_style_pass {
	struct wt_rect *document_shape;
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
WT_XMOD s8 wt_style_init(struct wt_style *style, struct wt_style *ref);


/*
 * Set a new reference.
 * Note that after linking, the style will have to be processed.
 *
 * @style: Pointer to the style struct to link
 * @ref: Pointer to the style struct to link to
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WT_XMOD s8 wt_style_link(struct wt_style *style, struct wt_style *ref);


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
WT_XMOD s8 wt_style_get(struct wt_style *style, enum wt_sheet_id id,
		struct wt_sheet_ret *ret);


/*
 * Take in a stylesheet and a reference, apply the specified configurations and
 * write the resutling style to the output.
 *
 * @style: The style struct to process
 * @pass: A buffer to pass through for calculation
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WT_XMOD s8 wt_style_process(struct wt_style *style, struct wt_style_pass *pass);


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
WT_API void wt_ResetStyle(struct wt_style *style);


/*
 * Modify the stylesheet of a style struct.
 *
 * @style: Pointer to the style struct
 * @in: A string containing the requested modifications
 */
WT_API void wt_ModifyStyle(struct wt_style *style, char *in);


/*
 * Get a specific restyle block like text, shape, etc.
 *
 * @style: Pointer to the style struct
 * @type: The restyle block type
 *
 * Returns: Either a pointer to the restyle wrapper or NULL
 */
WT_API void *wt_GetReStyle(struct wt_style *style, enum wt_restyle_type type);


/*
 * Display the attached stylesheet in the console.
 *
 * @style: Pointer to the style struct
 */
WT_API void wt_DumpStylesheet(struct wt_style *style);


/*
 * Display the active style settings in the console.
 *
 * @style: Pointer to the style struct
 */
WT_API void wt_DumpStyle(struct wt_style *style);

#endif /* _WT_STYLE_H */
