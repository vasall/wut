#include "graphic/inc/object.h"

#include "utility/inc/alarm.h"
#include "utility/inc/table.h"

#include "core/inc/core.h"

#include "system/inc/system.h"

#include <stdlib.h>

#define WUT_MDL_DEBUG 1




WUT_INTERN void obj_destroy(struct wut_Object *obj)
{
	u32 i;
	struct wut_ObjectUniform *uniform;

	glBindVertexArray(obj->vao);

	for(i = 0; i < obj->uniform_number; i++) {
		uniform =  &obj->uniforms[i];
		glDeleteBuffers(1, &uniform->bao);
		wut_free(uniform->data);	
	}

	glDeleteBuffers(1, &obj->ebo);
	glDeleteBuffers(1, &obj->bao);
	glDeleteVertexArrays(1, &obj->vao);

	wut_free(obj->vertex_buffer);
	wut_free(obj->index_buffer);

	wut_free(obj);

}


WUT_INTERN void obj_activate_uniforms(struct wut_Object *obj)
{
	u32 i;
	struct wut_ObjectUniform *uniform;

	for(i = 0; i < obj->uniform_number; i++) {
		uniform = &obj->uniforms[i];

		glBindBuffer(GL_UNIFORM_BUFFER, uniform->bao);
		glBufferData(GL_UNIFORM_BUFFER, uniform->size, uniform->data, GL_STATIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, uniform->location, uniform->bao);
	}
}


WUT_INTERN void obj_rmv_fnc(u32 size, void *ptr)
{
	struct wut_Object *obj;

	WUT_IGNORE(size);

	if(!ptr)
		return;

	obj = (struct wut_Object *)ptr;

	obj_destroy(obj);
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

WUT_API s8 wut_InitObjectTable(struct wut_Context *ctx)
{
	struct wut_Table *tbl;

	if(!ctx) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(!(tbl = wut_CreateTable(&obj_rmv_fnc))) {
		WUT_ALARM(WUT_ERROR, "Failed to create new table");
		goto err_return;
	}

	/* Attach the object table to the context */
	ctx->objects = tbl;

	return 0;

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to create new object table.");
	return -1;
}


WUT_API void wut_CloseObjectTable(struct wut_Context *ctx)
{
	if(!ctx)
		return;

	wut_DestroyTable(ctx->objects);
	ctx->objects = NULL;
}


WUT_API void wut_RemoveObject(struct wut_Object *obj)
{
	if(!obj)
		return;

	wut_ContextRemove(obj->context, WUT_CONTEXT_OBJECTS, obj->name);
}


WUT_API struct wut_Object *wut_GetObject(struct wut_Context *ctx, char *name)
{
	struct wut_Object *obj;

	if(!ctx || !name) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(wut_GetTable(ctx->objects, name, NULL, (void **)&obj) != 1) {
		WUT_ALARM(WUT_ERROR, "Object could not be found in wut_Table");
		goto err_return;
	}

	return obj;

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to get object from object table");
	return NULL;


}


WUT_API void wut_SetObjectUniform(struct wut_Object *obj, char *name, void *ptr)
{
	u32 i;
	struct wut_ObjectUniform *uniform;

	if(!obj || !name || !ptr) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		return;
	}

	for(i = 0; i < obj->uniform_number; i++) {
		uniform = &obj->uniforms[i];

		if(strcmp(uniform->name, name) == 0) {
			memcpy(uniform->data, ptr, uniform->size);
			return;
		}
	}

	WUT_ALARM(WUT_ERROR, "Uniform buffer not found");
}


WUT_API void wut_RenderObject(struct wut_Object *obj, struct wut_Shader *shd,
		struct wut_Texture *tex)
{
	if(!obj)
		return;

	/* Activate the vertex-array-object */
	glBindVertexArray(obj->vao);

	/* Activate shader */
	if(shd)
		wut_UseShader(shd);
	else
		wut_UseShader(obj->shader);

	/* Activate the uniform buffers */
	obj_activate_uniforms(obj);

	/* Activate texture */
	if(tex)
		wut_UseTexture(tex);
	else
		wut_UseTexture(obj->texture);

	/* Finally render the object */
	glDrawElements(GL_TRIANGLES, obj->index_number, GL_UNSIGNED_INT, NULL);

	/* Unuse the active texture and shader */
	wut_UnuseTexture();
	wut_UnuseShader();

	/* Unbind the vertex-array-object */
	glBindVertexArray(0);
}


