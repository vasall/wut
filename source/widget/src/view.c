#include "widget/inc/view.h"

#include "utility/inc/alarm.h"

#include "system/inc/system.h"

#include <stdlib.h>


WUT_INTERN s8 vie_get_slot(struct wut_ViewList *lst)
{
	s8 i;

	for(i = 0; i < WUT_VIEW_LIST_LIM; i++) {
		if(lst->views[i] == NULL)
			return i;
	}

	return -1;
}

WUT_INTERN void vie_render_objects(struct wut_Object *m)
{
	wut_RenderObject(m, NULL, NULL);
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


WUT_XMOD struct wut_ViewList *wut_vie_create_list(struct wut_Context *ctx)
{
	struct wut_ViewList *lst;
	u8 i;

	if(!ctx) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(!(lst = wut_malloc(sizeof(struct wut_ViewList)))) {
		WUT_ALARM(WUT_ERROR, "Failed to allocate memory for view list");
		goto err_return;
	}

	/* Set attributes */
	lst->context = ctx;
	lst->number = 0;

	for(i = 0; i < WUT_VIEW_LIST_LIM; i++)
		lst->views[i] = NULL;

	return lst;

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to create view list");
	return NULL;
}


WUT_XMOD void wut_vie_destroy_list(struct wut_ViewList *lst)
{
	s8 i;

	if(!lst) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

	for(i = 0; i < WUT_VIEW_LIST_LIM; i++) {
		if(lst->views[i]) {
			wut_vie_destroy(lst->views[i]);
		}
	}

	wut_free(lst);
}


WUT_XMOD void wut_vie_render_list(struct wut_ViewList *lst)
{
	s8 i;

	for(i = 0; i < WUT_VIEW_LIST_LIM; i++) {
		if(!lst->views[i])
			continue;

		wut_vie_render(lst->views[i]);
	}
}


WUT_XMOD struct wut_View *wut_vie_create(struct wut_ViewList *lst,
		wut_iRect *rect)
{
	struct wut_View *v;
	struct wut_CameraInfo cam_info;
	s8 slot;

	if(!lst) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if((slot = vie_get_slot(lst)) < 0) {
		WUT_ALARM(WUT_ERROR, "No more free slots in view list");
		goto err_return;
	}

	if(!(v = wut_malloc(sizeof(struct wut_View)))) {
		WUT_ALARM(WUT_ERROR, "Failed to allocate memory for view struct");
		goto err_return;
	}

	/* Set the attributes */
	v->slot = slot;
	v->list = lst;
	wut_irect_cpy(v->shape, *rect);

	/*
	 * Create a camera.
	 */
	cam_info.area_of_view = 60;
        cam_info.aspect_ratio = (f32)(*rect)[2] / (f32)(*rect)[3];
        cam_info.near = 0.01;
        cam_info.far = 1000;
	if(!(v->camera = wut_CreateCamera(cam_info, v))) {
		WUT_ALARM(WUT_ERROR, "Failed to create camera");
		goto err_free_v;
	}

	/* Create a new pipeline */
	if(!(v->pipe = wut_pip_create(v->camera->pos))) {
		WUT_ALARM(WUT_ERROR, "Failed to create new object pipeline");
		goto err_destroy_cam;
	}

	/*
	 * Add view to view list.
	 */

	lst->views[slot] = v;
	lst->number++;

	return v;

err_destroy_cam:
	wut_DestroyCamera(v->camera);

err_free_v:
	wut_free(v);

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to create new view struct");
	return NULL;
}


WUT_XMOD void wut_vie_destroy(struct wut_View *v)
{
	struct wut_ViewList *lst;

	if(!v)
		return;

	lst = v->list;

	lst->views[v->slot] = NULL;
	lst->number--;	

	wut_pip_destroy(v->pipe);
	wut_free(v);
}


WUT_XMOD void wut_vie_render(struct wut_View *v)
{
	struct wut_Context *ctx;

	if(!v)
		return;

	ctx = v->list->context;

	/*
	 * First translate.
	 */
	wut_SetViewport(ctx, v->shape);


	/*
	 * Then enable scissors.
	 */
	wut_ContextEnableScissor(ctx, v->shape);


	/*
	 * Now we can render all objects in order.
	 */
	wut_pip_apply(v->pipe, &vie_render_objects);			


	/*
	 * Lastly, reset everything.
	 */
	wut_ContextDisableScissor(ctx);
	wut_ResetViewport(ctx);
}


WUT_XMOD void wut_vie_resize(struct wut_View *v, wut_iRect *rect)
{
	struct wut_CameraInfo info;

	if(!v || !rect) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

	wut_irect_cpy(v->shape, *rect);

	info = wut_GetCameraInfo(v->camera);
	info.aspect_ratio = (f32)(*rect)[2] / (f32)(*rect)[3];
	wut_SetCameraInfo(v->camera, info);
}


WUT_XMOD struct wut_Camera *wut_vie_get_camera(struct wut_View *v)
{
	if(!v) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		return NULL;
	}

	return v->camera;
}


WUT_XMOD void wut_vie_update_pipe(struct wut_View *v)
{
	if(!v)
		return;

	wut_pip_set_reference(v->pipe, v->camera->pos);
}


WUT_XMOD s8 wut_vie_add_object(struct wut_View *v, struct wut_Object *obj)
{
	if(!v || !obj) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(wut_pip_add(v->pipe, obj) < 0) {
		WUT_ALARM(WUT_ERROR, "Failed to add object to pipe");
		goto err_return;
	}

	obj->view = v;

	return 0;

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to add object to view");
	return -1;
}


WUT_XMOD void wut_vie_remove_object(struct wut_Object *obj)
{
	struct wut_Pipe *pip;

	if(!obj) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

	if(!obj->view) {
		WUT_ALARM(WUT_WARNING, "Object is not attached to any view");
		return;
	}

	pip = obj->view->pipe;

	wut_pip_remove(pip, wut_pip_get(pip, obj->name));

	obj->view = NULL;
}
