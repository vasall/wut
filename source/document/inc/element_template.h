#ifndef _WUT_DOCUMENT_ELEMENT_TEMPLATE_H
#define _WUT_DOCUMENT_ELEMENT_TEMPLATE_H

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
WUT_API s8 wut_etm_load(struct wut_Element *ele, void *data);


#endif /* _WUT_DOCUMENT_ELEMENT_TEMPLATE_H */

