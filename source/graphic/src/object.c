#include "graphic/inc/object.h"

#include "core/inc/core.h"

#include "system/inc/system.h"

#include "utility/inc/table.h"

#include <stdlib.h>

#define FH_MDL_DEBUG 1




FH_INTERN void obj_destroy(struct fh_object *obj)
{
	u32 i;
	struct fh_object_uniform *uniform;

	glBindVertexArray(obj->vao);

	for(i = 0; i < obj->uniform_number; i++) {
		uniform =  &obj->uniforms[i];
		glDeleteBuffers(1, &uniform->bao);
		fh_free(uniform->data);	
	}

	glDeleteBuffers(1, &obj->ebo);
	glDeleteBuffers(1, &obj->bao);
	glDeleteVertexArrays(1, &obj->vao);

	fh_free(obj->vertex_buffer);
	fh_free(obj->index_buffer);

	fh_free(obj);

}


FH_INTERN void obj_activate_uniforms(struct fh_object *obj)
{
	u32 i;
	struct fh_object_uniform *uniform;

	for(i = 0; i < obj->uniform_number; i++) {
		uniform = &obj->uniforms[i];

		glBindBuffer(GL_UNIFORM_BUFFER, uniform->bao);
		glBufferData(GL_UNIFORM_BUFFER, uniform->size, uniform->data, GL_STATIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, uniform->location, uniform->bao);
	}
}


FH_INTERN void obj_rmv_fnc(u32 size, void *ptr)
{
	struct fh_object *obj;

	fh_Ignore(size);

	if(!ptr)
		return;

	obj = (struct fh_object *)ptr;

	obj_destroy(obj);
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

FH_API s8 fh_InitObjectTable(struct fh_context *ctx)
{
	struct fh_table *tbl;

	if(!ctx) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(!(tbl = fh_tbl_create(&obj_rmv_fnc))) {
		ALARM(ALARM_ERR, "Failed to create new table");
		goto err_return;
	}

	/* Attach the object table to the context */
	ctx->objects = tbl;

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to create new object table.");
	return -1;
}


FH_API void fh_CloseObjectTable(struct fh_context *ctx)
{
	if(!ctx)
		return;

	fh_tbl_destroy(ctx->objects);
	ctx->objects = NULL;
}


FH_API void fh_RemoveObject(struct fh_object *obj)
{
	if(!obj)
		return;

	fh_ContextRemove(obj->context, FH_CONTEXT_OBJECTS, obj->name);
}


FH_API struct fh_object *fh_GetObject(struct fh_context *ctx, char *name)
{
	struct fh_object *obj;

	if(!ctx || !name) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(fh_tbl_get(ctx->objects, name, NULL, (void **)&obj) != 1) {
		ALARM(ALARM_ERR, "Object could not be found in fh_table");
		goto err_return;
	}

	return obj;

err_return:
	ALARM(ALARM_ERR, "Failed to get object from object table");
	return NULL;


}


FH_API void fh_SetObjectUniform(struct fh_object *obj, char *name, void *ptr)
{
	u32 i;
	struct fh_object_uniform *uniform;

	if(!obj || !name || !ptr) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		return;
	}

	for(i = 0; i < obj->uniform_number; i++) {
		uniform = &obj->uniforms[i];

		if(strcmp(uniform->name, name) == 0) {
			memcpy(uniform->data, ptr, uniform->size);
			return;
		}
	}

	ALARM(ALARM_ERR, "Uniform buffer not found");
}


FH_API void fh_RenderObject(struct fh_object *obj, struct fh_shader *shd,
		struct fh_texture *tex)
{
	if(!obj)
		return;

	/* Activate the vertex-array-object */
	glBindVertexArray(obj->vao);

	/* Activate shader */
	if(shd)
		fh_UseShader(shd);
	else
		fh_UseShader(obj->shader);

	/* Activate the uniform buffers */
	obj_activate_uniforms(obj);

	/* Activate texture */
	if(tex)
		fh_UseTexture(tex);
	else
		fh_UseTexture(obj->texture);

	/* Finally render the object */
	glDrawElements(GL_TRIANGLES, obj->index_number, GL_UNSIGNED_INT, NULL);

	/* Unuse the active texture and shader */
	fh_UnuseTexture();
	fh_UnuseShader();

	/* Unbind the vertex-array-object */
	glBindVertexArray(0);
}


