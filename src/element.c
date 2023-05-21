#include "element.h"

#include "alarm.h"
#include "system.h"
#include "element_template.h"
#include "document.h"


#include <stdlib.h>



/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

FH_API struct fh_element *fh_CreateElement(char *name,
		enum fh_element_type type)
{
	struct fh_element *ele;
	struct fh_style *style_ref;
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

	
	if(!(ele = fh_zalloc(sizeof(struct fh_element)))) {
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

	/* Initialize the style structure */
	style_ref = (ele->parent) ? &ele->parent->style : NULL;
	if(fh_style_init(&ele->style, style_ref) < 0) {
		ALARM(ALARM_ERR, "Failed to initialize style for element");
		goto err_free_ele;
	}

	/* Load a template, if there is one for the given type */
	if(fh_eletemp_load(ele) < 0) {
		ALARM(ALARM_ERR, "Failed to load the template for the element");
		goto err_free_ele;
	}

	return ele;

err_free_ele:
	fh_free(ele);

err_return:
	ALARM(ALARM_ERR, "Failed to create new element");
	return NULL;
}


FH_API void fh_DestroyElement(struct fh_element *ele)
{
	if(!ele) {
		return;
	}

	fh_free(ele);
}


FH_API s8 fh_AttachElement(struct fh_element *parent, struct fh_element *ele)
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

			ele->document = parent->document;
			ele->body = parent->body;
			ele->layer = parent->layer + 1;
			break;
		}
	}

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to attach element to parent");
	return -1;
}


FH_API void fh_DetachElement(struct fh_element *ele)
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


FH_API void fh_ApplyElementsDown(struct fh_element *ele,
		s8 (*fnc)(struct fh_element *w, void *data), void *data)
{
	s8 i;

	if(!ele) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	/* Then apply the callback function to this element */
	if(fnc(ele, data) == 1)
		return;

	/* Call this function on all children */
	for(i = 0; i < FH_ELEMENT_CHILDREN_LIM; i++) {
		if(!ele->children[i])
			continue;

		fh_ApplyElementsDown(ele->children[i], fnc, data);
	}

	return;
}


FH_API void fh_ApplyElementsUp(struct fh_element *ele,
		s8 (*fnc)(struct fh_element *w, void *data), void *data)
{
	s8 i;

	if(!ele) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	/* Call this function on all children */
	for(i = 0; i < FH_ELEMENT_CHILDREN_LIM; i++) {
		if(!ele->children[i])
			continue;

		fh_ApplyElementsUp(ele->children[i], fnc, data);
	}

	/* Then apply the callback function to this element */
	fnc(ele, data);
}


FH_API void fh_UpdateElement(struct fh_element *ele)
{
	if(!ele) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}	

	printf("Update element: %s\n", ele->name);

	fh_style_process(&ele->style, ele->document->shape_ref);
}


FH_API void fh_RenderElement(struct fh_element *ele)
{
	struct fh_rect rect;
	struct fh_color col;

	if(!ele) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		return;
	}

	rect.w = ele->style.size.width;
	rect.h = ele->style.size.height;
	rect.x = ele->style.position.x - (rect.w / 2);
	rect.y = ele->style.position.y - (rect.h / 2);

#if 1
	printf("Render %s:  [%d, %d, %d, %d]\n",
			ele->name,
			rect.x,
			rect.y,
			rect.w,
			rect.h);
#endif

	col = ele->style.infill.color;

	fh_FlatRect(ele->document->flat, &rect, col);
}


FH_API struct fh_rect fh_GetElementShape(struct fh_element *ele)
{
	struct fh_rect r;

	if(!ele) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		fh_rect_set(&r, 0, 0, 100, 100);
		return r;
	}

	r.w = ele->style.size.width;
	r.h = ele->style.size.height;
	r.x = ele->style.position.x - (r.w / 2);
	r.y = ele->style.position.y - (r.h / 2);

	return r;
}
