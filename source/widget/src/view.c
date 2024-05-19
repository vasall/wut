#include "widget/inc/view.h"

#include "utility/inc/alarm.h"

#include "system/inc/system.h"

#include <stdlib.h>


WT_INTERN s8 view_get_slot(struct wt_view_list *lst)
{
	s8 i;

	for(i = 0; i < WT_VIEW_LIST_LIM; i++) {
		if(lst->views[i] == NULL)
			return i;
	}

	return -1;
}

WT_INTERN void view_render_objects(struct wt_object *m)
{
	wt_RenderObject(m, NULL, NULL);
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


WT_API struct wt_view_list *wt_CreateViewList(struct wt_context *ctx)
{
	struct wt_view_list *lst;
	u8 i;

	if(!ctx) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(!(lst = wt_malloc(sizeof(struct wt_view_list)))) {
		WT_ALARM(WT_ERROR, "Failed to allocate memory for view list");
		goto err_return;
	}

	/* Set attributes */
	lst->context = ctx;
	lst->number = 0;

	for(i = 0; i < WT_VIEW_LIST_LIM; i++)
		lst->views[i] = NULL;

	return lst;

err_return:
	WT_ALARM(WT_ERROR, "Failed to create view list");
	return NULL;
}


WT_API void wt_DestroyViewList(struct wt_view_list *lst)
{
	s8 i;

	if(!lst) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	for(i = 0; i < WT_VIEW_LIST_LIM; i++) {
		if(lst->views[i]) {
			wt_DestroyView(lst->views[i]);
		}
	}

	wt_free(lst);
}


WT_API void wt_RenderViewList(struct wt_view_list *lst)
{
	s8 i;

	for(i = 0; i < WT_VIEW_LIST_LIM; i++) {
		if(!lst->views[i])
			continue;

		wt_RenderView(lst->views[i]);
	}
}


WT_API struct wt_view *wt_CreateView(struct wt_view_list *lst,
		struct wt_rect *rect)
{
	struct wt_view *v;
	struct wt_camera_info cam_info;
	s8 slot;

	if(!lst) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if((slot = view_get_slot(lst)) < 0) {
		WT_ALARM(WT_ERROR, "No more free slots in view list");
		goto err_return;
	}

	if(!(v = wt_malloc(sizeof(struct wt_view)))) {
		WT_ALARM(WT_ERROR, "Failed to allocate memory for view struct");
		goto err_return;
	}

	/* Set the attributes */
	v->slot = slot;
	v->list = lst;
	wt_rect_cpy(&v->shape, rect);

	/*
	 * Create a camera.
	 */
	cam_info.area_of_view = 60;
        cam_info.aspect_ratio = (f32)rect->w / (f32)rect->h;
        cam_info.near = 0.01;
        cam_info.far = 1000;
	if(!(v->camera = wt_CreateCamera(cam_info, v))) {
		WT_ALARM(WT_ERROR, "Failed to create camera");
		goto err_free_v;
	}

	/* Create a new pipeline */
	if(!(v->pipe = wt_CreatePipe(v->camera->pos))) {
		WT_ALARM(WT_ERROR, "Failed to create new object pipeline");
		goto err_destroy_cam;
	}

	/*
	 * Add view to view list.
	 */

	lst->views[slot] = v;
	lst->number++;

	return v;

err_destroy_cam:
	wt_DestroyCamera(v->camera);

err_free_v:
	wt_free(v);

err_return:
	WT_ALARM(WT_ERROR, "Failed to create new view struct");
	return NULL;
}


WT_API void wt_DestroyView(struct wt_view *v)
{
	struct wt_view_list *lst;

	if(!v)
		return;

	lst = v->list;

	lst->views[v->slot] = NULL;
	lst->number--;	

	wt_DestroyPipe(v->pipe);
	wt_free(v);
}


WT_API void wt_RenderView(struct wt_view *v)
{
	struct wt_context *ctx;

	if(!v)
		return;

	ctx = v->list->context;

	/*
	 * First translate.
	 */
	wt_SetViewport(ctx, &v->shape);


	/*
	 * Then enable scissors.
	 */
	wt_ContextEnableScissor(ctx, &v->shape);


	/*
	 * Now we can render all objects in order.
	 */
	wt_PipeApply(v->pipe, &view_render_objects);			


	/*
	 * Lastly, reset everything.
	 */
	wt_ContextDisableScissor(ctx);
	wt_ResetViewport(ctx);
}


WT_API void wt_ResizeView(struct wt_view *v, struct wt_rect *rect)
{
	struct wt_camera_info info;

	if(!v || !rect) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	wt_rect_cpy(&v->shape, rect);

	info = wt_GetCameraInfo(v->camera);
	info.aspect_ratio = (f32)rect->w / (f32)rect->h;
	wt_SetCameraInfo(v->camera, info);
}


WT_API struct wt_camera *wt_GetViewCamera(struct wt_view *v)
{
	if(!v) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		return NULL;
	}

	return v->camera;
}


WT_API void wt_UpdateViewPipe(struct wt_view *v)
{
	if(!v)
		return;

	wt_PipeSetReference(v->pipe, v->camera->pos);
}


WT_API s8 wt_ViewAddObject(struct wt_view *v, struct wt_object *obj)
{
	if(!v || !obj) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(wt_PipeAddObject(v->pipe, obj) < 0) {
		WT_ALARM(WT_ERROR, "Failed to add object to pipe");
		goto err_return;
	}

	obj->view = v;

	return 0;

err_return:
	WT_ALARM(WT_ERROR, "Failed to add object to view");
	return -1;
}


WT_API void wt_ViewRemoveObject(struct wt_object *obj)
{
	struct wt_pipe *pip;

	if(!obj) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	if(!obj->view) {
		WT_ALARM(WT_WARNING, "Object is not attached to any view");
		return;
	}

	pip = obj->view->pipe;

	wt_PipeRemoveObject(pip, wt_PipeGetSlot(pip, obj->name));

	obj->view = NULL;
}
