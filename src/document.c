#include "document.h"

#include <stdlib.h>



FH_API struct fh_document *fh_doc_create(void)
{
	struct fh_document *doc;

	if(!(doc = szalloc(sizeof(struct fh_document)))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for document");
		goto err_return;
	}

	/* Create the body element */	
	if(!(doc->body = fh_ele_create("body", FH_BODY))) {
		ALARM(ALARM_ERR, "Failed to create body for document");
		goto err_free_doc;
	}

	/* Set the attributes for the body element */
	doc->body->document = doc;
	doc->body->body = doc->body;
	doc->body->parent = NULL;

	return doc;

err_free_doc:
	sfree(doc);

err_return:
	ALARM(ALARM_ERR, "Failed to create new document");
	return NULL;
}


FH_INTERN s8 fh_cfnc_remove_elements(struct fh_element *ele, void *data)
{
	/* SILENCIO! */
	if(data) {}

	fh_ele_remove(ele);

	return 0;
}

FH_API void fh_doc_destroy(struct fh_document *doc)
{
	if(!doc)
		return;

	/* If the document contains a body, recursivly remove it */
	if(doc->body) {
		fh_ele_hlf_up(doc->body, &fh_cfnc_remove_elements, NULL);
	}


	sfree(doc);
}
