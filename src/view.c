#include "view.h"

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

void view_render_models(struct fh_model *m)
{
	fh_RenderModel(m, NULL, NULL);
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
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(!(lst = fh_malloc(sizeof(struct fh_view_list)))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for view list");
		goto err_return;
	}

	/* Set attributes */
	lst->context = ctx;
	lst->number = 0;

	for(i = 0; i < FH_VIEW_LIST_LIM; i++)
		lst->views[i] = NULL;

	return lst;

err_return:
	ALARM(ALARM_ERR, "Failed to create view list");
	return NULL;
}


FH_API void fh_DestroyViewList(struct fh_view_list *lst)
{
	s8 i;

	if(!lst) {
		ALARM(ALARM_WARN, "Input parameters invalid");
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
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if((slot = view_get_slot(lst)) < 0) {
		ALARM(ALARM_ERR, "No more free slots in view list");
		goto err_return;
	}

	if(!(v = fh_malloc(sizeof(struct fh_view)))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for view struct");
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
		ALARM(ALARM_ERR, "Failed to create camera");
		goto err_free_v;
	}

	/* Create a new pipeline */
	if(!(v->pipe = fh_CreatePipe(v->camera->pos))) {
		ALARM(ALARM_ERR, "Failed to create new model pipeline");
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
	ALARM(ALARM_ERR, "Failed to create new view struct");
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
	 * Now we can render all models in order.
	 */
	fh_PipeApply(v->pipe, &view_render_models);			


	/*
	 * Lastly, reset everything.
	 */
	fh_ContextDisableScissor(ctx);
	fh_ResetViewport(ctx);
}


FH_API void fh_ResizeView(struct fh_view *v, struct fh_rect *rect)
{
	f32 ratio;
	struct fh_camera_info info;

	if(!v || !rect) {
		ALARM(ALARM_WARN, "Input parameters invalid");
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
		ALARM(ALARM_ERR, "Input parameters invalid");
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


FH_API s8 fh_ViewAddModel(struct fh_view *v, struct fh_model *mdl)
{
	if(!v || !mdl) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(fh_PipeAddModel(v->pipe, mdl) < 0) {
		ALARM(ALARM_ERR, "Failed to add model to pipe");
		goto err_return;
	}

	mdl->view = v;

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to add model to view");
	return -1;
}


FH_API void fh_ViewRemoveModel(struct fh_model *mdl)
{
	struct fh_pipe *pip;

	if(!mdl) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	if(!mdl->view) {
		ALARM(ALARM_WARN, "Model is not attached to any view");
		return;
	}

	pip = mdl->view->pipe;

	fh_PipeRemoveModel(pip, fh_PipeGetSlot(pip, mdl->name));

	mdl->view = NULL;
}
