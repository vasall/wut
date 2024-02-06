#include "widget/inc/widget.h"

#include "utility/alarm/inc/alarm.h"

#include "widget/inc/view.h"

#include "document/inc/document.h"

#include "system/inc/system.h"


#include <stdlib.h>

/*
 * 
 * 		CREATE
 *
 */

FH_INTERN s8 widget_create_text(struct fh_widget *w, void *data)
{
	fh_Ignore(w);
	fh_Ignore(data);

	return 0;
}


FH_INTERN s8 widget_create_image(struct fh_widget *w, void *data)
{
	fh_Ignore(w);

	w->ref = data;

	return 0;
}


FH_INTERN s8 widget_create_view(struct fh_widget *w, void *data)
{
	struct fh_rect rect = fh_GetElementBox(w->element);
	struct fh_view_list *lst;

	fh_Ignore(data);

	lst = w->element->document->views;

	if(!(w->ref = fh_CreateView(lst, &rect)))
		goto err_return;

	return 0;

err_return:
	FH_ALARM(FH_ERROR, "Failed to create view widget");
	return -1;
}


/*
 * 
 * 		DESTROY
 *
 */

FH_INTERN void widget_destroy_text(struct fh_widget *w)
{
	fh_Ignore(w);
}


FH_INTERN void widget_destroy_image(struct fh_widget *w)
{
	fh_Ignore(w);
}


FH_INTERN void widget_destroy_view(struct fh_widget *w)
{
	fh_DestroyView(w->ref);
}


/*
 * 
 * 		UPDATE
 *
 */

FH_INTERN void widget_update_text(struct fh_widget *w, void *data)
{
	fh_Ignore(w);
	fh_Ignore(data);

}


FH_INTERN void widget_update_image(struct fh_widget *w, void *data)
{
	fh_Ignore(w);
	fh_Ignore(data);

}


FH_INTERN void widget_update_view(struct fh_widget *w, void *data)
{
	struct fh_rect rect = fh_GetElementBox(w->element);

	fh_Ignore(data);

	fh_ResizeView(w->ref, &rect);
}


/*
 * 
 * 		RENDER
 *
 */

FH_INTERN void widget_render_text(struct fh_widget *w)
{
	fh_Ignore(w);
}


FH_INTERN void widget_render_image(struct fh_widget *w)
{

	s32 indices[4];
	s32 v_index[3];
	f32 color[4];

	struct tempStruct_vtx {
		f32 x;
		f32 y;
		f32 z;
		s32 index[3];
		s32 type;
	} vdata;

	s32 p0x = ele->element_rect.x;
	s32 p0y = ele->element_rect.y;
	s32 p1x = ele->element_rect.x + ele->element_rect.w;
	s32 p1y = ele->element_rect.y + ele->element_rect.h;


	/*
	 * Return if the element is not visible.
	 */
	if(!(ele->info_flags & FH_ELEMENT_F_VISIBLE))
		return;

	/* Unioform: u_rect */
	v_index[0] = fh_batch_push_uniform(ren, 1, &ele->element_rect);

	/* Uniform: u_color */
	fh_color_get_fv(ele->style.infill.color, color);
	v_index[2] = fh_batch_push_uniform(ren, 2, color);

	/* Uniform: u_radius */
	fh_batch_push_uniform(ren, 3, ele->style.radius.corner);

	/* Uniform: u_bwidth */
	fh_batch_push_uniform(ren, 4, &ele->style.border.width);

	/* Uniform: u_bcolor */
	fh_color_get_fv(ele->style.border.color, color);
	fh_batch_push_uniform(ren, 5, color);

	/* Uniform: u_limit */
	if(ele->parent) {
		v_index[1] = fh_batch_push_uniform(ren, 7, &ele->parent->inner_rect);
	}
	else {
		v_index[1] = -1;
	}
		
	vdata.z = (f32)ele->layer / 100.0;
	vdata.index[0] = v_index[0];	/* The element rectangle */
	vdata.index[1] = v_index[1];	/* The rendering zone */
	vdata.index[2] = v_index[2];	/* The color to use */
	vdata.type = FH_RENTYPE_DEFAULT;


	vdata.x = (f32)p0x;
	vdata.y = (f32)p0y;
	indices[0] = fh_batch_push_vertex(ren, (void *)&vdata);


	vdata.x = (f32)p1x;
	vdata.y = (f32)p0y;
	indices[1] = fh_batch_push_vertex(ren, (void *)&vdata);

	vdata.x = (f32)p1x;
	vdata.y = (f32)p1y;
	indices[2] = fh_batch_push_vertex(ren, (void *)&vdata);


	vdata.x = (f32)p0x;
	vdata.y = (f32)p1y;
	indices[3] = fh_batch_push_vertex(ren, (void *)&vdata);

	fh_batch_push_index(ren, indices[0]);
	fh_batch_push_index(ren, indices[2]);
	fh_batch_push_index(ren, indices[3]);

	fh_batch_push_index(ren, indices[0]);
	fh_batch_push_index(ren, indices[1]);
	fh_batch_push_index(ren, indices[2]);
}


FH_INTERN void widget_render_view(struct fh_widget *w)
{
	fh_RenderView(w->ref);
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

FH_API struct fh_widget *fh_CreateWidget(struct fh_element *ele,
		enum fh_widget_type type, void *data)
{
	struct fh_widget *w;
	s8 r = -1;

	if(!ele) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(!(w = fh_malloc(sizeof(struct fh_widget)))) {
		FH_ALARM(FH_ERROR, "Failed to allocate memory for widget");
		goto err_return;
	}

	/* Set attributes */
	w->element = ele;
	w->type = type;

	switch(type) {
		case FH_WIDGET_TEXT: 
			r = widget_create_text(w, data);
			break;
		case FH_WIDGET_IMAGE: 
			r = widget_create_image(w, data); 
			break;
		case FH_WIDGET_VIEW: 
			r = widget_create_view(w, data); 
			break;
		default: FH_ALARM(FH_ERROR, "Type not found"); break;
	}

	if(r < 0) goto err_free_comp;

	return w;

err_free_comp:
	fh_free(w);

err_return:
	FH_ALARM(FH_ERROR, "Failed to create widget");
	return NULL;
}


FH_API void fh_DestroyWidget(struct fh_widget *w)
{
	if(!w) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	switch(w->type) {
		case FH_WIDGET_TEXT:
			widget_destroy_text(w);
			break;
		case FH_WIDGET_IMAGE:
			widget_destroy_image(w);
			break;
		case FH_WIDGET_VIEW: 
			widget_destroy_view(w);
			break;	
	}

	fh_free(w);
}


FH_API void fh_UpdateWidget(struct fh_widget *w, void *data)
{
	if(!w) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	switch(w->type) {
		case FH_WIDGET_TEXT:
			widget_update_text(w, data);
			break;
		case FH_WIDGET_IMAGE:
			widget_update_image(w, data);
			break;
		case FH_WIDGET_VIEW:
			widget_update_view(w, data);
			break;
	}
}


FH_API void fh_RenderWidget(struct fh_widget *w)
{
	if(!w) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	switch(w->type) {
		case FH_WIDGET_TEXT:
			widget_render_text(w);
			break;
		case FH_WIDGET_IMAGE:
			widget_render_image(w);
			break;
		case FH_WIDGET_VIEW:
			widget_render_view(w);
			break;
	}
}
