#include "widget/inc/view.h"

#include "utility/inc/alarm.h"

#include "system/inc/system.h"

#include <stdlib.h>


FH_INTERN s8 view_get_slot(struct fh_view_list *lst)
{
	s8 i;

	for(i = 0; i < FH_VIEW_LIST_LIM; i++) {
		if(lst->views[i] == NULL)
			return i;
	}

	return -1;
}

FH_INTERN void view_render_objects(struct fh_object *m)
{
	fh_RenderObject(m, NULL, NULL);
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


FH_API struct fh_view_list *fh_CreateViewList(struct fh_context *ctx)
{
	struct fh_view_list *lst;
	u8 i;

	if(!ctx) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(!(lst = fh_malloc(sizeof(struct fh_view_list)))) {
		FH_ALARM(FH_ERROR, "Failed to allocate memory for view list");
		goto err_return;
	}

	/* Set attributes */
	lst->context = ctx;
	lst->number = 0;

	for(i = 0; i < FH_VIEW_LIST_LIM; i++)
		lst->views[i] = NULL;

	return lst;

err_return:
	FH_ALARM(FH_ERROR, "Failed to create view list");
	return NULL;
}


FH_API void fh_DestroyViewList(struct fh_view_list *lst)
{
	s8 i;

	if(!lst) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	for(i = 0; i < FH_VIEW_LIST_LIM; i++) {
		if(lst->views[i]) {
			fh_DestroyView(lst->views[i]);
		}
	}

	fh_free(lst);
}


FH_API void fh_RenderViewList(struct fh_view_list *lst)
{
	s8 i;

	for(i = 0; i < FH_VIEW_LIST_LIM; i++) {
		if(!lst->views[i])
			continue;

		fh_RenderView(lst->views[i]);
	}
}


FH_API struct fh_view *fh_CreateView(struct fh_view_list *lst,
		struct fh_rect *rect)
{
	struct fh_view *v;
	struct fh_camera_info cam_info;
	s8 slot;

	if(!lst) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if((slot = view_get_slot(lst)) < 0) {
		FH_ALARM(FH_ERROR, "No more free slots in view list");
		goto err_return;
	}

	if(!(v = fh_malloc(sizeof(struct fh_view)))) {
		FH_ALARM(FH_ERROR, "Failed to allocate memory for view struct");
		goto err_return;
	}

	/* Set the attributes */
	v->slot = slot;
	v->list = lst;
	fh_rect_cpy(&v->shape, rect);

	/*
	 * Create a camera.
	 */
	cam_info.area_of_view = 60;
        cam_info.aspect_ratio = (f32)rect->w / (f32)rect->h;
        cam_info.near = 0.01;
        cam_info.far = 1000;
	if(!(v->camera = fh_CreateCamera(cam_info, v))) {
		FH_ALARM(FH_ERROR, "Failed to create camera");
		goto err_free_v;
	}

	/* Create a new pipeline */
	if(!(v->pipe = fh_CreatePipe(v->camera->pos))) {
		FH_ALARM(FH_ERROR, "Failed to create new object pipeline");
		goto err_destroy_cam;
	}

	/*
	 * Add view to view list.
	 */

	lst->views[slot] = v;
	lst->number++;

	return v;

err_destroy_cam:
	fh_DestroyCamera(v->camera);

err_free_v:
	fh_free(v);

err_return:
	FH_ALARM(FH_ERROR, "Failed to create new view struct");
	return NULL;
}


FH_API void fh_DestroyView(struct fh_view *v)
{
	struct fh_view_list *lst;

	if(!v)
		return;

	lst = v->list;

	lst->views[v->slot] = NULL;
	lst->number--;	

	fh_DestroyPipe(v->pipe);
	fh_free(v);
}


FH_API void fh_RenderView(struct fh_view *v)
{
	struct fh_context *ctx;

	if(!v)
		return;

	ctx = v->list->context;

	/*
	 * First translate.
	 */
	fh_SetViewport(ctx, &v->shape);


	/*
	 * Then enable scissors.
	 */
	fh_ContextEnableScissor(ctx, &v->shape);


	/*
	 * Now we can render all objects in order.
	 */
	fh_PipeApply(v->pipe, &view_render_objects);			


	/*
	 * Lastly, reset everything.
	 */
	fh_ContextDisableScissor(ctx);
	fh_ResetViewport(ctx);
}


FH_API void fh_ResizeView(struct fh_view *v, struct fh_rect *rect)
{
	struct fh_camera_info info;

	if(!v || !rect) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	fh_rect_cpy(&v->shape, rect);

	info = fh_GetCameraInfo(v->camera);
	info.aspect_ratio = (f32)rect->w / (f32)rect->h;
	fh_SetCameraInfo(v->camera, info);
}


FH_API struct fh_camera *fh_GetViewCamera(struct fh_view *v)
{
	if(!v) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		return NULL;
	}

	return v->camera;
}


FH_API void fh_UpdateViewPipe(struct fh_view *v)
{
	if(!v)
		return;

	fh_PipeSetReference(v->pipe, v->camera->pos);
}


FH_API s8 fh_ViewAddObject(struct fh_view *v, struct fh_object *obj)
{
	if(!v || !obj) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(fh_PipeAddObject(v->pipe, obj) < 0) {
		FH_ALARM(FH_ERROR, "Failed to add object to pipe");
		goto err_return;
	}

	obj->view = v;

	return 0;

err_return:
	FH_ALARM(FH_ERROR, "Failed to add object to view");
	return -1;
}


FH_API void fh_ViewRemoveObject(struct fh_object *obj)
{
	struct fh_pipe *pip;

	if(!obj) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	if(!obj->view) {
		FH_ALARM(FH_WARNING, "Object is not attached to any view");
		return;
	}

	pip = obj->view->pipe;

	fh_PipeRemoveObject(pip, fh_PipeGetSlot(pip, obj->name));

	obj->view = NULL;
}
