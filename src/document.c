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



FH_API void fh_doc_destroy(struct fh_document *doc)
{
	if(!doc)
		return;

	/* If the document contains a body, recursivly remove it */
	fh_ele_remove(doc->body);	

	sfree(doc);
}


struct fh_ele_selector {
	s8 state;
	char *name;
	struct fh_element *element;
};

FH_INTERN s8 fh_cfnc_find_element(struct fh_element *ele, void *data)
{
	struct fh_ele_selector *sel = (struct fh_ele_selector *)data;

	if(sel->state == 1)
		return 1;

	if(strcmp(ele->name, sel->name) == 0) {
		sel->state = 1;
		sel->element = ele;
		return 1;
	}

	return 0;
}


FH_API struct fh_element *fh_doc_find_element(struct fh_document *doc, char *name)
{
	struct fh_ele_selector sel;

	if(!doc || !name) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	sel.state = 0;
	sel.name = name;
	sel.element = NULL;
	
	/* Recursivly search for the element in the document */
	fh_ele_hlf_down(doc->body, &fh_cfnc_find_element, &sel);

	if(sel.state == 1) {
		return sel.element;
	}


err_return:
	return NULL;
}
