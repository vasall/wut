#ifndef _FH_STYLE_LAYOUT_H
#define _FH_STYLE_LAYOUT_H

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
FH_XMOD void fh_layout_block(struct fh_element *ele);


/*
 * Set the offset position of all children elements of the given element, to
 * conform to the ROWS layout.
 *
 * @ele: Pointer to the parent element
 */
FH_XMOD void fh_layout_row(struct fh_element *ele);


/*
 * Set the offset position of all children elements of the given element, to
 * conform to the COLUMNS layout.
 *
 * @ele: Pointer to the parent element
 */
FH_XMOD void fh_layout_column(struct fh_element *ele);


#endif /* _FH_STYLE_LAYOUT_H */
