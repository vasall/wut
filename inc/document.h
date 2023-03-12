#ifndef _FH_DOCUMENT_H
#define _FH_DOCUMENT_H

#include "define.h"
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
 * Returns: Either a pointer to a new document or NULL if an error occurred
 */
FH_API struct fh_document *fh_doc_create(void);


/*
 * Destroy a document and free the allocated memory. This function is save to
 * call, even if doc is NULL.
 *
 * @doc: Pointer to the document to destroy
 */
FH_API void fh_doc_destroy(struct fh_document *doc);


/*
 * Get an element contained in the document, by looking for the given name.
 *
 * @doc: Pointer to the document the element belongs to
 * @name: The name of the element to look for
 *
 * Returns: Either a pointer to the element if found or NULL if an error
 * 	    occurred
 */
FH_API struct fh_element *fh_doc_find_element(struct fh_document *doc, char *name);


#endif /* _FH_DOCUMENT_H */
