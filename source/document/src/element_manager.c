#include "document/inc/element_manager.h"

#include "document/inc/document.h"

#include "utility/inc/alarm.h"
#include "utility/inc/shape.h"

#include "widget/inc/widget.h"

#include "system/inc/system.h"

#include "style/inc/layout.h"


#include <stdlib.h>


WT_INTERN void element_update_offset(struct wt_element *ele)
{
	struct wt_element *par = ele->parent;


	ele->relative_offset.x = 0;
	ele->relative_offset.y = 0;

	/*
	 * If the element has a parent, calculate the offset from the upper-left
	 * corner of the parents shape to the upper-left corner of this elements
	 * shape.
	 */
	if(par) {
		ele->relative_offset.x =
			par->style.shape_content_delta.x -	/* Border+Padding */
			par->content_offset.x +			/* Scrolling */
			ele->layout_offset.x;			/* Layout */
		ele->relative_offset.y =
			par->style.shape_content_delta.y -	/* Border+Padding */
			par->content_offset.y +			/* Scrolling */
			ele->layout_offset.y;			/* Layout */
	}

	ele->absolute_offset.x = ele->relative_offset.x;
	ele->absolute_offset.y = ele->relative_offset.y;

	/*
	 * If this element has a parent, move the absolute offset by the
	 * absolute offset of the parent.
	 */
	if(par) {
		ele->absolute_offset.x += par->absolute_offset.x;
		ele->absolute_offset.y += par->absolute_offset.y;
	}
}



WT_INTERN void element_calc_bounding_rect(struct wt_element *ele)
{
	struct wt_rect rect;

	wt_rect_mov(&rect, &ele->style.shape_bounding_box, &ele->absolute_offset);

	wt_rect_cpy(&ele->bounding_rect, &rect);
}


WT_INTERN void element_calc_element_rect(struct wt_element *ele)
{
	struct wt_rect rect;

	wt_rect_add(&rect, &ele->bounding_rect, &ele->style.shape_element_delta);

	wt_rect_cpy(&ele->element_rect, &rect);

}


WT_INTERN void element_calc_inner_rect(struct wt_element *ele)
{
	struct wt_rect rect;

	wt_rect_add(&rect, &ele->bounding_rect, &ele->style.shape_inner_delta);

	wt_rect_cpy(&ele->inner_rect, &rect);
}


WT_INTERN void element_calc_content_rect(struct wt_element *ele)
{
	struct wt_rect rect;

	wt_rect_add(&rect, &ele->bounding_rect, &ele->style.shape_content_delta);

	wt_rect_cpy(&ele->content_rect, &rect);
}


WT_INTERN void element_calc_visible(struct wt_element *ele)
{
	struct wt_rect out;
	struct wt_rect dif;

	/*
	 * First calculate the absolute position of the reference-area of the 
	 * bounding box in the window.
	 */
	wt_rect_mov(&out, &ele->style.shape_bounding_box, &ele->absolute_offset);

	/*
	 * Finally convert from the bounding box to the element box.
	 */
	wt_rect_add(&out, &out, &ele->style.shape_element_delta);

	wt_rect_cpy(&dif, &out);	/* Will be used later */

	/*
	 * Check intersecting area with the parent.
	 */
	if(ele->parent) {
		if(!wt_rect_intersect(&dif, &out, &ele->parent->output_rect)) {
			/* If the element is not inside the parent */
			ele->info_flags &= ~WT_ELEMENT_F_VISIBLE;
			return;
		}
	}


	/*
	 * Now we have to validate the intersecting area with the window.
	 * This will also check if the element even is inside the window.
	 */
	if(!wt_rect_intersect(&dif, &dif, ele->document->shape_ref)) {
		/* Element is not inside the window */
		ele->info_flags &= ~WT_ELEMENT_F_VISIBLE;
		return;
	}

	/*
	 * Copy the visible area into the according rectangle.
	 */
	wt_rect_cpy(&ele->visible_out_rect, &dif);

	/*
	 * Now we can copy the resulting rectangle to the element.
	 */
	wt_rect_cpy(&ele->output_rect, &out);
	ele->info_flags |= WT_ELEMENT_F_VISIBLE;
}


WT_INTERN void element_calc_shape(struct wt_element *ele)
{
	/* First calculate the relative and absolute offset */
	element_update_offset(ele);

	/* Then calculate the different shape rectangles */
	element_calc_bounding_rect(ele);
	element_calc_element_rect(ele);
	element_calc_inner_rect(ele);
	element_calc_content_rect(ele);

	/* Finally check visibility */
	element_calc_visible(ele);
}


