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

	vec3_t translation;
};


/*
 * Create a new view struct.
 *
 * @ctx: Pointer to the context
 * @trans: A translation vector
 * @cam: The camera to use for the view
 *
 * Returns: Either a new view or NULL if an error occurred
 */
FH_API struct fh_view *fh_CreateView(struct fh_context *ctx, vec3_t translation,
		struct fh_camera *cam);


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
