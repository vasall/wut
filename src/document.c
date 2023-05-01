#include "document.h"

#include "alarm.h"
#include "system.h"

#include <stdlib.h>


FH_API struct fh_document *fh_doc_create(struct fh_window *win)
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
	if(!(doc->body = fh_ele_create("body", FH_BODY))) {
		ALARM(ALARM_ERR, "Failed to create body for document");
		goto err_free_doc;
	}

	/* Set the attributes for the body element */
	doc->body->document = doc;
	doc->body->body = doc->body;
	doc->body->parent = NULL;
	doc->body->layer = 0;

	/* Update the body element */
	if(fh_doc_update_part(doc->body) < 0) {
		ALARM(ALARM_ERR, "Failed to update body element");
		goto err_destroy_body;
	}

	return doc;

err_destroy_body:
	fh_ele_destroy(doc->body);

err_free_doc:
	fh_free(doc);

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

	fh_free(doc);
}


FH_API struct fh_element *fh_doc_add_element(struct fh_document *doc,
		struct fh_element *parent, char *name,
		enum fh_element_type type)
{
	struct fh_element *ele;

	if(!doc || !parent || !name) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	/* Create new element */
	if(!(ele = fh_ele_create(name, type))) {
		ALARM(ALARM_ERR, "Failed to create new element for document");
		goto err_return;
	}

	/* Attach element to parent */
	if(fh_ele_attach(parent, ele) < 0) {
		ALARM(ALARM_ERR, "Failed to attach element to parent");
		goto err_destroy_ele;
	}

	/* Update the element */
	if(fh_doc_update_part(ele) < 0) {
		ALARM(ALARM_ERR, "Failed to update new element");
		goto err_detach_ele;
	}

	return ele;

err_detach_ele:
	fh_ele_detach(ele);

err_destroy_ele:
	fh_ele_destroy(ele);

err_return:
	ALARM(ALARM_ERR, "Failed to add element to document");
	return NULL;
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


FH_INTERN s8 fh_cfnc_update_elements(struct fh_element *ele, void *data)
{
	/* SILENCIO! */
	if(data) {}

	fh_ele_update(ele);

	return 0;
}


FH_API s8 fh_doc_update_part(struct fh_element *ele)
{
	if(!ele) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	fh_ele_hlf_down(ele, &fh_cfnc_update_elements, NULL);

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to partially update elements");
	return -1;
}


FH_API s8 fh_doc_update(struct fh_document *doc)
{
	if(!doc) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	fh_doc_update_part(doc->body);	

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to update document");
	return -1;
}

FH_INTERN s8 fh_doc_cfnc_render_elements(struct fh_element *ele, void *data)
{
	/* SILENCIO! */
	if(data) {}

	printf("Render: %s\n", ele->name);

	fh_ele_render(ele);

	return 0;
}


FH_API s8 fh_doc_render(struct fh_document *doc)
{
	if(!doc) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}


		
	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to render document");
	return -1;
}


FH_INTERN s8 fh_cfnc_show_element(struct fh_element  *ele, void *data)
{
	u8 i;

	/* SILCENCIO! */
	if(data) {}


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

FH_API void fh_doc_tree(struct fh_document *doc)
{
	if(!doc) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	fh_ele_hlf_down(doc->body, &fh_cfnc_show_element, NULL);
}
