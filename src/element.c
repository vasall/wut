#include "element.h"
#include "element_template.h"

#include <stdlib.h>




FH_API struct fh_element *fh_ele_create(char *name, enum fh_element_type type)
{
	struct fh_element *ele;
	s8 name_len;
	s8 i;

	if(!name) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	name_len = strlen(name);
	if(name_len < 1 || name_len > FH_ELEMENT_NAME_LIM) {
		ALARM(ALARM_ERR, "Element name is invalid");
		goto err_return;
	}

	
	if(!(ele = szalloc(sizeof(struct fh_element)))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for new element");
		goto err_return;
	}

	/* Set the basic attributes for the element */
	strcpy(ele->name, name);
	ele->type = type;

	/* ...and reset the rest */
	ele->layer = 100;
	ele->document = NULL;
	ele->body = NULL;	
	ele->parent = NULL;
	ele->children_num = 0;
	for(i = 0; i < FH_ELEMENT_CHILDREN_LIM; i++)
		ele->children[i] = NULL;


	/* Load a template, if there is one for the given type */
	if(fh_eletemp_load(ele) < 0) {
		ALARM(ALARM_ERR, "Failed to load the template for the element");
		goto err_free_ele;
	}

	return ele;

err_free_ele:
	sfree(ele);

err_return:
	ALARM(ALARM_ERR, "Failed to create new element");
	return NULL;
}


FH_API void fh_ele_destroy(struct fh_element *ele)
{
	if(!ele) {
		return;
	}


	sfree(ele);
}


FH_API s8 fh_ele_attach(struct fh_element *parent, struct fh_element *ele)
{
	s8 i;

	if(!parent || !ele) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(parent->children_num + 1 > FH_ELEMENT_CHILDREN_LIM) {
		ALARM(ALARM_ERR, "Can not add more children to parent element");
		goto err_return;
	}

	for(i = 0; i < FH_ELEMENT_CHILDREN_LIM; i++) {
		if(!parent->children[i]) {
			parent->children[i] = ele;
			parent->children_num++;

			ele->parent = parent;
			break;
		}
	}

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to attach element to parent");
	return -1;
}


FH_API void fh_ele_detach(struct fh_element *ele)
{
	s8 i;
	struct fh_element *parent;

	if(!ele) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	if(!ele->parent) {
		return;
	}


	parent = ele->parent;

	for(i = 0; i < FH_ELEMENT_CHILDREN_LIM; i++) {
		if(!parent->children[i])
			continue;

		if(strcmp(parent->children[i]->name, ele->name) == 0) {
			parent->children[i] = NULL;
			parent->children_num--;

			ele->parent = NULL;
			return;
		}
	}
}


FH_INTERN s8 fh_cfnc_remove_elements(struct fh_element *ele, void *data)
{
	/* SILENCIO! */
	if(data) {}

	/* First detach the element from the parent */
	fh_ele_detach(ele);

	/* ...and finally destroy it */
	fh_ele_destroy(ele);

	return 0;
}

FH_API void fh_ele_remove(struct fh_element *ele)
{
	if(!ele) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		goto err_return;
	}

	/* Recursivly destroy all elements attached to ele and ele itself */
	fh_ele_hlf_up(ele, &fh_cfnc_remove_elements, NULL);

	return;

err_return:
	ALARM(ALARM_WARN, "Failed to remove element");
}


FH_API void fh_ele_hlf_down(struct fh_element *str,
		s8 (*cfnc)(struct fh_element *e, void *data), void *data)
{
	s8 i;

	if(!str) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	/* Then apply the callback function to this element */
	if(cfnc(str, data) == 1)
		return;

	/* Call this function on all children */
	for(i = 0; i < FH_ELEMENT_CHILDREN_LIM; i++) {
		if(!str->children[i])
			continue;

		fh_ele_hlf_down(str->children[i], cfnc, data);
	}

	return;
}

FH_API void fh_ele_hlf_up(struct fh_element *str,
		s8 (*cfnc)(struct fh_element *w, void *data), void *data)
{
	s8 i;

	if(!str) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	/* Call this function on all children */
	for(i = 0; i < FH_ELEMENT_CHILDREN_LIM; i++) {
		if(!str->children[i])
			continue;

		fh_ele_hlf_up(str->children[i], cfnc, data);
	}

	/* Then apply the callback function to this element */
	if(cfnc(str, data) == 1)
		return;

	return;
}
