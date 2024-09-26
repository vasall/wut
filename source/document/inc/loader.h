#ifndef _WUT_DOCUMENT_LOADER_H
#define _WUT_DOCUMENT_LOADER_H

#include "core/inc/define.h"

#include "document/inc/document.h"
#include "document/inc/element.h"


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

/*
 * Load an element-tree from a file.
 *
 * @doc: Pointer to the document
 * @pth: Pointer to the file containing the element structure
 * @[atc]: The attachment element or NULL for body
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WUT_API s8 wut_LoadElements(struct wut_Document *doc, char *pth,
                struct wut_Element *ele);

/*
 * Load classes from a style file and add them to the class table.
 *
 * @doc: Pointer to the document
 * @pth: The path to the style file
 *
 * Returns: 0 on success or -1 if na error occurred
 */
WUT_API s8 wut_LoadClasses(struct wut_Document *doc, char *pth);

#endif /* _WUT_DOCUMENT_LOADER_H */
