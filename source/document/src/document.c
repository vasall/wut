#include "document/inc/document.h"

#include "document/inc/element_rendering.h"

#include "utility/inc/alarm.h"

#include "core/inc/predefined.h"

#include "system/inc/system.h"

#include "utility/inc/utility.h"

#include <stdlib.h>

/*
 * 
 *     CALLBACK-FUNCTIONS
 *
 */

WT_INTERN s8 doc_cfnc_remove(struct wt_element *ele, void *data)
{
	wt_Ignore(data);

	/* First detach the element */
	wt_DetachElement(ele);

	/* Then destroy it */
	wt_DestroyElement(ele);

	return 0;
}

WT_INTERN s8 doc_cfnc_find(struct wt_element *ele, void *data)
{
	struct wt_ele_selector *sel = (struct wt_ele_selector *)data;

	if(sel->state == 1)
		return 1;

	if(strcmp(ele->name, sel->name) == 0) {
		sel->state = 1;
		sel->element = ele;
		return 1;
	}

	return 0;
}

WT_INTERN s8 doc_cfnc_findpos(struct wt_element *ele, void *data)
{
	struct wt_ele_selector *sel = (struct wt_ele_selector *)data;
	struct wt_rect rect;

	rect = wt_GetElementBox(ele);

	if(sel->state == 1)
		return 1;

	if(sel->pos->x < rect.x || sel->pos->x > (rect.x + rect.w))
		return 0;

	if(sel->pos->y < rect.y || sel->pos->y > (rect.y + rect.h))
		return 0;

	sel->element = ele;
	sel->state = 1;

	return 1;
}


WT_INTERN s8 doc_cfnc_update_style(struct wt_element *ele, void *data)
{
	wt_Ignore(data);

	wt_UpdateElementStyle(ele);

	return 0;
}


WT_INTERN s8 doc_cfnc_update_shape(struct wt_element *ele, void *data)
{
	wt_Ignore(data);

	wt_UpdateElementChildrenShape(ele);

	return 0;
}


WT_INTERN s8 doc_cfnc_render_ui(struct wt_element *ele, void *data)
{
	struct wt_batch *batch = (struct wt_batch *)data;

	wt_element_render(batch, ele);

	return 0;
}


WT_INTERN s8 doc_cfnc_render_ui_post(struct wt_element *ele, void *data)
{
	struct wt_batch *batch = (struct wt_batch *)data;

	wt_element_ren_scrollbar(batch, ele);

	return 0;
}


WT_INTERN s8 doc_cfnc_show(struct wt_element  *ele, void *data)
{
	u8 i;

	s32 lim = 16;

	struct wt_rect r;

	wt_Ignore(data);

	for(i = 0; i < ele->layer; i++) {
		printf("  ");
		lim -= 2;
	}

	printf("%s ", ele->name);
	lim -= strlen(ele->name);

	for(i = 0; i < lim; i++)
		printf(" ");


	r = wt_GetBoundingBox(ele);
	wt_rect_dump(&r);
	printf("  --  ");
	r = wt_GetElementBox(ele);
	wt_rect_dump(&r);
	printf("  --  ");
	r = wt_GetContentBox(ele);
	wt_rect_dump(&r);

	printf("\n");

	return 0;

}

WT_INTERN void doc_batch_cfnc_push(struct wt_batch *ren, void *data)
{
	s32 frame[2];
	struct wt_rect *ref = (struct wt_rect *)data;

	frame[0] = ref->w;
	frame[1] = ref->h;
	wt_batch_push_uniform(ren, 0, frame);

}


