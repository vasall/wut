#ifndef _WUT_DOCUMENT_ELEMENT_TEMPLATE_H
#define _WUT_DOCUMENT_ELEMENT_TEMPLATE_H

#include "source/core/inc/define.h"
#include "source/core/inc/import.h"

#include "source/document/inc/element.h"

#include "source/component/inc/dictionary.h"

/*
 * Load a predefined template for an element.
 *
 * @ele: The element to load the template to
 * @[data]: Optional dictionary containing additionaly info
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WUT_API s8 wut_etm_load(struct wut_Element *ele, struct wut_Dictionary *data);


#endif /* _WUT_DOCUMENT_ELEMENT_TEMPLATE_H */

