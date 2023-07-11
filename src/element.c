#include "element.h"

#include "system.h"
#include "template.h"
#include "widget.h"
#include "document.h"


#include <stdlib.h>


/*
 * In case an element gets removed, move the other elements in the children list
 * to close up the empty slot. The given slot has to be NULL.
 *
 * @ele: Pointer to the parent element
 * @slot: The slot that became empty
 */
FH_INTERN void ele_reorder_children(struct fh_element *ele, s8 slot)
{
	s8 i;

	for(i = slot + 1; i < FH_ELEMENT_CHILDREN_LIM; i++) {
		if(!ele->children[i])
			return;

		ele->children[i]->slot = i - 1;
		ele->children[i-1] = ele->children[i];
	}
}


FH_INTERN void ele_adjust_shape(struct fh_element *ele)
{
	struct fh_style *style = &ele->style;

	fh_rect_add(&ele->shape, &ele->bounding_shape, &style->shape);
	fh_rect_add(&ele->inner_shape, &ele->bounding_shape,
			&style->inner_shape);

	printf("Finale shapes of %s\n", ele->name);
	printf("Bounding: "); fh_rect_dump(&ele->bounding_shape); printf("\n");
	printf("Shape: "); fh_rect_dump(&ele->shape); printf("\n");
	printf("Inner: "); fh_rect_dump(&ele->inner_shape); printf("\n");

}


FH_INTERN void ele_layout_blocks(struct fh_element *ele)
{
	u8 i;
	u32 off_x = 0;
	u32 off_y = 0;
	
	u32 lim_y = 0;

	u8 w;
	u8 h;

	u32 content_width = 0;
	u32 content_height = 0;

	struct fh_element *run;

	printf("Use layout: blocks\n");

	printf("[ELEMENT] x=%d, y=%d, w=%d, h=%d\n",
			ele->bounding_shape.x, ele->bounding_shape.y,
			ele->bounding_shape.w, ele->bounding_shape.h);

	for(i = 0; i < ele->children_num; i++) {
		run = ele->children[i];

		printf("%d: %s\n", i, ele->name);

		w = run->style.bounding_shape.w;
		h = run->style.bounding_shape.h;

		if(off_x + w > (u32)ele->inner_shape.w) {
			off_y += lim_y;

			if(content_width < off_x)
				content_width = off_x;

			if(content_height < off_y)
				content_height = off_y;

			off_x = 0;
			lim_y = 0;
		}

		run->bounding_shape.x = off_x;
		run->bounding_shape.y = off_y;

		run->bounding_shape.w = w;
		run->bounding_shape.h = h;

		if(h > lim_y) {
			lim_y = h;
		}

		/* Adjust the offset */
		off_x += w;

		/* Fit the shape and inner shape to the bounding shape */
		ele_adjust_shape(ele);
	}

	ele->content_width = content_width;
	ele->content_height = content_height;
}




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
	ele->slot = -1;
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
	ele->widget = NULL;
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

	/* If the element has a widget attached to it, destroy that aswell */
	if(ele->widget) {
		fh_DestroyWidget(ele->widget);
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
		ele->slot = i;

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

			/* Reorder elements */
			ele_reorder_children(parent, i);

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


FH_API void fh_UpdateElementStyle(struct fh_element *ele)
{
	struct fh_style_pass pass;

	if(!ele) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	printf("Process style for %s \n", ele->name);

	/* First process the style for this element */
	pass.document_shape = ele->document->shape_ref;
	fh_style_process(&ele->style, &pass);

	/* Then update the size and position according to the style */
	

	/* Then if the element has a widget, update that aswell */
	if(ele->widget) {
		fh_UpdateWidget(ele->widget, NULL);
	}
}


FH_API void fh_UpdateElementChildrenShape(struct fh_element *ele)
{
	u8 i;	

	if(!ele) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	switch(ele->style.layout.mode) {
		case FH_LAYOUT_BLOCKS: ele_layout_blocks(ele); break;
		default: break;
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

	rect.x = ele->shape.x;
	rect.y = ele->shape.y;
	rect.w = ele->shape.w;
	rect.h = ele->shape.h;

	col = ele->style.infill.color;

#if 1
	printf("Render %s:  [%d, %d, %d, %d], [%d, %d, %d, %d]\n",
			ele->name,
			rect.x,
			rect.y,
			rect.w,
			rect.h,
			col.r,
			col.g,
			col.b,
			col.a);
#endif


	if(ele->type == FH_VIEW) {
		col = fh_col_set(0, 0, 0, 0);
		fh_FlatRectSet(ele->document->flat, &rect, col);
	}
	else {
		fh_FlatRect(ele->document->flat, &rect, col);
	}

	/* If the element has a context, render that aswell */
	if(ele->widget) {
		fh_RenderWidget(ele->widget);
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


FH_API struct fh_rect fh_GetElementBoundingShape(struct fh_element *ele)
{
	if(!ele) {
		struct fh_rect rect;
		fh_rect_rst(&rect);
		return rect;
	}

	return ele->bounding_shape;
}


FH_API struct fh_rect fh_GetElementShape(struct fh_element *ele)
{
	if(!ele) {
		struct fh_rect rect;
		fh_rect_rst(&rect);
		return rect;
	}

	return ele->shape;
}


FH_API struct fh_rect fh_GetElementInnerShape(struct fh_element *ele)
{
	if(!ele) {
		struct fh_rect rect;
		fh_rect_rst(&rect);
		return rect;
	}

	return ele->inner_shape;
}


FH_API s8 fh_ModifyElementStyle(struct fh_element *ele, char *str)
{
	if(!ele || !str) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	fh_ModifyStyle(&ele->style, str);

	return 0;

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

	if(ele->type != FH_VIEW || !ele->widget) {
		ALARM(ALARM_ERR, "Wrong element type");
		goto err_return;
	}

	return (struct fh_view *)ele->widget->ref;

err_return:
	ALARM(ALARM_ERR, "Failed to get view");
	return NULL;
}
