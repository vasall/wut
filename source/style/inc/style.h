#ifndef _WUT_STYLE_H
#define _WUT_STYLE_H

struct wut_Style;

#include "core/inc/define.h"

#include "style/inc/stylesheet.h"

#include "utility/inc/color.h"

#include "math/inc/polygon.h"



/*
 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *
 *
 * Compact return structures.
 * They are used when handling with active styling.
 */

enum wut_eRestyleType {
        WUT_STYLE_DISPLAY,
        WUT_STYLE_REFERENCE,
        WUT_STYLE_SHAPE,
        WUT_STYLE_BORDER,
        WUT_STYLE_RADIUS,
        WUT_STYLE_INFILL,
        WUT_STYLE_LAYOUT,
        WUT_STYLE_TEXT
};



#define WUT_RESTYLE_SCROLL_V	(1<<1)
#define WUT_RESTYLE_SCROLL_H	(1<<2)



struct wut_Style {
        struct wut_Style		*ref;
        struct wut_Stylesheet		sheet;

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
        wut_iRect	shape_bounding_box;	
        wut_iRect	shape_element_delta; /* -Spacing */
        wut_iRect	shape_inner_delta;   /* -Spacing, -Border */
        wut_iRect	shape_content_delta; /* -Spacing, -Border, -Padding */

        /*
         * BORDER
         */
        u8		        border_mode;
        s32		        border_width;
        struct wut_Color	border_color;

        /*
         * RADIUS
         */
        s32		        radius_corner[4];

        /*
         * INFILL
         */
        u8		        infill_mode;
        struct wut_Color	infill_color;

        /*
         * LAYOUT
         */
        u8		        layout_mode;

        /* 
         * SCROLLBAR
         */
        u8		        scrollbar_flags;

        /*
         * TEXT
         */
        u16 		        text_size;
        struct wut_Color	text_color;
        u16		        text_mass;
        u8		        text_options;
        u8		        text_wrap_mode;
        u16		        text_spacing;
        u16		        text_line_height;
};


struct wut_StylePass {
        wut_iRect *document_shape;
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
WUT_XMOD s8 wut_stl_init(struct wut_Style *style, struct wut_Style *ref);


/*
 * Set a new reference to a parent style struct, to enable inheriting
 * attribute-values which have not been set for this style struct.
 * Note that after linking, the style will have to be processed.
 *
 * @style: Pointer to the style struct to link
 * @ref: Pointer to the style struct to link to
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WUT_XMOD s8 wut_stl_link(struct wut_Style *style, struct wut_Style *ref);


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
WUT_XMOD s8 wut_stl_get(struct wut_Style *style, enum wut_eSheetAttribId id,
                struct wut_SheetEntry *ret);


/*
 * Take in a stylesheet and a reference, apply the specified configurations and
 * write the resutling style to the output.
 *
 * @style: The style struct to process
 * @pass: A buffer to pass through for calculation
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WUT_XMOD s8 wut_stl_process(struct wut_Style *style, struct wut_StylePass *pass);


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
WUT_API void wut_ResetStyle(struct wut_Style *style);


/*
 * Modify the stylesheet of a style struct.
 *
 * @style: Pointer to the style struct
 * @in: A string containing the requested modifications
 */
WUT_API void wut_ModifyStyle(struct wut_Style *style, char *in);


/*
 * Get a specific restyle block like shape, color, etc.
 *
 * @style: Pointer to the style struct
 * @type: The restyle block type
 *
 * Returns: Either a pointer to the restyle wrapper or NULL
 */
WUT_API void *wut_GetReStyle(struct wut_Style *style, enum wut_eRestyleType type);


/*
 * Display the attached stylesheet in the console.
 *
 * @style: Pointer to the style struct
 */
WUT_API void wut_DumpStylesheet(struct wut_Style *style);


/*
 * Display the active style settings in the console.
 *
 * @style: Pointer to the style struct
 */
WUT_API void wut_DumpStyle(struct wut_Style *style);

#endif /* _WUT_STYLE_H */
