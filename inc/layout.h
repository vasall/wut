#ifndef _FH_LAYOUT_H
#define _FH_LAYOUT_H

#include "element.h"


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
FH_CROSS void fh_layout_blocks(struct fh_element *ele);


#endif /* _FH_LAYOUT_H */
