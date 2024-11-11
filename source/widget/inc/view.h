#ifndef _WUT_WIDGET_VIEW_H
#define _WUT_WIDGET_VIEW_H

#include "source/core/inc/define.h"
#include "source/core/inc/import.h"

#include "source/graphic/inc/context.h"
#include "source/graphic/inc/pipe.h"
#include "source/graphic/inc/camera.h"
#include "source/graphic/inc/object.h"

#include "source/math/inc/polygon.h"



#define WUT_VIEW_LIST_LIM	8

struct wut_ViewList;

struct wut_View {
	s8 slot;
	struct wut_ViewList *list;

	struct wut_Pipe *pipe;

	struct wut_Camera *camera;

	wut_iRect shape;
};


struct wut_ViewList {
	struct wut_Context *context;

	u8 number;
	struct wut_View *views[WUT_VIEW_LIST_LIM];
};



/*
 * Create a new view list.
 *
 * @ctx: Pointer to the context
 *
 * Returns: Either a pointer to the view list or NULL if an error occurred
 */
WUT_XMOD struct wut_ViewList *wut_vie_create_list(struct wut_Context *ctx);


/*
 * Destroy a view list and all views in it.
 *
 * @lst: Pointer to the view list.
 */
WUT_XMOD void wut_vie_destroy_list(struct wut_ViewList *lst);


/*
 * Render all views in the view list.
 *
 * @lst: Pointer to the view list
 */
WUT_XMOD void wut_vie_render_list(struct wut_ViewList *lst);


/*
 * Create a new view struct.
 *
 * @lst: Pointer to the view list
 * @rect: The shape of the view in the window
 *
 * Returns: Either a new view or NULL if an error occurred
 */
WUT_XMOD struct wut_View *wut_vie_create(struct wut_ViewList *lst,
		wut_iRect *rect);


/*
 * Destroy a view and free the allocated memory.
 *
 * @v: Pointer to the view struct
 */
WUT_XMOD void wut_vie_destroy(struct wut_View *v);


/*
 * Render a view onto the window.
 *
 * @v: Pointer to the view struct
 */
WUT_XMOD void wut_vie_render(struct wut_View *v);


/*
 * Resize a view.
 *
 * @v: Pointer to the view
 * @rect: The new shape of the view
 */
WUT_XMOD void wut_vie_resize(struct wut_View *v, wut_iRect *rect);


/*
 * Get the camera of a view struct.
 *
 * @v: Pointer to the view struct
 *
 * Returns: A pointer to the camera
 */
WUT_XMOD struct wut_Camera *wut_vie_get_camera(struct wut_View *v);


/*
 * This function should be called, anytime the camera moves, so the reference
 * point for the pipe can be adjusted.
 *
 * @v: Pointer to the view
 */
WUT_XMOD void wut_vie_update_pipe(struct wut_View *v);


/*
 * Attach a object to a view.
 *
 * @v: Pointer to the view
 * @obj: Pointer to the object
 */
WUT_XMOD s8 wut_vie_add_object(struct wut_View *v, struct wut_Object *obj);


/*
 * Remove a object from the view.
 *
 * @obj: Pointer to the object
 */
WUT_XMOD void wut_vie_remove_object(struct wut_Object *obj);

#endif /* _WUT_WIDGET_VIEW_H */
