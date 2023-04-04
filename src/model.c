#include "model.h"

#include "core.h"

#include <stdlib.h>



FH_API u8 fh_mdl_init(void)
{
	struct dbs_table *tbl;

	if(!(tbl = dbs_tbl_create(&fh_mdl_rmv_fnc))) {
		ALARM(ALARM_ERR, "Failed to create dbs_table for models");
		return -1;
	}

	g_fh_core.models = tbl;
	return 0;
}


FH_API void fh_mdl_close(void)
{
	dbs_tbl_destroy(g_fh_core.models);
	g_fh_core.models = NULL;
}


FH_API struct fh_model *fh_mdl_create(char *name)
{
	struct fh_model *mdl;

	if(!name) {
		ALARM(ALARM_ERR, "Input parameters invlaid");
		goto err_return;
	}

	if(!(mdl = smalloc(sizeof(struct fh_model)))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for model");
		goto err_return;
	}

	strcpy(mdl->name, name);

	return mdl;

err_return:
	ALARM(ALARM_ERR,  "Failed to create new model");
	return NULL;
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
