#ifndef _WT_DOCUMENT_ELEMENT_RENDERING_H
#define _WT_DOCUMENT_ELEMENT_RENDERING_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "document/inc/element.h"

#include "graphic/inc/batch.h"

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

/*
 * Render an element onto a flat struct using the given renderer.
 *
 * @ren: Pointer to the batch renderer
 * @ele: Pointer to the element
 */
WT_API void wt_element_render(struct wt_batch *ren, struct wt_element *ele);


/*
 * Render the scrollbar of the element. This feature is configure through the
 * scrollbar-setting in the stylesheet.
 *
 * @ren: Pointer to the batch renderer
 * @ele: Pointer to the element
 */
WT_XMOD void wt_element_ren_scrollbar(struct wt_batch *ren, struct wt_element *ele);



#endif /* _WT_DOCUMENT_ELEMENT_RENDERING_H */
