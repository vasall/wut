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
FH_XMOD void fh_layout_blocks(struct fh_element *ele);


/*
 * Set the offset position of all children elements of the given element, to
 * conform to the ROWS layout.
 *
 * @ele: Pointer to the parent element
 */
FH_XMOD void fh_layout_rows(struct fh_element *ele);


/*
 * Set the offset position of all children elements of the given element, to
 * conform to the COLUMNS layout.
 *
 * @ele: Pointer to the parent element
 */
FH_XMOD void fh_layout_columns(struct fh_element *ele);


#endif /* _FH_STYLE_LAYOUT_H */
