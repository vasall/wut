#ifndef _WUT_STYLE_STYLESHEET_H
#define _WUT_STYLE_STYLESHEET_H

struct wut_Stylesheet;

#include "core/inc/define.h"

#include "style/inc/stylesheet_attribute.h"
#include "style/inc/class.h"

#include "utility/inc/color.h"
#include "utility/inc/flex.h"

/*
 * IMPORTANT:
 * When reading from or writing to the stylesheet, always use the appropriate
 * functions sht_read() and sht_write(). Never write to the stylesheet directly!
 * This is especially important when working with the flex-pointer.
 * The only exception to that rule is wut_sht_reset(). 
 */


struct wut_Stylesheet {
        /* The attribute mask */
        u64 		        mask;

        /*  	 DISPLAY						   */
        u8		        display_mode;

        /*  	 SIZE							   */
        struct wut_Flex	        *width;
        struct wut_Flex	        *height;

        /*  	 REFERENCE						   */
        u8		        reference_mode;

        /*  	 SPACING						   */
        struct wut_Flex	        *spacing_top;
        struct wut_Flex	        *spacing_right;
        struct wut_Flex	        *spacing_bottom;
        struct wut_Flex	        *spacing_left;

        /*  	 PADDING						   */
        struct wut_Flex	        *padding_top;
        struct wut_Flex	        *padding_right;
        struct wut_Flex	        *padding_bottom;
        struct wut_Flex	        *padding_left;

        /*  	 BORDER							   */
        u8		        border_mode;
        struct wut_Flex	        *border_width;
        struct wut_Color	border_color;

        /*  	 RADIUS							   */
        struct wut_Flex 	*radius_top_left;
        struct wut_Flex 	*radius_top_right;
        struct wut_Flex 	*radius_bottom_right;
        struct wut_Flex 	*radius_bottom_left;

        /*  	 INFILL							   */
        u8		        infill_mode;
        struct wut_Color	infill_color;

        /*  	 LAYOUT							   */
        u8		        layout_mode;

        /*  	 ALIGNMENT						   */
        u8		        align_v;
        u8		        align_h;

        /*  	 SCROLLBAR						   */
        u8	        	scrollbar_mode;

        /*	 TEXT							   */
        struct wut_Flex	        *text_size;
        struct wut_Color	text_color;
        struct wut_Flex	        *text_mass;
        u8		        text_options;
        u8		        text_wrap_mode;
        struct wut_Flex	        *text_spacing;
        struct wut_Flex	        *line_height;
} __attribute__((__packed__));



/*
 * Reset a stylesheet and free all potentially allocated memory.
 *
 * @sheet: Pointer to the stylesheet
 */
WUT_XMOD void wut_sht_reset(struct wut_Stylesheet *sheet);


/*
 * Parse an input string and update the stylesheet accordingly.
 *
 * @sheet: Pointer to the stylesheet
 * @s: The input string
 */
WUT_XMOD void wut_sht_parse(struct wut_Stylesheet *sheet, char *s);


/*
 * Write an entry to the stylesheet. If the attribute has already been set, it
 * will be overwritten.
 *
 * @sheet: Pointer to the stylesheet
 * @ent: The parsed attribute entry
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WUT_XMOD s8 wut_sht_set(struct wut_Stylesheet *sheet,
                struct wut_SheetEntry *ret);


/*
 * Retrieve the value of an attribute from the stylesheet. Watch out as all flex
 * attributes are pointers, and by reading the pointer is just copied. So
 * accidental overwrites are possible when working with the retuned pointers.
 *
 * @sheet: Pointer to the stylesheet
 * @id: The id of the attribute to get
 * @out: The return struct
 *
 * Returns: 1 if the attribute value has been returned, 0 if the requested
 * 	    attribute is not available and -1 if an error occurred
 */
WUT_XMOD s8 wut_sht_get(struct wut_Stylesheet *sheet, enum wut_eSheetAttribId id,
                struct wut_SheetEntry *out);


/*
 * Apply a class to the stylesheet and write all the attributes.
 *
 * @sht: pointer to the stylesheet
 * @cls: Pointer to the class
 */
WUT_XMOD void wut_sht_apply(struct wut_Stylesheet *sht, struct wut_Class *cls);

/*
 * Print the stylesheet in the console.
 *
 * @sheet: Pointer to the stylesheet
 */
WUT_XMOD void wut_sht_print(struct wut_Stylesheet *sheet);


#endif /* _WUT_STYLE_STYLESHEET_H */
