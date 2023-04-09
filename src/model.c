#include "model.h"

#include "alarm.h"
#include "system.h"
#include "core.h"
#include "table.h"

#include <stdlib.h>



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


FH_API void fh_mdl_render(struct fh_model *mdl)
{
	if(!mdl)
		return;

	/* Activate the vertex-array-object */
	glBindVertexArray(mdl->vao);

	/* Activate shader */
	fh_shd_use(mdl->shader);

	/* Activate texture */
	fh_tex_use(mdl->texture);

	glDrawElements(GL_TRIANGLES, mdl->index_number, GL_UNSIGNED_INT, NULL);	

	
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
