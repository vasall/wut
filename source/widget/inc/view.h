#ifndef _WT_WIDGET_VIEW_H
#define _WT_WIDGET_VIEW_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "graphic/inc/context.h"
#include "graphic/inc/pipe.h"
#include "graphic/inc/camera.h"
#include "graphic/inc/object.h"

#include "utility/inc/shape.h"



#define WT_VIEW_LIST_LIM	8

struct wt_view_list;

struct wt_view {
	s8 slot;
	struct wt_view_list *list;

	struct wt_pipe *pipe;

	struct wt_camera *camera;

	struct wt_rect shape;
};


struct wt_view_list {
	struct wt_context *context;

	u8 number;
	struct wt_view *views[WT_VIEW_LIST_LIM];
};



/*
 * Create a new view list.
 *
 * @ctx: Pointer to the context
 *
 * Returns: Either a pointer to the view list or NULL if an error occurred
 */
WT_API struct wt_view_list *wt_CreateViewList(struct wt_context *ctx);


/*
 * Destroy a view list and all views in it.
 *
 * @lst: Pointer to the view list.
 */
WT_API void wt_DestroyViewList(struct wt_view_list *lst);


/*
 * Render all views in the view list.
 *
 * @lst: Pointer to the view list
 */
WT_API void wt_RenderViewList(struct wt_view_list *lst);


/*
 * Create a new view struct.
 *
 * @lst: Pointer to the view list
 * @rect: The shape of the view in the window
 *
 * Returns: Either a new view or NULL if an error occurred
 */
WT_API struct wt_view *wt_CreateView(struct wt_view_list *lst,
		struct wt_rect *rect);


/*
 * Destroy a view and free the allocated memory.
 *
 * @v: Pointer to the view struct
 */
WT_API void wt_DestroyView(struct wt_view *v);


/*
 * Render a view onto the window.
 *
 * @v: Pointer to the view struct
 */
WT_API void wt_RenderView(struct wt_view *v);


/*
 * Resize a view.
 *
 * @v: Pointer to the view
 * @rect: The new shape of the view
 */
WT_API void wt_ResizeView(struct wt_view *v, struct wt_rect *rect);


/*
 * Get the camera of a view struct.
 *
 * @v: Pointer to the view struct
 *
 * Returns: A pointer to the camera
 */
WT_API struct wt_camera *wt_GetViewCamera(struct wt_view *v);


/*
 * This function should be called, anytime the camera moves, so the reference
 * point for the pipe can be adjusted.
 *
 * @v: Pointer to the view
 */
WT_API void wt_UpdateViewPipe(struct wt_view *v);


/*
 * Attach a object to a view.
 *
 * @v: Pointer to the view
 * @obj: Pointer to the object
 */
WT_API s8 wt_ViewAddObject(struct wt_view *v, struct wt_object *obj);


/*
 * Remove a object from the view.
 *
 * @obj: Pointer to the object
 */
WT_API void wt_ViewRemoveObject(struct wt_object *obj);

#endif /* _WT_WIDGET_VIEW_H */
