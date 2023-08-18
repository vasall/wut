#include "model.h"

#include "system.h"
#include "core.h"
#include "table.h"

#include <stdlib.h>

#define FH_MDL_DEBUG 1




FH_INTERN void mdl_destroy(struct fh_model *mdl)
{
	u32 i;
	struct fh_model_uniform *uniform;

	glBindVertexArray(mdl->vao);

	for(i = 0; i < mdl->uniform_number; i++) {
		uniform =  &mdl->uniforms[i];
		glDeleteBuffers(1, &uniform->bao);
		fh_free(uniform->data);	
	}

	glDeleteBuffers(1, &mdl->ebo);
	glDeleteBuffers(1, &mdl->bao);
	glDeleteVertexArrays(1, &mdl->vao);

	fh_free(mdl->vertex_buffer);
	fh_free(mdl->index_buffer);

	fh_free(mdl);

}


FH_INTERN void mdl_activate_uniforms(struct fh_model *mdl)
{
	u32 i;
	struct fh_model_uniform *uniform;

	for(i = 0; i < mdl->uniform_number; i++) {
		uniform = &mdl->uniforms[i];

		glBindBuffer(GL_UNIFORM_BUFFER, uniform->bao);
		glBufferData(GL_UNIFORM_BUFFER, uniform->size, uniform->data, GL_STATIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, uniform->location, uniform->bao);
	}
}


FH_INTERN void mdl_rmv_fnc(u32 size, void *ptr)
{
	struct fh_model *mdl;

	fh_Ignore(size);

	if(!ptr)
		return;

	mdl = (struct fh_model *)ptr;

	mdl_destroy(mdl);
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

FH_API s8 fh_InitModelTable(struct fh_context *ctx)
{
	struct fh_table *tbl;

	if(!ctx) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(!(tbl = fh_tbl_create(&mdl_rmv_fnc))) {
		ALARM(ALARM_ERR, "Failed to create new table");
		goto err_return;
	}

	/* Attach the model table to the context */
	ctx->models = tbl;

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to create new model table.");
	return -1;
}


FH_API void fh_CloseModelTable(struct fh_context *ctx)
{
	if(!ctx)
		return;

	fh_tbl_destroy(ctx->models);
	ctx->models = NULL;
}


FH_API void fh_RemoveModel(struct fh_model *mdl)
{
	if(!mdl)
		return;

	fh_ContextRemove(mdl->context, FH_CONTEXT_MODELS, mdl->name);
}


FH_API struct fh_model *fh_GetModel(struct fh_context *ctx, char *name)
{
	struct fh_model *mdl;

	if(!ctx || !name) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(fh_tbl_get(ctx->models, name, NULL, (void **)&mdl) != 1) {
		ALARM(ALARM_ERR, "Model could not be found in fh_table");
		goto err_return;
	}

	return mdl;

err_return:
	ALARM(ALARM_ERR, "Failed to get model from model table");
	return NULL;


}


FH_API void fh_SetModelUniform(struct fh_model *mdl, char *name, void *ptr)
{
	u32 i;
	struct fh_model_uniform *uniform;

	if(!mdl || !name || !ptr) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		return;
	}

	for(i = 0; i < mdl->uniform_number; i++) {
		uniform = &mdl->uniforms[i];

		if(strcmp(uniform->name, name) == 0) {
			memcpy(uniform->data, ptr, uniform->size);
			return;
		}
	}

	ALARM(ALARM_ERR, "Uniform buffer not found");
}


FH_API void fh_RenderModel(struct fh_model *mdl, struct fh_shader *shd,
		struct fh_texture *tex)
{
	if(!mdl)
		return;

	/* Activate the vertex-array-object */
	glBindVertexArray(mdl->vao);

	/* Activate shader */
	if(shd)
		fh_UseShader(shd);
	else
		fh_UseShader(mdl->shader);

	/* Activate the uniform buffers */
	mdl_activate_uniforms(mdl);

	/* Activate texture */
	if(tex)
		fh_UseTexture(tex);
	else
		fh_UseTexture(mdl->texture);

	/* Finally render the model */
	glDrawElements(GL_TRIANGLES, mdl->index_number, GL_UNSIGNED_INT, NULL);

	/* Unuse the active texture and shader */
	fh_UnuseTexture();
	fh_UnuseShader();

	/* Unbind the vertex-array-object */
	glBindVertexArray(0);
}


