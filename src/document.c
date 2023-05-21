#include "document.h"

#include "alarm.h"
#include "system.h"

#include <stdlib.h>


FH_INTERN s8 doc_cfnc_remove(struct fh_element *ele, void *data)
{
	fh_Ignore(data);

	/* First detach the element */
	fh_DetachElement(ele);

	/* Then destroy it */
	fh_DestroyElement(ele);

	return 0;
}

FH_INTERN s8 doc_cfnc_find(struct fh_element *ele, void *data)
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


FH_INTERN s8 doc_cfnc_update(struct fh_element *ele, void *data)
{
	fh_Ignore(data);

	fh_UpdateElement(ele);

	return 0;
}


FH_INTERN s8 doc_cfnc_render(struct fh_element *ele, void *data)
{
	fh_Ignore(data);

	fh_RenderElement(ele);

	return 0;
}


FH_INTERN s8 doc_cfnc_show(struct fh_element  *ele, void *data)
{
	u8 i;

	fh_Ignore(data);

	for(i = 0; i < ele->layer; i++)
		printf("  ");

	printf("%s ", ele->name);

	printf("[");
	printf("x: %d, ", ele->style.position.x);
	printf("y: %d, ", ele->style.position.y);
	printf("w: %d, ", ele->style.size.width);
	printf("h: %d", ele->style.size.height);
	printf("]\n");

	return 0;
	
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


FH_API struct fh_document *fh_CreateDocument(struct fh_window *win)
{
	struct fh_document *doc;

	/* Allocate memory for the document */
	if(!(doc = fh_zalloc(sizeof(struct fh_document)))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for document");
		goto err_return;
	}

	/* Set the attributes for the document */
	doc->window = win;

	/* Create the body element */	
	if(!(doc->body = fh_CreateElement("body", FH_BODY))) {
		ALARM(ALARM_ERR, "Failed to create body for document");
		goto err_free_doc;
	}

	/* Set the attributes for the body element */
	doc->body->document = doc;
	doc->body->body = doc->body;
	doc->body->parent = NULL;
	doc->body->layer = 0;

	/* Update the body element */
	fh_UpdateDocumentBranch(doc, doc->body);

	return doc;

err_free_doc:
	fh_free(doc);

err_return:
	ALARM(ALARM_ERR, "Failed to create new document");
	return NULL;
}


FH_API void fh_DestroyDocument(struct fh_document *doc)
{
	if(!doc)
		return;

	/* If the document contains a body, recursivly remove it */
	fh_RemoveElement(doc, doc->body);	

	fh_free(doc);
}


FH_API struct fh_element *fh_AddElement(struct fh_document *doc,
		struct fh_element *parent, char *name,
		enum fh_element_type type)
{
	struct fh_element *ele;

	if(!doc || !parent || !name) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	/* Create new element */
	if(!(ele = fh_CreateElement(name, type))) {
		ALARM(ALARM_ERR, "Failed to create new element for document");
		goto err_return;
	}

	/* Attach element to parent */
	if(fh_AttachElement(parent, ele) < 0) {
		ALARM(ALARM_ERR, "Failed to attach element to parent");
		goto err_destroy_ele;
	}

	/* Update the new element */
	fh_UpdateDocumentBranch(doc, ele);

	return ele;

err_destroy_ele:
	fh_DestroyElement(ele);

err_return:
	ALARM(ALARM_ERR, "Failed to add element to document");
	return NULL;
}


FH_API void fh_RemoveElement(struct fh_document *doc, struct fh_element *ele)
{
	if(!doc || !ele) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	fh_ApplyElementsUp(ele, &doc_cfnc_remove, NULL);
}


FH_API struct fh_element *fh_GetElement(struct fh_document *doc, char *name)
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
	fh_ApplyElementsDown(doc->body, &doc_cfnc_find, &sel);

	if(sel.state == 1) {
		return sel.element;
	}

err_return:
	ALARM(ALARM_ERR, "Failed to get element");
	return NULL;
}


FH_API void fh_UpdateDocumentBranch(struct fh_document *doc,
		struct fh_element *ele)
{
	if(!doc || !ele) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	fh_ApplyElementsDown(ele, &doc_cfnc_update, NULL);
}


FH_API void fh_UpdateDocument(struct fh_document *doc)
{
	if(!doc) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	fh_UpdateDocumentBranch(doc, doc->body);
}


FH_API void fh_RenderDocumentBranch(struct fh_document *doc,
		struct fh_element *ele)
{
	if(!doc || !ele) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	fh_ApplyElementsDown(ele, &doc_cfnc_render, NULL);
}


FH_API void fh_RenderDocument(struct fh_document *doc)
{
	if(!doc) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	fh_RenderDocumentBranch(doc, doc->body);
}


FH_API void fh_ShowDocumentTree(struct fh_document *doc,
		struct fh_element *ele)
{
	struct fh_element *start;

	if(!doc) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	if(ele)
		start = ele;
	else
		start = doc->body;

	fh_ApplyElementsDown(start, &doc_cfnc_show, NULL);
}
