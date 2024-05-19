#include "graphic/inc/constructor.h"

#include "utility/inc/alarm.h"

#include "graphic/inc/shader.h"
#include "graphic/inc/texture.h"
#include "graphic/inc/camera.h"

#include "system/inc/system.h"

#include <stdlib.h>
#include <stdint.h>

#define WT_MDLC_DEBUG 0


WT_INTERN u32 objc_type_size(GLenum type) {
	switch(type) {
		case GL_FLOAT: return sizeof(f32);
	}

	return 0;
}


WT_INTERN u32 objc_calc_stride(struct wt_object_c *c)
{
	u32 stride = 0;
	u8 i;
	struct wt_object_c_attrib *attr;

	for(i = 0; i < c->attrib_num; i++) {
		attr = &c->attribs[i];
		stride += (attr->elements * attr->element_size);
	}

	return stride;
}


WT_INTERN void objc_concate_data(struct wt_object *obj, struct wt_object_c *c)
{
	u8 *ptr;
	u32 i;
	u32 j;
	struct wt_object_c_attrib *attr;
	u32 tmp = 0;

	ptr = obj->vertex_buffer;
	
	/* Iterate through all vertices... */
	for(i = 0; i < obj->vertex_number; i++) {
		/* ...and for every vertex write all attributes */
		for(j = 0; j < c->attrib_num; j++) {
			attr = &c->attribs[j];

			tmp = (attr->elements * attr->element_size);
			memcpy(ptr, attr->data + (tmp * i), tmp);

			ptr += tmp;
		}
	}
}


WT_INTERN void objc_create_buffers(struct wt_object *obj)
{
	u32 size;

	/* Create the vertex-array-object */
	glGenVertexArrays(1, &obj->vao);
	glBindVertexArray(obj->vao);

	/* Create the buffer-array-object */
	glGenBuffers(1, &obj->bao);
	glBindBuffer(GL_ARRAY_BUFFER, obj->bao);
	size = obj->vertex_number * obj->vertex_stride;
	glBufferData(GL_ARRAY_BUFFER, size, obj->vertex_buffer,
			GL_STATIC_DRAW);

	/* Create the element-buffer-object */
	glGenBuffers(1, &obj->ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->ebo);
	size = obj->index_number * U32_S;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, obj->index_buffer,
			GL_STATIC_DRAW);


	glBindVertexArray(0);
}


WT_INTERN void objc_enable_attr(struct wt_object *obj, struct wt_object_c *c)
{
	u32 i;
	struct wt_object_c_attrib *attr;
	u16 slot_lst[WT_OBJECT_ATTRIB_LIM];
	s8 slot;
	void *p;
	u32 count;

	wt_zeros(slot_lst, WT_OBJECT_ATTRIB_LIM * U16_S);

	for(i = 0; i < c->attrib_num; i++) {
		attr = &c->attribs[i];

		if((slot = wt_ShaderGetInputLoc(obj->shader, attr->name)) < 0) {
			WT_ALARM(WT_ERROR, "Input variable not found");
			return;
		}

		slot_lst[slot] = i;
	}


	/* Bind the vertex-array-object */
	glBindVertexArray(obj->vao);

	/* Bind the buffer-array-object */
	glBindBuffer(GL_ARRAY_BUFFER, obj->bao);


	/*
	 * Enable the attributes.
	 */
	count = 0;
	for(i = 0; i < c->attrib_num; i++) {
		slot = slot_lst[i];
		attr = &c->attribs[i];

		p = (void *)((uintptr_t)(count * U8_S));

		glVertexAttribPointer(
				i,
				attr->elements,
				attr->type,
				GL_FALSE,
				obj->vertex_stride,
				p
				);

		count += (attr->elements * attr->element_size);
	}
}

