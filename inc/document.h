#ifndef _FH_DOCUMENT_H
#define _FH_DOCUMENT_H

#include "define.h"
#include "datatype.h"
#include "import.h"
#include "window.h"
#include "element.h"
#include "model.h"
#include "model_constructor.h"
#include "flat.h"


struct fh_document {
	/* A pointer to the window this document belongs to */
	struct fh_window 	*window;

	/* A pointer to the context */
	struct fh_context 	*context;

	/* The body element, to which all future elements will be attached */
	struct fh_element 	*body;


	struct fh_element	*selected;
	struct fh_element	*hovered;
	

	/* A reference to the window size */
	struct fh_rect 		*shape_ref;

	/* The flat surface used for rendering */
	struct fh_flat 		*flat;

	struct fh_model 	*ui;

	/* A list of all views */
	struct fh_view_list 	*views;

#if 0
	/* A list of all canvas widgets */
	struct fh_canvas_list *canvases
#endif
};

struct fh_ele_selector {
	s8 state;
	char *name;
	struct fh_sin2 *pos;
	struct fh_element *element;
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
FH_API struct fh_document *fh_CreateDocument(struct fh_window *win);


/*
 * Destroy a document and free the allocated memory. This function is save to
 * call, even if doc is NULL.
 *
 * @doc: Pointer to the document
 */
FH_API void fh_DestroyDocument(struct fh_document *doc);


/*
 * This function will resize the document to fit the given reference shape.
 *
 * @doc: Pointer to the document
 */
FH_API void fh_ResizeDocument(struct fh_document *doc);


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
FH_API struct fh_element *fh_AddElement(struct fh_document *doc,
		struct fh_element *parent, char *name,
		enum fh_element_type type, void *data);


/*
 * Remove an element from the document.
 *
 * @doc: Pointer to the document
 * @ele: Pointer to the element
 */
FH_API void fh_RemoveElement(struct fh_document *doc, struct fh_element *ele);


/*
 * Get an element from a document by searching for the given name.
 *
 * @doc: Pointer to the document
 * @name: The name of the element to look for
 *
 * Returns: Either a pointer to the element or NULL if an error occurred or no
 * 	    element could be found
 */
FH_API struct fh_element *fh_GetElement(struct fh_document *doc, char *name);


/*
 * Get the most relevant element at the given position.
 *
 * @doc: Pointer to the document
 * @pos: The position to look at
 *
 * Returns: Either a pointer to the most relevant element or NULL if an error
 * 	    occurred or no element could be found
 */
FH_API struct fh_element *fh_GetHoveredElement(struct fh_document *doc,
		struct fh_sin2 *pos);


/*
 * Only update a branch of the element tree starting from <ele>.
 *
 * @doc: Pointer to the document
 * @ele: The starting element of the branch
 */
FH_API void fh_UpdateDocumentBranch(struct fh_document *doc,
		struct fh_element *ele);


/*
 * Update the whole document.
 *
 * @doc: Pointer to the document
 */
FH_API void fh_UpdateDocument(struct fh_document *doc);


/*
 * Only render a branch of the element tree onto the UI texture, starting from
 * <ele>.
 *
 * @doc: Pointer to the document
 * @ele: The starting element of the branch
 */
FH_API void fh_RenderDocumentUIBranch(struct fh_document *doc,
		struct fh_element *ele);


/*
 * Render the whole element tree onto the UI texture.
 *
 * @doc: Pointer to the document
 */
FH_API void fh_RenderDocumentUI(struct fh_document *doc);


/*
 * Render the document onto the screen.
 *
 * @doc: Pointer to the document
 */
FH_API void fh_RenderDocument(struct fh_document *doc);


/*
 * Display the element tree starting from <ele>. If <ele> is NULL, the whole
 * tree starting from the body-element will be shown.
 *
 * @doc: Pointer to the document
 * @ele: Pointer to the starting element or NULL
 */
FH_API void fh_ShowDocumentTree(struct fh_document *doc,
		struct fh_element *ele);

#endif /* _FH_DOCUMENT_H */
