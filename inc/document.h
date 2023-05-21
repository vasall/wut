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
	struct fh_window *window;

	/* A pointer to the context */
	struct fh_context *context;

	/* The body element, to which all future elements will be attached */
	struct fh_element *body;

	/* A reference to the window size */
	struct fh_rect *shape_ref;

	struct fh_flat *flat;
	struct fh_model *ui;


};

struct fh_ele_selector {
	s8 state;
	char *name;
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
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API struct fh_element *fh_AddElement(struct fh_document *doc,
		struct fh_element *parent, char *name,
		enum fh_element_type type);


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
 * Returns: Either a pointer to the element or NULL if an error occurred
 */
FH_API struct fh_element *fh_GetElement(struct fh_document *doc, char *name);


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
 * Only render a branch of the element tree starting from <ele>.
 *
 * @doc: Pointer to the document
 * @ele: Pointer to the element
 */
FH_API void fh_RenderDocumentBranch(struct fh_document *doc,
		struct fh_element *ele);


/*
 * Render the document and all elements in it.
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
