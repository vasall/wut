#ifndef _FH_VIEW_H
#define _FH_VIEW_H

#include "stdinc.h"
#include "pipe.h"
#include "camera.h"
#include "context.h"


struct fh_view {
	struct fh_context *context;

	struct fh_pipe *pipe;

	struct fh_camera *camera;

	struct fh_rect shape;
};


/*
 * Create a new view struct.
 *
 * @ctx: Pointer to the context
 * @cam: The camera to use for the view
 * @rect: The shape of the view in the window
 *
 * Returns: Either a new view or NULL if an error occurred
 */
FH_API struct fh_view *fh_CreateView(struct fh_context *ctx,
		struct fh_camera *cam, struct fh_rect *rect);


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



#endif /* _FH_VIEW_H */
