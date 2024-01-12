#ifndef _FH_DOCUMENT_ELEMENT_RENDERING_H
#define _FH_DOCUMENT_ELEMENT_RENDERING_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "document/inc/element.h"

#include "graphic/batching/inc/batch.h"

/*
 * The following types are used to use different rendering options in the
 * shader.
 */
#define FH_RENTYPE_DEFAULT	1	/* Block with border and color */
#define FH_RENTYPE_SCROLL_V	2	/* Vertical scrollbar */
#define FH_RENTYPE_SCROLL_H	3	/* Horizontal scrollbar */

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
FH_API void fh_element_render(struct fh_batch *ren, struct fh_element *ele);


/*
 * Render the scrollbar of the element. This feature is configure through the
 * scrollbar-setting in the stylesheet.
 *
 * @ren: Pointer to the batch renderer
 * @ele: Pointer to the element
 */
FH_XMOD void fh_element_ren_scrollbar(struct fh_batch *ren, struct fh_element *ele);



#endif /* _FH_DOCUMENT_ELEMENT_RENDERING_H */
