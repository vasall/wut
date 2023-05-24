#include "element.h"

#include "alarm.h"
#include "system.h"
#include "element_template.h"
#include "component.h"
#include "document.h"


#include <stdlib.h>



/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

FH_API struct fh_element *fh_CreateElement(struct fh_document *doc, char *name,
		enum fh_element_type type, void *data)
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
	ele->document = doc;
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
	ele->component = NULL;
	if(fh_eletemp_load(ele, data) < 0) {
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

	/* If the element has a component attached to it, destroy that aswell */
	if(ele->component) {
		fh_DestroyComponent(ele->component);
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
		if(parent->children[i])
			continue;

		parent->children[i] = ele;
		parent->children_num++;

		ele->parent = parent;

		ele->body = parent->body;
		ele->layer = parent->layer + 1;
		break;
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

	/* First process the style for this element */
	fh_style_process(&ele->style, ele->document->shape_ref);

	/* Then if the element has a component, update that aswell */
	if(ele->component) {
		fh_UpdateComponent(ele->component, NULL);
	}
}


FH_API void fh_RenderElement(struct fh_element *ele)
{
	struct fh_rect rect;
	struct fh_color col;

	if(!ele) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		return;
	}

	rect.w = ele->style.outer_size.width;
	rect.h = ele->style.outer_size.height;
	rect.x = ele->style.outer_position.x - (rect.w / 2);
	rect.y = ele->style.outer_position.y - (rect.h / 2);

#if 1
	printf("Render %s:  [%d, %d, %d, %d]\n",
			ele->name,
			rect.x,
			rect.y,
			rect.w,
			rect.h);
#endif

	col = ele->style.infill.color;

	if(ele->type == FH_VIEW) {
		col = fh_col_set(0, 0, 0, 0);
		fh_FlatRectSet(ele->document->flat, &rect, col);
	}
	else {
		fh_FlatRect(ele->document->flat, &rect, col);
	}

	/* If the element has a context, render that aswell */
	if(ele->component) {
		fh_RenderComponent(ele->component);
	}
}


FH_API struct fh_context *fh_GetElementContext(struct fh_element *ele)
{
	if(!ele) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		return NULL;
	}

	return ele->document->context;
}


FH_API struct fh_rect fh_GetElementOuterShape(struct fh_element *ele)
{
	struct fh_rect r;

	if(!ele) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		fh_rect_set(&r, 0, 0, 100, 100);
		return r;
	}

	r.w = ele->style.outer_size.width;
	r.h = ele->style.outer_size.height;
	r.x = ele->style.outer_position.x - (r.w / 2);
	r.y = ele->style.outer_position.y - (r.h / 2);

	return r;
}


FH_API struct fh_rect fh_GetElementInnerShape(struct fh_element *ele)
{
	struct fh_rect r;

	if(!ele) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		fh_rect_set(&r, 0, 0, 100, 100);
		return r;
	}

	r.w = ele->style.inner_size.width;
	r.h = ele->style.inner_size.height;
	r.x = ele->style.inner_position.x - (r.w / 2);
	r.y = ele->style.inner_position.y - (r.h / 2);

	return r;
}


FH_API s8 fh_ModifyElementStyle(struct fh_element *ele, char *str)
{
	char buf[128];

	if(!ele || !str) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}


	

err_return:
	ALARM(ALARM_ERR, "Failed to set attribute");
	return -1;
}


FH_API struct fh_view *fh_GetView(struct fh_element *ele)
{
	if(!ele) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(ele->type != FH_VIEW || !ele->component) {
		ALARM(ALARM_ERR, "Wrong element type");
		goto err_return;
	}

	return (struct fh_view *)ele->component->ref;

err_return:
	ALARM(ALARM_ERR, "Failed to get view");
	return NULL;
}
