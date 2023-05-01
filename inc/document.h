#ifndef _FH_DOCUMENT_H
#define _FH_DOCUMENT_H

#include "define.h"
#include "datatype.h"
#include "import.h"
#include "window.h"
#include "element.h"


struct fh_document {
	/* A pointer to the window this document belongs to */
	struct fh_window *window;

	/* The body element, to which all future elements will be attached */
	struct fh_element *body;
};



/*
 * Create a new document.
 *
 * @win: Pointer to the window
 *
 * Returns: Either a pointer to a new document or NULL if an error occurred
 */
FH_API struct fh_document *fh_doc_create(struct fh_window *win);


/*
 * Destroy a document and free the allocated memory. This function is save to
 * call, even if doc is NULL.
 *
 * @doc: Pointer to the document
 */
FH_API void fh_doc_destroy(struct fh_document *doc);


/*
 * Add a new element to a document.
 *
 * @doc: Pointer to the document
 * @parent: Pointer to the parent
 * @name: The name of the element
 * @type: The type of the element
 *
 * Returns: Either a pointer to the newly created element or NULL if an error
 * 	    occurred
 */
FH_API struct fh_element *fh_doc_add_element(struct fh_document *doc,
		struct fh_element *parent, char *name,
		enum fh_element_type type);


/*
 * Get an element contained in the document, by looking for the given name.
 *
 * @doc: Pointer to the document
 * @name: The name of the element to look for
 *
 * Returns: Either a pointer to the element if found or NULL if an error
 * 	    occurred
 */
FH_API struct fh_element *fh_doc_find_element(struct fh_document *doc, char *name);


/*
 * Update the elements in the document from ele downwards.
 *
 * @ele: Pointer to the start element
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_doc_update_part(struct fh_element *ele);


/*
 * Update all element in the document.
 *
 * @doc: Pointer to the document
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_doc_update(struct fh_document *doc);


/*
 * Finally draw the document surface onto the window.
 *
 * @doc: Pointer to the document
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_doc_render(struct fh_document *doc);



/*
 * This function will display the element-tree in the console for the given
 * document.
 *
 * @doc: Pointer to the document
 */
FH_API void fh_doc_tree(struct fh_document *doc);



#endif /* _FH_DOCUMENT_H */
