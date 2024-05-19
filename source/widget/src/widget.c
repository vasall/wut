#include "widget/inc/widget.h"

#include "utility/inc/alarm.h"

#include "widget/inc/textfield.h"
#include "widget/inc/view.h"

#include "document/inc/document.h"

#include "system/inc/system.h"

#include"graphic/inc/shader.h"

#include <stdlib.h>

/*
 * 
 * 		CREATE
 *
 */

WT_INTERN s8 widget_create_text(struct wt_widget *w, void *data)
{
	struct wt_textfield *txt;

	if(!(txt = wt_textfield_create(w->element, data))) {
		WT_ALARM(WT_ERROR, "Failed to create textfield");
		return -1;
	}

	w->ref = txt;

	return 0;
}


WT_INTERN s8 widget_create_image(struct wt_widget *w, void *data)
{
	wt_Ignore(w);

	w->ref = data;

	return 0;
}


WT_INTERN s8 widget_create_view(struct wt_widget *w, void *data)
{
	struct wt_rect rect = wt_GetElementBox(w->element);
	struct wt_view_list *lst;

	wt_Ignore(data);

	lst = w->element->document->views;

	if(!(w->ref = wt_CreateView(lst, &rect)))
		goto err_return;

	return 0;

err_return:
	WT_ALARM(WT_ERROR, "Failed to create view widget");
	return -1;
}


/*
 * 
 * 		DESTROY
 *
 */

WT_INTERN void widget_destroy_text(struct wt_widget *w)
{
	wt_textfield_destroy(w->ref);
}


WT_INTERN void widget_destroy_image(struct wt_widget *w)
{
	wt_Ignore(w);
}


WT_INTERN void widget_destroy_view(struct wt_widget *w)
{
	wt_DestroyView(w->ref);
}


/*
 * 
 * 		UPDATE
 *
 */

WT_INTERN void widget_update_text(struct wt_widget *w, void *data)
{
	wt_Ignore(w);
	wt_Ignore(data);

	wt_textfield_update(w->ref);
}


WT_INTERN void widget_update_image(struct wt_widget *w, void *data)
{
	wt_Ignore(w);
	wt_Ignore(data);

}


WT_INTERN void widget_update_view(struct wt_widget *w, void *data)
{
	struct wt_rect rect = wt_GetElementBox(w->element);

	wt_Ignore(data);

	wt_ResizeView(w->ref, &rect);
}


/*
 * 
 * 		RENDER
 *
 */

WT_INTERN void widget_render_text(struct wt_widget *w)
{
	wt_textfield_render(w->ref);
}


WT_INTERN void widget_render_image(struct wt_widget *w)
{
	struct wt_element *ele;
	struct wt_texture *tex;
	struct wt_batch *ren;

	s32 indices[4];
	s32 v_index[3];

	struct tempStruct_vtx {
		f32 x;
		f32 y;
		f32 z;
		f32 u;
		f32 v;
		s32 index[3];
	} vdata;
	
	s32 p0x;
	s32 p0y;
	s32 p1x;
	s32 p1y;


	ele = w->element;
	tex = w->ref;
	ren = wt_ContextGetBatch(ele->document->context, tex->batch_id);
		
	p0x = ele->inner_rect.x;
	p0y = ele->inner_rect.y;
	p1x = ele->inner_rect.x + ele->inner_rect.w;
	p1y = ele->inner_rect.y + ele->inner_rect.h;

	/* Uniform: u_rect */
	v_index[0] = wt_batch_push_uniform(ren, 1, &ele->inner_rect);

	/* Uniform: u_radius */
	v_index[2] = wt_batch_push_uniform(ren, 2, ele->style.radius_corner);

	/* Uniform: u_limit */
	if(ele->parent) {
		v_index[1] = wt_batch_push_uniform(ren, 3, &ele->parent->inner_rect);
	}
	else {
		v_index[1] = -1;
	}
		
	vdata.z = (f32)ele->layer / 100.0;
	vdata.index[0] = v_index[0];	/* The element rectangle */
	vdata.index[1] = v_index[1];	/* The rendering zone */
	vdata.index[2] = v_index[2];

	vdata.x = (f32)p0x;
	vdata.y = (f32)p0y;
	vdata.u = 0;
	vdata.v = 1;
	indices[0] = wt_batch_push_vertex(ren, (void *)&vdata);


	vdata.x = (f32)p1x;
	vdata.y = (f32)p0y;
	vdata.u = 1;
	vdata.v = 1;
	indices[1] = wt_batch_push_vertex(ren, (void *)&vdata);

	vdata.x = (f32)p1x;
	vdata.y = (f32)p1y;
	vdata.u = 1;
	vdata.v = 0;
	indices[2] = wt_batch_push_vertex(ren, (void *)&vdata);


	vdata.x = (f32)p0x;
	vdata.y = (f32)p1y;
	vdata.u = 0;
	vdata.v = 0;
	indices[3] = wt_batch_push_vertex(ren, (void *)&vdata);

	wt_batch_push_index(ren, indices[0]);
	wt_batch_push_index(ren, indices[2]);
	wt_batch_push_index(ren, indices[3]);

	wt_batch_push_index(ren, indices[0]);
	wt_batch_push_index(ren, indices[1]);
	wt_batch_push_index(ren, indices[2]);
}


WT_INTERN void widget_render_view(struct wt_widget *w)
{
	wt_RenderView(w->ref);
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

WT_API struct wt_widget *wt_CreateWidget(struct wt_element *ele,
		enum wt_widget_type type, void *data)
{
	struct wt_widget *w;
	s8 r = -1;

	if(!ele) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(!(w = wt_malloc(sizeof(struct wt_widget)))) {
		WT_ALARM(WT_ERROR, "Failed to allocate memory for widget");
		goto err_return;
	}

	/* Set attributes */
	w->element = ele;
	w->type = type;

	printf("Create widget\n");

	switch(type) {
		case WT_WIDGET_TEXT: 
			r = widget_create_text(w, data);
			break;
		case WT_WIDGET_IMAGE: 
			r = widget_create_image(w, data); 
			break;
		case WT_WIDGET_VIEW: 
			r = widget_create_view(w, data); 
			break;
		default: WT_ALARM(WT_ERROR, "Type not found"); break;
	}

	if(r < 0) goto err_free_comp;

	return w;

err_free_comp:
	wt_free(w);

err_return:
	WT_ALARM(WT_ERROR, "Failed to create widget");
	return NULL;
}


WT_API void wt_DestroyWidget(struct wt_widget *w)
{
	if(!w) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	switch(w->type) {
		case WT_WIDGET_TEXT:
			widget_destroy_text(w);
			break;
		case WT_WIDGET_IMAGE:
			widget_destroy_image(w);
			break;
		case WT_WIDGET_VIEW: 
			widget_destroy_view(w);
			break;	
	}

	wt_free(w);
}


WT_API void wt_UpdateWidget(struct wt_widget *w, void *data)
{
	if(!w) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	switch(w->type) {
		case WT_WIDGET_TEXT:
			widget_update_text(w, data);
			break;
		case WT_WIDGET_IMAGE:
			widget_update_image(w, data);
			break;
		case WT_WIDGET_VIEW:
			widget_update_view(w, data);
			break;
	}
}


WT_API void wt_RenderWidget(struct wt_widget *w)
{
	if(!w) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	switch(w->type) {
		case WT_WIDGET_TEXT:
			widget_render_text(w);
			break;
		case WT_WIDGET_IMAGE:
			widget_render_image(w);
			break;
		case WT_WIDGET_VIEW:
			widget_render_view(w);
			break;
	}
}
