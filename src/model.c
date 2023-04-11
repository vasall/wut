#include "model.h"

#include "alarm.h"
#include "system.h"
#include "core.h"
#include "table.h"

#include <stdlib.h>

#define FH_MDL_DEBUG 1


FH_API s8 fh_mdl_init(void)
{
	struct fh_table *tbl;

	if(!(tbl = fh_tbl_create(&fh_mdl_rmv_fnc))) {
		ALARM(ALARM_ERR, "Failed to create fh_table for models");
		return -1;
	}

	g_fh_core.models = tbl;
	return 0;
}


FH_API void fh_mdl_close(void)
{
	fh_tbl_destroy(g_fh_core.models);
	g_fh_core.models = NULL;
}




FH_API void fh_mdl_destroy(struct fh_model *mdl)
{

}


FH_API s8 fh_mdl_insert(struct fh_model *mdl)
{

}


FH_API void fh_mdl_remove(struct fh_model *mdl)
{

}

FH_INTERN void fh_mdl_activate_uniforms(struct fh_model *mdl)
{
	u32 i;
	struct fh_model_uniform *uniform;

	if(!mdl)
		return;

	for(i = 0; i < mdl->uniform_number; i++) {
		uniform = &mdl->uniforms[i];

		glBindBuffer(GL_UNIFORM_BUFFER, uniform->bao);
		glBufferData(GL_UNIFORM_BUFFER, uniform->size, uniform->data, GL_STATIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniform->bao);
	}
}


FH_API void fh_mdl_render(struct fh_model *mdl)
{
	float c;
	float s;
	
	mdl->r += M_PI/200;


	if(!mdl)
		return;

#if FH_MDL_DEBUG
	printf("Bind VAO\n");
#endif

	/* Activate the vertex-array-object */
	glBindVertexArray(mdl->vao);

#if FH_MDL_DEBUG
	printf("done\n");

	printf("Activate shader\n");
#endif

	/* Activate shader */
	fh_shd_use(mdl->shader);

#if FH_MDL_DEBUG
	printf("done\n");

	printf("Activate uniforms\n");
#endif

	/* Activate the uniform buffers */
	fh_mdl_activate_uniforms(mdl);

#if FH_MDL_DEBUG
	printf("done\n");

	printf("Activate texture\n");
#endif

	/* Activate texture */
	fh_tex_use(mdl->texture);

#if FH_MDL_DEBUG
	printf("done\n");

	printf("Draw elements\n");
#endif

	/* Finally render the model */
	glDrawElements(GL_TRIANGLES, mdl->index_number, GL_UNSIGNED_INT, NULL);

#if FH_MDL_DEBUG
	printf("done\n");

	printf("Unuse shader and texture\n");
#endif

	/* Unuse the active texture and shader */
	fh_tex_unuse();
	fh_shd_unuse();
}


FH_API void fh_mdl_rmv_fnc(u32 size, void *ptr)
{
	struct fh_model *mdl;

	/* SILENCIO! */
	if(size) {}

	if(!ptr)
		return;

	mdl = (struct fh_model *)ptr;

	fh_mdl_destroy(mdl);
}
