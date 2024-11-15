#include "source/widget/inc/widget.h"

#include "source/utility/inc/alarm.h"

#include "source/widget/inc/textfield.h"
#include "source/widget/inc/view.h"

#include "source/document/inc/document.h"

#include "source/system/inc/system.h"

#include "source/graphic/inc/shader.h"

#include <stdlib.h>

/*
 * 
 * 		CREATE
 *
 */

WUT_INTERN s8 wgt_create_text(struct wut_Widget *w, void *data)
{
	struct wut_Textfield *txt;

	if(!(txt = wut_tfd_create(w->element, data))) {
		WUT_ALARM(WUT_ERROR, "Failed to create textfield");
		return -1;
	}

	w->ref = txt;

	return 0;
}


WUT_INTERN s8 wgt_create_image(struct wut_Widget *w, void *data)
{
	WUT_IGNORE(w);

	w->ref = data;

	return 0;
}


WUT_INTERN s8 wgt_create_view(struct wut_Widget *w, void *data)
{
	wut_iRect rect;
	struct wut_ViewList *lst;

	WUT_IGNORE(data);

	lst = w->element->document->views;

        wut_GetElementBox(w->element, rect);

	if(!(w->ref = wut_vie_create(lst, &rect)))
		goto err_return;

	return 0;

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to create view widget");
	return -1;
}


/*
 * 
 * 		DESTROY
 *
 */

WUT_INTERN void wgt_destroy_text(struct wut_Widget *w)
{
	wut_tfd_destroy(w->ref);
}


WUT_INTERN void wgt_destroy_image(struct wut_Widget *w)
{
	WUT_IGNORE(w);
}


WUT_INTERN void wgt_destroy_view(struct wut_Widget *w)
{
	wut_vie_destroy(w->ref);
}


/*
 * 
 * 		UPDATE
 *
 */

WUT_INTERN void wgt_update_text(struct wut_Widget *w, void *data)
{
	WUT_IGNORE(w);
	WUT_IGNORE(data);

	wut_tfd_update(w->ref);
}


WUT_INTERN void wgt_update_image(struct wut_Widget *w, void *data)
{
	WUT_IGNORE(w);
	WUT_IGNORE(data);

}


WUT_INTERN void wgt_update_view(struct wut_Widget *w, void *data)
{
	wut_iRect rect;

	WUT_IGNORE(data);

        wut_GetElementBox(w->element, rect);
	wut_vie_resize(w->ref, &rect);
}


/*
 * 
 * 		RENDER
 *
 */

WUT_INTERN void wgt_render_text(struct wut_Widget *w)
{
	wut_tfd_render(w->ref);
}


