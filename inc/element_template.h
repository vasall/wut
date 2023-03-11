#ifndef _FH_ELEMENT_TEMPLATE_H
#define _FH_ELEMENT_TEMPLATE_H

#include "define.h"
#include "imports.h"
#include "element.h"


/*
 * Load a predefined template for an element.
 *
 * @ele: The element to load the template to
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_eletemp_load(struct fh_element *ele);




#endif /* _FH_ELEMENT_TEMPLATE_H */

