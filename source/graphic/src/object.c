#include "graphic/inc/object.h"

#include "utility/inc/alarm.h"
#include "utility/inc/table.h"

#include "core/inc/core.h"

#include "system/inc/system.h"

#include <stdlib.h>

#define WT_MDL_DEBUG 1




WT_INTERN void obj_destroy(struct wt_object *obj)
{
	u32 i;
	struct wt_object_uniform *uniform;

	glBindVertexArray(obj->vao);

	for(i = 0; i < obj->uniform_number; i++) {
		uniform =  &obj->uniforms[i];
		glDeleteBuffers(1, &uniform->bao);
		wt_free(uniform->data);	
	}

	glDeleteBuffers(1, &obj->ebo);
	glDeleteBuffers(1, &obj->bao);
	glDeleteVertexArrays(1, &obj->vao);

	wt_free(obj->vertex_buffer);
	wt_free(obj->index_buffer);

	wt_free(obj);

}


WT_INTERN void obj_activate_uniforms(struct wt_object *obj)
{
	u32 i;
	struct wt_object_uniform *uniform;

	for(i = 0; i < obj->uniform_number; i++) {
		uniform = &obj->uniforms[i];

		glBindBuffer(GL_UNIFORM_BUFFER, uniform->bao);
		glBufferData(GL_UNIFORM_BUFFER, uniform->size, uniform->data, GL_STATIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, uniform->location, uniform->bao);
	}
}


WT_INTERN void obj_rmv_fnc(u32 size, void *ptr)
{
	struct wt_object *obj;

	wt_Ignore(size);

	if(!ptr)
		return;

	obj = (struct wt_object *)ptr;

	obj_destroy(obj);
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

WT_API s8 wt_InitObjectTable(struct wt_context *ctx)
{
	struct wt_table *tbl;

	if(!ctx) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(!(tbl = wt_tbl_create(&obj_rmv_fnc))) {
		WT_ALARM(WT_ERROR, "Failed to create new table");
		goto err_return;
	}

	/* Attach the object table to the context */
	ctx->objects = tbl;

	return 0;

err_return:
	WT_ALARM(WT_ERROR, "Failed to create new object table.");
	return -1;
}


WT_API void wt_CloseObjectTable(struct wt_context *ctx)
{
	if(!ctx)
		return;

	wt_tbl_destroy(ctx->objects);
	ctx->objects = NULL;
}


WT_API void wt_RemoveObject(struct wt_object *obj)
{
	if(!obj)
		return;

	wt_ContextRemove(obj->context, WT_CONTEXT_OBJECTS, obj->name);
}


WT_API struct wt_object *wt_GetObject(struct wt_context *ctx, char *name)
{
	struct wt_object *obj;

	if(!ctx || !name) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(wt_tbl_get(ctx->objects, name, NULL, (void **)&obj) != 1) {
		WT_ALARM(WT_ERROR, "Object could not be found in wt_table");
		goto err_return;
	}

	return obj;

err_return:
	WT_ALARM(WT_ERROR, "Failed to get object from object table");
	return NULL;


}


WT_API void wt_SetObjectUniform(struct wt_object *obj, char *name, void *ptr)
{
	u32 i;
	struct wt_object_uniform *uniform;

	if(!obj || !name || !ptr) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		return;
	}

	for(i = 0; i < obj->uniform_number; i++) {
		uniform = &obj->uniforms[i];

		if(strcmp(uniform->name, name) == 0) {
			memcpy(uniform->data, ptr, uniform->size);
			return;
		}
	}

	WT_ALARM(WT_ERROR, "Uniform buffer not found");
}


WT_API void wt_RenderObject(struct wt_object *obj, struct wt_shader *shd,
		struct wt_texture *tex)
{
	if(!obj)
		return;

	/* Activate the vertex-array-object */
	glBindVertexArray(obj->vao);

	/* Activate shader */
	if(shd)
		wt_UseShader(shd);
	else
		wt_UseShader(obj->shader);

	/* Activate the uniform buffers */
	obj_activate_uniforms(obj);

	/* Activate texture */
	if(tex)
		wt_UseTexture(tex);
	else
		wt_UseTexture(obj->texture);

	/* Finally render the object */
	glDrawElements(GL_TRIANGLES, obj->index_number, GL_UNSIGNED_INT, NULL);

	/* Unuse the active texture and shader */
	wt_UnuseTexture();
	wt_UnuseShader();

	/* Unbind the vertex-array-object */
	glBindVertexArray(0);
}