WT_INTERN s8 doc_create_batch(struct wt_document *doc)
{
	struct wt_shader *shd;
	struct wt_batch *ren;

	struct wt_vertex_attrib v_attributes[] = {
		{3, GL_FLOAT},		/* position */
		{3, GL_INT},		/* 0: shape, 1: limits, 2: everything else */
		{1, GL_INT}		/* type */
	};

	struct wt_uniform_temp uniforms[] = {
		{"u_frame", WT_UNIFORM_2IV, 1, WT_UNIFORM_F_DEFAULT},	 /* 0 */
		{"u_rect", WT_UNIFORM_4IV, 200, WT_UNIFORM_F_DEFAULT},	 /* 1 */
		{"u_color", WT_UNIFORM_4FV, 200, WT_UNIFORM_F_DEFAULT},	 /* 2 */
		{"u_radius", WT_UNIFORM_4IV, 200, WT_UNIFORM_F_DEFAULT}, /* 3 */
		{"u_bwidth", WT_UNIFORM_1IV, 200, WT_UNIFORM_F_DEFAULT}, /* 4 */
		{"u_bcolor", WT_UNIFORM_4FV, 200, WT_UNIFORM_F_DEFAULT}, /* 5 */
		{"u_scroll", WT_UNIFORM_2IV, 200, WT_UNIFORM_F_DEFAULT}, /* 6 */
		{"u_limit", WT_UNIFORM_4IV, 200, WT_UNIFORM_F_DEFAULT}	 /* 7 */
	};

	shd = wt_GetShader(doc->context, "__def_block_shader");

	ren = wt_batch_create(
			shd,		/* Pointer to the shader to use */
			NULL,		/* Pointer to the texture to use */
			3,		/* Number of vertex attributes */
			v_attributes,	/* List of all vertex attributes */
			6000,		/* Vertex capacity */
			6000,		/* Index capacity */
			8,		/* Number of uniform buffers */
			uniforms,	/* List of all uniforms */
			&doc_batch_cfnc_push,
			doc->shape_ref
			);

	if(!ren)
		return -1;

	if((doc->batch_id = wt_ContextAddBatch(doc->context, &ren)) < 0) {
		wt_batch_destroy(ren);
		return -1;
	}

	return 0;
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


WT_API struct wt_document *wt_CreateDocument(struct wt_window *win)
{
	struct wt_document *doc;

	/* Allocate memory for the document */
	if(!(doc = wt_zalloc(sizeof(struct wt_document)))) {
		WT_ALARM(WT_ERROR, "Failed to allocate memory for document");
		goto err_return;
	}

	/* Set the attributes for the document */
	doc->window = win;
	doc->context = win->context;

	/* Get a reference to the shape/size of the window */
	doc->shape_ref = &win->shape;

	/* Create the body element */	
	if(!(doc->body = wt_CreateElement(doc, "body", WT_BODY, NULL))) {
		WT_ALARM(WT_ERROR, "Failed to create body for document");
		goto err_free_doc;
	}

	/* Set the attributes for the body element */
	doc->body->body = doc->body;
	doc->body->parent = NULL;
	doc->body->layer = 0;

	doc->selected	= NULL;
	doc->hovered	= NULL;

	/* Create the view list */
	if(!(doc->views = wt_CreateViewList(doc->context)))
		goto err_destroy_body;

	/* Create the default batch renderer */
	if(doc_create_batch(doc) < 0)
		goto err_destroy_views;

	/* Update the body element */
	wt_UpdateDocument(doc);

	return doc;


err_destroy_views:
	wt_DestroyViewList(doc->views);

err_destroy_body:
	wt_DestroyElement(doc->body);

err_free_doc:
	wt_free(doc);

err_return:
	WT_ALARM(WT_ERROR, "Failed to create new document");
	return NULL;
}


WT_API void wt_DestroyDocument(struct wt_document *doc)
{
	if(!doc)
		return;

	/* If the document contains a body, recursivly remove it */
	wt_RemoveElement(doc, doc->body);	

	/* Then destroy all left over views */
	wt_DestroyViewList(doc->views);

	wt_free(doc);
}


WT_API void wt_ResizeDocument(struct wt_document *doc)
{
	if(!doc) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	/* Then resize the elements */
	wt_UpdateDocument(doc);
}


WT_API struct wt_element *wt_AddElement(struct wt_document *doc,
		struct wt_element *parent, char *name,
		enum wt_element_type type, void *data)
{
	struct wt_element *ele;

	if(!doc || !parent || !name) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	/* Create new element */
	if(!(ele = wt_CreateElement(doc, name, type, data))) {
		WT_ALARM(WT_ERROR, "Failed to create new element for document");
		goto err_return;
	}

	/* Attach element to parent */
	if(wt_AttachElement(parent, ele) < 0) {
		WT_ALARM(WT_ERROR, "Failed to attach element to parent");
		goto err_destroy_ele;
	}

	/* Update the new element */
	wt_UpdateDocumentBranch(doc, ele);

	return ele;

err_destroy_ele:
	wt_DestroyElement(ele);

err_return:
	WT_ALARM(WT_ERROR, "Failed to add element to document");
	return NULL;
}


WT_API void wt_RemoveElement(struct wt_document *doc, struct wt_element *ele)
{
	if(!doc || !ele) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	wt_element_hlf(ele, &doc_cfnc_remove, NULL, NULL);
}


WT_API struct wt_element *wt_GetElement(struct wt_document *doc, char *name)
{
	struct wt_ele_selector sel;

	if(!doc || !name) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	sel.state = 0;
	sel.name = name;
	sel.element = NULL;

	/* Recursivly search for the element in the document */
	wt_element_hlf(doc->body, NULL, &doc_cfnc_find, &sel);

	if(sel.state == 1) {
		return sel.element;
	}

err_return:
	WT_ALARM(WT_ERROR, "Failed to get element");
	return NULL;
}


WT_API struct wt_element *wt_GetHoveredElement(struct wt_document *doc,
		struct wt_sin2 *pos)
{
	struct wt_ele_selector sel;

	if(!doc) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	sel.state = 0;
	sel.pos = pos;
	sel.element = NULL;

	wt_element_hlf(doc->body, NULL, &doc_cfnc_findpos, &sel);

	if(sel.state == 1) {
		printf("Found an element\n");
		return sel.element;
	}

	printf("Found no element\n");

	return NULL;

err_return:
	WT_ALARM(WT_ERROR, "Failed to get hovered element");
	return NULL;
}


WT_API void wt_UpdateDocumentBranch(struct wt_document *doc,
		struct wt_element *ele)
{
	if(!doc || !ele) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	/* First update the style */
	wt_element_hlf(ele, &doc_cfnc_update_style, NULL, NULL);

	/* 
	 * Then update the shape of the element. Note that this works in a very
	 * specifiy way. The function to update the shape will apply the new
	 * layout to the children-elements. Because of that, we have to call the
	 * function with the parent element. To update the position and shape of
	 * the wanted element.
	 */
	if(!ele->parent) {
		wt_element_adjust_shape(ele);

		wt_element_hlf(ele, &doc_cfnc_update_shape, NULL, NULL);
	}
	else {
		wt_element_hlf(ele->parent, &doc_cfnc_update_shape, NULL, NULL);
	}
}


WT_API void wt_UpdateDocument(struct wt_document *doc)
{
	if(!doc) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	wt_UpdateDocumentBranch(doc, doc->body);
}


WT_API void wt_RenderDocumentUIBranch(struct wt_document *doc,
		struct wt_element *ele)
{
	struct wt_batch *ren;

	if(!doc || !ele) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	ren = wt_ContextGetBatch(doc->context, doc->batch_id);

	wt_element_hlf(ele,
			&doc_cfnc_render_ui,
			&doc_cfnc_render_ui_post,
			ren);
}


WT_API void wt_RenderDocumentUI(struct wt_document *doc)
{
	if(!doc) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	wt_RenderDocumentUIBranch(doc, doc->body);

}


WT_API void wt_RenderDocument(struct wt_document *doc)
{
	struct wt_batch *ren;

	if(!doc) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	ren = wt_ContextGetBatch(doc->context, doc->batch_id);

	wt_RenderDocumentUI(doc);
}


WT_API void wt_ShowDocumentTree(struct wt_document *doc,
		struct wt_element *ele)
{
	struct wt_element *start;

	if(!doc) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	if(ele) start = ele;
	else start = doc->body;

	wt_element_hlf(start, &doc_cfnc_show, NULL, NULL);
}