WT_XMOD s8 wt_element_scroll(struct wt_element *ele, s32 *val)
{
	s32 limits[2];
	u8 ret = 0;

	if(val[0] != 0 && (ele->scrollbar_flags & WT_RESTYLE_SCROLL_H)) {
		limits[0] = 0;
		limits[1] = ele->content_size.x - ele->content_rect.w; 

		ele->content_offset.x -= val[0];

		if(ele->content_offset.x < limits[0]) ele->content_offset.x = limits[0];
		if(ele->content_offset.x > limits[1]) ele->content_offset.x = limits[1];

		ret = 1;
	}
	if(val[1] != 0 && (ele->scrollbar_flags & WT_RESTYLE_SCROLL_V)) {
		limits[0] = 0;
		limits[1] = ele->content_size.y - ele->content_rect.h; 

		ele->content_offset.y -= val[1];

		if(ele->content_offset.y < limits[0]) ele->content_offset.y = limits[0];
		if(ele->content_offset.y > limits[1]) ele->content_offset.y = limits[1];

		ret = 1;
	}

	if(ret) {
		/* Update after scrolling */
		wt_UpdateDocumentBranch(ele->document, ele);
	}

	return ret;
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


WT_XMOD void wt_element_adjust_shape(struct wt_element *ele)
{
	element_calc_shape(ele);
}


WT_XMOD void wt_element_hdl_scrollbar(struct wt_element *ele)
{
	u8 flag = 0;
	struct wt_rect inner_rect;

	inner_rect = wt_GetContentBox(ele); 

	if(inner_rect.h < ele->content_size.y)
		flag |= WT_RESTYLE_SCROLL_V;

	if(inner_rect.w < ele->content_size.x)
		flag |= WT_RESTYLE_SCROLL_H;


	ele->scrollbar_flags = flag & ele->style.scrollbar_flags;
}


WT_XMOD s8 wt_element_compare(struct wt_element *in1, struct wt_element *in2)
{
	if(in1 == in2) {
		return 1;
	}
	return 0;
}



WT_XMOD s8 wt_element_hlf(struct wt_element *ele, wt_ele_cfnc prefnc,
		wt_ele_cfnc postfnc, void *data)
{
	struct wt_element *run;
	struct wt_element *next;

	if(!ele)
		return 0;

	if(prefnc) {
		if(prefnc(ele, data))
			return 1;
	}


	run = ele->firstborn;
	while(run) {
		next = run->younger_sibling;

		if(wt_element_hlf(run, prefnc, postfnc, data))
			return 1;

		run = next;
	}

	if(postfnc) {
		return postfnc(ele, data);
	}

	return 0;
}


WT_XMOD void wt_element_mod_info(struct wt_element *ele, u8 flag, u8 val)
{
	if(!ele)
		return;

	if(val)
		ele->info_flags &= ~flag;
	else 
		ele->info_flags |= flag;
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

WT_API struct wt_element *wt_CreateElement(struct wt_document *doc, char *name,
		enum wt_element_type type, void *data)
{
	struct wt_element *ele;
	s8 name_len;

	if(!name) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	name_len = strlen(name);
	if(name_len < 1 || name_len > WT_ELEMENT_NAME_LIM) {
		WT_ALARM(WT_ERROR, "Element name is invalid");
		goto err_return;
	}


	if(!(ele = wt_zalloc(sizeof(struct wt_element)))) {
		WT_ALARM(WT_ERROR, "Failed to allocate memory for new element");
		goto err_return;
	}

	/* Set the identifier */
	ele->identity = WT_IDT_ELEMENT;

	/* Set the basic attributes for the element */
	strcpy(ele->name, name);
	ele->type = type;

	printf("Element \"%s\" has type %d\n", name, type);

	/* ...and reset the rest */
	ele->layer = 0;
	ele->document = doc;
	ele->body = NULL;
	ele->parent = NULL;
	ele->older_sibling = NULL;
	ele->younger_sibling = NULL;
	ele->slot = -1;
	ele->children_num = 0;
	ele->firstborn = NULL;

	/* Create the event handler */
	if(!(ele->event_handler = wt_handler_create())) {
		WT_ALARM(WT_ERROR, "Failed to create event handler");
		goto err_free_ele;
	}

	/* Initialize the style structure */
	if(wt_style_init(&ele->style, NULL) < 0) {
		WT_ALARM(WT_ERROR, "Failed to initialize style for element");
		goto err_destroy_handler;
	}

	/* Load a template, if there is one for the given type */
	ele->widget = NULL;
	printf("Create widget with %p\n", data);
	if(wt_eletemp_load(ele, data) < 0) {
		WT_ALARM(WT_ERROR, "Failed to load the template for the element");
		goto err_destroy_handler;
	}

	return ele;

err_destroy_handler:
	wt_handler_destroy(ele->event_handler);

err_free_ele:
	wt_free(ele);

err_return:
	WT_ALARM(WT_ERROR, "Failed to create new element");
	return NULL;
}


WT_API void wt_DestroyElement(struct wt_element *ele)
{
	if(!ele) {
		return;
	}

	wt_handler_destroy(ele->event_handler);

	/* If the element has a widget attached to it, destroy that aswell */
	if(ele->widget) {
		wt_DestroyWidget(ele->widget);
	}

	wt_free(ele);
}


WT_API s8 wt_AttachElement(struct wt_element *parent, struct wt_element *ele)
{
	struct wt_element *run;

	if(!parent || !ele) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(!parent->firstborn) {
		parent->firstborn = ele;
	}
	else {	
		run = parent->firstborn;	
		while(run->younger_sibling) {
			run = run->younger_sibling;
		}

		run->younger_sibling = ele;
		ele->older_sibling = run;
	}

	parent->children_num++;

	ele->parent = parent;
	ele->body = parent->body;
	ele->layer = parent->layer + 1;

	/* Link the stylesheet */
	wt_style_link(&ele->style, &parent->style);

	return 0;

err_return:
	WT_ALARM(WT_ERROR, "Failed to attach element to parent");
	return -1;
}


WT_API void wt_DetachElement(struct wt_element *ele)
{
	struct wt_element *par;

	if(!ele) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	par = ele->parent;

	if(!par) {
		return;
	}

	if(!ele->older_sibling)
		par->firstborn = ele->younger_sibling;
	else
		(ele->older_sibling)->younger_sibling = ele->younger_sibling;

	if(ele->younger_sibling)
		(ele->younger_sibling)->older_sibling = ele->older_sibling;

	par->children_num--;

	ele->older_sibling = NULL;
	ele->younger_sibling = NULL;
	ele->parent = NULL;
}


WT_API void wt_ApplyElementRise(struct wt_element *ele, wt_ele_cfnc fnc,
		void *data)
{
	struct wt_element *run;
	struct wt_element *next;

	if(!ele) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	run = ele;
	while(run) {
		next = run->parent;

		if(fnc(ele, data) == 1)
			return;

		run = next;
	}
}


WT_API void wt_UpdateElementStyle(struct wt_element *ele)
{
	struct wt_style_pass pass;

	if(!ele) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	/* First process the style for this element */
	pass.document_shape = ele->document->shape_ref;
	wt_style_process(&ele->style, &pass);


	/* Then if the element has a widget, update that aswell */
	if(ele->widget) {
		wt_UpdateWidget(ele->widget, NULL);
	}
}


WT_API void wt_UpdateElementChildrenShape(struct wt_element *ele)
{
	if(!ele) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	switch(ele->style.layout_mode) {
		case WT_KW_LAYOUT_BLOCK:	wt_layout_block(ele);	break;
		case WT_KW_LAYOUT_ROW: 		wt_layout_row(ele); 	break;
		case WT_KW_LAYOUT_COLUMN: 	wt_layout_column(ele);  break;
		default: break;
	}
}


WT_API struct wt_context *wt_GetElementContext(struct wt_element *ele)
{
	if(!ele) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		return NULL;
	}

	return ele->document->context;
}


WT_API struct wt_rect wt_GetBoundingBox(struct wt_element *ele)
{
	struct wt_rect r;

	if(!ele) {
		wt_rect_rst(&r);
		return r;
	}

	wt_rect_mov(&r, &ele->style.shape_bounding_box, &ele->absolute_offset);

	return r;
}


WT_API struct wt_rect wt_GetElementBox(struct wt_element *ele)
{
	struct wt_rect r;

	if(!ele) {
		wt_rect_rst(&r);
		return r;
	}

	wt_rect_mov(&r, &ele->style.shape_bounding_box, &ele->absolute_offset);
	wt_rect_add(&r, &r, &ele->style.shape_element_delta);

	return r;
}


WT_API struct wt_rect wt_GetContentBox(struct wt_element *ele)
{
	struct wt_rect r;

	if(!ele) {
		wt_rect_rst(&r);
		return r;
	}

	wt_rect_mov(&r, &ele->style.shape_bounding_box, &ele->absolute_offset);
	wt_rect_add(&r, &r, &ele->style.shape_content_delta);

	return r;
}


WT_API struct wt_rect *wt_GetContextBoxRef(struct wt_element *ele)
{
	if(!ele) {
		return NULL;
	}

	return &ele->content_rect;
}


WT_API s8 wt_ModifyElementStyle(struct wt_element *ele, char *str)
{
	if(!ele || !str) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	wt_ModifyStyle(&ele->style, str);

	wt_UpdateDocumentBranch(ele->document, ele);

	return 0;

err_return:
	WT_ALARM(WT_ERROR, "Failed to set attribute");
	return -1;
}


WT_API struct wt_view *wt_GetView(struct wt_element *ele)
{
	if(!ele) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(ele->type != WT_VIEW || !ele->widget) {
		WT_ALARM(WT_ERROR, "Wrong element type");
		goto err_return;
	}

	return (struct wt_view *)ele->widget->ref;

err_return:
	WT_ALARM(WT_ERROR, "Failed to get view");
	return NULL;
}
