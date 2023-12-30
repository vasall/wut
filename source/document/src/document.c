#include "document/inc/document.h"

#include "document/inc/element_rendering.h"

#include "utility/alarm/inc/alarm.h"

#include "core/inc/predefined.h"

#include "system/inc/system.h"

#include "utility/inc/utility.h"

#include <stdlib.h>

/*
 * 
 *     CALLBACK-FUNCTIONS
 *
 */

FH_INTERN s8 doc_cfnc_remove(struct fh_element *ele, void *data)
{
	fh_Ignore(data);

	/* First detach the element */
	fh_DetachElement(ele);

	/* Then destroy it */
	fh_DestroyElement(ele);

	return 0;
}

FH_INTERN s8 doc_cfnc_find(struct fh_element *ele, void *data)
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

FH_INTERN s8 doc_cfnc_findpos(struct fh_element *ele, void *data)
{
	struct fh_ele_selector *sel = (struct fh_ele_selector *)data;
	struct fh_rect rect;

	rect = fh_GetElementBox(ele);

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


FH_INTERN s8 doc_cfnc_update_style(struct fh_element *ele, void *data)
{
	fh_Ignore(data);

	fh_UpdateElementStyle(ele);

	return 0;
}


FH_INTERN s8 doc_cfnc_update_shape(struct fh_element *ele, void *data)
{
	fh_Ignore(data);

	fh_UpdateElementChildrenShape(ele);

	return 0;
}


FH_INTERN s8 doc_cfnc_render_ui(struct fh_element *ele, void *data)
{
	struct fh_batch *batch = (struct fh_batch *)data;

	fh_element_render(batch, ele);

	return 0;
}


FH_INTERN s8 doc_cfnc_render_ui_post(struct fh_element *ele, void *data)
{
	return 0;

	fh_Ignore(data);

	fh_element_ren_scrollbar(ele);

	return 0;
}


FH_INTERN s8 doc_cfnc_show(struct fh_element  *ele, void *data)
{
	u8 i;

	s32 lim = 16;

	struct fh_rect r;

	fh_Ignore(data);

	for(i = 0; i < ele->layer; i++) {
		printf("  ");
		lim -= 2;
	}

	printf("%s ", ele->name);
	lim -= strlen(ele->name);

	for(i = 0; i < lim; i++)
		printf(" ");


	r = fh_GetBoundingBox(ele);
	fh_rect_dump(&r);
	printf("  --  ");
	r = fh_GetElementBox(ele);
	fh_rect_dump(&r);
	printf("  --  ");
	r = fh_GetContentBox(ele);
	fh_rect_dump(&r);

	printf("\n");

	return 0;

}


FH_INTERN struct fh_shader *doc_create_batch_shader(struct fh_document *doc)
{
	return fh_CreateShader(
			doc->context, 
			"batch_shader", 
			(const char *)fh_ps_batch_vshader,
			(const char *)fh_ps_batch_fshader
			);
}


FH_INTERN struct fh_batch *doc_create_batch(struct fh_shader *shd)
{
	struct fh_vertex_attrib v_attributes[] = {
		{2, GL_INT},		/* position */
		{1, GL_INT}		/* index */
	};

	struct fh_uniform_temp uniforms[] = {
		{"u_frame", FH_UNIFORM_2IV, 1, FH_UNIFORM_F_ALL|FH_UNIFORM_F_CLEANUP},
		{"u_rect", FH_UNIFORM_4IV, 500, FH_UNIFORM_F_ALL|FH_UNIFORM_F_CLEANUP},
		{"u_color", FH_UNIFORM_4FV, 500, FH_UNIFORM_F_ALL|FH_UNIFORM_F_CLEANUP},
		{"u_radius", FH_UNIFORM_4IV, 500, FH_UNIFORM_F_ALL|FH_UNIFORM_F_CLEANUP},
		{"u_bwidth", FH_UNIFORM_1IV, 500, FH_UNIFORM_F_ALL|FH_UNIFORM_F_CLEANUP},
		{"u_bcolor", FH_UNIFORM_4FV, 500, FH_UNIFORM_F_ALL|FH_UNIFORM_F_CLEANUP}
	};

	return fh_batch_create(
			shd,			/* Pointer to the shader to use */
			2,			/* Number of vertex attributes */
			v_attributes,		/* List of all vertex attributes */
			6000,			/* Vertex capacity */
			6000,			/* Index capacity */
			5,			/* Number of uniform buffers */
			uniforms		/* List of all uniforms */
			);
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


FH_API struct fh_document *fh_CreateDocument(struct fh_window *win)
{
	struct fh_document *doc;

	/* Allocate memory for the document */
	if(!(doc = fh_zalloc(sizeof(struct fh_document)))) {
		FH_ALARM(FH_ERROR, "Failed to allocate memory for document");
		goto err_return;
	}

	/* Set the attributes for the document */
	doc->window = win;
	doc->context = win->context;

	/* Get a reference to the shape/size of the window */
	doc->shape_ref = &win->shape;

	/* Create the body element */	
	if(!(doc->body = fh_CreateElement(doc, "body", FH_BODY, NULL))) {
		FH_ALARM(FH_ERROR, "Failed to create body for document");
		goto err_free_doc;
	}

	/* Set the attributes for the body element */
	doc->body->body = doc->body;
	doc->body->parent = NULL;
	doc->body->layer = 0;

	doc->selected	= NULL;
	doc->hovered	= NULL;

	/* Create the view list */
	if(!(doc->views = fh_CreateViewList(doc->context)))
		goto err_destroy_body;

	/* Create the shader used for batch rendering */
	if(!(doc->batch_shader = doc_create_batch_shader(doc)))
		goto err_destroy_views;

	/* Create the batch renderer */
	if(!(doc->batch = doc_create_batch(doc->batch_shader)))
		goto err_remove_shader;

	/* Update the body element */
	fh_UpdateDocument(doc);

	return doc;

err_remove_shader:
	fh_RemoveShader(doc->batch_shader);

err_destroy_views:
	fh_DestroyViewList(doc->views);

err_destroy_body:
	fh_DestroyElement(doc->body);

err_free_doc:
	fh_free(doc);

err_return:
	FH_ALARM(FH_ERROR, "Failed to create new document");
	return NULL;
}


FH_API void fh_DestroyDocument(struct fh_document *doc)
{
	if(!doc)
		return;


	/* If the document contains a body, recursivly remove it */
	fh_RemoveElement(doc, doc->body);	

	/* Then destroy all left over views */
	fh_DestroyViewList(doc->views);

	fh_free(doc);
}


FH_API void fh_ResizeDocument(struct fh_document *doc)
{
	if(!doc) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	/* Then resize the elements */
	fh_UpdateDocument(doc);
}


FH_API struct fh_element *fh_AddElement(struct fh_document *doc,
		struct fh_element *parent, char *name,
		enum fh_element_type type, void *data)
{
	struct fh_element *ele;

	if(!doc || !parent || !name) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		goto err_return;
	}

	/* Create new element */
	if(!(ele = fh_CreateElement(doc, name, type, data))) {
		FH_ALARM(FH_ERROR, "Failed to create new element for document");
		goto err_return;
	}

	/* Attach element to parent */
	if(fh_AttachElement(parent, ele) < 0) {
		FH_ALARM(FH_ERROR, "Failed to attach element to parent");
		goto err_destroy_ele;
	}

	/* Update the new element */
	fh_UpdateDocumentBranch(doc, ele);

	return ele;

err_destroy_ele:
	fh_DestroyElement(ele);

err_return:
	FH_ALARM(FH_ERROR, "Failed to add element to document");
	return NULL;
}


FH_API void fh_RemoveElement(struct fh_document *doc, struct fh_element *ele)
{
	if(!doc || !ele) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	fh_element_hlf(ele, &doc_cfnc_remove, NULL, NULL);
}


FH_API struct fh_element *fh_GetElement(struct fh_document *doc, char *name)
{
	struct fh_ele_selector sel;

	if(!doc || !name) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		goto err_return;
	}

	sel.state = 0;
	sel.name = name;
	sel.element = NULL;

	/* Recursivly search for the element in the document */
	fh_element_hlf(doc->body, NULL, &doc_cfnc_find, &sel);

	if(sel.state == 1) {
		return sel.element;
	}

err_return:
	FH_ALARM(FH_ERROR, "Failed to get element");
	return NULL;
}


