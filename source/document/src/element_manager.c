#include "document/inc/element_manager.h"

#include "document/inc/document.h"

#include "widget/inc/widget.h"

#include "system/inc/system.h"

#include "style/inc/layout.h"

#include <stdlib.h>



/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


FH_XMOD void fh_element_adjust_shape(struct fh_element *ele)
{
	fh_element_calc_off(ele);

	fh_element_calc_render_rect(ele);
}


FH_XMOD void fh_element_calc_off(struct fh_element *ele)
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
			par->style.content_delta.x -
			par->content_offset.x +
			ele->layout_offset.x;
		ele->relative_offset.y =
			par->style.content_delta.y -
			par->content_offset.y +
			ele->layout_offset.y;
	}


	ele->absolute_offset.x = ele->relative_offset.x;
	ele->absolute_offset.y = ele->relative_offset.y;

	if(par) {
		ele->absolute_offset.x += 
			par->absolute_offset.x;
		ele->absolute_offset.y +=
			par->absolute_offset.y;
	}
}


FH_XMOD void fh_element_calc_render_rect(struct fh_element *ele)
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
	 * Now we can copy the resulting rectangle to the element.
	 */
	fh_rect_cpy(&ele->output_rect, &dif);
	ele->info_flags |= FH_ELEMENT_F_VISIBLE;
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


FH_API void fh_element_render(struct fh_element *ele)
{
	struct fh_rect out;
	struct fh_color col;
		
	if(!ele) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		return;
	}

	/*
	 * Return if the element is not visible.
	 */
	if(!(ele->info_flags & FH_ELEMENT_F_VISIBLE))
		return;


	fh_rect_cpy(&out, &ele->output_rect);

	col = ele->style.infill.color;

	if(ele->type == FH_VIEW) {
		col = fh_col_set(0, 0, 0, 0);
		fh_FlatRectSet(ele->document->flat, &out, col);
	}
	else {
		fh_FlatRect(ele->document->flat, &out, col);
	}

	/* If the element has a context, render that aswell */
	if(ele->widget) {
		fh_RenderWidget(ele->widget);
	}
}


FH_XMOD void fh_element_ren_scrollbar(struct fh_element *ele)
{
	s32 size;
	s32 width = 5;
	
	f32 prop;

	struct fh_color col;
	struct fh_rect	elebox;
	struct fh_sin2	ele_rect;
	struct fh_rect out;

	elebox = fh_GetElementBox(ele);

	/* vertical */
	if(ele->scrollbar_flags & FH_RESTYLE_SCROLL_V) {
		prop = (f32)elebox.h / (f32)ele->content_size.y;
		size = prop * elebox.h;

		out.x = (elebox.x + elebox.w) - (width + 5);
		out.y = elebox.y;
		out.w = width;
		out.h = size;

		printf("Render scrollbar at: ");
		fh_rect_dump(&out);
		printf("\n");

		col = fh_col_set(255, 0, 255, 255);
		fh_FlatRectSet(ele->document->flat, &out, col);
	}
		


	/* horizontal */
}


FH_XMOD s8 fh_element_compare(struct fh_element *in1, struct fh_element *in2)
{
	if(in1 == in2)
		return 1;

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

	/* Set the identifier */
	ele->identifier = FH_IDT_ELEMENT;

	/* Set the basic attributes for the element */
	strcpy(ele->name, name);
	ele->type = type;

	/* ...and reset the rest */
	ele->layer = 100;
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
		ALARM(ALARM_ERR, "Failed to initialize style for element");
		goto err_destroy_handler;
	}

	/* Load a template, if there is one for the given type */
	ele->widget = NULL;
	if(fh_eletemp_load(ele, data) < 0) {
		ALARM(ALARM_ERR, "Failed to load the template for the element");
		goto err_destroy_handler;
	}

	return ele;

err_destroy_handler:
	fh_handler_destroy(ele->event_handler);

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
		ALARM(ALARM_ERR, "Input parameters invalid");
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
	ALARM(ALARM_ERR, "Failed to attach element to parent");
	return -1;
}


FH_API void fh_DetachElement(struct fh_element *ele)
{
	struct fh_element *par;

	if(!ele) {
		ALARM(ALARM_WARN, "Input parameters invalid");
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
		ALARM(ALARM_WARN, "Input parameters invalid");
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
		ALARM(ALARM_WARN, "Input parameters invalid");
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
		ALARM(ALARM_WARN, "Input parameters invalid");
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
		ALARM(ALARM_ERR, "Input parameters invalid");
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
