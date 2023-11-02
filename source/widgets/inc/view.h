#ifndef _FH_VIEW_H
#define _FH_VIEW_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "graphics/inc/pipe.h"
#include "graphics/inc/camera.h"
#include "graphics/inc/model.h"
#include "graphics/inc/shape.h"

#include "document/inc/context.h"


#define FH_VIEW_LIST_LIM	8

struct fh_view_list;

struct fh_view {
	s8 slot;
	struct fh_view_list *list;

	struct fh_pipe *pipe;

	struct fh_camera *camera;

	struct fh_rect shape;
};


struct fh_view_list {
	struct fh_context *context;

	u8 number;
	struct fh_view *views[FH_VIEW_LIST_LIM];
};



/*
 * Create a new view list.
 *
 * @ctx: Pointer to the context
 *
 * Returns: Either a pointer to the view list or NULL if an error occurred
 */
FH_API struct fh_view_list *fh_CreateViewList(struct fh_context *ctx);


/*
 * Destroy a view list and all views in it.
 *
 * @lst: Pointer to the view list.
 */
FH_API void fh_DestroyViewList(struct fh_view_list *lst);


/*
 * Render all views in the view list.
 *
 * @lst: Pointer to the view list
 */
FH_API void fh_RenderViewList(struct fh_view_list *lst);


/*
 * Create a new view struct.
 *
 * @lst: Pointer to the view list
 * @rect: The shape of the view in the window
 *
 * Returns: Either a new view or NULL if an error occurred
 */
FH_API struct fh_view *fh_CreateView(struct fh_view_list *lst,
		struct fh_rect *rect);


/*
 * Destroy a view and free the allocated memory.
 *
 * @v: Pointer to the view struct
 */
FH_API void fh_DestroyView(struct fh_view *v);


/*
 * Render a view onto the window.
 *
 * @v: Pointer to the view struct
 */
FH_API void fh_RenderView(struct fh_view *v);


/*
 * Resize a view.
 *
 * @v: Pointer to the view
 * @rect: The new shape of the view
 */
FH_API void fh_ResizeView(struct fh_view *v, struct fh_rect *rect);


/*
 * Get the camera of a view struct.
 *
 * @v: Pointer to the view struct
 *
 * Returns: A pointer to the camera
 */
FH_API struct fh_camera *fh_GetViewCamera(struct fh_view *v);


/*
 * This function should be called, anytime the camera moves, so the reference
 * point for the pipe can be adjusted.
 *
 * @v: Pointer to the view
 */
FH_API void fh_UpdateViewPipe(struct fh_view *v);


/*
 * Attach a model to a view.
 *
 * @v: Pointer to the view
 * @mdl: Pointer to the model
 */
FH_API s8 fh_ViewAddModel(struct fh_view *v, struct fh_model *mdl);


/*
 * Remove a model from the view.
 *
 * @mdl: Pointer to the model
 */
FH_API void fh_ViewRemoveModel(struct fh_model *mdl);

#endif /* _FH_VIEW_H */
