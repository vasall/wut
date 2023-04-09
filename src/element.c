#include "element.h"

#include "alarm.h"
#include "system.h"
#include "element_template.h"
#include "document.h"


#include <stdlib.h>




FH_API struct fh_element *fh_ele_create(char *name, enum fh_element_type type)
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


FH_API void fh_ele_destroy(struct fh_element *ele)
{
	if(!ele) {
		return;
	}


	fh_free(ele);
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


FH_API s8 fh_ele_update(struct fh_element *ele)
{
	SDL_Rect rect;

	rect.x = 0;
	rect.y = 0;
	rect.w = ele->document->window->width;
	rect.h = ele->document->window->height;
	

	fh_style_process(&ele->style, &rect);

	return 0;		
}


FH_API s8 fh_ele_render(struct fh_element *ele)
{
	SDL_Rect rect;
	s32 t;
	struct fh_color col;

	if(!ele) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}



	rect.w = ele->style.size.width;
	rect.h = ele->style.size.height;
	rect.x = ele->style.position.x - (rect.w / 2);
	rect.y = ele->style.position.y - (rect.h / 2);

	printf("Render %s:  [%d, %d, %d, %d]\n",
			ele->name,
			rect.x,
			rect.y,
			rect.w,
			rect.h);

	t = rand() % 0xff; 
	col = fh_col_set(0xff - t, t, t / 2, 0xff);

	fh_ren_rect(ele->document->renderer, rect, col);

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to render element");
	return -1;
}