WUT_INTERN void wgt_render_image(struct wut_Widget *w)
{
	struct wut_Element *ele;
        struct wut_Context *ctx;
	struct wut_Texture *tex;
	struct wut_Batch *ren;

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
        ctx = ele->document->context;
	tex = w->ref;
	ren = wut_ContextGetBatch(ctx, tex->batch_id);
		
	p0x = ele->element_rect[0];
	p0y = ele->element_rect[1];
	p1x = ele->element_rect[0] + ele->element_rect[2];
	p1y = ele->element_rect[1] + ele->element_rect[3];

	/* Uniform: u_rect */
	v_index[0] = wut_bat_push_uniform(ren, 1, ele->inner_rect);

	/* Uniform: u_radius */
	v_index[2] = wut_bat_push_uniform(ren, 2, ele->style.radius_corner);

	/* Uniform: u_limit */
	if(ele->parent) {
		v_index[1] = wut_bat_push_uniform(ren, 3, ele->parent->content_rect);
	}
	else {
		v_index[1] = -1;
	}

        printf("Render image\n");

	vdata.z = (f32)ele->layer;
	vdata.index[0] = v_index[0];	/* The element rectangle */
	vdata.index[1] = v_index[1];	/* The rendering zone */
	vdata.index[2] = v_index[2];

	vdata.x = (f32)p0x;
	vdata.y = (f32)p0y;
	vdata.u = 0.0;
	vdata.v = 1.0;
	indices[0] = wut_bat_push_vertex(ren, (void *)&vdata);


        printf("%d, %d\n", p0x, p0y);

	vdata.x = (f32)p1x;
	vdata.y = (f32)p0y;
	vdata.u = 1.0;
	vdata.v = 1.0;
	indices[1] = wut_bat_push_vertex(ren, (void *)&vdata);

        printf("%d, %d\n", p1x, p0y);

	vdata.x = (f32)p1x;
	vdata.y = (f32)p1y;
	vdata.u = 1.0;
	vdata.v = 0.0;
	indices[2] = wut_bat_push_vertex(ren, (void *)&vdata);

        printf("%d, %d\n", p1x, p1y);

	vdata.x = (f32)p0x;
	vdata.y = (f32)p1y;
	vdata.u = 0.0;
	vdata.v = 0.0;
	indices[3] = wut_bat_push_vertex(ren, (void *)&vdata);

        printf("%d, %d\n", p0x, p1y);

	wut_bat_push_index(ren, indices[0]);
	wut_bat_push_index(ren, indices[2]);
	wut_bat_push_index(ren, indices[3]);

	wut_bat_push_index(ren, indices[0]);
	wut_bat_push_index(ren, indices[1]);
	wut_bat_push_index(ren, indices[2]);
}


WUT_INTERN void wgt_render_view(struct wut_Widget *w)
{
	wut_vie_render(w->ref);
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

WUT_API struct wut_Widget *wut_CreateWidget(struct wut_Element *ele,
		enum wut_eWidgetType type, void *data)
{
	struct wut_Widget *w;
	s8 r = -1;

	if(!ele) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(!(w = wut_malloc(sizeof(struct wut_Widget)))) {
		WUT_ALARM(WUT_ERROR, "Failed to allocate memory for widget");
		goto err_return;
	}

	/* Set attributes */
	w->element = ele;
	w->type = type;

	printf("Create widget\n");

	switch(type) {
		case WUT_WIDGET_TEXT: 
			r = wgt_create_text(w, data);
			break;
		case WUT_WIDGET_IMAGE: 
			r = wgt_create_image(w, data); 
			break;
		case WUT_WIDGET_VIEW: 
			r = wgt_create_view(w, data); 
			break;
		default: WUT_ALARM(WUT_ERROR, "Type not found"); break;
	}

	if(r < 0) goto err_free_comp;

	return w;

err_free_comp:
	wut_free(w);

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to create widget");
	return NULL;
}


WUT_API void wut_DestroyWidget(struct wut_Widget *w)
{
	if(!w) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

	switch(w->type) {
		case WUT_WIDGET_TEXT:
			wgt_destroy_text(w);
			break;
		case WUT_WIDGET_IMAGE:
			wgt_destroy_image(w);
			break;
		case WUT_WIDGET_VIEW: 
			wgt_destroy_view(w);
			break;	
	}

	wut_free(w);
}


WUT_API void wut_UpdateWidget(struct wut_Widget *w, void *data)
{
	if(!w) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

	switch(w->type) {
		case WUT_WIDGET_TEXT:
			wgt_update_text(w, data);
			break;
		case WUT_WIDGET_IMAGE:
			wgt_update_image(w, data);
			break;
		case WUT_WIDGET_VIEW:
			wgt_update_view(w, data);
			break;
	}
}


WUT_API void wut_RenderWidget(struct wut_Widget *w)
{
	if(!w) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

	switch(w->type) {
		case WUT_WIDGET_TEXT:
			wgt_render_text(w);
			break;
		case WUT_WIDGET_IMAGE:
			wgt_render_image(w);
			break;
		case WUT_WIDGET_VIEW:
			wgt_render_view(w);
			break;
	}
}
