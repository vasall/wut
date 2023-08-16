#include "widget.h"

#include "document.h"
#include "view.h"

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
	fh_Ignore(data);

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
	ALARM(ALARM_ERR, "Failed to create view widget");
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
	fh_Ignore(w);
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
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(!(w = fh_malloc(sizeof(struct fh_widget)))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for widget");
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
		default: ALARM(ALARM_ERR, "Type not found"); break;
	}

	if(r < 0) goto err_free_comp;

	return w;

err_free_comp:
	fh_free(w);

err_return:
	ALARM(ALARM_ERR, "Failed to create widget");
	return NULL;
}


FH_API void fh_DestroyWidget(struct fh_widget *w)
{
	if(!w) {
		ALARM(ALARM_WARN, "Input parameters invalid");
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
		ALARM(ALARM_WARN, "Input parameters invalid");
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
		ALARM(ALARM_WARN, "Input parameters invalid");
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
