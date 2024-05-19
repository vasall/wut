#ifndef _WT_DOCUMENT_ELEMENT_TEMPLATE_H
#define _WT_DOCUMENT_ELEMENT_TEMPLATE_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "document/inc/element.h"


/*
 * Load a predefined template for an element.
 *
 * @ele: The element to load the template to
 * @[data]: Optional data
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WT_API s8 wt_eletemp_load(struct wt_element *ele, void *data);


#endif /* _WT_DOCUMENT_ELEMENT_TEMPLATE_H */

