#ifndef _WT_STYLE_LAYOUT_H
#define _WT_STYLE_LAYOUT_H

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
WT_XMOD void wt_layout_block(struct wt_element *ele);


/*
 * Set the offset position of all children elements of the given element, to
 * conform to the ROWS layout.
 *
 * @ele: Pointer to the parent element
 */
WT_XMOD void wt_layout_row(struct wt_element *ele);


/*
 * Set the offset position of all children elements of the given element, to
 * conform to the COLUMNS layout.
 *
 * @ele: Pointer to the parent element
 */
WT_XMOD void wt_layout_column(struct wt_element *ele);


#endif /* _WT_STYLE_LAYOUT_H */