FH_API struct fh_element *fh_GetHoveredElement(struct fh_document *doc,
		struct fh_sin2 *pos)
{
	struct fh_ele_selector sel;

	if(!doc) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		goto err_return;
	}

	sel.state = 0;
	sel.pos = pos;
	sel.element = NULL;

	fh_element_hlf(doc->body, NULL, &doc_cfnc_findpos, &sel);

	if(sel.state == 1) {
		return sel.element;
	}

	return NULL;

err_return:
	FH_ALARM(FH_ERROR, "Failed to get hovered element");
	return NULL;
}


FH_API void fh_UpdateDocumentBranch(struct fh_document *doc,
		struct fh_element *ele)
{
	if(!doc || !ele) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	/* First update the style */
	fh_element_hlf(ele, &doc_cfnc_update_style, NULL, NULL);

	/* 
	 * Then update the shape of the element. Note that this works in a very
	 * specifiy way. The function to update the shape will apply the new
	 * layout to the children-elements. Because of that, we have to call the
	 * function with the parent element. To update the position and shape of
	 * the wanted element.
	 */
	if(!ele->parent) {
		fh_element_calc_render_rect(ele);

		fh_element_hlf(ele, &doc_cfnc_update_shape, NULL, NULL);
	}
	else {
		fh_element_hlf(ele->parent, &doc_cfnc_update_shape, NULL, NULL);
	}
}


FH_API void fh_UpdateDocument(struct fh_document *doc)
{
	if(!doc) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	fh_UpdateDocumentBranch(doc, doc->body);
}


FH_API void fh_RenderDocumentUIBranch(struct fh_document *doc,
		struct fh_element *ele)
{
	if(!doc || !ele) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	fh_element_hlf(ele,
			&doc_cfnc_render_ui,
			&doc_cfnc_render_ui_post,
			doc->batch);
}


FH_API void fh_RenderDocumentUI(struct fh_document *doc)
{
	if(!doc) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	fh_RenderDocumentUIBranch(doc, doc->body);

}


FH_API void fh_RenderDocument(struct fh_document *doc)
{
	s32 frame[2];

	if(!doc) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	frame[0] = doc->shape_ref->w;
	frame[1] = doc->shape_ref->h;
	fh_batch_push_uniform(doc->batch, 0, frame);

	fh_RenderDocumentUI(doc);

	fh_batch_flush(doc->batch);
}


FH_API void fh_ShowDocumentTree(struct fh_document *doc,
		struct fh_element *ele)
{
	struct fh_element *start;

	if(!doc) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	if(ele) start = ele;
	else start = doc->body;

	fh_element_hlf(start, &doc_cfnc_show, NULL, NULL);
}
