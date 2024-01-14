#include "document/inc/element_manager.h"

#include "document/inc/document.h"

#include "utility/alarm/inc/alarm.h"
#include "utility/inc/shape.h"

#include "widget/inc/widget.h"

#include "system/inc/system.h"

#include "style/inc/layout.h"


#include <stdlib.h>


FH_INTERN void element_calc_offset(struct fh_element *ele)
{
	struct fh_element *par = ele->parent;


	ele->relative_offset.x = 0;
	ele->relative_offset.y = 0;

	/*
	 * If the element has a parent, calculate the offset from the upper-left
	 * corner of the parents shape to the upper-left corner of this elements
	 * shape.
	 */
	if(par) {
		ele->relative_offset.x =
			par->style.content_delta.x -	/* Border+Padding */
			par->content_offset.x +		/* Scrolling */
			ele->layout_offset.x;		/* Layout */
		ele->relative_offset.y =
			par->style.content_delta.y -	/* Border+Paddaing */
			par->content_offset.y +		/* Scrolling */
			ele->layout_offset.y;		/* Layout */
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



FH_INTERN void element_calc_bounding_rect(struct fh_element *ele)
{
	struct fh_rect rect;

	fh_rect_mov(&rect, &ele->style.bounding_box, &ele->absolute_offset);

	fh_rect_cpy(&ele->bounding_rect, &rect);
}


FH_INTERN void element_calc_element_rect(struct fh_element *ele)
{
	struct fh_rect rect;

	fh_rect_add(&rect, &ele->bounding_rect, &ele->style.element_delta);

	fh_rect_cpy(&ele->element_rect, &rect);

}


FH_INTERN void element_calc_content_rect(struct fh_element *ele)
{
	struct fh_rect rect;

	fh_rect_add(&rect, &ele->bounding_rect, &ele->style.content_delta);

	fh_rect_cpy(&ele->content_rect, &rect);
}


FH_INTERN void element_calc_visible(struct fh_element *ele)
{
	struct fh_rect out;
	struct fh_rect dif;

	/*
	 * First calculate the absolute position of the reference-area of the 
	 * bounding box in the window.
	 */
	fh_rect_mov(&out, &ele->style.bounding_box, &ele->absolute_offset);

	/*
	 * Finally convert from the bounding box to the element box.
	 */
	fh_rect_add(&out, &out, &ele->style.element_delta);

	fh_rect_cpy(&dif, &out);	/* Will be used later */

	/*
	 * Check intersecting area with the parent.
	 */
	if(ele->parent) {
		if(!fh_rect_intersect(&dif, &out, &ele->parent->output_rect)) {
			/* If the element is not inside the parent */
			ele->info_flags &= ~FH_ELEMENT_F_VISIBLE;
			return;
		}
	}


	/*
	 * Now we have to validate the intersecting area with the window.
	 * This will also check if the element even is inside the window.
	 */
	if(!fh_rect_intersect(&dif, &dif, ele->document->shape_ref)) {
		/* Element is not inside the window */
		ele->info_flags &= ~FH_ELEMENT_F_VISIBLE;
		return;
	}

	/*
	 * Copy the visible area into the according rectangle.
	 */
	fh_rect_cpy(&ele->visible_out_rect, &dif);

	/*
	 * Now we can copy the resulting rectangle to the element.
	 */
	fh_rect_cpy(&ele->output_rect, &out);
	ele->info_flags |= FH_ELEMENT_F_VISIBLE;
}


FH_INTERN void element_calc_shape(struct fh_element *ele)
{
	/* First calculate the relative and absolute offset */
	element_calc_offset(ele);

	/* Then calculate the different shape rectangles */
	element_calc_bounding_rect(ele);
	element_calc_element_rect(ele);
	element_calc_content_rect(ele);

	/* Finally check visibility */
	element_calc_visible(ele);
}


FH_XMOD s8 fh_element_scroll(struct fh_element *ele, s32 *val)
{
	s32 limits[2];
	u8 ret = 0;

	if(val[0] != 0 && (ele->scrollbar_flags & FH_RESTYLE_SCROLL_H)) {
		limits[0] = 0;
		limits[1] = ele->content_size.x - ele->content_rect.w; 

		ele->content_offset.x -= val[0];

		if(ele->content_offset.x < limits[0]) ele->content_offset.x = limits[0];
		if(ele->content_offset.x > limits[1]) ele->content_offset.x = limits[1];

		ret = 1;
	}
	if(val[1] != 0 && (ele->scrollbar_flags & FH_RESTYLE_SCROLL_V)) {
		limits[0] = 0;
		limits[1] = ele->content_size.y - ele->content_rect.h; 

		ele->content_offset.y -= val[1];

		if(ele->content_offset.y < limits[0]) ele->content_offset.y = limits[0];
		if(ele->content_offset.y > limits[1]) ele->content_offset.y = limits[1];

		ret = 1;
	}

	if(ret) {
		/* Update after scrolling */
		fh_UpdateDocumentBranch(ele->document, ele);
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


FH_XMOD void fh_element_adjust_shape(struct fh_element *ele)
{
	element_calc_shape(ele);
}


FH_XMOD void fh_element_hdl_scrollbar(struct fh_element *ele)
{
	u8 flag = 0;
	struct fh_rect inner_rect;

	inner_rect = fh_GetContentBox(ele); 

	if(inner_rect.h < ele->content_size.y)
		flag |= FH_RESTYLE_SCROLL_V;

	if(inner_rect.w < ele->content_size.x)
		flag |= FH_RESTYLE_SCROLL_H;


	ele->scrollbar_flags = flag & ele->style.scrollbar.flags;
}


FH_XMOD s8 fh_element_compare(struct fh_element *in1, struct fh_element *in2)
{
	if(in1 == in2) {
		return 1;
	}
	return 0;
}



FH_XMOD s8 fh_element_hlf(struct fh_element *ele, fh_ele_cfnc prefnc,
		fh_ele_cfnc postfnc, void *data)
{
	struct fh_element *run;
	struct fh_element *next;

	if(!ele)
		return 0;

	if(prefnc) {
		if(prefnc(ele, data))
			return 1;
	}


	run = ele->firstborn;
	while(run) {
		next = run->younger_sibling;

		if(fh_element_hlf(run, prefnc, postfnc, data))
			return 1;

		run = next;
	}

	if(postfnc) {
		return postfnc(ele, data);
	}

	return 0;
}


FH_XMOD void fh_element_mod_info(struct fh_element *ele, u8 flag, u8 val)
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

FH_API struct fh_element *fh_CreateElement(struct fh_document *doc, char *name,
		enum fh_element_type type, void *data)
{
	struct fh_element *ele;
	s8 name_len;

	if(!name) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		goto err_return;
	}

	name_len = strlen(name);
	if(name_len < 1 || name_len > FH_ELEMENT_NAME_LIM) {
		FH_ALARM(FH_ERROR, "Element name is invalid");
		goto err_return;
	}


	if(!(ele = fh_zalloc(sizeof(struct fh_element)))) {
		FH_ALARM(FH_ERROR, "Failed to allocate memory for new element");
		goto err_return;
	}

	/* Set the identifier */
	ele->identity = FH_IDT_ELEMENT;

	/* Set the basic attributes for the element */
	strcpy(ele->name, name);
	ele->type = type;

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
	if(!(ele->event_handler = fh_handler_create()))
		goto err_free_ele;

	/* Initialize the style structure */
	if(fh_style_init(&ele->style, NULL) < 0) {
		FH_ALARM(FH_ERROR, "Failed to initialize style for element");
		goto err_destroy_handler;
	}

	/* Load a template, if there is one for the given type */
	ele->widget = NULL;
	if(fh_eletemp_load(ele, data) < 0) {
		FH_ALARM(FH_ERROR, "Failed to load the template for the element");
		goto err_destroy_handler;
	}

	return ele;

err_destroy_handler:
	fh_handler_destroy(ele->event_handler);

err_free_ele:
	fh_free(ele);

err_return:
	FH_ALARM(FH_ERROR, "Failed to create new element");
	return NULL;
}


FH_API void fh_DestroyElement(struct fh_element *ele)
{
	if(!ele) {
		return;
	}

	fh_handler_destroy(ele->event_handler);

	/* If the element has a widget attached to it, destroy that aswell */
	if(ele->widget) {
		fh_DestroyWidget(ele->widget);
	}

	fh_free(ele);
}


FH_API s8 fh_AttachElement(struct fh_element *parent, struct fh_element *ele)
{
	struct fh_element *run;

	if(!parent || !ele) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
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
	fh_style_link(&ele->style, &parent->style);

	return 0;

err_return:
	FH_ALARM(FH_ERROR, "Failed to attach element to parent");
	return -1;
}


FH_API void fh_DetachElement(struct fh_element *ele)
{
	struct fh_element *par;

	if(!ele) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
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


FH_API void fh_ApplyElementRise(struct fh_element *ele, fh_ele_cfnc fnc,
		void *data)
{
	struct fh_element *run;
	struct fh_element *next;

	if(!ele) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
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


FH_API void fh_UpdateElementStyle(struct fh_element *ele)
{
	struct fh_style_pass pass;

	if(!ele) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	/* First process the style for this element */
	pass.document_shape = ele->document->shape_ref;
	fh_style_process(&ele->style, &pass);


	/* Then if the element has a widget, update that aswell */
	if(ele->widget) {
		fh_UpdateWidget(ele->widget, NULL);
	}
}


FH_API void fh_UpdateElementChildrenShape(struct fh_element *ele)
{
	if(!ele) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	switch(ele->style.layout.mode) {
		case FH_LAYOUT_BLOCKS:	fh_layout_blocks(ele);	break;
		case FH_LAYOUT_ROWS: 	fh_layout_rows(ele); 	break;
		case FH_LAYOUT_COLUMNS: fh_layout_columns(ele); break;
		default: break;
	}
}


FH_API struct fh_context *fh_GetElementContext(struct fh_element *ele)
{
	if(!ele) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		return NULL;
	}

	return ele->document->context;
}


FH_API struct fh_rect fh_GetBoundingBox(struct fh_element *ele)
{
	struct fh_rect r;

	if(!ele) {
		fh_rect_rst(&r);
		return r;
	}

	fh_rect_mov(&r, &ele->style.bounding_box, &ele->absolute_offset);

	return r;
}


FH_API struct fh_rect fh_GetElementBox(struct fh_element *ele)
{
	struct fh_rect r;

	if(!ele) {
		fh_rect_rst(&r);
		return r;
	}

	fh_rect_mov(&r, &ele->style.bounding_box, &ele->absolute_offset);
	fh_rect_add(&r, &r, &ele->style.element_delta);

	return r;
}


FH_API struct fh_rect fh_GetContentBox(struct fh_element *ele)
{
	struct fh_rect r;

	if(!ele) {
		fh_rect_rst(&r);
		return r;
	}

	fh_rect_mov(&r, &ele->style.bounding_box, &ele->absolute_offset);
	fh_rect_add(&r, &r, &ele->style.content_delta);

	return r;
}


FH_API s8 fh_ModifyElementStyle(struct fh_element *ele, char *str)
{
	if(!ele || !str) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		goto err_return;
	}

	fh_ModifyStyle(&ele->style, str);

	fh_UpdateDocumentBranch(ele->document, ele);		

	return 0;

err_return:
	FH_ALARM(FH_ERROR, "Failed to set attribute");
	return -1;
}


FH_API struct fh_view *fh_GetView(struct fh_element *ele)
{
	if(!ele) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(ele->type != FH_VIEW || !ele->widget) {
		FH_ALARM(FH_ERROR, "Wrong element type");
		goto err_return;
	}

	return (struct fh_view *)ele->widget->ref;

err_return:
	FH_ALARM(FH_ERROR, "Failed to get view");
	return NULL;
}
