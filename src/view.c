#include "view.h"

#include <stdlib.h>



/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

FH_API struct fh_view *fh_CreateView(struct fh_context *ctx, vec3_t trans,
		struct fh_camera *cam)
{
	struct fh_view *v;

	if(!ctx || !cam) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(!(v = fh_malloc(sizeof(struct fh_view)))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for view struct");
		goto err_return;
	}

	/* Set the attributes */
	v->context = ctx;
	v->camera = cam;
	vec3_cpy(v->translation, trans);

	/* Create a new pipeline */
	if(!(v->pipe = fh_CreatePipe(v->camera->pos))) {
		ALARM(ALARM_ERR, "Failed to create new model pipeline");
		goto err_free_v;
	}

	return v;

err_free_v:
	fh_free(v);

err_return:
	ALARM(ALARM_ERR, "Failed to create new view struct");
	return NULL;
}


FH_API void fh_DestroyView(struct fh_view *v)
{
	if(!v)
		return;


	fh_DestroyPipe(v->pipe);
	fh_free(v);
}


FH_API void fh_RenderView(struct fh_view *v)
{
	if(!v)
		return;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/*
	 * First translate.
	 */
	glTranslate(v->translation[0], v->translation[1], v->translation[2]);


	/*
	 * Then enable scissors.
	 */
	


	/*
	 * Now we can render all models in order.
	 */

	/*
	 * Lastly, reset everything.
	 */
}
