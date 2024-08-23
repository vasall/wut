#ifndef _WUT_STYLE_LAYOUT_H
#define _WUT_STYLE_LAYOUT_H

#include "document/inc/element.h"


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

/*
 * Set the offset position of all children elements of the given one, to conform
 * to the BLOCKS layout.
 *
 * @ele: Pointer to the parent element
 */
WUT_XMOD void wut_lay_block(struct wut_Element *ele);


/*
 * Set the offset position of all children elements of the given element, to
 * conform to the ROWS layout.
 *
 * @ele: Pointer to the parent element
 */
WUT_XMOD void wut_lay_row(struct wut_Element *ele);


/*
 * Set the offset position of all children elements of the given element, to
 * conform to the COLUMNS layout.
 *
 * @ele: Pointer to the parent element
 */
WUT_XMOD void wut_lay_column(struct wut_Element *ele);


#endif /* _WUT_STYLE_LAYOUT_H */