WT_INTERN s8 objc_init_uniforms(struct wt_object *obj, struct wt_object_c *c)
{
	u32 i;
	s32 j;
	struct wt_object_c_unibuf *unibuf;
	struct wt_object_uniform *uniform;

	s32 slot;

	/* Bind the vertex-array-object */
	glBindVertexArray(obj->vao);

	for(i = 0; i < c->unibuf_num; i++) {
		unibuf = &c->unibufs[i];
		uniform = &obj->uniforms[i];

		/* First write everything to the uniform buffer */
		strcpy(uniform->name, unibuf->name);
		uniform->size = unibuf->size;
		if(!(uniform->data = wt_malloc(uniform->size))) {
			WT_ALARM(WT_ERROR, "Failed to allocate memory");
			goto err_free;
		}

		/* Then register it with OpenGL */
		glGenBuffers(1, &uniform->bao);
		glBindBuffer(GL_UNIFORM_BUFFER, uniform->bao);
		glBufferData(GL_UNIFORM_BUFFER, uniform->size, NULL,
				GL_STATIC_DRAW);

		printf("Uniform %d: %s\n", i, uniform->name);

		/* Retrieve the binding location in the shader */
		if((slot = wt_ShaderGetUniformLoc(obj->shader, uniform->name)) < 0) {
			WT_ALARM(WT_ERROR, "Uniform not found");
			goto err_free;
		}

		printf("Location: %d\n", slot);

		uniform->location = slot;
	}

	obj->uniform_number = c->unibuf_num;


	return 0;

err_free:
	for(j = i - 1; j >= 0; j--) {
		uniform = &obj->uniforms[j];
		glDeleteBuffers(1, &uniform->bao);
		wt_free(uniform->data);
	}


	WT_ALARM(WT_ERROR, "Failed to initialize uniform buffers");
	return -1;
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

WT_API struct wt_object_c *wt_BeginObjectConstr(char *name,
		u32 vnum, u32 inum, u32 *idx)
{
	struct wt_object_c *c;

	if(!name || vnum < 1 || inum < 1 || !idx) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(!(c = wt_malloc(sizeof(struct wt_object_c)))) {
		WT_ALARM(WT_ERROR, "Failed to allocate memory for constructor");
		goto err_return;
	}

	/* Set the basic attributes */
	strcpy(c->name, name);
	c->vtx_num = vnum;
	c->idx_num = inum;
	c->attrib_num = 0;
	c->unibuf_num = 0;

	/* Copy the indices */
	if(!(c->idx = wt_malloc(inum * sizeof(u32)))) {
		WT_ALARM(WT_ERROR, "Failed to allocate memory for index buffer");
		goto err_free_c;
	}
	memcpy(c->idx, idx, inum * sizeof(u32));

	/* Reset configuration attributes */
	c->shader = NULL;
	c->texture = NULL;

	return c;

err_free_c:
	wt_free(c);

err_return:
	WT_ALARM(WT_ERROR, "Failed to begin creating new object");
	return NULL;


}


WT_API struct wt_object *wt_EndObjectConstr(struct wt_object_c *c,
		struct wt_context *ctx, wt_vec3_t pos, wt_vec3_t rot)
{
	u32 i;

	struct wt_object *obj;
	u32 tmp;

	u32 size;
	void **p;

	if(!c || !ctx) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(!(obj = wt_malloc(sizeof(struct wt_object)))) {
		WT_ALARM(WT_ERROR, "Failed to alloctae memory for object");
		goto err_return;
	}

	
	/* Copy basic attributes */
	strcpy(obj->name, c->name);
	
	/* Copy the resource references over */
	obj->shader = c->shader;
	obj->texture = c->texture;
	
	/*
	 * Copy the indices.
	 */

	obj->index_number = c->idx_num;

	tmp = obj->index_number * U32_S;
	if(!(obj->index_buffer = wt_malloc(tmp))) {
		WT_ALARM(WT_ERROR, "Failed to allocate memory for index buffer");
		goto err_free_obj;
	}

	memcpy(obj->index_buffer, c->idx, tmp);


	/*
	 * Set the basic vertex data attributes and allocate the buffer.
	 */

	obj->vertex_number = c->vtx_num;
	obj->vertex_stride = objc_calc_stride(c);

	tmp = obj->vertex_stride * obj->vertex_number;
	if(!(obj->vertex_buffer = wt_malloc(tmp))) {
		WT_ALARM(WT_ERROR, "Failed to allocate memory for vertex buffer");
		goto err_free_index_buffer;
	}

	/*
	 * Concate the vertex data in the following pattern:
	 *
	 *        vtx 0              vtx 1              vtx 2
	 *  < pos  tex  nrm >  < pos  tex  nrm >  < pos  tex  nrm >
	 */

	objc_concate_data(obj, c);

	/*
	 * Create and initialize the data objects in OpenGL.
	 */
	objc_create_buffers(obj);

	/*
	 * Enable the vertex attributes.
	 */
	objc_enable_attr(obj, c);

	/*
	 * Create and initialize the uniform buffers.
	 */
	if(objc_init_uniforms(obj, c) < 0)
		goto err_destroy_buffers;


	wt_vec3_cpy(obj->position, pos);
	wt_vec3_cpy(obj->rotation, rot);

	/* Reset view pointer */
	obj->view = NULL;

	/* Insert the new object into the context object table */
	obj->context = ctx;

	size = sizeof(struct wt_object);
	p = (void **)&obj;

	if(wt_tbl_add(ctx->objects, obj->name, size, p) < 0) {
		WT_ALARM(WT_ERROR, "Failed to insert entry into wt_table");
		goto err_destroy_uniforms;
	}

	return obj;

err_destroy_uniforms:
	for(i = 0; i < obj->uniform_number; i++) {
		glDeleteBuffers(1, &obj->uniforms[i].bao);
		wt_free(obj->uniforms[i].data);	
	}

err_destroy_buffers:

	glDeleteBuffers(1, &obj->ebo);
	glDeleteBuffers(1, &obj->bao);
	glDeleteVertexArrays(1, &obj->vao);

err_free_index_buffer:
	wt_free(obj->index_buffer);

err_free_obj:
	wt_free(obj);

err_return:
	WT_ALARM(WT_ERROR, "Failed to convert constructor");
	return NULL;
}


WT_API void wt_ObjectConstrCleanup(struct wt_object_c *c)
{
	u8 i;

	if(!c)
		return;

	for(i = 0; i < c->attrib_num; i++) {
		wt_free(c->attribs[i].data);
	}

	wt_free(c->idx);

	wt_free(c);
}


/*
 * GENERAL
 */

WT_API void wt_ObjectConstrTexture(struct wt_object_c *c, struct wt_texture *tex)
{
	if(!c || !tex) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	c->texture = tex;
}



/*
 * CUSTOM MODE
 */

WT_API void wt_ObjectConstrShader(struct wt_object_c *c, struct wt_shader *shd)
{
	if(!c || !shd) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	c->shader = shd;
}


WT_API void wt_ObjectConstrAttrib(struct wt_object_c *c, char *name, u8 size,
		GLenum type, void *data)
{
	struct wt_object_c_attrib *a;
	u32 s;

	if(!c || !name || size < 1 || !data) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		return;
	}

	if(c->attrib_num + 1 > WT_OBJECT_ATTRIB_LIM)
		return;

	a = &c->attribs[c->attrib_num];

	strcpy(a->name, name);
	a->elements = size;
	a->element_size = objc_type_size(type);
	a->type = type;

	s = c->vtx_num * a->elements * a->element_size;
	if(!(a->data = wt_malloc(s))) {
		WT_ALARM(WT_ERROR, "Failed to allocate memory for attribute");
		return;
	}

	memcpy(a->data, data, s);

	c->attrib_num++;
}


WT_API void wt_ObjectConstrUniform(struct wt_object_c *c, char *name, u32 size)
{
	struct wt_object_c_unibuf *u;

	if(!c || !name || size < 1) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		return;
	}

	if(c->unibuf_num + 1 > WT_OBJECT_UNIFORM_LIM)
		return;

	u = &c->unibufs[c->unibuf_num];

	strcpy(u->name, name);
	u->size = size;

	c->unibuf_num++;
}
