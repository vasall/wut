#ifndef _WT_DOCUMENT_H
#define _WT_DOCUMENT_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "window/inc/window.h"

#include "document/inc/element.h"
#include "document/inc/element_manager.h"

#include "graphic/inc/batch.h"
#include "graphic/inc/object.h"
#include "graphic/inc/constructor.h"
#include "graphic/inc/flat.h"


struct wt_document {
	/* A pointer to the window this document belongs to */
	struct wt_window 		*window;

	/* A pointer to the context */
	struct wt_context 		*context;

	/* The body element, to which all future elements will be attached */
	struct wt_element 		*body;

	struct wt_element		*selected;
	struct wt_element		*hovered;
	
	/* A reference to the window size */
	struct wt_rect 			*shape_ref;

	/* A list of all views */
	struct wt_view_list 		*views;

	/* The main batch render */
	s16 				batch_id;

#if 0
	/* A list of all canvas widgets */
	struct wt_canvas_list *canvases
#endif
};

struct wt_ele_selector {
	s8 					state;
	char 				*name;
	struct wt_sin2 		*pos;
	struct wt_element 	*element;
};


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


/*
 * Create a new document.
 *
 * @win: Pointer to the window
 *
 * Returns: Either a pointer to a new document or NULL if an error occurred
 */
WT_API struct wt_document *wt_CreateDocument(struct wt_window *win);


/*
 * Destroy a document and free the allocated memory. This function is save to
 * call, even if doc is NULL.
 *
 * @doc: Pointer to the document
 */
WT_API void wt_DestroyDocument(struct wt_document *doc);


/*
 * This function will resize the document to fit the given reference shape.
 *
 * @doc: Pointer to the document
 */
WT_API void wt_ResizeDocument(struct wt_document *doc);


/*
 * Add a new element to the document.
 *
 * @doc: Pointer to the document
 * @parent: A pointer to the parent element
 * @name: The name of the element
 * @type: The type of the new element
 * @[data]: Optional data for the element
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WT_API struct wt_element *wt_AddElement(struct wt_document *doc,
		struct wt_element *parent, char *name,
		enum wt_element_type type, void *data);


/*
 * Remove an element from the document.
 *
 * @doc: Pointer to the document
 * @ele: Pointer to the element
 */
WT_API void wt_RemoveElement(struct wt_document *doc, struct wt_element *ele);


/*
 * Get an element from a document by searching for the given name.
 *
 * @doc: Pointer to the document
 * @name: The name of the element to look for
 *
 * Returns: Either a pointer to the element or NULL if an error occurred or no
 * 	    element could be found
 */
WT_API struct wt_element *wt_GetElement(struct wt_document *doc, char *name);


/*
 * Get the most relevant element at the given position.
 *
 * @doc: Pointer to the document
 * @pos: The position to look at
 *
 * Returns: Either a pointer to the most relevant element or NULL if an error
 * 	    occurred or no element could be found
 */
WT_API struct wt_element *wt_GetHoveredElement(struct wt_document *doc,
		struct wt_sin2 *pos);


/*
 * Only update a branch of the element tree starting from <ele>.
 *
 * @doc: Pointer to the document
 * @ele: The starting element of the branch
 */
WT_API void wt_UpdateDocumentBranch(struct wt_document *doc,
		struct wt_element *ele);


/*
 * Update the whole document.
 *
 * @doc: Pointer to the document
 */
WT_API void wt_UpdateDocument(struct wt_document *doc);


/*
 * Only render a branch of the element tree onto the UI texture, starting from
 * <ele>.
 *
 * @doc: Pointer to the document
 * @ele: The starting element of the branch
 */
WT_API void wt_RenderDocumentUIBranch(struct wt_document *doc,
		struct wt_element *ele);


/*
 * Render the whole element tree onto the UI texture.
 *
 * @doc: Pointer to the document
 */
WT_API void wt_RenderDocumentUI(struct wt_document *doc);


/*
 * Render the document onto the screen.
 *
 * @doc: Pointer to the document
 */
WT_API void wt_RenderDocument(struct wt_document *doc);


/*
 * Display the element tree starting from <ele>. If <ele> is NULL, the whole
 * tree starting from the body-element will be shown.
 *
 * @doc: Pointer to the document
 * @ele: Pointer to the starting element or NULL
 */
WT_API void wt_ShowDocumentTree(struct wt_document *doc,
		struct wt_element *ele);

#endif /* _WT_DOCUMENT_H */
